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
#include <string>
#include <map>
#include <iostream>
// #include <functional>

namespace jited {

extern "C" void printInt(int x);
extern "C" void printIntEndl(int x);
extern "C" int readInt();

bool defaultHeatFunction(std::string called, std::string lastCalled, std::map<std::string, int> callCounts);

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

class JIT : public jited::MiniInterpreter {
protected:
    std::unique_ptr<ModuleCompiler> mc;
    std::shared_ptr<jited::ast::ASTVisitor> compiler;
    std::string lastCalled;
    std::map<std::string, int> callCounts;
    std::function<bool(std::string, std::string, std::map<std::string, int>)> heatFunction;
public:
    JIT(jited::ast::ProgramPtr program, std::shared_ptr<jited::ast::ASTVisitor> compiler);
    ~JIT();
    
    // WE PROBABLY NEED A LOOKUP/EVALUATEDSYMBOL FOR STRUCTS
    antlrcpp::Any visit(jited::ast::InvocationExpression* expression) override;

    jited::PackedStruct* reverseLookup(uint8_t* buf, std::string structName);

    // contains the LLVM for defining necessary structs
    std::string structsString();

    std::string globalString();

    // contains the declarations of external functions required
    // this may need to take a function, as well? need access to all functions
    // that may be possible to fudge using error handlers tho
    std::string declareString(std::string fname);

    std::string moduleString(std::string fname);
    
    std::string entryFunction(std::string name);

    void compileFunction(std::string fname);

    void makeGlobals();

    void setHeatFunction(std::function<bool(std::string, std::string, std::map<std::string, int>)> newHeatFn);

    static void initialize();
};

}
