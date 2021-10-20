#include <iostream>
#include <llvm/IR/Module.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/AsmParser/Parser.h>
#include <llvm/IR/LegacyPassManager.h>
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

int main(int argc, char** argv) {
    if (argc < 2) return 1;
    auto ttriple = sys::getDefaultTargetTriple();
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    std::string error;
    auto target = TargetRegistry::lookupTarget(ttriple, error);

    if (!target) {
        errs() << error;
        return 1;
    }

    auto cpu = "generic";
    auto features = "";
    TargetOptions opt;
    auto rm = Optional<Reloc::Model>();
    auto target_machine = target->createTargetMachine(ttriple, cpu, features, opt, rm);

    LLVMContext ctx;
    SMDiagnostic err;
    auto m = parseAssemblyFile(argv[1], err, ctx);
    if (m) {
        std::cout << "loaded LL as module\n";
    } else {
        std::cout << err.getMessage().str() << std::endl;
        return 1;
    }

    m->setDataLayout(target_machine->createDataLayout());
    m->setTargetTriple(ttriple);

    auto filename = "out.o";
    std::error_code EC;
    raw_fd_ostream dest(filename, EC, sys::fs::F_None);

    legacy::PassManager pass;
    auto file_type = LLVMTargetMachine::CGFT_ObjectFile;
    
    if (target_machine->addPassesToEmitFile(pass, dest, file_type)) {
        errs() << "can't emit file of given type";
        return 1;
    }
    pass.run(*m);
    dest.flush();
}