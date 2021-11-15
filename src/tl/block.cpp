#include "../ast/statements.h"
#include "instructions.h"
#include "values.h"
#include "block.h"

#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <memory>


namespace minic {
// const std::string EMPTY_TYPE = " "; // space is an invalid type name by the parser

int Block::idCounter = 0;

Block::Block() : sealed(false) {};
Block::Block(jited::ast::Function* func) : sealed(false), func(func) {};

void Block::insertInstruction(int idx, std::shared_ptr<Instruction> instruction) {
    instructions.insert(instructions.begin() + idx, instruction);
}

void Block::addInstruction(std::shared_ptr<Instruction> instruction) {
    instructions.push_back(instruction);
}

void Block::connectTo(Block* other) {
    this->toNodes.push_back(other);
    other->fromNodes.push_back(this);
    for (auto it = types.begin(); it != types.end(); ++it) {
        // other->types.insert({it->first, it->second});
        other->types[it->first] = it->second;
    }
}

void Block::connectTo(std::shared_ptr<Block> other) {
    connectTo(other.get());
}

std::string Block::findType(std::string name) {
    if (types.count(name)) {
        return types.at(name);
    } else {
        return EMPTY_TYPE;
    }
}

ValuePtr Block::readVar(std::string name) {
    if (bindings.count(name)) {
        return bindings.at(name);
    } else {
        return readVarFromPreds(name);
    }
}

ValuePtr Block::readVarFromPreds(std::string name) {
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

void Block::writeVariable(std::string name, ValuePtr val) {
    bindings[name] = val; //({name, val});
    if (findType(name) == EMPTY_TYPE) {
        // types.insert({name, val->typeStr()});
        types[name] = val->typeStr();
    }
}

void Block::assignLabel() {
    label = "LU" + std::to_string(idCounter++);
}

void Block::assignType(std::string name, std::string type) {
    // types.insert({name, type});
    types[name] = type;
}

void Block::seal() {
    for (PhiPtr p : incompletePhis) {
        p->resolve();
    }
    sealed = true;
}

std::string Block::toString() {
    std::ostringstream oss;
    oss << header << label << ":\n";
    for (std::shared_ptr<Instruction> inst : instructions) {
        oss << inst->toString() << std::endl;
    }
    oss << footer;
    return oss.str();
}



// defns proper...
Phi::Phi(Block* block, std::string variable, std::string type) : block(block), variable(variable) {
    target = Register::get(type);
    target->label = "phi" + std::to_string(counter++);
    resolved = false;
}

void Phi::resolve() {
    if (resolved) {
        return;
    }

    std::vector<Block*> blocks;
    Values values;
    std::ostringstream oss;

    oss << target->toString() << " = phi " << target->typeStr() << " ";
    for (int i = 0; i < block->fromNodes.size(); ++i) {
        Block* b = block->fromNodes.at(i);
        ValuePtr val = b->readVar(variable);
        blocks.push_back(b);
        values.push_back(val);

        oss << " [" << val->toString() << ", %" << b->label << "]";
        if (i + 1 < block->fromNodes.size()) {
            oss << ", ";
        }
    }

    printableInst = oss.str();
    std::shared_ptr<PhiInstruction> p = std::make_shared<PhiInstruction>(target, blocks, values);
    block->instructions.insert(block->instructions.begin(), p);
    resolved = true;
}

std::shared_ptr<Phi> Phi::get(Block* block, std::string variable, std::string type) {
    return std::make_shared<Phi>(block, variable, type);
}
int Phi::counter = 0;

}