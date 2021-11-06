#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"

#include "../inputs/generic.h"

#include "MiniCompiler.h"
#include "MiniFrontend.h"
#include "MiniInterpreter.h"

#include <cstdint>
#include <iostream>

using namespace llvm;

namespace jited {
    
extern "C" void escapeCtx() {
    std::cout << "escaped";
};

extern "C" void printInt(int x) {
    std::cout << x;
}

extern "C" void printIntEndl(int x) {
    std::cout << x << std::endl;
}

extern "C" int readInt() {
    int x;
    std::cin >> x;
    return x;
}

class ModuleCompiler {
private:
    // static Expected<std::unique_ptr<ModuleCompiler>> _create() {
    //     auto jit = orc::LLJITBuilder().create();
    //     orc::MangleAndInterner mangler((*jit)->getExecutionSession(), (*jit)->getDataLayout());
    
    //     if (!jit)
    //         return jit.takeError();
        
    //     (*jit)->getExecutionSession().setErrorReporter(dontLogErrors);

    //     return std::make_unique<ModuleCompiler>(std::move(*jit), mangler);
    // }
public:
    static void dontLogErrors(Error Err) {
        // logAllUnhandledErrors(std::move(Err), errs(), "JIT session error (...): ");
    }

    std::unique_ptr<orc::LLJIT> jit;
    orc::MangleAndInterner mangler;

    ModuleCompiler(std::unique_ptr<orc::LLJIT> jit, orc::MangleAndInterner mangler) : jit(std::move(jit)), mangler(mangler) {
        loadCommon();
        // std::cout << &jit->getExecutionSession();
    };

    static std::unique_ptr<ModuleCompiler> create() {
        auto jit = orc::LLJITBuilder().create();
        orc::MangleAndInterner mangler((*jit)->getExecutionSession(), (*jit)->getDataLayout());
    
        if (!jit)
            return nullptr;
            // return jit.takeError();
        
        (*jit)->getExecutionSession().setErrorReporter(dontLogErrors);

        return std::make_unique<ModuleCompiler>(std::move(*jit), mangler);
    }

    void loadCommon() {
        auto DG = orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
            jit->getDataLayout().getGlobalPrefix(),
            [&](const orc::SymbolStringPtr &S) {
                DenseSet<orc::SymbolStringPtr> allowed({
                    mangler("printf"),
                    mangler("malloc"),
                    mangler("free")
                }); 
                return allowed.count(S);
            }
        );

        auto &jd = jit->getMainJITDylib();
        jd.addGenerator(std::move(DG.get()));
        auto s = orc::absoluteSymbols({
            {
                mangler("escapeCtx"),
                JITEvaluatedSymbol(pointerToJITTargetAddress(&escapeCtx), JITSymbolFlags::Exported)
            },
            {
                mangler("readInt"),
                JITEvaluatedSymbol(pointerToJITTargetAddress(&readInt), JITSymbolFlags::Exported)
            },
            {
                mangler("printInt"),
                JITEvaluatedSymbol(pointerToJITTargetAddress(&printInt), JITSymbolFlags::Exported)
            },
            {
                mangler("printIntEndl"),
                JITEvaluatedSymbol(pointerToJITTargetAddress(&printIntEndl), JITSymbolFlags::Exported)
            },
            {
                mangler("struct.union_t"),
                JITEvaluatedSymbol(pointerToJITTargetAddress(&printIntEndl), JITSymbolFlags::Exported)
            }
        });
        Error e = jd.define(s);
        if (e) {
            errs() << e;
            return;
        }
    }

    int addFile(char* fname) {
        orc::ThreadSafeContext tsc(std::make_unique<LLVMContext>());
        SMDiagnostic err;
        auto m = parseIRFile(fname, err, *tsc.getContext());
        if (!m) {
            std::cout << "<" << fname << ">";
            std::cout << err.getMessage().str() << std::endl;
            return -1;
        }

        orc::ThreadSafeModule tsm(std::move(m), std::move(tsc));

        if (auto err = this->jit->addIRModule(std::move(tsm))) {
            errs() << err;
            return 1;
        }
        return 0;
    }

    int addString(std::string input) {
        orc::ThreadSafeContext tsc(std::make_unique<LLVMContext>());
        SMDiagnostic err;
        auto m = parseAssemblyString(input, err, *tsc.getContext());
        if (!m) {
            std::cout << err.getMessage().str() << std::endl;
            return -1;
        }
        
        orc::ThreadSafeModule tsm(std::move(m), std::move(tsc));

        if (auto err = this->jit->addIRModule(std::move(tsm))) {
            errs() << err;
            return 1;
        }
        return 0;
    }

    void* getSym(std::string name) {
        auto res = jit->lookup(name);
        if (auto e = res.takeError()) {
            errs() << e;
            std::cerr << std::endl;
            return NULL;
        } else {
            return (void*) res->getAddress();
        }
    }
};

class JIT : public mini::MiniInterpreter {
protected:
    std::unique_ptr<ModuleCompiler> mc;
    std::unique_ptr<ast::ASTVisitor> compiler;
public:
    JIT(ast::ProgramPtr program, std::unique_ptr<ast::ASTVisitor> compiler) :  
        mini::MiniInterpreter(program),
        mc(std::move(jited::ModuleCompiler::create())),
        compiler(std::move(compiler)) {};
    
    // WE PROBABLY NEED A LOOKUP/EVALUATEDSYMBOL FOR STRUCTS
    antlrcpp::Any visit(ast::InvocationExpression* expression) override {
        // branch based on whether it's compiled already or not
        testbed(expression);
        if (mc->getSym(expression->name)) {
            // run jitted
            intptr_t* args = new intptr_t[expression->arguments.size()];

            for (ast::ExpressionPtr e : expression->arguments) {

            }

            delete[] args;
        } else {
            return MiniInterpreter::visit(expression);
        }
    }

    void compileFunction(std::string fname) {
        for (ast::FunctionPtr f : program->funcs) {
            if (f->name == fname) {
                std::string moduleStr = f->accept(compiler.get());
                // TODO do processing first to add structs and fns
                mc->addString(moduleStr);
                return;
            }
        }
    }

    void testbed(ast::InvocationExpression* expression) {

    }
};

}
// parse assembly string...?

int main(int argc, char** argv) {
    if (argc < 3) return 1;
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmPrinters();

    auto J = std::move(jited::ModuleCompiler::create());

    J->addFile("../inputs/431/struct.ll");
    J->addFile(argv[1]);
    J->jit->getExecutionSession().dump(llvm::outs());
    // auto* fptr = (struct union_t (*)(struct union_t)) J->getSym(argv[2]);
    // if (fptr) {
    //     struct union_t empty = {NULL, NULL};
    //     auto res = fptr(empty);
    //     std::cout << res.ints << std::endl;
    //     std::cout << res.strs << std::endl;
    //     std::cout << res.strs[0] << std::endl;
    // }








    // if (argc == 1) {
    //     return 0;
    // }

    // mini::MiniFrontend fe;
    // ast::ProgramPtr p = fe.parseFile(argv[1]);
    // if (!p) {
    //     return 1;
    // }
    // jited::JIT j(p, std::make_unique<minic::StatementToBlockVisitor>(p));
    // return p->accept(&j);
}


