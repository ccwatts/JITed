#ifndef JIT_TYPES__H
#define JIT_TYPES__H

#include <string>

class MiniType {
private:
public:
    virtual std::string to_string() = 0;
};

class BoolType : MiniType {
public:
    std::string to_string() {
        return "bool";
    }
};

class IntType : MiniType {
public:
    std::string to_string() {
        return "int";
    }
};

class NullType : MiniType {
public:
    std::string to_string() {
        return "nulltype";
    }
};

class StructType : MiniType {
private:
    std::string struct_name;
public:
    StructType(std::string name) : struct_name(name) {};
    std::string to_string() {
        return "struct " + struct_name;
    }
};

class FunctionType : MiniType {
private:
public:
    std::string to_string() {
        return "func";
    }
};

#endif