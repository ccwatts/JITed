#include <exception>
#include <string>
#include <memory>

#include "expressions.h"
#include "visitors.h"
#include "antlr4-runtime.h"

using antlrcpp::Any;

namespace ast {

Expression::Expression(int line) : line(line) {};
Any Expression::accept(ASTVisitor* v) {
    throw std::runtime_error("Error: Visit function not implemented.");
};
Any Expression::accept(ExpressionVisitor* v) {
    throw std::runtime_error("Error: Visit function not implemented.");
};



Lvalue::Lvalue(int line) : line(line) {};
Any Lvalue::accept(ASTVisitor* v) {
    throw std::runtime_error("Error: Visit function not implemented.");
};
Any Lvalue::accept(ExpressionVisitor* v) {
    throw std::runtime_error("Error: Visit function not implemented.");
};



LvalueDot::LvalueDot(int line, ExpressionPtr left, std::string id) : Lvalue(line), left(left), id(id) {};
Any LvalueDot::accept(ASTVisitor* v) { return v->visit(this); };
Any LvalueDot::accept(ExpressionVisitor* v) { return v->visit(this); };


// not an expression.
LvalueId::LvalueId(int line, std::string id) : Lvalue(line), id(id) {};
Any LvalueId::accept(ASTVisitor* v) { return v->visit(this); };
Any LvalueId::accept(ExpressionVisitor* v) { return v->visit(this); };



BinaryExpression::BinaryExpression(int line, Operator op, ExpressionPtr left, ExpressionPtr right) : Expression(line), op(op), left(left), right(right) {}
BinaryExpression::BinaryExpression(int line, std::string ops, ExpressionPtr left, ExpressionPtr right) : Expression(line), left(left), right(right) {
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
Any BinaryExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any BinaryExpression::accept(ExpressionVisitor* v) { return v->visit(this); };



DotExpression::DotExpression(int line, ExpressionPtr left, std::string id) : Expression(line), left(left), id(id) {};
Any DotExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any DotExpression::accept(ExpressionVisitor* v) { return v->visit(this); };



FalseExpression::FalseExpression(int line) : Expression(line) {};
Any FalseExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any FalseExpression::accept(ExpressionVisitor* v) { return v->visit(this); };



IdentifierExpression::IdentifierExpression(int line, std::string id) : Expression(line), id(id) {};
Any IdentifierExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any IdentifierExpression::accept(ExpressionVisitor* v) { return v->visit(this); };



IntegerExpression::IntegerExpression(int line, int value) : Expression(line), value(value) {};
Any IntegerExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any IntegerExpression::accept(ExpressionVisitor* v) { return v->visit(this); };



InvocationExpression::InvocationExpression(int line, std::string name, std::vector<ExpressionPtr> arguments) : Expression(line), name(name), arguments(arguments) {};
Any InvocationExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any InvocationExpression::accept(ExpressionVisitor* v) { return v->visit(this); };



NewExpression::NewExpression(int line, std::string id) : Expression(line), id(id) {};
Any NewExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any NewExpression::accept(ExpressionVisitor* v) { return v->visit(this); };



NullExpression::NullExpression(int line) : Expression(line) {};
Any NullExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any NullExpression::accept(ExpressionVisitor* v) { return v->visit(this); };



ReadExpression::ReadExpression(int line) : Expression(line) {};
Any ReadExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any ReadExpression::accept(ExpressionVisitor* v) { return v->visit(this); };



TrueExpression::TrueExpression(int line) : Expression(line) {};
Any TrueExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any TrueExpression::accept(ExpressionVisitor* v) { return v->visit(this); };



UnaryExpression::UnaryExpression(int line, Operator op, ExpressionPtr operand) : Expression(line), op(op), operand(operand) {};
UnaryExpression::UnaryExpression(int line, std::string ops, ExpressionPtr operand) : Expression(line), operand(operand) {
    if (ops == "!") op = Operator::NOT;
    else if (ops == "-") op = Operator::MINUS;
    else {
        throw std::runtime_error("error: invalid opcode for binaryexpression");
    }
};
Any UnaryExpression::accept(ASTVisitor* v) { return v->visit(this); };
Any UnaryExpression::accept(ExpressionVisitor* v) { return v->visit(this); };

}