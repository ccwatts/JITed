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
class ExpressionToValueVisitor : public ast::ExpressionVisitor {
private:
    ast::ProgramPtr program;
    BlockPtr current;

    template <typename T>
    antlrcpp::Any proxy_val(std::shared_ptr<T> from);

    BinaryInstruction::Operator convertBop(ast::BinaryExpression::Operator op);
public:
    std::map<std::string, StructInfoTable>* structTable;
    std::map<std::string, std::string>* locals;
    std::map<std::string, std::string>* globals;
    std::map<std::string, ast::FunctionPtr>* functions;

    ExpressionToValueVisitor();
    ExpressionToValueVisitor(BlockPtr current, std::map<std::string, StructInfoTable>* structTable,
                            std::map<std::string, std::string>* locals, std::map<std::string, std::string>* globals,
                            std::map<std::string, ast::FunctionPtr>* functions);

    ValuePtr zext(ValuePtr v);
    ValuePtr trunc(ValuePtr v);
    void setCurrent(BlockPtr newCurrent);
    std::string structName(std::string structType);
    antlrcpp::Any visit(ast::BinaryExpression* expression) override;
    antlrcpp::Any visit(ast::DotExpression* expression) override;
    antlrcpp::Any visit(ast::FalseExpression* expression) override;
    antlrcpp::Any visit(ast::IdentifierExpression* expression) override;
    antlrcpp::Any visit(ast::IntegerExpression* expression) override;
    antlrcpp::Any visit(ast::InvocationExpression* expression) override;
    antlrcpp::Any visit(ast::LvalueDot* lvalue) override;
    antlrcpp::Any visit(ast::LvalueId* lvalue) override;
    antlrcpp::Any visit(ast::NewExpression* expression) override;
    antlrcpp::Any visit(ast::NullExpression* expression) override;
    antlrcpp::Any visit(ast::ReadExpression* expression) override;
    antlrcpp::Any visit(ast::TrueExpression* expression) override;
    antlrcpp::Any visit(ast::UnaryExpression* expression) override;
};

class StatementToBlockVisitor : public ast::ASTVisitor {
private:
    ast::ProgramPtr program;
    ast::Function* currentFunction;
    ExpressionToValueVisitor expVisitor;
    Blocks blocks;
    BlockPtr current, retBlock;
    RegisterPtr returnReg;
    std::string returnType;
    
    std::map<std::string, StructInfoTable> structTable;
    std::map<std::string, std::string> locals;
    std::map<std::string, std::string> globals;
    std::map<std::string, ast::FunctionPtr> functions;

    void syncExpVisitor();
    void setCurrent(BlockPtr newCurrent);
    bool isStruct(ast::TypePtr type);
    void processTypedec(ast::TypeDeclarationPtr typeDec);
    void processGlobals(ast::DeclarationPtr declaration);
    void processFunctionHeader(ast::Function* function);
    void processRetval();
    BlockPtr newBlock();
public:
    StatementToBlockVisitor(ast::ProgramPtr program);
        
    antlrcpp::Any visit(ast::AssignmentStatement* statement) override;
    antlrcpp::Any visit(ast::BlockStatement* statement) override;
    antlrcpp::Any visit(ast::ConditionalStatement* statement) override;
    antlrcpp::Any visit(ast::Declaration* declaration) override;
    antlrcpp::Any visit(ast::DeleteStatement* statement) override;
    antlrcpp::Any visit(ast::Function* function) override;
    antlrcpp::Any visit(ast::InvocationStatement* statement) override;
    antlrcpp::Any visit(ast::PrintLnStatement* statement) override;
    antlrcpp::Any visit(ast::PrintStatement* statement) override;
    antlrcpp::Any visit(ast::Program* program) override;
    antlrcpp::Any visit(ast::ReturnEmptyStatement* statement) override;
    antlrcpp::Any visit(ast::ReturnStatement* statement) override;
    antlrcpp::Any visit(ast::WhileStatement* statement) override;
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
//         ast::ProgramPtr p = programVisitor.visit(tree);
//         StatementToBlockVisitor c(p);
//         c.run();
//     } else {
//         std::cout << "could not parse input file\n";
//         return 1;
//     }
// }
}