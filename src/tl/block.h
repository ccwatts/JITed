#pragma once 

#ifndef BLOCK_H
#define BLOCK_H


// #include "instruction.h"
#include "../ast/statements.h"
#include "values.h"

#include <map>
#include <string>
#include <vector>
#include <memory>


namespace minic {
const std::string EMPTY_TYPE = " "; // space is an invalid type name by the parser
class Instruction;
class Block;

class Phi {
private:
    bool resolved;
    static int counter;
public:
    RegisterPtr target;
    Block* block;
    std::string variable;
    std::string printableInst;

    Phi(Block* block, std::string variable, std::string type);
    void resolve();
    static std::shared_ptr<Phi> get(Block* block, std::string variable, std::string type);
};
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
    ast::Function* func;

    Block();
    Block(ast::Function* func);
    void insertInstruction(int idx, std::shared_ptr<Instruction> instruction);
    void addInstruction(std::shared_ptr<Instruction> instruction);
    void connectTo(Block* other);
    void connectTo(std::shared_ptr<Block> other);
    std::string findType(std::string name);
    ValuePtr readVar(std::string name);
    ValuePtr readVarFromPreds(std::string name);
    void writeVariable(std::string name, ValuePtr val);
    void assignLabel();
    void assignType(std::string name, std::string type);
    void seal();
    std::string toString();
};
typedef std::shared_ptr<Block> BlockPtr;
typedef std::vector<BlockPtr> Blocks;

}
#endif