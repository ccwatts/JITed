#include <iostream>
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "inputs/generic.h"
using namespace llvm;

class ModuleCompiler {
private:
public:
    std::unique_ptr<orc::LLJIT> jit;
    orc::MangleAndInterner mangler;

    ModuleCompiler(std::unique_ptr<orc::LLJIT> jit, orc::MangleAndInterner mangler) : jit(std::move(jit)), mangler(mangler) {
        loadCommon();
    };

    static Expected<std::unique_ptr<ModuleCompiler>> create() {
        auto jit = orc::LLJITBuilder().create();
        orc::MangleAndInterner mangler((*jit)->getExecutionSession(), (*jit)->getDataLayout());
    
        if (!jit)
            return jit.takeError();

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

        jit->getMainJITDylib().addGenerator(std::move(DG.get()));
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

    /*
    int fromString(std::string ir) {

        return 0;
    }
    */

    void* getSym(char* name) {
        auto sym = jit->lookup(name);
        if (sym) {
            return (void*) sym->getAddress();
        } else {
            return NULL;
        }
    }
};

// parse assembly string...?

int main(int argc, char** argv) {
    if (argc < 3) return 1;
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmPrinters();

    std::unique_ptr<ModuleCompiler> J = std::move(*ModuleCompiler::create());

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