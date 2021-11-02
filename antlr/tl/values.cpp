#include "values.h"
#include <string>
#include <vector>
#include <memory>

namespace minic {

std::string Value::toString() {
    throw std::runtime_error("error: unimplemented value method");
}

std::string Value::typeStr() {
    throw std::runtime_error("error: unimplemented value method");
}

bool Value::isRegister() {
    throw std::runtime_error("error: unimplemented value method");
}

bool Value::isNull() {
    throw std::runtime_error("error: unimplemented value method");
}

int Register::ctr = 0;

Register::Register() : type("i32"), label("u" + std::to_string(ctr++)) {};
Register::Register(std::string type) : type(type), label("u" + std::to_string(ctr++)) {};

std::string Register::toString() {
    return "%" + label;
}

std::string Register::typeStr() {
    return type;
}

bool Register::equals(Register* other) {
    return other != nullptr && this->toString() == other->toString();
}

bool Register::isRegister() {
    return true;
}

bool Register::isNull() {
    return false;
}

std::shared_ptr<Register> Register::get(std::string type) {
    return std::make_shared<Register>(type);
}



Immediate::Immediate(int value) : value(value) {};

std::string Immediate::toString() {
    return std::to_string(value);
}

std::string Immediate::typeStr() {
    return "i32";
}

bool Immediate::isRegister() {
    return false;
}

bool Immediate::isNull() {
    return false;
}

std::shared_ptr<Immediate> Immediate::get(int value) {
    return std::make_shared<Immediate>(value);
}



NullValue::NullValue(std::string structType) : structType(structType) {};

std::string NullValue::toString() {
    return "null";
}

std::string NullValue::typeStr() {
    return structType;
}

bool NullValue::isRegister() {
    return false;
}

bool NullValue::isNull() {
    return true;
}

std::shared_ptr<NullValue> NullValue::get(std::string structType) {
    return std::make_shared<NullValue>(structType);
}



Global::Global(std::string name, std::string type) : type(type), name(name) {};

std::string Global::toString() {
    return "@" + name;
}

std::string Global::typeStr() {
    return type;
}

bool Global::isRegister() {
    return false; // ???
}

bool Global::isNull() {
    return false;
}

std::shared_ptr<Global> Global::get(std::string name, std::string type) {
    return std::make_shared<Global>(name, type);
}


}
