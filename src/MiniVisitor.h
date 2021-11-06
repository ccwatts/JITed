
   /* package declaration here */


// Generated from Mini.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "MiniParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by MiniParser.
 */
class  MiniVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by MiniParser.
   */
    virtual antlrcpp::Any visitProgram(MiniParser::ProgramContext *context) = 0;

    virtual antlrcpp::Any visitTypes(MiniParser::TypesContext *context) = 0;

    virtual antlrcpp::Any visitTypeDeclaration(MiniParser::TypeDeclarationContext *context) = 0;

    virtual antlrcpp::Any visitNestedDecl(MiniParser::NestedDeclContext *context) = 0;

    virtual antlrcpp::Any visitDecl(MiniParser::DeclContext *context) = 0;

    virtual antlrcpp::Any visitIntType(MiniParser::IntTypeContext *context) = 0;

    virtual antlrcpp::Any visitBoolType(MiniParser::BoolTypeContext *context) = 0;

    virtual antlrcpp::Any visitStructType(MiniParser::StructTypeContext *context) = 0;

    virtual antlrcpp::Any visitDeclarations(MiniParser::DeclarationsContext *context) = 0;

    virtual antlrcpp::Any visitDeclaration(MiniParser::DeclarationContext *context) = 0;

    virtual antlrcpp::Any visitFunctions(MiniParser::FunctionsContext *context) = 0;

    virtual antlrcpp::Any visitFunction(MiniParser::FunctionContext *context) = 0;

    virtual antlrcpp::Any visitParameters(MiniParser::ParametersContext *context) = 0;

    virtual antlrcpp::Any visitReturnTypeReal(MiniParser::ReturnTypeRealContext *context) = 0;

    virtual antlrcpp::Any visitReturnTypeVoid(MiniParser::ReturnTypeVoidContext *context) = 0;

    virtual antlrcpp::Any visitNestedBlock(MiniParser::NestedBlockContext *context) = 0;

    virtual antlrcpp::Any visitAssignment(MiniParser::AssignmentContext *context) = 0;

    virtual antlrcpp::Any visitPrint(MiniParser::PrintContext *context) = 0;

    virtual antlrcpp::Any visitPrintLn(MiniParser::PrintLnContext *context) = 0;

    virtual antlrcpp::Any visitConditional(MiniParser::ConditionalContext *context) = 0;

    virtual antlrcpp::Any visitWhile(MiniParser::WhileContext *context) = 0;

    virtual antlrcpp::Any visitDelete(MiniParser::DeleteContext *context) = 0;

    virtual antlrcpp::Any visitReturn(MiniParser::ReturnContext *context) = 0;

    virtual antlrcpp::Any visitInvocation(MiniParser::InvocationContext *context) = 0;

    virtual antlrcpp::Any visitBlock(MiniParser::BlockContext *context) = 0;

    virtual antlrcpp::Any visitStatementList(MiniParser::StatementListContext *context) = 0;

    virtual antlrcpp::Any visitLvalueId(MiniParser::LvalueIdContext *context) = 0;

    virtual antlrcpp::Any visitLvalueDot(MiniParser::LvalueDotContext *context) = 0;

    virtual antlrcpp::Any visitIntegerExpr(MiniParser::IntegerExprContext *context) = 0;

    virtual antlrcpp::Any visitTrueExpr(MiniParser::TrueExprContext *context) = 0;

    virtual antlrcpp::Any visitIdentifierExpr(MiniParser::IdentifierExprContext *context) = 0;

    virtual antlrcpp::Any visitBinaryExpr(MiniParser::BinaryExprContext *context) = 0;

    virtual antlrcpp::Any visitNewExpr(MiniParser::NewExprContext *context) = 0;

    virtual antlrcpp::Any visitNestedExpr(MiniParser::NestedExprContext *context) = 0;

    virtual antlrcpp::Any visitDotExpr(MiniParser::DotExprContext *context) = 0;

    virtual antlrcpp::Any visitUnaryExpr(MiniParser::UnaryExprContext *context) = 0;

    virtual antlrcpp::Any visitInvocationExpr(MiniParser::InvocationExprContext *context) = 0;

    virtual antlrcpp::Any visitFalseExpr(MiniParser::FalseExprContext *context) = 0;

    virtual antlrcpp::Any visitNullExpr(MiniParser::NullExprContext *context) = 0;

    virtual antlrcpp::Any visitArguments(MiniParser::ArgumentsContext *context) = 0;


};

