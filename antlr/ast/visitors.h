#pragma once

#include <stdexcept>

#include "antlr4-runtime.h"
using antlrcpp::Any;

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
    virtual Any visit(AssignmentStatement* statement) {
        throw std::runtime_error("Error: Visit function not implemented. (AssignStatement)");
    }

    virtual Any visit(BinaryExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (BinaryExpression)");
    }

    virtual Any visit(BlockStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (BlockStatement) ");
    }
    
    virtual Any visit(BoolType* type){
        throw std::runtime_error("Error: Visit function not implemented. (BoolType)");
    }

    virtual Any visit(ConditionalStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (ConditionalStatement)");
    }
    
    virtual Any visit(Declaration* declaration){
        throw std::runtime_error("Error: Visit function not implemented. (Declaration)");
    }
    
    virtual Any visit(DeleteStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (DeleteStatement)");
    }
    
    virtual Any visit(DotExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (DotExpression)");
    }
    
    virtual Any visit(FalseExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (FalseExpression)");
    }
    
    virtual Any visit(Function* function){
        throw std::runtime_error("Error: Visit function not implemented. (Function)");
    }
    
    virtual Any visit(IdentifierExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (IdentifierExpression)");
    }
    
    virtual Any visit(IntegerExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (IntegerExpression)");
    }
    
    virtual Any visit(IntType* type){
        throw std::runtime_error("Error: Visit function not implemented. (IntType)");
    }
    
    virtual Any visit(InvocationExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (InvocationExpression)");
    }
    
    virtual Any visit(InvocationStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (InvocationStatement)");
    }
    
    virtual Any visit(LvalueDot* lvalue){
        throw std::runtime_error("Error: Visit function not implemented. (LvalueDot)");
    }

    virtual Any visit(LvalueId* lvalue){
        throw std::runtime_error("Error: Visit function not implemented. (LvalueId)");
    }
    
    virtual Any visit(NewExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (NewExpression)");
    }
    
    virtual Any visit(NullExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (NullExpression)");
    }
    
    virtual Any visit(PrintLnStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (PrintLnStatement)");
    }
    
    virtual Any visit(PrintStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (PrintStatement)");
    }
    
    virtual Any visit(Program* program){
        throw std::runtime_error("Error: Visit function not implemented. (Program)");
    }
    
    virtual Any visit(ReadExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (ReadExpression)");
    }

    virtual Any visit(ReturnEmptyStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (ReturnEmptyStatement)");
    }

    virtual Any visit(ReturnStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (ReturnStatement)");
    }
    
    virtual Any visit(StructType* type){
        throw std::runtime_error("Error: Visit function not implemented. (StructType)");
    }
    
    virtual Any visit(TrueExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (TrueExpression)");
    }
    
    virtual Any visit(TypeDeclaration* typeDeclaration){
        throw std::runtime_error("Error: Visit function not implemented. (TypeDeclaration)");
    }
    
    virtual Any visit(UnaryExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (UnaryExpression)");
    }
    
    virtual Any visit(VoidType* type){
        throw std::runtime_error("Error: Visit function not implemented. (VoidType)");
    }

    virtual Any visit(WhileStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (WhileStatement)");
    }
};

class ExpressionVisitor {
public:
    virtual Any visit(BinaryExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (BinaryExpression)");
    }
    
    virtual Any visit(DotExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (DotExpression)");
    }
    
    virtual Any visit(FalseExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (FalseExpression)");
    }
    
    virtual Any visit(IdentifierExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (IdentifierExpression)");
    }
    
    virtual Any visit(IntegerExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (IntegerExpression)");
    }
    
    virtual Any visit(InvocationExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (InvocationExpression)");
    }
    
    virtual Any visit(LvalueDot* lvalue) {
        throw std::runtime_error("Error: Visit function not implemented. (LvalueDot)");
    }

    virtual Any visit(LvalueId* lvalue){
        throw std::runtime_error("Error: Visit function not implemented. (LvalueId)");
    }
    
    virtual Any visit(NewExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (NewExpression)");
    }
    
    virtual Any visit(NullExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (NullExpression)");
    }
    
    virtual Any visit(ReadExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (ReadExpression)");
    }
    
    virtual Any visit(TrueExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (TrueExpression)");
    }
    
    virtual Any visit(UnaryExpression* expression){
        throw std::runtime_error("Error: Visit function not implemented. (UnaryExpression)");
    }
};

class StatementVisitor {
public:
    virtual Any visit(AssignmentStatement* statement) {
        throw std::runtime_error("Error: Visit function not implemented. (AssignmentStatement)");
    }

    virtual Any visit(BlockStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (BlockStatement)");
    }

    virtual Any visit(ConditionalStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (ConditionalStatement)");
    }
    
    virtual Any visit(Declaration* declaration){
        throw std::runtime_error("Error: Visit function not implemented. (Declaration)");
    }
    
    virtual Any visit(DeleteStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (DeleteStatement)");
    }
    
    virtual Any visit(Function* function){
        throw std::runtime_error("Error: Visit function not implemented. (Function)");
    }
    
    virtual Any visit(InvocationStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (InvocationStatement)");
    }
    
    virtual Any visit(LvalueDot* lvalue){
        throw std::runtime_error("Error: Visit function not implemented. (LvalueDot)");
    }

    virtual Any visit(LvalueId* lvalue){
        throw std::runtime_error("Error: Visit function not implemented. (LvalueId)");
    }

    virtual Any visit(PrintLnStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (PrintLnStatement)");
    }
    
    virtual Any visit(PrintStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (PrintStatement)");
    }
    
    virtual Any visit(Program* program){
        throw std::runtime_error("Error: Visit function not implemented. (Program)");
    }

    virtual Any visit(ReturnEmptyStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (ReturnEmptyStatement)");
    }

    virtual Any visit(ReturnStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (ReturnStatement)");
    }
    
    virtual Any visit(WhileStatement* statement){
        throw std::runtime_error("Error: Visit function not implemented. (WhileStatement)");
    }
};

}