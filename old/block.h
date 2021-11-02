#pragma once 

// #include "instruction.h"
#include "values.h"

#include <map>
#include <string>
#include <vector>
#include <memory>

namespace minic {
const std::string EMPTY_TYPE = " "; // space is an invalid type name by the parser
class Instruction;
class Phi;
typedef std::shared_ptr<Phi> PhiPtr;

// TODO THIS THING ISN'T DONE.
class Block {
private:
    bool sealed;
    static int idCounter;
public:
    std::map<std::string, std::string> types;
    std::map<std::string, ValuePtr> bindings;
    std::vector<std::shared_ptr<Instruction>> instructions;
    std::vector<Block*> toNodes, fromNodes;
    std::vector<PhiPtr> incompletePhis, phis;
    std::string label, header, footer;

    Block() {};

    void insertInstruction(int idx, std::shared_ptr<Instruction> instruction) {
        instructions.insert(instructions.begin() + idx, instruction);
    }

    void addInstruction(std::shared_ptr<Instruction> instruction) {
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
            return EMPTY_TYPE;
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
            PhiPtr phi = Phi::get(this, name, type);
            val = phi->target;

            incompletePhis.push_back(phi);
            phis.push_back(phi);
        } else if (fromNodes.size() == 0) {
            throw std::runtime_error("error: " + name + " was never initialized\n");
        } else if (fromNodes.size() == 1) {
            val = fromNodes.at(0)->readVar(name);
        } else {
            std::string type = findType(name);
            PhiPtr phi = Phi::get(this, name, type);
            val = phi->target;

            phis.push_back(phi);
            writeVariable(name, val);
            phi->resolve();
        }
        writeVariable(name, val);
        return val;
    }

    void writeVariable(std::string name, ValuePtr val) {
        bindings.insert({name, val});
        if (findType(name) == EMPTY_TYPE) {
            types.insert({name, val->typeStr()});
        }
    }

    void assignLabel() {
        label = "LU" + std::to_string(idCounter++);
    }
};
typedef std::shared_ptr<Block> BlockPtr;
typedef std::vector<BlockPtr> Blocks;
int Block::idCounter = 0;

class Phi {
private:
    bool resolved;
    static int counter;
public:
    RegisterPtr target;
    Block* block;
    std::string variable;
    std::string printableInst;

    Phi(Block* block, std::string variable, std::string type) : block(block), variable(variable) {
        target = Register::get(type);
        target->label = "phi" + std::to_string(counter++);
        resolved = false;
    }

    void resolve() {
        throw std::runtime_error("unimplemented");
    }

    static std::shared_ptr<Phi> get(Block* block, std::string variable, std::string type) {
        return std::make_shared<Phi>(block, variable, type);
    }
};
typedef std::shared_ptr<Phi> PhiPtr;
int Phi::counter = 0;

}