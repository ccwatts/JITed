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
    virtual bool isNull();
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

    Register();
    Register(std::string type);

    std::string toString() override;
    std::string typeStr() override;
    bool equals(Register* other);
    bool isRegister() override;
    bool isNull() override;
    static std::shared_ptr<Register> get(std::string type="i32");
};
typedef std::shared_ptr<Register> RegisterPtr;

class Immediate : public Value {
public:
    int value;

    Immediate(int value);

    std::string toString();

    std::string typeStr() override;

    bool isRegister() override;

    bool isNull() override;

    static std::shared_ptr<Immediate> get(int value);
};
typedef std::shared_ptr<Immediate> ImmediatePtr;

class NullValue : public Value {
private:
public:
    std::string structType;
    NullValue(std::string structType);

    std::string toString() override;

    std::string typeStr() override;

    bool isRegister() override;

    bool isNull() override;

    static std::shared_ptr<NullValue> get(std::string structType="");
};
// i don't want it to be "NullPtr" since it's too close to nullptr
typedef std::shared_ptr<NullValue> NullValuePtr; 

class Global : public Value {
private:
    std::string type;
public:
    std::string name;

    Global(std::string name, std::string type);

    std::string toString() override;

    std::string typeStr() override;

    bool isRegister() override;

    bool isNull() override;

    static std::shared_ptr<Global> get(std::string name, std::string type);
};

}
