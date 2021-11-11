#include "llvm/IR/Module.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
// #include "LLVMContextImpl.h"
#include "llvm/IR/DerivedTypes.h"
#include "../inputs/generic.h"

#include "MiniCompiler.h"
#include "MiniFrontend.h"
#include "MiniInterpreter.h"

#include <cstdint>
#include <iostream>
#include <regex>

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

class StructAssembler {
private:
    LLVMContext ctx;
    PointerType* pointerType;
    IntegerType* i1Type;
    IntegerType* i8Type;
    IntegerType* i16Type;
    IntegerType* i32Type;
    IntegerType* i64Type;
public:
    enum Ty {
        PTR,
        I1,
        I8,
        I16,
        I32,
        I64
    };

    StructAssembler() :
        pointerType(Type::getInt8PtrTy(ctx)),
        i1Type(Type::getInt1Ty(ctx)),
        i8Type(Type::getInt8Ty(ctx)),
        i16Type(Type::getInt16Ty(ctx)),
        i32Type(Type::getInt32Ty(ctx)),
        i64Type(Type::getInt64Ty(ctx)) {};

    StructType* assemble(std::vector<Ty> types) {
        // return StructType::create(ArrayRef<Type*>(types));
        std::vector<Type*> translated;
        for (auto ty : types) {
            switch (ty) {
                case PTR:
                    translated.push_back(pointerType);
                    break;
                case I1:
                    translated.push_back(i1Type);
                    break;
                case I8:
                    translated.push_back(i8Type);
                    break;
                case I16:
                    translated.push_back(i16Type);
                    break;
                case I32:
                    translated.push_back(i32Type);
                    break;
                case I64:
                    translated.push_back(i64Type);
                    break;
                default:
                    throw std::runtime_error("unexpected enum Ty in structassembler");
                    break;
            }
        }
        ArrayRef<Type*> ar(translated);
        return StructType::create(ar);
    }
};

#include <typeinfo>
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
    StructAssembler sa;
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
            std::cout << "<" << fname << "> ";
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

        // StringMap<StructType*> st = tsm.getContext().getContext()->pImpl->NamedStructTypes;
        // StructType* stp = StructType::getTypeByName(tsm.getContext(), "struct.union_t");

        if (auto err = this->jit->addIRModule(std::move(tsm))) {
            errs() << err;
            return 1;
        }
        return 0;
    }

    void* getSym(std::string name) {
        auto res = jit->lookup(name);
        if (auto e = res.takeError()) {
            // errs() << e;
            return NULL;
        } else {
            // std::cout << typeid(res->getAddress()).name() << std::endl;
            return (void*) res->getAddress();
        }
    }

    const StructLayout* layoutOf(std::vector<StructAssembler::Ty> types) {
        StructType* st = sa.assemble(types);
        return jit->getDataLayout().getStructLayout(st);
    }
};

class JIT : public mini::MiniInterpreter {
protected:
    std::unique_ptr<ModuleCompiler> mc;
    std::shared_ptr<ast::ASTVisitor> compiler;
public:
    JIT(ast::ProgramPtr program, std::shared_ptr<ast::ASTVisitor> compiler) :
        mini::MiniInterpreter(program),
        mc(std::move(jited::ModuleCompiler::create())),
        compiler(compiler) {};
    
    // WE PROBABLY NEED A LOOKUP/EVALUATEDSYMBOL FOR STRUCTS
    antlrcpp::Any visit(ast::InvocationExpression* expression) override {
        // branch based on whether it's compiled already or not
        if (mc->getSym(expression->name)) {
            // run jitted
            // we MAY need to change the function prototypes to have more control over args
            // TODO
            // int8_t* args = new int8_t[expression->arguments.size()];
            
            std::vector<mini::TypedValue> vals;
            int totalSize = 0;
            for (ast::ExpressionPtr e : expression->arguments) {
                
            }
            

            // delete[] args;
            return nullptr;
        } else {
            antlrcpp::Any rv = MiniInterpreter::visit(expression);
            if (true) {
                compileFunction(expression->name);
            }
            return rv;
        }
    }

    // contains the LLVM for defining necessary structs
    std::string structsString() {
        std::ostringstream oss;

        for (auto pair : structs) {
            ast::TypeDeclarationPtr typeDec = pair.second;
            oss << "%struct." << typeDec->name << " = type <{";
            std::string typeList = "";
            for (ast::DeclarationPtr field : typeDec->fields) {
                typeList += field->type->toString() + ", ";
            }
            if (typeList.length() > 0) {
                oss << typeList.substr(0, typeList.length() - 2);
            }
            oss << "}>\n";
        }
        return oss.str();
    }

    // contains the declarations of external functions required
    // this may need to take a function, as well? need access to all functions
    // that may be possible to fudge using error handlers tho
    std::string declareString(std::string fname) {
        // start with "" to have a newline
        // TODO
        std::ostringstream oss;
        oss << "declare i8* @malloc(i32)\n"
            << "declare void @free(i8*)\n"
            << "declare void @printInt(i32)\n"
            << "declare void @printIntEndl(i32)\n"
            << "declare i32 @readInt()\n";

        for (auto pair : funcs) {
            ast::FunctionPtr f = pair.second;
            if (f->name != fname) {
                oss << "declare " << f->retType->toString() << " @" << f->name << "(";
                std::string paramList = "";
                for (ast::DeclarationPtr param : f->params) {
                    paramList += param->type->toString() + " %" + param->name + ", ";
                }
                if (f->params.size() > 0) {
                    oss << paramList.substr(0, paramList.length() - 2) << ")\n";
                } else {
                    oss << ")\n";
                }
            }
        }

        return oss.str();
    }

    std::string moduleString(std::string fname) {
        std::ostringstream oss;
        std::string fbody = funcs.at(fname)->accept(compiler.get());

        std::regex label("^([a-zA-Z][a-zA-Z0-9]*):(.*)");
        std::smatch sm;

        oss << structsString();
        // std::cout << fbody << std::endl;
        if (std::regex_search(fbody, sm, label)) {
            oss << functionPrefix(fname);
            oss << "br label %" << sm[1] << "\n";
            oss << fbody;
        } else {
            throw std::runtime_error("start of compiled function body was not a label");
        }
        oss << declareString(fname);


        // 
        return oss.str();
    }
    
    std::string functionPrefix(std::string name) {
        if (funcs.count(name)) {
            std::string argsName = "%argsarray";
            int ctr = 0;
            int i = 0;
            int ptrStep = sizeof(intptr_t) / sizeof(int32_t);
            std::ostringstream oss;
            ast::FunctionPtr function = funcs.at(name);
            oss << "define " + function->retType->toString() + " @" + function->name + "(i32* " + argsName + ") {\nARGPARSE:\n";
            for (ast::DeclarationPtr param : function->params) {
                oss << "%" << param->name << "PTR = getelementptr i32, i32* " << argsName << ", i64 " << i << "\n";
                oss << "%" << param->name;
                if (param->type->toString() == "i32") {
                    // dereference the i32
                    oss << " = load i32, i32* %" << param->name << "PTR\n";
                    i += 1;
                } else {
                    // bitcast it to a struct pointer
                    oss << " = bitcast i32* %" << param->name << "PTR to " << param->type->toString() << "\n";
                    i += ptrStep;
                }
            }
            return oss.str();
        } else {
            return "";
        }
    }

    void compileFunction(std::string fname) {
        // TODO same as above, may need to change the function headers so we can actually change things...
        
        // for (ast::FunctionPtr f : program->funcs) {
        //     if (f->name == fname) {
        //         std::string moduleStr = f->accept(compiler.get());
        //         // TODO do processing first to add structs and fns
        //         mc->addString(moduleStr);
        //         return;
        //     }
        // }
        if (funcs.count(fname)) {
            ast::FunctionPtr f = funcs.at(fname);
            std::string moduleStr = moduleString(fname);
            mc->addString(moduleStr);
        } else {
            throw std::runtime_error("compilation requested for nonexistent function");
        }
    }
};

}
// parse assembly string...?

int main(int argc, char** argv) {
    // if (argc < 3) return 1;
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmPrinters();

    // auto J = std::move(jited::ModuleCompiler::create());
    // LLVMContext lc;
    // IntegerType* intType = Type::getInt32Ty(lc);
    // PointerType* pType = Type::getInt8PtrTy(lc);
    // StructType* sType = StructType::get(std::vector<Type*>{intType, pType});

    // jited::StructAssembler sa;
    // StructType* sType = sa.assemble({jited::StructAssembler::Ty::I32, jited::StructAssembler::Ty::PTR});
    // const StructLayout* sLayout = J->getLayout(sType);
    // const StructLayout* sLayout = J->layoutOf({jited::StructAssembler::Ty::I32, jited::StructAssembler::Ty::PTR});
    // J->addFile("../inputs/431/glob.ll");
    // J->addFile(argv[1]);

    // // std::cout << sLayout->getElementOffset(0) << std::endl;
    // // std::cout << sLayout->getElementOffset(1) << std::endl;
    // // std::cout << sLayout->hasPadding() << std::endl;
    // // std::cout << sLayout->getSizeInBytes() << std::endl;
    // // std::cout << sLayout->getAlignment().value() << std::endl;
    // // std::cout << "exiting.\n";
    // // auto sl = J->getLayout();
    // // J->jit->getExecutionSession().dump(llvm::outs());
    // auto* fptr = (struct union_t (*)(struct union_t)) J->getSym(argv[2]);
    // if (fptr) {
    //     struct union_t empty = {NULL, NULL};
    //     auto res = fptr(empty);
    //     std::cout << res.ints << std::endl;
    //     std::cout << res.strs << std::endl;
    //     std::cout << res.strs[0] << std::endl;
    // }








    if (argc == 1) {
        return 0;
    }

    mini::MiniFrontend fe;
    ast::ProgramPtr p = fe.parseFile(argv[1]);
    if (!p) {
        return 1;
    }
    std::shared_ptr<ast::ASTVisitor> compiler = std::make_shared<minic::StatementToBlockVisitor>(p);
    jited::JIT j(p, compiler);
    return j.run();
}


