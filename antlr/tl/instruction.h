#pragma once

// #include "block.h"
#include "values.h"
#include <memory>

namespace minic {

    // std::string toString() override;
    // RegisterPtr getTarget() override;
    // Values getSources() override;
    // Values allValues() override;

class Instruction {
public:
    // virtual void replace(ValuePtr v, ValuePtr with);
    virtual std::string toString();
    virtual RegisterPtr getTarget();
    virtual Values getSources();
    virtual Values allValues();
};
typedef std::shared_ptr<Instruction> InstructionPtr;

class Block {
private:
    std::map<std::string, std::string> types;
    std::map<std::string, ValuePtr> bindings;
    std::vector<InstructionPtr> instructions;
    bool sealed;
public:
    std::vector<Block*> toNodes, fromNodes;
    std::string label;

    Block() {};

    void insertInstruction(int idx, InstructionPtr instruction) {
        instructions.insert(instructions.begin() + idx, instruction);
    }

    void addInstruction(InstructionPtr instruction) {
        instructions.push_back(instruction);
    }

    void connectTo(Block* other) {
        this->toNodes.push_back(other);
        other->fromNodes.push_back(this);
        for (auto it = types.begin(); it != types.end(); ++it) {
            other->types.insert({it->first, it->second});
        }
    }

    std::string findType(std::string name) {
        if (types.count(name)) {
            return types.at(name);
        } else {
            return "";
        }
    }

    ValuePtr readVar(std::string name) {
        if (bindings.count(name)) {
            return bindings.at(name);
        } else {
            return readVarFromPreds(name);
        }
    }

    ValuePtr readVarFromPreds(std::string name) {
        ValuePtr val;

        if (!sealed) {
            std::string type = findType(name);
            // fucking phis...
        } else if (fromNodes.size() == 0) {

        } else if (fromNodes.size() == 1) {

        } else {

        }

        return val;
    }

    void writeVariable(std::string name, ValuePtr val) {
        // bindings.insert(name, val);
        // if (findType(name) == nullptr) {

        // }
    }
};
typedef std::shared_ptr<Block> BlockPtr;
// typedef std::vector<Block> Blocks;
typedef std::vector<BlockPtr> Blocks;

class AllocInstruction : public Instruction {
private:
    RegisterPtr target;
    std::string type;
public:
    AllocInstruction(RegisterPtr target, std::string type) : target(target), type(type) {};

    std::string toString() {
        return target->toString() + " = alloca " + type;
    }

    RegisterPtr getTarget() override {
        return target;
    }

    Values allValues() override {
        return {target};
    }

    Values getSources() override {
        return {};
    }
};
typedef std::shared_ptr<AllocInstruction> AllocInstructionPtr;

class BinaryInstruction : public Instruction {
public:
    enum Operator {
        ADD,
        MUL,
        DIV,
        SUB,
        AND,
        OR,
        XOR,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE
    };

private:
    RegisterPtr target;
    ValuePtr lhs, rhs;
    Operator operator_;
public:
    BinaryInstruction(Operator operator_, RegisterPtr target, ValuePtr lhs, ValuePtr rhs) :
        operator_(operator_), target(target), lhs(lhs), rhs(rhs) {};

    std::string opToString(Operator op) {
        switch (op) {
            case ADD:
                return "add";
            case MUL:
                return "mul";
            case DIV:
                return "sdiv";
            case SUB:
                return "sub";
            case AND:
                return "and";
            case OR:
                return "or";
            case XOR:
                return "xor";
            case EQ:
                return "icmp eq";
            case NE:
                return "icmp ne";
            case LT:
                return "icmp slt";
            case GT:
                return "icmp sgt";
            case LE:
                return "icmp sle";
            case GE:
                return "icmp sge";
        }
        return "";
    }

    std::string toString() override {
        return target->toString() + " = "
                + opToString(operator_) + " "
                + lhs->typeStr() + " "
                + lhs->toString() + ", "
                + rhs->toString();
    }

    RegisterPtr getTarget() override {
        return target;
    }

    Values getSources() override {
        return {lhs, rhs};
    }

    Values allValues() override {
        return {target, lhs, rhs};
    }
};
typedef std::shared_ptr<BinaryInstruction> BinaryInstructionPtr;


class BranchInstruction : public Instruction {
private:
    ValuePtr condition;
public:
    BlockPtr ifTrue, ifFalse;

    BranchInstruction(BlockPtr unconditional) :
        condition(nullptr), ifTrue(unconditional), ifFalse(nullptr) {};
    BranchInstruction(ValuePtr condition, BlockPtr ifTrue, BlockPtr ifFalse) :
        condition(condition), ifTrue(ifTrue), ifFalse(ifFalse) {};

    void makeUnconditional(bool takeTrueBranch) {
        if (condition) {
            condition = nullptr;
            if (takeTrueBranch) {
                ifFalse = nullptr;
            } else {
                ifTrue = ifFalse;
                ifFalse = nullptr;
            }
        }
    }

    std::string toString() override {
        if (condition) {
            return "br i1 " + condition->toString()
                    + ", label %" + ifTrue->label
                    + ", label %" + ifFalse->label;
        } else {
            return "br label %" + ifTrue->label;
        }
    }
    
    RegisterPtr getTarget() override {
        return nullptr;
    }

    Values getSources() override {
        return {condition};
    }

    Values allValues() override {
        return {condition};
    }
};
typedef std::shared_ptr<BranchInstruction> BranchInstructionPtr;

class CastInstruction : public Instruction {
public:
    enum Operator {
        BITCAST, TRUNC, ZEXT
    };
private:
    RegisterPtr target;
    Operator operator_;
    ValuePtr operand;
    std::string fromType, toType;
public:
    CastInstruction(Operator operator_, RegisterPtr target, ValuePtr operand) :
        operator_(operator_), target(target), operand(operand) {
        switch (operator_) {
            case BITCAST:
                throw std::runtime_error("error: cannot infer bitcast type");
                break;
            case TRUNC:
                fromType = "i32";
                toType = "i1";
                break;
            case ZEXT:
                fromType = "i1";
                toType = "i32";
                break;
        }
    }
    CastInstruction(Operator operator_, RegisterPtr target, ValuePtr operand, std::string fromType, std::string toType) :
        operator_(operator_), target(target), operand(operand), fromType(fromType), toType(toType) {};

    std::string opToString(Operator op) {
        switch (op)
        {
            case ZEXT:
                return "zext";
            case TRUNC:
                return "trunc";
            case BITCAST:
                return "bitcast";
        }
        return "";
    }
    
    std::string toString() override {
        return target->toString() + " = "
                + opToString(operator_) + " "
                + fromType + " "
                + operand->toString() + " to "
                + toType;
    }

    RegisterPtr getTarget() override {
        return target;
    }

    Values getSources() override {
        return {operand};
    }
    Values allValues() override {
        return {target, operand};
    }
};
typedef std::shared_ptr<CastInstruction> CastInstructionPtr;

class InvocationInstruction : public Instruction {
private:
    RegisterPtr target;
    Values args;
    std::string retType, name;
public:


    std::string toString() override {
        std::ostringstream oss;
        std::string params;
        for (ValuePtr arg : args) {
            oss << arg->typeStr() << " " << arg->toString() << ", ";
        }
        params = oss.str();
        if (args.size() > 0) {
            params = params.substr(0, params.length() - 2);
        }

        if (target) {
            return target->toString() + " = call " + retType + " @" + name + "(" + params + ")";
        } else {
            return "call " + retType + " @" + name + "(" + params + ")";
        }
    }

    RegisterPtr getTarget() override {
        return target;
    }

    Values getSources() override {
        return {};
    }

    Values allValues() override {
        Values vals(args);
        vals.push_back(target);
        return vals;
    }
};
typedef std::shared_ptr<InvocationInstruction> InvocationInstructionPtr;

class LoadStoreInstruction : public Instruction {
public:
    enum Operator {
        LOAD, STORE, GETPTR
    };
private:
    ValuePtr dest, source;
    std::string sourceType, sourceName;
    int offset;
    Operator operator_;

public:
    LoadStoreInstruction(Operator operator_, ValuePtr dest, ValuePtr source, int offset=0) :
        operator_(operator_), dest(dest), source(source), offset(offset) {};

    LoadStoreInstruction(Operator operator_, ValuePtr dest, std::string sourceType, std::string sourceName) :
        operator_(operator_), dest(dest), sourceType(sourceType), sourceName(sourceName) {};

    std::string toString() override {
        std::string sourceStr, sourceTypeStr;
        sourceStr = (source) ? source->toString() : "@" + sourceName;
        sourceTypeStr = (source) ? source->typeStr() : sourceType;
        switch (operator_) {
            case Operator::LOAD:
                return dest->toString() + " = load "
                        + sourceTypeStr + "* "
                        + sourceStr;
            case Operator::STORE:
                return "store "
                        + dest->typeStr() + " "
                        + sourceStr + ", "
                        + dest->typeStr() + "* "
                        + dest->toString();
            case Operator::GETPTR:
                return dest->toString() + " = getelementptr"
                    + sourceTypeStr + " "
                    + sourceStr + ", i1 0, i32 "
                    + std::to_string(offset);
        }
    }

    RegisterPtr getTarget() override {
        if (operator_ == Operator::LOAD) {
            return std::static_pointer_cast<Register>(dest);
        } else {
            return nullptr;
        }
    }

    Values getSources() override {
        if (operator_ == Operator::STORE) {
            return {source};
        } else {
            return {};
        }
    }

    Values allValues() override {
        if (source) {
            return {dest, source};
        } else {
            return {dest};
        }
    }
};
typedef std::shared_ptr<LoadStoreInstruction> LoadStoreInstructionPtr;

class PhiInstruction : public Instruction {
public:
    RegisterPtr target;
    Blocks blocks;
    Values values;

    PhiInstruction(RegisterPtr target, Blocks blocks, Values values) :
        target(target), blocks(blocks), values(values) {};

    std::string toString() override {
        std::string prefix = target->toString() + " = phi " + target->typeStr();
        std::ostringstream oss;

        for (int i = 0; i < blocks.size(); ++i) {
            oss << "[" << values.at(i)->toString()
                << ", %" << blocks.at(i)->label << "]";
            if (i < blocks.size() - 1) {
                oss << ", ";
            }
        }
        return prefix + oss.str();
    }
    RegisterPtr getTarget() override {
        return target;
    }
    Values getSources() override {
        return values;
    }

    Values allValues() override {
        Values vals(values);
        vals.push_back(target);
        return vals;
    }

    bool isTrivial() {
        if (values.size()) {
            return std::count(values.begin(), values.end(), values.at(0)) == values.size();
        } else {
            return true;
        }
    }
};
typedef std::shared_ptr<PhiInstruction> PhiInstructionPtr;

class ReturnInstruction : public Instruction {
private:
    ValuePtr retVal;
public:
    ReturnInstruction(ValuePtr retVal=nullptr) : retVal(retVal) {};

    std::string toString() override {
        if (retVal) {
            return "ret " + retVal->typeStr() + " " + retVal->toString();
        } else {
            return "ret void";
        }
    }
    RegisterPtr getTarget() override { 
        return nullptr;
    }
    Values getSources() override {
        if (retVal) {
            return {retVal};
        } else {
            return {};
        }
    }
    Values allValues() override {
        if (retVal) {
            return {retVal};
        } else {
            return {};
        }
    }
};
typedef std::shared_ptr<ReturnInstruction> ReturnInstructionPtr;

}