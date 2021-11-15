#pragma once

#include <stdexcept>

#include "antlr4-runtime.h"
using antlrcpp::Any;

namespace jited {
namespace ast {

class AssignmentStatement;
class BinaryExpression;
class BlockStatement;
class BoolType;
class ConditionalStatement;
class Declaration;
class DeleteStatement;
class DotExpression;
class FalseExpression;
class Function;
class IdentifierExpression;
class IntegerExpression;
class IntType;
class InvocationExpression;
class InvocationStatement;
class LvalueDot;
class LvalueId;
class NewExpression;
class NullExpression;
class PrintLnStatement;
class PrintStatement;
class Program;
class ReadExpression;
class ReturnEmptyStatement;
class ReturnStatement;
class StructType;
class TrueExpression;
class TypeDeclaration;
class UnaryExpression;
class VoidType;
class WhileStatement;

class ASTVisitor {
public:
    virtual Any visit(AssignmentStatement* statement);
    virtual Any visit(BinaryExpression* expression);
    virtual Any visit(BlockStatement* statement);
    virtual Any visit(BoolType* type);
    virtual Any visit(ConditionalStatement* statement);
    virtual Any visit(Declaration* declaration);
    virtual Any visit(DeleteStatement* statement);
    virtual Any visit(DotExpression* expression);
    virtual Any visit(FalseExpression* expression);
    virtual Any visit(Function* function);
    virtual Any visit(IdentifierExpression* expression);
    virtual Any visit(IntegerExpression* expression);
    virtual Any visit(IntType* type);
    virtual Any visit(InvocationExpression* expression);
    virtual Any visit(InvocationStatement* statement);
    virtual Any visit(LvalueDot* lvalue);
    virtual Any visit(LvalueId* lvalue);
    virtual Any visit(NewExpression* expression);
    virtual Any visit(NullExpression* expression);
    virtual Any visit(PrintLnStatement* statement);
    virtual Any visit(PrintStatement* statement);
    virtual Any visit(Program* program);
    virtual Any visit(ReadExpression* expression);
    virtual Any visit(ReturnEmptyStatement* statement);
    virtual Any visit(ReturnStatement* statement);
    virtual Any visit(StructType* type);
    virtual Any visit(TrueExpression* expression);
    virtual Any visit(TypeDeclaration* typeDeclaration);
    virtual Any visit(UnaryExpression* expression);
    virtual Any visit(VoidType* type);
    virtual Any visit(WhileStatement* statement);
};

class ExpressionVisitor {
public:
    virtual Any visit(BinaryExpression* expression);
    virtual Any visit(DotExpression* expression);
    virtual Any visit(FalseExpression* expression);
    virtual Any visit(IdentifierExpression* expression);
    virtual Any visit(IntegerExpression* expression);
    virtual Any visit(InvocationExpression* expression);
    virtual Any visit(LvalueDot* lvalue);
    virtual Any visit(LvalueId* lvalue);
    virtual Any visit(NewExpression* expression);
    virtual Any visit(NullExpression* expression);
    virtual Any visit(ReadExpression* expression);
    virtual Any visit(TrueExpression* expression);
    virtual Any visit(UnaryExpression* expression);
};

class StatementVisitor {
public:
    virtual Any visit(AssignmentStatement* statement);
    virtual Any visit(BlockStatement* statement);
    virtual Any visit(ConditionalStatement* statement);
    virtual Any visit(Declaration* declaration);
    virtual Any visit(DeleteStatement* statement);
    virtual Any visit(Function* function);
    virtual Any visit(InvocationStatement* statement);
    virtual Any visit(LvalueDot* lvalue);
    virtual Any visit(LvalueId* lvalue);
    virtual Any visit(PrintLnStatement* statement);
    virtual Any visit(PrintStatement* statement);
    virtual Any visit(Program* program);
    virtual Any visit(ReturnEmptyStatement* statement);
    virtual Any visit(ReturnStatement* statement);
    virtual Any visit(WhileStatement* statement);
};

}
}