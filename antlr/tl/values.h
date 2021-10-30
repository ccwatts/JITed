#pragma once

#include <string>
#include <vector>
#include <memory>

namespace minic {

class Value {
public:
    virtual std::string toString();
    virtual std::string typeStr();
    virtual bool isRegister();
};
typedef std::shared_ptr<Value> ValuePtr;
typedef std::vector<ValuePtr> Values;


class Register : public Value {
private:
    // may need definedIn, but i think that was for optimizations
    static int ctr;
    std::string type;
public:
    std::string label;

    Register() : type("i32"), label("u" + std::to_string(ctr++)) {};
    Register(std::string type) : type(type), label("u" + std::to_string(ctr++)) {};

    std::string toString() override {
        return "%" + label;
    }

    std::string typeStr() override {
        return type;
    }

    bool equals(Register* other) {
        return other != nullptr && this->toString() == other->toString();
    }

    bool isRegister() override {
        return true;
    }

    static std::shared_ptr<Register> get(std::string type="i32") {
        return std::make_shared<Register>(type);
    }
};
typedef std::shared_ptr<Register> RegisterPtr;
int Register::ctr = 0;

class Immediate : public Value {
public:
    int value;

    Immediate(int value) : value(value) {};

    std::string toString() override {
        return std::to_string(value);
    }

    std::string typeStr() override {
        return "i32";
    }

    bool isRegister() override {
        return false;
    }

    static std::shared_ptr<Immediate> get(int value) {
        return std::make_shared<Immediate>(value);
    }
};
typedef std::shared_ptr<Immediate> ImmediatePtr;

class NullValue : public Value {
private:
    std::string structType;
public:
    NullValue(std::string structType="null") : structType(structType) {};

    std::string toString() override {
        return "null";
    }

    std::string typeStr() override {
        return structType;
    }

    bool isRegister() override {
        return false;
    }

    static std::shared_ptr<NullValue> get(std::string structType="null") {
        return std::make_shared<NullValue>(structType);
    }
};
// i don't want it to be "NullPtr" since it's too close to nullptr
typedef std::shared_ptr<NullValue> NullValuePtr; 

}