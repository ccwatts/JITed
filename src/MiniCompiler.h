#pragma once


#include "MiniParser.h"
#include "MiniLexer.h"

#include "antlr4-common.h"
#include "antlr4-runtime.h"

#include "ast/program.h"
#include "ast/expressions.h"
#include "ast/statements.h"
#include "ast/types.h"
#include "ast/visitors.h"

#include "MiniToAstVisitors.h"

#include "tl/values.h"
#include "tl/block.h"
#include "tl/instructions.h"

#include <typeinfo>
#include <map>
#include <limits>

namespace minic {

// #include "tl/values.h"
// #include "tl/block.h"

typedef std::map<std::string, std::pair<std::string, int>> StructInfoTable;
class ExpressionToValueVisitor : public jited::ast::ExpressionVisitor {
private:
    jited::ast::ProgramPtr program;
    BlockPtr current;

    template <typename T>
    antlrcpp::Any proxy_val(std::shared_ptr<T> from);

    BinaryInstruction::Operator convertBop(jited::ast::BinaryExpression::Operator op);
public:
    std::map<std::string, StructInfoTable>* structTable;
    std::map<std::string, std::string>* locals;
    std::map<std::string, std::string>* globals;
    std::map<std::string, jited::ast::FunctionPtr>* functions;

    ExpressionToValueVisitor();
    ExpressionToValueVisitor(BlockPtr current, std::map<std::string, StructInfoTable>* structTable,
                            std::map<std::string, std::string>* locals, std::map<std::string, std::string>* globals,
                            std::map<std::string, jited::ast::FunctionPtr>* functions);

    ValuePtr zext(ValuePtr v);
    ValuePtr trunc(ValuePtr v);
    void setCurrent(BlockPtr newCurrent);
    std::string structName(std::string structType);
    antlrcpp::Any visit(jited::ast::BinaryExpression* expression) override;
    antlrcpp::Any visit(jited::ast::DotExpression* expression) override;
    antlrcpp::Any visit(jited::ast::FalseExpression* expression) override;
    antlrcpp::Any visit(jited::ast::IdentifierExpression* expression) override;
    antlrcpp::Any visit(jited::ast::IntegerExpression* expression) override;
    antlrcpp::Any visit(jited::ast::InvocationExpression* expression) override;
    antlrcpp::Any visit(jited::ast::LvalueDot* lvalue) override;
    antlrcpp::Any visit(jited::ast::LvalueId* lvalue) override;
    antlrcpp::Any visit(jited::ast::NewExpression* expression) override;
    antlrcpp::Any visit(jited::ast::NullExpression* expression) override;
    antlrcpp::Any visit(jited::ast::ReadExpression* expression) override;
    antlrcpp::Any visit(jited::ast::TrueExpression* expression) override;
    antlrcpp::Any visit(jited::ast::UnaryExpression* expression) override;
};

class StatementToBlockVisitor : public jited::ast::ASTVisitor {
private:
    jited::ast::ProgramPtr program;
    jited::ast::Function* currentFunction;
    ExpressionToValueVisitor expVisitor;
    Blocks blocks;
    BlockPtr current, retBlock;
    RegisterPtr returnReg;
    std::string returnType;
    
    std::map<std::string, StructInfoTable> structTable;
    std::map<std::string, std::string> locals;
    std::map<std::string, std::string> globals;
    std::map<std::string, jited::ast::FunctionPtr> functions;

    void syncExpVisitor();
    void setCurrent(BlockPtr newCurrent);
    bool isStruct(jited::ast::TypePtr type);
    void processTypedec(jited::ast::TypeDeclarationPtr typeDec);
    void processGlobals(jited::ast::DeclarationPtr declaration);
    void processFunctionHeader(jited::ast::Function* function);
    void processRetval();
    BlockPtr newBlock();
public:
    StatementToBlockVisitor(jited::ast::ProgramPtr program);
        
    antlrcpp::Any visit(jited::ast::AssignmentStatement* statement) override;
    antlrcpp::Any visit(jited::ast::BlockStatement* statement) override;
    antlrcpp::Any visit(jited::ast::ConditionalStatement* statement) override;
    antlrcpp::Any visit(jited::ast::Declaration* declaration) override;
    antlrcpp::Any visit(jited::ast::DeleteStatement* statement) override;
    antlrcpp::Any visit(jited::ast::Function* function) override;
    antlrcpp::Any visit(jited::ast::InvocationStatement* statement) override;
    antlrcpp::Any visit(jited::ast::PrintLnStatement* statement) override;
    antlrcpp::Any visit(jited::ast::PrintStatement* statement) override;
    antlrcpp::Any visit(jited::ast::Program* program) override;
    antlrcpp::Any visit(jited::ast::ReturnEmptyStatement* statement) override;
    antlrcpp::Any visit(jited::ast::ReturnStatement* statement) override;
    antlrcpp::Any visit(jited::ast::WhileStatement* statement) override;
};

// int main(int argc, char** argv) {
//     if (argc == 1) {
//         return 0;
//     }
//     antlr4::ANTLRFileStream input(argv[1]);
//     MiniLexer lexer(&input);
//     antlr4::CommonTokenStream tokens(&lexer);
//     MiniParser parser(&tokens);
//     antlr4::tree::ParseTree* tree = parser.program();

//     if (parser.getNumberOfSyntaxErrors() == 0) {
//         MiniToAstProgramVisitor programVisitor;
//         jited::ast::ProgramPtr p = programVisitor.visit(tree);
//         StatementToBlockVisitor c(p);
//         c.run();
//     } else {
//         std::cout << "could not parse input file\n";
//         return 1;
//     }
// }
}