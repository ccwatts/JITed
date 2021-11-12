#pragma

#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
// #include "LLVMContextImpl.h"
#include "llvm/IR/DerivedTypes.h"

#include "MiniCompiler.h"
#include "MiniFrontend.h"
#include "MiniInterpreter.h"

#include <cstdint>
#include <iostream>

namespace jited {

extern "C" void printInt(int x);
extern "C" void printIntEndl(int x);
extern "C" int readInt();

class ModuleCompiler {
private:
public:
    static void dontLogErrors(llvm::Error Err);

    std::unique_ptr<llvm::orc::LLJIT> jit;
    llvm::orc::MangleAndInterner mangler;

    ModuleCompiler(std::unique_ptr<llvm::orc::LLJIT> jit, llvm::orc::MangleAndInterner mangler);

    static std::unique_ptr<ModuleCompiler> create();

    void loadCommon();
    int addFile(char* fname);
    int addString(std::string input);
    void* getSym(std::string name);
};

class JIT : public mini::MiniInterpreter {
protected:
    std::unique_ptr<ModuleCompiler> mc;
    std::shared_ptr<ast::ASTVisitor> compiler;
public:
    JIT(ast::ProgramPtr program, std::shared_ptr<ast::ASTVisitor> compiler);
    
    // WE PROBABLY NEED A LOOKUP/EVALUATEDSYMBOL FOR STRUCTS
    antlrcpp::Any visit(ast::InvocationExpression* expression) override;

    mini::PackedStruct* reverseLookup(uint8_t* buf, std::string structName);

    // contains the LLVM for defining necessary structs
    std::string structsString();

    std::string globalString();

    // contains the declarations of external functions required
    // this may need to take a function, as well? need access to all functions
    // that may be possible to fudge using error handlers tho
    std::string declareString(std::string fname);

    std::string moduleString(std::string fname);
    
    std::string functionPrefix(std::string name);

    void compileFunction(std::string fname);

    void makeGlobals();

    static void initialize();
};

}
// parse assembly string...?

// int main(int argc, char** argv) {
//     // if (argc < 3) return 1;
//     InitializeAllTargetInfos();
//     InitializeAllTargets();
//     InitializeAllTargetMCs();
//     InitializeAllAsmPrinters();

//     if (argc == 1) {
//         return 0;
//     }

//     mini::MiniFrontend fe;
//     ast::ProgramPtr p = fe.parseFile(argv[1]);
//     if (!p) {
//         return 1;
//     }
//     std::shared_ptr<ast::ASTVisitor> compiler = std::make_shared<minic::StatementToBlockVisitor>(p);
//     jited::JIT j(p, compiler);
//     return j.run();
// }


