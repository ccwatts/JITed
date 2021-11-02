#pragma once

#include <string>
#include <vector>
#include <memory>
#include "types.h"
#include "visitors.h"
#include "expressions.h"

#include "antlr4-runtime.h"
using antlrcpp::Any;

namespace ast {
    
class ASTVisitor;
class ExpressionVisitor;
class StatementVisitor;

class Statement {
public:
    Statement(int line) : line(line) {};

    int line;
    virtual Any accept(ASTVisitor* v) {
        throw std::runtime_error("Error: Visit function not implemented.");
    }
    virtual Any accept(StatementVisitor* v) {
        throw std::runtime_error("Error: Visit function not implemented.");
    }
};
typedef std::shared_ptr<Statement> StatementPtr;


// unsure if these need to be Statements for Any to not blow up
class Declaration {
public:
    int line;
    TypePtr type;
    std::string name;

    Declaration(int line, TypePtr type, std::string name) : line(line), type(type), name(name) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<Declaration> DeclarationPtr;


// this too
class Function {
public:
    int line;
    std::string name;
    TypePtr retType;
    std::vector<DeclarationPtr> params, locals;
    StatementPtr body;

    Function(int line, std::string name, std::vector<DeclarationPtr> params, TypePtr retType, std::vector<DeclarationPtr> locals, StatementPtr body) :
        line(line), name(name), retType(retType), params(params), locals(locals), body(body) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<Function> FunctionPtr;



class TypeDeclaration {
public:
    int line;
    std::string name;
    std::vector<DeclarationPtr> fields;

    // TypeDeclaration(int line, std::string name, std::vector<Declaration> fields);
    TypeDeclaration(int line, std::string name, std::vector<DeclarationPtr> fields) : line(line), name(name), fields(fields) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<TypeDeclaration> TypeDeclarationPtr;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class AssignmentStatement : public Statement {
public:
    LvaluePtr target;
    ExpressionPtr source;

    AssignmentStatement(int line, LvaluePtr target, ExpressionPtr source) : Statement(line), target(target), source(source) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<AssignmentStatement> AssignmentStatementPtr;



class BlockStatement : public Statement {
public:
    std::vector<StatementPtr> statements;
    BlockStatement(int line, std::vector<StatementPtr> statements) : Statement(line), statements(statements) {};

    static std::shared_ptr<BlockStatement> emptyBlock() {
        return std::make_shared<BlockStatement>(-1, std::vector<StatementPtr>());
    }
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<BlockStatement> BlockStatementPtr;



class ConditionalStatement : public Statement {
public:
    ExpressionPtr guard;
    StatementPtr thenBlock, elseBlock;

    ConditionalStatement(int line, ExpressionPtr guard, StatementPtr thenBlock, StatementPtr elseBlock) :
        Statement(line), guard(guard), thenBlock(thenBlock), elseBlock(elseBlock) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<ConditionalStatement> ConditionalStatementPtr;



class DeleteStatement : public Statement {
public:
    ExpressionPtr expression;

    DeleteStatement(int line, ExpressionPtr expression) : Statement(line), expression(expression) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<DeleteStatement> DeleteStatementPtr;



class InvocationStatement : public Statement {
public:
    ExpressionPtr expression;

    InvocationStatement(int line, ExpressionPtr expression) : Statement(line), expression(expression) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<InvocationStatement> InvocationStatementPtr;



class PrintLnStatement : public Statement {
public:
    ExpressionPtr expression;

    PrintLnStatement(int line, ExpressionPtr expression) : Statement(line), expression(expression) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<PrintLnStatement> PrintLnStatementPtr;



class PrintStatement : public Statement {
public:
    ExpressionPtr expression;

    PrintStatement(int line, ExpressionPtr expression) : Statement(line), expression(expression) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<PrintStatement> PrintStatementPtr;



class ReturnEmptyStatement : public Statement {
public:
    ReturnEmptyStatement(int line) : Statement(line) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<ReturnEmptyStatement> ReturnEmptyStatementPtr;


#include <iostream>
class ReturnStatement : public Statement {
public:
    ExpressionPtr expression;

    ReturnStatement(int line, ExpressionPtr expression) : Statement(line), expression(expression) {};
    ~ReturnStatement() {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<ReturnStatement> ReturnStatementPtr;



class WhileStatement : public Statement {
public:
    ExpressionPtr guard;
    StatementPtr body;

    WhileStatement(int line, ExpressionPtr guard, StatementPtr body) : Statement(line), guard(guard), body(body) {};
    
    Any accept(ASTVisitor* v) { return v->visit(this); }
    Any accept(StatementVisitor* v) { return v->visit(this); }
};
typedef std::shared_ptr<WhileStatement> WhileStatementPtr;

}