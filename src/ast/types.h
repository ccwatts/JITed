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
    static std::string name();
    static std::string nullName();
    virtual std::string toString();
    virtual Any accept(ASTVisitor* v);
};
typedef std::shared_ptr<Type> TypePtr;

class BoolType : public Type {
public:
    static std::string name();
    std::string toString() override;
    Any accept(ASTVisitor* v);
};
typedef std::shared_ptr<BoolType> BoolTypePtr;

class IntType : public Type {
public:
    static std::string name();
    std::string toString() override;
    Any accept(ASTVisitor* v);
};
typedef std::shared_ptr<IntType> IntTypePtr;

class VoidType : public Type {
public:
    static std::string name();
    std::string toString() override;
    Any accept(ASTVisitor* v);
};
typedef std::shared_ptr<VoidType> VoidTypePtr;

class StructType : public Type {
public:
    int line;
    std::string structName;
    StructType(int line, std::string structName);
    std::string name();
    std::string toString() override;
    Any accept(ASTVisitor* v);
};
typedef std::shared_ptr<StructType> StructTypePtr;

}