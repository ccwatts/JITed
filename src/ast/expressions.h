#pragma once

#include <exception>
#include <string>
#include <memory>

#include "visitors.h"
#include "antlr4-runtime.h"

using antlrcpp::Any;

namespace ast {

class Expression {
public:
    int line;
    Expression(int line);
    virtual Any accept(ASTVisitor* v);
    virtual Any accept(ExpressionVisitor* v);
};
typedef std::shared_ptr<Expression> ExpressionPtr;

class Lvalue {
public:
    int line;
    Lvalue(int line);
    virtual bool isDot();
    virtual Any accept(ASTVisitor* v);
    virtual Any accept(ExpressionVisitor* v);
};
typedef std::shared_ptr<Lvalue> LvaluePtr;


// not an expression.
class LvalueDot : public Lvalue {
public:
    ExpressionPtr left;
    std::string id;

    LvalueDot(int line, ExpressionPtr left, std::string id);
    bool isDot() override;
    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<LvalueDot> LvalueDotPtr;


// not an expression.
class LvalueId : public Lvalue {
public:
    std::string id;
    LvalueId(int line, std::string id);

    bool isDot() override;
    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<LvalueId> LvalueIdPtr;



class BinaryExpression : public Expression {
public:
    enum Operator {
        TIMES,
        DIVIDE,
        PLUS,
        MINUS,
        LT,
        LE,
        GT,
        GE,
        EQ,
        NE,
        AND,
        OR
    };

    Operator op;
    ExpressionPtr left, right;
private:
public:
    BinaryExpression(int line, Operator op, ExpressionPtr left, ExpressionPtr right);
    BinaryExpression(int line, std::string ops, ExpressionPtr left, ExpressionPtr right);
    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<BinaryExpression> BinaryExpressionPtr;



class DotExpression : public Expression {
public:
    ExpressionPtr left;
    std::string id;

    DotExpression(int line, ExpressionPtr left, std::string id);
    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<DotExpression> DotExpressionPtr;



class FalseExpression : public Expression {
public:
    FalseExpression(int line);

    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<FalseExpression> FalseExpressionPtr;



class IdentifierExpression : public Expression {
public:
    std::string id;
    IdentifierExpression(int line, std::string id);

    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<IdentifierExpression> IdentifierExpressionPtr;



class IntegerExpression : public Expression {
public:
    int value;
    IntegerExpression(int line, int value);

    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<IntegerExpression> IntegerExpressionPtr;



class InvocationExpression : public Expression {
public:
    std::string name;
    std::vector<ExpressionPtr> arguments;
    InvocationExpression(int line, std::string name, std::vector<ExpressionPtr> arguments);
    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<InvocationExpression> InvocationExpressionPtr;



class NewExpression : public Expression {
public:
    std::string id;
    NewExpression(int line, std::string id);

    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<NewExpression> NewExpressionPtr;



class NullExpression : public Expression {
public:
    NullExpression(int line);

    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<NullExpression> NullExpressionPtr;



class ReadExpression : public Expression {
public:
    ReadExpression(int line);

    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<ReadExpression> ReadExpressionPtr;



class TrueExpression : public Expression {
public:
    TrueExpression(int line);

    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<TrueExpression> TrueExpressionPtr;



class UnaryExpression : public Expression {
public:
public:
    enum Operator {
        NOT,
        MINUS
    };

    Operator op;
    ExpressionPtr operand;

    UnaryExpression(int line, Operator op, ExpressionPtr operand);
    UnaryExpression(int line, std::string ops, ExpressionPtr operand);

    Any accept(ASTVisitor* v) override;
    Any accept(ExpressionVisitor* v) override;
};
typedef std::shared_ptr<UnaryExpression> UnaryExpressionPtr;

}