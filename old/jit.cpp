#include <iostream>
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "../inputs/generic.h"
using namespace llvm;

namespace lljit {
    
extern "C" void escapeCtx() {
    std::cout << "!!!";
};

class ModuleCompiler {
private:
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

    static Expected<std::unique_ptr<ModuleCompiler>> create() {
        auto jit = orc::LLJITBuilder().create();
        orc::MangleAndInterner mangler((*jit)->getExecutionSession(), (*jit)->getDataLayout());
    
        if (!jit)
            return jit.takeError();
        
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

    // int addString(std::string input) {
    //     orc::ThreadSafeContext tsc(std::make_unique<LLVMContext>());
    //     SMDiagnostic err;
    //     auto m = parseAssemblyString(INP, err, *tsc.getContext());
    //     if (!m) {
    //         std::cout << err.getMessage().str() << std::endl;
    //         return -1;
    //     }
        
    //     orc::ThreadSafeModule tsm(std::move(m), std::move(tsc));

    //     if (auto err = this->jit->addIRModule(std::move(tsm))) {
    //         errs() << err;
    //         return 1;
    //     }
    //     return 0;
    // }

    void* getSym(char* name) {
        auto res = jit->lookup(name);
        if (auto e = res.takeError()) {
            return NULL;
        } else {
            return (void*) res->getAddress();
        }
    }

    void runFunction(char* name, struct union_t args) {
        ;
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

    auto J = std::move(*lljit::ModuleCompiler::create());

    J->addFile("inputs/struct.ll");
    J->addFile(argv[1]);
    auto* fptr = (struct union_t (*)(struct union_t)) J->getSym(argv[2]);
    if (fptr) {
        struct union_t empty = {NULL, NULL};
        auto res = fptr(empty);
        std::cout << res.ints << std::endl;
        std::cout << res.strs << std::endl;
        std::cout << res.strs[0] << std::endl;
    }
}
//