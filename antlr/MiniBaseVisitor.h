
   /* package declaration here */


// Generated from Mini.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "MiniVisitor.h"


/**
 * This class provides an empty implementation of MiniVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  MiniBaseVisitor : public MiniVisitor {
public:

  virtual antlrcpp::Any visitProgram(MiniParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypes(MiniParser::TypesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeDeclaration(MiniParser::TypeDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNestedDecl(MiniParser::NestedDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDecl(MiniParser::DeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntType(MiniParser::IntTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBoolType(MiniParser::BoolTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStructType(MiniParser::StructTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeclarations(MiniParser::DeclarationsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeclaration(MiniParser::DeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunctions(MiniParser::FunctionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunction(MiniParser::FunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParameters(MiniParser::ParametersContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnTypeReal(MiniParser::ReturnTypeRealContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnTypeVoid(MiniParser::ReturnTypeVoidContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNestedBlock(MiniParser::NestedBlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignment(MiniParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrint(MiniParser::PrintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrintLn(MiniParser::PrintLnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConditional(MiniParser::ConditionalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhile(MiniParser::WhileContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDelete(MiniParser::DeleteContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturn(MiniParser::ReturnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInvocation(MiniParser::InvocationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlock(MiniParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatementList(MiniParser::StatementListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLvalueId(MiniParser::LvalueIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLvalueDot(MiniParser::LvalueDotContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntegerExpr(MiniParser::IntegerExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTrueExpr(MiniParser::TrueExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIdentifierExpr(MiniParser::IdentifierExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBinaryExpr(MiniParser::BinaryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNewExpr(MiniParser::NewExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNestedExpr(MiniParser::NestedExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDotExpr(MiniParser::DotExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnaryExpr(MiniParser::UnaryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInvocationExpr(MiniParser::InvocationExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFalseExpr(MiniParser::FalseExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNullExpr(MiniParser::NullExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitArguments(MiniParser::ArgumentsContext *ctx) override {
    return visitChildren(ctx);
  }


};

