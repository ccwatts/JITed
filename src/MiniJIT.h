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

class DependencyFinder : public jited::ast::ASTVisitor {
private:
    const std::map<std::string, jited::ast::FunctionPtr>* funcs;
    std::set<std::string> dependencies;
public:
    DependencyFinder(const std::map<std::string, jited::ast::FunctionPtr>* funcs);
    std::set<std::string> getDependencies(std::string fname);
    Any visit(ast::AssignmentStatement* statement);
    Any visit(ast::BinaryExpression* expression);
    Any visit(ast::BlockStatement* statement);
    Any visit(ast::ConditionalStatement* statement);
    Any visit(ast::DeleteStatement* statement);
    Any visit(ast::DotExpression* expression);
    Any visit(ast::FalseExpression* expression);
    Any visit(ast::Function* function);
    Any visit(ast::IdentifierExpression* expression);
    Any visit(ast::IntegerExpression* expression);
    Any visit(ast::InvocationExpression* expression);
    Any visit(ast::InvocationStatement* statement);
    Any visit(ast::NewExpression* expression);
    Any visit(ast::NullExpression* expression);
    Any visit(ast::PrintLnStatement* statement);
    Any visit(ast::PrintStatement* statement);
    Any visit(ast::ReadExpression* expression);
    Any visit(ast::ReturnEmptyStatement* statement);
    Any visit(ast::ReturnStatement* statement);
    Any visit(ast::TrueExpression* expression);
    Any visit(ast::UnaryExpression* expression);
    Any visit(ast::WhileStatement* statement);
};

class JIT : public jited::MiniInterpreter {
protected:
    std::unique_ptr<ModuleCompiler> mc;
    std::unique_ptr<DependencyFinder> df;
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
