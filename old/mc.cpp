#include <iostream>
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"

using namespace llvm;

int main(int argc, char** argv) {
    if (argc < 3) return 1;
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmPrinters();

    // LLVMContext ctx;
    orc::ThreadSafeContext tsc(std::make_unique<LLVMContext>());
    SMDiagnostic err;
    auto m = parseIRFile(argv[1], err, *tsc.getContext());
    if (!m) {
        std::cout << err.getMessage().str() << std::endl;
        return 1;
    }

    auto JIT = orc::LLJITBuilder().create();

    if (!JIT) {
        std::cerr << "could not initialize JIT";
        return -1;
    }
    
    orc::ThreadSafeModule tsm(std::move(m), std::move(tsc));
    // auto DG = orc::DynamicLibrarySearchGenerator::Load("/lib/x86_64-linux-gnu/libc.so.6",(*JIT)->getDataLayout().getGlobalPrefix());

    orc::MangleAndInterner Mangle((*JIT)->getExecutionSession(), (*JIT)->getDataLayout());

    DenseSet<orc::SymbolStringPtr> AllowList({
        Mangle("printf")
    });

    auto DG = orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
        (*JIT)->getDataLayout().getGlobalPrefix(),
        [&](const orc::SymbolStringPtr &S) {return AllowList.count(S);}
    );

    (*JIT)->getMainJITDylib().addGenerator(std::move(DG.get()));
    if (auto Err = (*JIT)->addIRModule(std::move(tsm))) {
        errs() << Err;
        return 1;
    }
    
    auto sym = (*JIT)->lookup(argv[2]);
    if (sym) {
        auto* fptr = (int(*)()) sym->getAddress();
        std::cout << fptr() << std::endl;
    } else {
        std::cerr << "could not load symbol " << argv[2] << std::endl;
    }
}
