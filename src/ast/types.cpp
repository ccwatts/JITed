#include <string>
#include <memory>
#include "types.h"
#include "visitors.h"
#include "antlr4-runtime.h"
using antlrcpp::Any;

namespace ast {

class ASTVisitor;

std::string Type::name() { return "generic_type"; }
std::string Type::toMiniString() { return Type::name(); }
std::string Type::nullName() { return "null"; }
std::string Type::toString() {
    return "no_type";
}
Any Type::accept(ASTVisitor* v) {
    throw std::runtime_error("Error: Visit function not implemented.");
};



std::string BoolType::name() { return "bool"; }
std::string BoolType::toMiniString() { return BoolType::name(); }
std::string BoolType::toString() {
    return "i32";
}
Any BoolType::accept(ASTVisitor* v) { return v->visit(this); };



std::string IntType::name() { return "int"; }
std::string IntType::toMiniString() { return IntType::name(); }
std::string IntType::toString() {
    return "i32";
}
Any IntType::accept(ASTVisitor* v) { return v->visit(this); };



std::string VoidType::name() { return "void"; }
std::string VoidType::toMiniString() { return VoidType::name(); }
std::string VoidType::toString() {
    return "void";
}
Any VoidType::accept(ASTVisitor* v) { return v->visit(this); };



StructType::StructType(int line, std::string structName) : line(line), structName(structName) {};
std::string StructType::name() { return "%struct." + structName; }
std::string StructType::toMiniString() { return structName; }
std::string StructType::toString() {
    return "%struct." + structName + "*";
}
Any StructType::accept(ASTVisitor* v) { return v->visit(this); };


}