#ifndef JIT_ASM__H
#define JIT_ASM__H

#include <string>
#include <vector>
#include <memory>

// typedef std::string Operand;

// namespace ccwthesis {

// enum class LLVMOp {
//     Assign,
//     Add,
//     Mul,
//     Getelementptr,
//     Ret,
//     Call,
//     Alloca,
//     Branch
// };

class LLVMOperand {
private:
    std::string type;
    std::string name;
    int indirection; // pointer count, essentially
public:
    LLVMOperand(std::string type, std::string name, int indirection=0) : type(type), name(name), indirection(indirection) {};

    LLVMOperand(const LLVMOperand& other) : type(other.type), name(other.name), indirection(other.indirection) {};

    std::string str() {
        std::string s = name;
        for (int i = 0; i < indirection; ++i)
            name += "*";
        return s;
    }
};

class LLVMStatement {
protected:
    std::vector<LLVMOperand> operands;
public:
    LLVMStatement(std::vector<LLVMOperand> operands) : operands(operands) {};
    std::vector<LLVMOperand> getOperands() { return operands; }

    // virtual LLVMOp getOp() = 0;
    virtual std::string toString() = 0;
};

class NullStatement : public LLVMStatement {
protected:
public:
    NullStatement(std::vector<LLVMOperand> operands) : LLVMStatement(operands) {};

    std::string getOp() {
        return "na";
    }

    std::string toString() {
        return ""; // ignores null statements; this may not be desired for better error handling
    }
};

class AssignStatement : public LLVMStatement {
protected:
public:
    AssignStatement(std::vector<LLVMOperand> operands) : LLVMStatement(operands) {};

    std::string getOp() {
        return "assign";
    }

    std::string toString() {
        return operands.at(0).str() + " = " + operands.at(1).str();
    }
};

class ArithmeticStatement : public LLVMStatement {
protected:
    std::string op;
public:
    ArithmeticStatement(std::vector<LLVMOperand> operands, std::string op) : LLVMStatement(operands), op(op) {};

    std::string getOp() {
        return op;
    }

    std::string toString() {
        return "unimplemented";
    }
};

class LoadStoreStatement : public LLVMStatement {
protected:
    std::string op;
public:
    LoadStoreStatement(std::vector<LLVMOperand> operands, std::string op) : LLVMStatement(operands), op(op) {};

    std::string getOp() {
        return op;
    }

    std::string toString() {
        return "unimplemented";
    }
};

class AllocateStatement : public LLVMStatement {
protected:
public:
    AllocateStatement(std::vector<LLVMOperand> operands) : LLVMStatement(operands) {};

    std::string toString() {
        return "unimplemented";
    }
};

class GEPStatement : public LLVMStatement {
protected:
public:
    GEPStatement(std::vector<LLVMOperand> operands) : LLVMStatement(operands) {};

    std::string toString() {
        return "unimplemented";
    }
};

// this all needs to be moved out eventually
LLVMStatement* parseLine(std::string line) {
    return nullptr;
}


// } // namespace ccwthesis

#endif