#include "expressions.h"
#include "program.h"
#include "statements.h"
#include <stdexcept>

#include "antlr4-runtime.h"
using antlrcpp::Any;

namespace jited {
namespace ast {

Any ASTVisitor::visit(AssignmentStatement* statement) {
    throw std::runtime_error("Error: Visit function not implemented. (AssignStatement)");
}

Any ASTVisitor::visit(BinaryExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (BinaryExpression)");
}

Any ASTVisitor::visit(BlockStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (BlockStatement) ");
}

Any ASTVisitor::visit(BoolType* type){
    throw std::runtime_error("Error: Visit function not implemented. (BoolType)");
}

Any ASTVisitor::visit(ConditionalStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (ConditionalStatement)");
}

Any ASTVisitor::visit(Declaration* declaration){
    throw std::runtime_error("Error: Visit function not implemented. (Declaration)");
}

Any ASTVisitor::visit(DeleteStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (DeleteStatement)");
}

Any ASTVisitor::visit(DotExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (DotExpression)");
}

Any ASTVisitor::visit(FalseExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (FalseExpression)");
}

Any ASTVisitor::visit(Function* function){
    throw std::runtime_error("Error: Visit function not implemented. (Function)");
}

Any ASTVisitor::visit(IdentifierExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (IdentifierExpression)");
}

Any ASTVisitor::visit(IntegerExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (IntegerExpression)");
}

Any ASTVisitor::visit(IntType* type){
    throw std::runtime_error("Error: Visit function not implemented. (IntType)");
}

Any ASTVisitor::visit(InvocationExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (InvocationExpression)");
}

Any ASTVisitor::visit(InvocationStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (InvocationStatement)");
}

Any ASTVisitor::visit(LvalueDot* lvalue){
    throw std::runtime_error("Error: Visit function not implemented. (LvalueDot)");
}

Any ASTVisitor::visit(LvalueId* lvalue){
    throw std::runtime_error("Error: Visit function not implemented. (LvalueId)");
}

Any ASTVisitor::visit(NewExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (NewExpression)");
}

Any ASTVisitor::visit(NullExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (NullExpression)");
}

Any ASTVisitor::visit(PrintLnStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (PrintLnStatement)");
}

Any ASTVisitor::visit(PrintStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (PrintStatement)");
}

Any ASTVisitor::visit(Program* program){
    throw std::runtime_error("Error: Visit function not implemented. (Program)");
}

Any ASTVisitor::visit(ReadExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (ReadExpression)");
}

Any ASTVisitor::visit(ReturnEmptyStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (ReturnEmptyStatement)");
}

Any ASTVisitor::visit(ReturnStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (ReturnStatement)");
}

Any ASTVisitor::visit(StructType* type){
    throw std::runtime_error("Error: Visit function not implemented. (StructType)");
}

Any ASTVisitor::visit(TrueExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (TrueExpression)");
}

Any ASTVisitor::visit(TypeDeclaration* typeDeclaration){
    throw std::runtime_error("Error: Visit function not implemented. (TypeDeclaration)");
}

Any ASTVisitor::visit(UnaryExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (UnaryExpression)");
}

Any ASTVisitor::visit(VoidType* type){
    throw std::runtime_error("Error: Visit function not implemented. (VoidType)");
}

Any ASTVisitor::visit(WhileStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (WhileStatement)");
}





Any ExpressionVisitor::visit(BinaryExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (BinaryExpression)");
}

Any ExpressionVisitor::visit(DotExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (DotExpression)");
}

Any ExpressionVisitor::visit(FalseExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (FalseExpression)");
}

Any ExpressionVisitor::visit(IdentifierExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (IdentifierExpression)");
}

Any ExpressionVisitor::visit(IntegerExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (IntegerExpression)");
}

Any ExpressionVisitor::visit(InvocationExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (InvocationExpression)");
}

Any ExpressionVisitor::visit(LvalueDot* lvalue) {
    throw std::runtime_error("Error: Visit function not implemented. (LvalueDot)");
}

Any ExpressionVisitor::visit(LvalueId* lvalue){
    throw std::runtime_error("Error: Visit function not implemented. (LvalueId)");
}

Any ExpressionVisitor::visit(NewExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (NewExpression)");
}

Any ExpressionVisitor::visit(NullExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (NullExpression)");
}

Any ExpressionVisitor::visit(ReadExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (ReadExpression)");
}

Any ExpressionVisitor::visit(TrueExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (TrueExpression)");
}

Any ExpressionVisitor::visit(UnaryExpression* expression){
    throw std::runtime_error("Error: Visit function not implemented. (UnaryExpression)");
}





Any StatementVisitor::visit(AssignmentStatement* statement) {
    throw std::runtime_error("Error: Visit function not implemented. (AssignmentStatement)");
}

Any StatementVisitor::visit(BlockStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (BlockStatement)");
}

Any StatementVisitor::visit(ConditionalStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (ConditionalStatement)");
}

Any StatementVisitor::visit(Declaration* declaration){
    throw std::runtime_error("Error: Visit function not implemented. (Declaration)");
}

Any StatementVisitor::visit(DeleteStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (DeleteStatement)");
}

Any StatementVisitor::visit(Function* function){
    throw std::runtime_error("Error: Visit function not implemented. (Function)");
}

Any StatementVisitor::visit(InvocationStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (InvocationStatement)");
}

Any StatementVisitor::visit(LvalueDot* lvalue){
    throw std::runtime_error("Error: Visit function not implemented. (LvalueDot)");
}

Any StatementVisitor::visit(LvalueId* lvalue){
    throw std::runtime_error("Error: Visit function not implemented. (LvalueId)");
}

Any StatementVisitor::visit(PrintLnStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (PrintLnStatement)");
}

Any StatementVisitor::visit(PrintStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (PrintStatement)");
}

Any StatementVisitor::visit(Program* program){
    throw std::runtime_error("Error: Visit function not implemented. (Program)");
}

Any StatementVisitor::visit(ReturnEmptyStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (ReturnEmptyStatement)");
}

Any StatementVisitor::visit(ReturnStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (ReturnStatement)");
}

Any StatementVisitor::visit(WhileStatement* statement){
    throw std::runtime_error("Error: Visit function not implemented. (WhileStatement)");
}

}
}