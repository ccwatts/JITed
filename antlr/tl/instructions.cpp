// #include "block.h"
#include "block.h"
#include "values.h"
#include "instructions.h"
#include <sstream>
#include <string>
#include <map>
#include <memory>

namespace minic {

    // std::string toString() override;
    // RegisterPtr getTarget() override;
    // Values getSources() override;
    // Values allValues() override;

// class Instruction {
// public:
//     // virtual void replace(ValuePtr v, ValuePtr with);
//     virtual std::string toString() {
//         throw std::runtime_error("error: unimplemented instruction method");
//     }
//     virtual RegisterPtr getTarget() {
//         throw std::runtime_error("error: unimplemented instruction method");
//     }
//     virtual Values getSources() {
//         throw std::runtime_error("error: unimplemented instruction method");
//     }
//     virtual Values allValues() {
//         throw std::runtime_error("error: unimplemented instruction method");
//     }
// };
// typedef std::shared_ptr<Instruction> InstructionPtr;


std::string Instruction::toString() {
    throw std::runtime_error("error: unimplemented instruction method");
}
RegisterPtr Instruction::getTarget() {
    throw std::runtime_error("error: unimplemented instruction method");
}
Values Instruction::getSources() {
    throw std::runtime_error("error: unimplemented instruction method");
}
Values Instruction::allValues() {
    throw std::runtime_error("error: unimplemented instruction method");
}

AllocInstruction::AllocInstruction(RegisterPtr target, std::string type) : target(target), type(type) {};

std::string AllocInstruction::toString() {
    return target->toString() + " = alloca " + type;
}

RegisterPtr AllocInstruction::getTarget() {
    return target;
}

Values AllocInstruction::allValues() {
    return {target};
}

Values AllocInstruction::getSources() {
    return {};
}



BinaryInstruction::BinaryInstruction(Operator operator_, RegisterPtr target, ValuePtr lhs, ValuePtr rhs) :
    operator_(operator_), target(target), lhs(lhs), rhs(rhs) {};

std::string BinaryInstruction::opToString(Operator op) {
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

std::string BinaryInstruction::toString() {
    return target->toString() + " = "
            + opToString(operator_) + " "
            + lhs->typeStr() + " "
            + lhs->toString() + ", "
            + rhs->toString();
}

RegisterPtr BinaryInstruction::getTarget() {
    return target;
}

Values BinaryInstruction::getSources() {
    return {lhs, rhs};
}

Values BinaryInstruction::allValues() {
    return {target, lhs, rhs};
}



BranchInstruction::BranchInstruction(BlockPtr unconditional) :
    condition(nullptr), ifTrue(unconditional), ifFalse(nullptr) {};
BranchInstruction::BranchInstruction(ValuePtr condition, BlockPtr ifTrue, BlockPtr ifFalse) :
    condition(condition), ifTrue(ifTrue), ifFalse(ifFalse) {};

void BranchInstruction::makeUnconditional(bool takeTrueBranch) {
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

std::string BranchInstruction::toString() {
    if (condition) {
        return "br i1 " + condition->toString()
                + ", label %" + ifTrue->label
                + ", label %" + ifFalse->label;
    } else {
        return "br label %" + ifTrue->label;
    }
}

RegisterPtr BranchInstruction::getTarget() {
    return nullptr;
}

Values BranchInstruction::getSources() {
    return {condition};
}

Values BranchInstruction::allValues() {
    return {condition};
}



CastInstruction::CastInstruction(Operator operator_, RegisterPtr target, ValuePtr operand) :
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
CastInstruction::CastInstruction(Operator operator_, RegisterPtr target, ValuePtr operand, std::string fromType, std::string toType) :
    operator_(operator_), target(target), operand(operand), fromType(fromType), toType(toType) {};

std::string CastInstruction::opToString(Operator op) {
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

std::string CastInstruction::toString() {
    return target->toString() + " = "
            + opToString(operator_) + " "
            + fromType + " "
            + operand->toString() + " to "
            + toType;
}

RegisterPtr CastInstruction::getTarget() {
    return target;
}

Values CastInstruction::getSources() {
    return {operand};
}
Values CastInstruction::allValues() {
    return {target, operand};
}



InvocationInstruction::InvocationInstruction(RegisterPtr target, Values args, std::string ret, std::string name) :
    target(target), args(args), ret(ret), name(name) {};

InvocationInstruction::InvocationInstruction(Values args, std::string ret, std::string name) :
    target(nullptr), args(args), ret(ret), name(name) {};


std::string InvocationInstruction::toString() {
    std::ostringstream oss;
    std::string params;
    for (ValuePtr arg : args) {
        oss << arg->typeStr() << " " << arg->toString() << ", ";
    }
    params = oss.str();
    if (args.size() > 0) {
        params = params.substr(0, params.length() - 2);
    }

    // THIS NEEDS TO BE THE CALLBACK...
    if (target) {
        return target->toString() + " = call " + ret + " @" + name + "(" + params + ")";
    } else {
        return "call " + ret + " @" + name + "(" + params + ")";
    }
}

RegisterPtr InvocationInstruction::getTarget() {
    return target;
}

Values InvocationInstruction::getSources() {
    return {};
}

Values InvocationInstruction::allValues() {
    Values vals(args);
    vals.push_back(target);
    return vals;
}



LoadStoreInstruction::LoadStoreInstruction(Operator operator_, ValuePtr dest, ValuePtr source, int offset) :
    operator_(operator_), dest(dest), source(source), offset(offset) {};

LoadStoreInstruction::LoadStoreInstruction(Operator operator_, ValuePtr dest, std::string sourceType, std::string sourceName) :
    operator_(operator_), dest(dest), sourceType(sourceType), sourceName(sourceName) {};

std::string LoadStoreInstruction::toString() {
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

RegisterPtr LoadStoreInstruction::getTarget() {
    if (operator_ == Operator::LOAD) {
        return std::static_pointer_cast<Register>(dest);
    } else {
        return nullptr;
    }
}

Values LoadStoreInstruction::getSources() {
    if (operator_ == Operator::STORE) {
        return {source};
    } else {
        return {};
    }
}

Values LoadStoreInstruction::allValues() {
    if (source) {
        return {dest, source};
    } else {
        return {dest};
    }
}



PhiInstruction::PhiInstruction(RegisterPtr target, std::vector<Block*> blocks, Values values) :
    target(target), blocks(blocks), values(values) {};

std::string PhiInstruction::toString() {
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
RegisterPtr PhiInstruction::getTarget() {
    return target;
}
Values PhiInstruction::getSources() {
    return values;
}

Values PhiInstruction::allValues() {
    Values vals(values);
    vals.push_back(target);
    return vals;
}

bool PhiInstruction::isTrivial() {
    if (values.size()) {
        return std::count(values.begin(), values.end(), values.at(0)) == values.size();
    } else {
        return true;
    }
}



ReturnInstruction::ReturnInstruction(ValuePtr retVal) : retVal(retVal) {};

std::string ReturnInstruction::toString() {
    if (retVal) {
        return "ret " + retVal->typeStr() + " " + retVal->toString();
    } else {
        return "ret void";
    }
}
RegisterPtr ReturnInstruction::getTarget() { 
    return nullptr;
}
Values ReturnInstruction::getSources() {
    if (retVal) {
        return {retVal};
    } else {
        return {};
    }
}
Values ReturnInstruction::allValues() {
    if (retVal) {
        return {retVal};
    } else {
        return {};
    }
}

}