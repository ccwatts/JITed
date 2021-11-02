#ifndef INTERP_AST__H
#define INTERP_AST__H

#include <string>
#include <memory>


namespace ast {

class TreeNode {
private:
public:
};

/*****************************************/

class Expression : public TreeNode {};
typedef std::shared_ptr<Expression> EXP_P;

class BinaryOp : public Expression {
protected:
    std::string op_str;
    std::shared_ptr<Expression> left, right;
public:
    BinaryOp(std::string op_str, EXP_P left, EXP_P right) : op_str(op_str), left(left), right(right) {};
};

class UnaryOp : public Expression {
protected:
    std::string op_str;
    std::shared_ptr<Expression> operand;
public:
    UnaryOp(std::string op_str, EXP_P operand) : op_str(op_str), operand(operand) {};
};


class Identifier : public Expression {
protected:
    std::string id;
public:
    std::string get_id() { return id; }
};

class Number : public Expression {
protected:
    int value;
public:
    int get_value() { return value; }
};

class BoolConstant : public Expression {
protected:
    bool value;
public:
    bool get_value() { return value; }
};

/*****************************************/

class Statement : public TreeNode {};

}

#endif