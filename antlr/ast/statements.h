#pragma once

#include <string>
#include <vector>
#include <memory>
#include "expressions.h"
#include "types.h"
#include "visitors.h"

#include "antlr4-runtime.h"
using antlrcpp::Any;

namespace ast {
    
class ASTVisitor;
class ExpressionVisitor;
class StatementVisitor;

class Statement {
public:
    int line;
    Statement(int line);
    virtual Any accept(ASTVisitor* v);
    virtual Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<Statement> StatementPtr;


// unsure if these need to be Statements for Any to not blow up
class Declaration {
public:
    int line;
    TypePtr type;
    std::string name;

    Declaration(int line, TypePtr type, std::string name);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
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

    Function(int line, std::string name, std::vector<DeclarationPtr> params, TypePtr retType, std::vector<DeclarationPtr> locals, StatementPtr body);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<Function> FunctionPtr;



class TypeDeclaration {
public:
    int line;
    std::string name;
    std::vector<DeclarationPtr> fields;

    TypeDeclaration(int line, std::string name, std::vector<DeclarationPtr> fields);
    Any accept(ASTVisitor* v);
};
typedef std::shared_ptr<TypeDeclaration> TypeDeclarationPtr;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class AssignmentStatement : public Statement {
public:
    LvaluePtr target;
    ExpressionPtr source;

    AssignmentStatement(int line, LvaluePtr target, ExpressionPtr source);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<AssignmentStatement> AssignmentStatementPtr;



class BlockStatement : public Statement {
public:
    std::vector<StatementPtr> statements;
    BlockStatement(int line, std::vector<StatementPtr> statements);
    static std::shared_ptr<BlockStatement> emptyBlock();
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<BlockStatement> BlockStatementPtr;



class ConditionalStatement : public Statement {
public:
    ExpressionPtr guard;
    StatementPtr thenBlock, elseBlock;

    ConditionalStatement(int line, ExpressionPtr guard, StatementPtr thenBlock, StatementPtr elseBlock);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<ConditionalStatement> ConditionalStatementPtr;



class DeleteStatement : public Statement {
public:
    ExpressionPtr expression;

    DeleteStatement(int line, ExpressionPtr expression);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<DeleteStatement> DeleteStatementPtr;



class InvocationStatement : public Statement {
public:
    ExpressionPtr expression;

    InvocationStatement(int line, ExpressionPtr expression);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<InvocationStatement> InvocationStatementPtr;



class PrintLnStatement : public Statement {
public:
    ExpressionPtr expression;

    PrintLnStatement(int line, ExpressionPtr expression);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<PrintLnStatement> PrintLnStatementPtr;



class PrintStatement : public Statement {
public:
    ExpressionPtr expression;

    PrintStatement(int line, ExpressionPtr expression);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<PrintStatement> PrintStatementPtr;



class ReturnEmptyStatement : public Statement {
public:
    ReturnEmptyStatement(int line);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<ReturnEmptyStatement> ReturnEmptyStatementPtr;


#include <iostream>
class ReturnStatement : public Statement {
public:
    ExpressionPtr expression;

    ReturnStatement(int line, ExpressionPtr expression);
    // ~ReturnStatement() {};
    
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<ReturnStatement> ReturnStatementPtr;



class WhileStatement : public Statement {
public:
    ExpressionPtr guard;
    StatementPtr body;

    WhileStatement(int line, ExpressionPtr guard, StatementPtr body);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<WhileStatement> WhileStatementPtr;

}