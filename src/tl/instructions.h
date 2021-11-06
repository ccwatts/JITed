#pragma once

#ifndef TLINSTRUCTIONS_H
#define TLINSTRUCTIONS_H

#include "block.h"
#include "values.h"
#include <sstream>
#include <string>
#include <map>
#include <memory>

namespace minic {

class Instruction {
public:
    // virtual void replace(ValuePtr v, ValuePtr with);
    virtual std::string toString();
    virtual RegisterPtr getTarget();
    virtual Values getSources();
    virtual Values allValues();
};
typedef std::shared_ptr<Instruction> InstructionPtr;

class AllocInstruction : public Instruction {
private:
    RegisterPtr target;
    std::string type;
public:
    AllocInstruction(RegisterPtr target, std::string type);
    std::string toString() override;
    RegisterPtr getTarget() override;
    Values allValues() override;
    Values getSources() override;
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
    BinaryInstruction(Operator operator_, RegisterPtr target, ValuePtr lhs, ValuePtr rhs);
    std::string opToString(Operator op);
    std::string toString() override;
    RegisterPtr getTarget() override;
    Values getSources() override;
    Values allValues() override;
};
typedef std::shared_ptr<BinaryInstruction> BinaryInstructionPtr;


class BranchInstruction : public Instruction {
private:
    ValuePtr condition;
public:
    BlockPtr ifTrue, ifFalse;

    BranchInstruction(BlockPtr unconditional);
    BranchInstruction(ValuePtr condition, BlockPtr ifTrue, BlockPtr ifFalse);
    void makeUnconditional(bool takeTrueBranch);
    std::string toString() override;
    RegisterPtr getTarget() override;
    Values getSources() override;
    Values allValues() override;
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
    CastInstruction(Operator operator_, RegisterPtr target, ValuePtr operand);
    CastInstruction(Operator operator_, RegisterPtr target, ValuePtr operand, std::string fromType, std::string toType);
    std::string opToString(Operator op);
    std::string toString() override;
    RegisterPtr getTarget() override;
    Values getSources() override;
    Values allValues() override;
};
typedef std::shared_ptr<CastInstruction> CastInstructionPtr;

class InvocationInstruction : public Instruction {
private:
    RegisterPtr target;
    Values args;
    std::string ret, name;
public:
    InvocationInstruction(RegisterPtr target, Values args, std::string ret, std::string name);
    InvocationInstruction(Values args, std::string ret, std::string name);

    std::string toString() override;
    RegisterPtr getTarget() override;
    Values getSources() override;
    Values allValues() override;
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
    LoadStoreInstruction(Operator operator_, ValuePtr dest, ValuePtr source, int offset=0);
    LoadStoreInstruction(Operator operator_, ValuePtr dest, std::string sourceType, std::string sourceName);
    std::string toString() override;
    RegisterPtr getTarget() override;
    Values getSources() override;
    Values allValues() override;
};
typedef std::shared_ptr<LoadStoreInstruction> LoadStoreInstructionPtr;

class PhiInstruction : public Instruction {
public:
    RegisterPtr target;
    std::vector<Block*> blocks;
    Values values;

    PhiInstruction(RegisterPtr target, std::vector<Block*> blocks, Values values);
    std::string toString() override;
    RegisterPtr getTarget() override;
    Values getSources() override;
    Values allValues() override;
    bool isTrivial();
};
typedef std::shared_ptr<PhiInstruction> PhiInstructionPtr;

class ReturnInstruction : public Instruction {
private:
    ValuePtr retVal;
public:
    ReturnInstruction(ValuePtr retVal=nullptr);

    std::string toString() override;
    RegisterPtr getTarget() override;
    Values getSources() override;
    Values allValues() override;
};
typedef std::shared_ptr<ReturnInstruction> ReturnInstructionPtr;

}
#endif