#pragma once

#include "MiniBaseVisitor.h"

#include "antlr4-common.h"

#include "ast/program.h"
#include "ast/expressions.h"
#include "ast/statements.h"
#include "ast/types.h"

class MiniToAstTypeVisitor : public MiniBaseVisitor {
protected:
    antlrcpp::Any defaultResult() override;
public:
    MiniToAstTypeVisitor();

    antlrcpp::Any visitIntType(MiniParser::IntTypeContext* ctx) override;
    antlrcpp::Any visitBoolType(MiniParser::BoolTypeContext* ctx) override;
    antlrcpp::Any visitStructType(MiniParser::StructTypeContext* ctx) override;
    antlrcpp::Any visitReturnTypeReal(MiniParser::ReturnTypeRealContext* ctx) override;
    antlrcpp::Any visitReturnTypeVoid(MiniParser::ReturnTypeVoidContext* ctx) override;
};

class MiniToAstExpressionVisitor : public MiniBaseVisitor {
protected:
    antlrcpp::Any defaultResult() override;
public:
    MiniToAstExpressionVisitor();

    antlrcpp::Any visitIntegerExpr(MiniParser::IntegerExprContext* ctx) override;
    antlrcpp::Any visitTrueExpr(MiniParser::TrueExprContext* ctx) override;
    antlrcpp::Any visitIdentifierExpr(MiniParser::IdentifierExprContext* ctx) override;
    antlrcpp::Any visitBinaryExpr(MiniParser::BinaryExprContext* ctx) override;
    antlrcpp::Any visitNewExpr(MiniParser::NewExprContext* ctx) override;
    antlrcpp::Any visitDotExpr(MiniParser::DotExprContext* ctx) override;
    antlrcpp::Any visitFalseExpr(MiniParser::FalseExprContext* ctx) override;
    antlrcpp::Any visitUnaryExpr(MiniParser::UnaryExprContext* ctx) override;
    std::vector<ast::ExpressionPtr> gatherArguments(MiniParser::ArgumentsContext* ctx);
    antlrcpp::Any visitInvocationExpr(MiniParser::InvocationExprContext* ctx) override;
    antlrcpp::Any visitNestedExpr(MiniParser::NestedExprContext* ctx) override;
};

class MiniToAstStatementVisitor : public MiniBaseVisitor {
private:
    MiniToAstExpressionVisitor expressionVisitor;

    ast::ExpressionPtr visitLvalueNested(MiniParser::LvalueContext* ctx);
    ast::LvaluePtr visitLvalue(MiniParser::LvalueContext* ctx);
    std::vector<ast::ExpressionPtr> gatherArguments(MiniParser::ArgumentsContext* ctx);
protected:
    antlrcpp::Any defaultResult() override;
public:
    MiniToAstStatementVisitor();
    antlrcpp::Any visitNestedBlock(MiniParser::NestedBlockContext* ctx) override;
    antlrcpp::Any visitAssignment(MiniParser::AssignmentContext* ctx) override;
    antlrcpp::Any visitPrint(MiniParser::PrintContext* ctx) override;
    antlrcpp::Any visitPrintLn(MiniParser::PrintLnContext* ctx) override;
    antlrcpp::Any visitConditional(MiniParser::ConditionalContext* ctx) override;
    antlrcpp::Any visitWhile(MiniParser::WhileContext* ctx) override;
    antlrcpp::Any visitDelete(MiniParser::DeleteContext* ctx) override;
    antlrcpp::Any visitReturn(MiniParser::ReturnContext* ctx) override;
    antlrcpp::Any visitInvocation(MiniParser::InvocationContext* ctx) override;
    antlrcpp::Any visitStatementList(MiniParser::StatementListContext* ctx) override;
    antlrcpp::Any visitBlock(MiniParser::BlockContext* ctx);
};

class MiniToAstDeclarationsVisitor : public MiniBaseVisitor {
private:
    MiniToAstTypeVisitor typeVisitor;
    void addDeclarationsTo(MiniParser::DeclarationContext* ctx, std::vector<ast::DeclarationPtr>& decls);
protected:
    antlrcpp::Any defaultResult() override;
public:
    antlrcpp::Any visitDeclarations(MiniParser::DeclarationsContext* ctx) override;
};

class MiniToAstFunctionVisitor : public MiniBaseVisitor {
private:
    MiniToAstTypeVisitor typeVisitor;
    MiniToAstDeclarationsVisitor declarationsVisitor;
    MiniToAstStatementVisitor statementVisitor;
    std::vector<ast::DeclarationPtr> gatherParameters(MiniParser::ParametersContext* ctx);
protected:
    antlrcpp::Any defaultResult() override;
public:
    MiniToAstFunctionVisitor();

    antlrcpp::Any visitFunction(MiniParser::FunctionContext* ctx);
};

class MiniToAstTypeDeclarationVisitor : public MiniBaseVisitor {
private:
    MiniToAstTypeVisitor typeVisitor;
    std::vector<ast::DeclarationPtr> gatherFieldDeclarations(MiniParser::NestedDeclContext* ctx);
protected:
    antlrcpp::Any defaultResult() override;
public:
    MiniToAstTypeDeclarationVisitor();

    antlrcpp::Any visitTypeDeclaration(MiniParser::TypeDeclarationContext* ctx) override;
};


class MiniToAstProgramVisitor : public MiniBaseVisitor {
private:
    MiniToAstTypeDeclarationVisitor typeDeclarationVisitor;
    MiniToAstDeclarationsVisitor declarationsVisitor;
    MiniToAstFunctionVisitor functionVisitor;
private:
    std::vector<ast::TypeDeclarationPtr> gatherTypes(MiniParser::TypesContext* ctx);
    std::vector<ast::DeclarationPtr> gatherDeclarations(MiniParser::DeclarationsContext* ctx);
    std::vector<ast::FunctionPtr> gatherFunctions(MiniParser::FunctionsContext* ctx);
protected:
    antlrcpp::Any defaultResult();
public:
    MiniToAstProgramVisitor();

    antlrcpp::Any visitProgram(MiniParser::ProgramContext* ctx) override;
};
