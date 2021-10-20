#pragma once

#include <string>
#include <memory>

#include "visitors.h"
#include "antlr4-runtime.h"
using antlrcpp::Any;

namespace ast {

class ASTVisitor;

class Type {
public:
    static std::string name() { return "null"; }

    virtual Any accept(ASTVisitor* v) {
        throw std::runtime_error("Error: Visit function not implemented.");
    };
};
typedef std::shared_ptr<Type> TypePtr;

class BoolType : public Type {
public:
    static std::string name() { return "bool"; }

    Any accept(ASTVisitor* v) { return v->visit(this); };
};
typedef std::shared_ptr<BoolType> BoolTypePtr;

class IntType : public Type {
public:
    static std::string name() { return "int"; }
    Any accept(ASTVisitor* v) { return v->visit(this); };
};
typedef std::shared_ptr<IntType> IntTypePtr;

class VoidType : public Type {
public:
    static std::string name() { return "void"; }
    Any accept(ASTVisitor* v) { return v->visit(this); };
};
typedef std::shared_ptr<VoidType> VoidTypePtr;

class StructType : public Type {
public:
    int line;
    std::string structName;
    StructType(int line, std::string structName) : line(line), structName(structName) {};
    std::string name() { return "struct " + structName; }
    Any accept(ASTVisitor* v) { return v->visit(this); };
};
typedef std::shared_ptr<StructType> StructTypePtr;

}