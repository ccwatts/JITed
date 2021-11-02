#ifndef JITPROGRAM__H
#define JITPROGRAM__H

#include "type.h"

#include <string>
#include <vector>
#include <memory>

// class TypeBinding {
// private:
//     std::string name;
//     std::unique_ptr<MiniType> type;
// public:
//     TypeBinding(std::string name, std::unique_ptr<MiniType> type) : name(name), type(std::move(type)) {};
// };

enum class MiniOp {
    Add,
    Sub
};

class MiniStatement {
private:
// oh heck
// this will be virtual
    MiniOp op;
public:
    MiniStatement(MiniOp op) : op(op) {};
    virtual std::string str() = 0;
    MiniOp get_op() { return op; }
};

class ProgramCtx {
private:
// variables
// function headers
public:
};

#endif