#pragma once

#include <exception>
#include <string>
#include <memory>

#include "visitors.h"
#include "antlr4-runtime.h"

using antlrcpp::Any;

namespace ast {

class ASTVisitor;
class ExpressionVisitor;

class Expression {
public:
    int line;
    Expression(int line) : line(line) {};

    virtual Any accept(ASTVisitor* v) {
        throw std::runtime_error("Error: Visit function not implemented.");
    };

    virtual Any accept(ExpressionVisitor* v) {
        throw std::runtime_error("Error: Visit function not implemented.");
    };
};
typedef std::shared_ptr<Expression> ExpressionPtr;

class Lvalue {
public:
    int line;
    Lvalue(int line) : line(line) {};

    virtual Any accept(ASTVisitor* v) {
        throw std::runtime_error("Error: Visit function not implemented.");
    };
    virtual Any accept(ExpressionVisitor* v) {
        throw std::runtime_error("Error: Visit function not implemented.");
    };
};
typedef std::shared_ptr<Lvalue> LvaluePtr;


// not an expression.
class LvalueDot : public Lvalue {
public:
    ExpressionPtr left;
    std::string id;

    LvalueDot(int line, ExpressionPtr left, std::string id) : Lvalue(line), left(left), id(id) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<LvalueDot> LvalueDotPtr;


// not an expression.
class LvalueId : public Lvalue {
public:
    std::string id;
    LvalueId(int line, std::string id) : Lvalue(line), id(id) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
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
    BinaryExpression(int line, Operator op, ExpressionPtr left, ExpressionPtr right) : Expression(line), op(op), left(left), right(right) {}
    BinaryExpression(int line, std::string ops, ExpressionPtr left, ExpressionPtr right) : Expression(line), left(left), right(right) {
        if      (ops == "*")  op = Operator::TIMES;
        else if (ops == "/")  op = Operator::DIVIDE;
        else if (ops == "+")  op = Operator::PLUS;
        else if (ops == "-")  op = Operator::MINUS;
        else if (ops == "<")  op = Operator::LT;
        else if (ops == "<=") op = Operator::LE;
        else if (ops == ">")  op = Operator::GT;
        else if (ops == ">=") op = Operator::GE;
        else if (ops == "==") op = Operator::EQ;
        else if (ops == "!=") op = Operator::NE;
        else if (ops == "&&") op = Operator::AND;
        else if (ops == "||") op = Operator::OR;
        else {
            throw std::runtime_error("error: invalid opcode for binaryexpression");
        }
    }

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<BinaryExpression> BinaryExpressionPtr;



class DotExpression : public Expression {
public:
    ExpressionPtr left;
    std::string id;

    DotExpression(int line, ExpressionPtr left, std::string id) : Expression(line), left(left), id(id) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<DotExpression> DotExpressionPtr;



class FalseExpression : public Expression {
public:
    FalseExpression(int line) : Expression(line) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<FalseExpression> FalseExpressionPtr;



class IdentifierExpression : public Expression {
public:
    std::string id;
    IdentifierExpression(int line, std::string id) : Expression(line), id(id) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<IdentifierExpression> IdentifierExpressionPtr;



class IntegerExpression : public Expression {
public:
    int value;
    IntegerExpression(int line, int value) : Expression(line), value(value) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<IntegerExpression> IntegerExpressionPtr;



class InvocationExpression : public Expression {
public:
    std::string name;
    std::vector<ExpressionPtr> arguments;
    InvocationExpression(int line, std::string name, std::vector<ExpressionPtr> arguments) : Expression(line), name(name), arguments(arguments) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<InvocationExpression> InvocationExpressionPtr;



class NewExpression : public Expression {
public:
    std::string id;
    NewExpression(int line, std::string id) : Expression(line), id(id) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<NewExpression> NewExpressionPtr;



class NullExpression : public Expression {
public:
    NullExpression(int line) : Expression(line) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<NullExpression> NullExpressionPtr;



class ReadExpression : public Expression {
public:
    ReadExpression(int line) : Expression(line) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<ReadExpression> ReadExpressionPtr;



class TrueExpression : public Expression {
public:
    TrueExpression(int line) : Expression(line) {};

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
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

    UnaryExpression(int line, Operator op, ExpressionPtr operand) : Expression(line), op(op), operand(operand) {};
    UnaryExpression(int line, std::string ops, ExpressionPtr operand) : Expression(line), operand(operand) {
        if (ops == "!") op = Operator::NOT;
        else if (ops == "-") op = Operator::MINUS;
        else {
            throw std::runtime_error("error: invalid opcode for binaryexpression");
        }
    };

    Any accept(ASTVisitor* v) override { return v->visit(this); };
    Any accept(ExpressionVisitor* v) override { return v->visit(this); };
};
typedef std::shared_ptr<UnaryExpression> UnaryExpressionPtr;

}