
#include "MiniParser.h"
#include "MiniLexer.h"

#include "antlr4-common.h"
#include "antlr4-runtime.h"

#include "ast/program.h"
#include "ast/expressions.h"
#include "ast/statements.h"
#include "ast/types.h"
#include "ast/visitors.h"

#include "lib/proxy.h"
#include "lib/rtti.h"

#include "MiniToAstVisitors.h"

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <map>
#include <limits>

#include "MiniInterpreter.h"

using antlrcpp::Any;
using jited::proxy;

namespace jited {

/*
    Container class for both return values (or lack thereof) and expression evaluation results
*/
TypedValue::TypedValue(const std::string type, Any value, bool initialized) : type(type), value(value), initialized(initialized) {};

template <typename T>
T TypedValue::as() {
    // UNSAFE
    return value.as<T>();
}

// welcome to dubai
void TypedValue::setValue(antlrcpp::Any val, PackedStruct* parentStruct, std::string lastField) {
    // parent struct is unused for int and bool
    // only needed to perform linkage in the case of member structs
    if (val.is<IntPtr>()) val = *(val.as<IntPtr>());
    else if (val.is<int*>()) val = *(val.as<int*>());
    else if (val.is<BoolPtr>()) val = *(val.as<BoolPtr>());
    else if (val.is<bool*>()) val = *(val.as<bool*>());

    if (val.is<int>()) { // int
        if (value.is<int32_t*>()) {
            *(value.as<int32_t*>()) = (int32_t) val.as<int>();
        } else if (value.is<int*>()) {
            *(value.as<int*>()) = val.as<int>();
        } else if (value.is<IntPtr>()) {
            *(value.as<IntPtr>()) = val.as<int>();
        } else {
            value = val;
        }

    } else if (val.is<bool>()) { // bool
        if (value.is<int32_t*>()) {
            *(value.as<int32_t*>()) = (int32_t) val.as<bool>();
        } else if (value.is<BoolPtr>()) {
            *(value.as<BoolPtr>()) = val.as<bool>();
        } else if (value.is<bool*>()) {
            *(value.as<bool*>()) = val.as<bool>();
        } else {
            value = val;
        }

    } else if (val.is<PackedStruct*>()) { // struct
        if (parentStruct) {
            // TypedValuePtr parent = parentStruct->getStruct();
            // set parent's accessor to point to val
            // set parent's struct data to point to val's struct data
            // set value to val
            parentStruct->setMember(lastField, val.as<PackedStruct*>());
            value = val;
            // throw std::runtime_error("unimplemented");
        } else {
            value = val;
        }
    } else {
        throw std::runtime_error("value in assignment is not an int, bool, or struct");
    }
    initialized = true;
    return;
}

int TypedValue::asInt() {
    if (value.is<int>()) {
        return value.as<int>();
    } else if (value.is<int32_t*>()) {
        return (int) *(value.as<int32_t*>());
    } else if (value.is<int*>()) {
        return (int) *(value.as<int*>());
    } else if (value.is<IntPtr>()) {
        return (int) *(value.as<IntPtr>());
    } else {
        return 0;
    }
}

bool TypedValue::asBool() {
    if (value.is<bool>()) {
        return value.as<bool>();
    } else if (value.is<int32_t*>()) {
        return (bool) *(value.as<int32_t*>());
    } else if (value.is<bool*>()) {
        return (bool) *(value.as<bool*>());
    } else if (value.is<BoolPtr>()) {
        return (bool) *(value.as<BoolPtr>());
    } else {
        return 0;
    }
}

int32_t TypedValue::asI32() {
    if (value.is<int32_t*>()) {
        return *(value.as<int32_t*>());
    } else if (value.is<bool>()) {
        return (int32_t) value.as<bool>();
    } else if (value.is<BoolPtr>()) {
        return (int32_t) *(value.as<BoolPtr>());
    } else if (value.is<int>()) {
        return (int32_t) value.as<int>();
    } else if (value.is<IntPtr>()) {
        return (int32_t) *(value.as<IntPtr>());
    } else {
        return 0;
    }
}

PackedStruct* TypedValue::asStruct() {
    return value.as<PackedStruct*>();
}

bool TypedValue::isStruct() {
    return value.is<PackedStruct*>();
}

bool TypedValue::isNull() {
    // THIS WILL NEED TO CHANGE TODO
    // return isStruct && value.as<_RawValueMapPtr>() == nullptr;
    return value.is<PackedStruct*>() && value.as<PackedStruct*>() == NULL;
}

std::string TypedValue::toString() {
    if (type == jited::ast::IntType::name()) {
        return std::to_string(asInt());
    } else if (type == jited::ast::BoolType::name()) {
        return (asBool()) ? "true" : "false";
    } else if (type == jited::ast::VoidType::name()) {
        return "void"; // should not be returned from anything...
    } else if (isNull()) {
        return "null";
    } else {
        std::ostringstream oss;
        oss << value.as<PackedStruct*>();
        return oss.str();
    }
}










// 
// std::map<std::string, TypedValue> offsets; // field name -> int offset
// size_t size;
// uint8_t* buf;
std::map<uint8_t*, PackedStruct*> PackedStruct::lookupTable;

PackedStruct::PackedStruct(jited::ast::TypeDeclarationPtr fieldInfo, uint8_t* existingBuf) : buf(existingBuf), totalBytes(0) {
    const size_t ptrBytes = sizeof(intptr_t) / sizeof(int32_t);
    const size_t i32Bytes = sizeof(int32_t);
    // size_t totalBytes = 0;

    // calculate size and offsets
    for (jited::ast::DeclarationPtr dec : fieldInfo->fields) {
        types[dec->name] = dec->type->toMiniString();
        offsets[dec->name] = totalBytes;
        if (dec->type->toMiniString() == "int" || dec->type->toMiniString() == "bool") {
            totalBytes += i32Bytes;
        } else if (dec->type->toString() == "void") {
            throw std::runtime_error("void term in struct");
        } else {
            totalBytes += ptrBytes;
        }
    }

    if (!buf) {
        if (totalBytes == 0) {
            throw std::runtime_error("tried to make struct with 0 size");
        } else {
            buf = new uint8_t[totalBytes](); // () gives zeroed array
        }
    }

    // create typed values
    for (auto p : offsets) {
        std::string name = p.first;
        int offset = p.second;
        std::string type = types.at(name);
        if (type == "int") {
            accessors[name] = std::make_shared<TypedValue>(jited::ast::IntType::name(), (int32_t*) (buf + offset));
        } else if (type == "bool") {
            accessors[name] = std::make_shared<TypedValue>(jited::ast::BoolType::name(), (int32_t*) (buf + offset));
        } else {
            accessors[name] = std::make_shared<TypedValue>(type, (PackedStruct*) NULL);
            // accessor only used by interpreter
            // the jit will access the real thing in a diff way
            // struct
            // this one's different
        }
    }
    lookupTable[buf] = this;
};

PackedStruct::~PackedStruct() {
    // this probably needs more, namely accessors.
    lookupTable.erase(buf);
    if (buf) {
        delete[] buf;
    }
};

bool PackedStruct::has(std::string fieldName) {
    return offsets.count(fieldName);
}

TypedValuePtr PackedStruct::at(std::string fieldName) {
    if (has(fieldName)) {
        return accessors.at(fieldName);
    } else {
        return nullptr;
    }
}

template <typename T>
T PackedStruct::get(std::string fieldName) {
    if (has(fieldName)) {
        int offset = offsets.at(fieldName);
        uint8_t* adjusted = buf + offset;
        return *((T*) adjusted);
    } else {
        throw std::runtime_error("invalid fieldname in getI32");
    }
}

int32_t PackedStruct::getInt(std::string fieldName) {
    return get<int32_t>(fieldName);
}

uint8_t* PackedStruct::getPtr(std::string fieldName) {
    return get<uint8_t*>(fieldName);
}

void PackedStruct::setMember(std::string fieldName, PackedStruct* member) {
    if (has(fieldName)) {
        accessors[fieldName]->setValue(member);
        if (member) {
            uint8_t* atField = buf + offsets.at(fieldName);
            // buf[offsets.at(fieldName)] = member->buf;
            *((uintptr_t*) atField) = (uintptr_t) member->buf;
            // std::cout << "member: <" << (uintptr_t*) atField << ">" << std::endl;
        }
    } else {
        return;
    }
}













MiniInterpreter::MiniInterpreter(jited::ast::ProgramPtr program) : program(program), parentStruct(NULL) {
    for (jited::ast::DeclarationPtr d : program->decls) {
        // get the globals
        TypedValuePtr decRes = d->accept(this);
        globals.insert({d->name, decRes});
    }

    for (jited::ast::TypeDeclarationPtr td : program->types) {
        structs.insert({td->name, td});
    }

    for (jited::ast::FunctionPtr f : program->funcs) {
        funcs.insert({f->name, f});
    };
};
MiniInterpreter::~MiniInterpreter() {};

void MiniInterpreter::resetStruct() {
    lastField = "";
    parentStruct = NULL;
}

bool MiniInterpreter::isBool(std::string eType) {
    return eType == typeid(jited::ast::TrueExpression).name() || eType == typeid(jited::ast::FalseExpression).name();
}

bool MiniInterpreter::getBoolState(TypedValue& tv) {
    if (!tv.initialized) {
        throw std::runtime_error("use of uninitialized value");
    } else if (tv.type == jited::ast::IntType::name()) {
        return tv.asInt();
    } else if (tv.type == jited::ast::BoolType::name()) {
        return tv.asBool();
    } else {
        throw std::runtime_error("error: cannot extract bool state of non-int/bool");
    }
}

TypedValuePtr MiniInterpreter::lookup(std::string name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name);
        }
    }
    if (globals.count(name)) {
        return globals.at(name);
    }
    return nullptr;
}

// TODO WATCH THIS, THIS MAY NEED TO CHANGE + SCOPES IN GENERAL MAY NEED TO...
ValueMap MiniInterpreter::bindArgs(std::vector<jited::ast::ExpressionPtr>& args, std::vector<jited::ast::DeclarationPtr>& params) {
    ValueMap newScope;
    if (params.size() != args.size()) {
        throw std::runtime_error("arg and param list were not the same length");
    }
    for (int i = 0; i < args.size(); ++i) {
        TypedValue tv = args.at(i)->accept(this);
        TypedValuePtr dec = params.at(i)->accept(this);
        if (tv.type != dec->type && !(tv.isStruct() && dec->isStruct())) {
            throw std::runtime_error("incorrect type for arg-param binding");
        }
        // dec->value = tv.value;
        dec->setValue(tv.value);
        // newScope.insert({params.at(i)->name, dec});
        newScope[params.at(i)->name] = dec;
    }
    return newScope;
}

Any MiniInterpreter::visit(jited::ast::AssignmentStatement* statement) {
    TypedValue source = statement->source->accept(this);

    resetStruct();
    Any targetRes = statement->target->accept(this);
    TypedValue* target = (targetRes.is<TypedValuePtr>()) ? targetRes.as<TypedValuePtr>().get() : targetRes.as<TypedValue*>();
    if (target->type == source.type || (target->isStruct() && source.isStruct())) {
        target->setValue(source.value, parentStruct, lastField);
    } else {
        // error
        throw std::runtime_error("error in assignment");
    }
    resetStruct();
    return nullptr;
}

// this is gonna be a monster
Any MiniInterpreter::visit(jited::ast::BinaryExpression* expression) {
    // holy shit this is a mess...
    TypedValue lft = expression->left->accept(this), rht = expression->right->accept(this);
    if (!lft.initialized || !rht.initialized) {
        throw std::runtime_error("use of uninitialized value");
    }
    /*
    TIMES,DIVIDE,PLUS,MINUS,
    LT,LE,GT,GE,
    EQ,NE,
    AND,OR
    */

    // these are done in advance since the operand types aren't constrained
    if (expression->op == jited::ast::BinaryExpression::Operator::EQ) {
        if (lft.isStruct() && rht.isStruct()) {
            return TypedValue(jited::ast::BoolType::name(), lft.as<PackedStruct*>() == rht.as<PackedStruct*>());
        } else if (lft.type != rht.type) {
            return TypedValue(jited::ast::BoolType::name(), false);
        } else if (lft.type == jited::ast::IntType::name()) {
            return TypedValue(jited::ast::BoolType::name(), lft.asInt() == rht.asInt());
        } else if (lft.type == jited::ast::BoolType::name()) {
            return TypedValue(jited::ast::BoolType::name(), lft.asBool() == rht.asBool());
        } else {
            return TypedValue(jited::ast::BoolType::name(), false);
            // throw std::runtime_error("struct comparison unimplemented");
        }
    } else if (expression->op == jited::ast::BinaryExpression::Operator::NE) {
        if (lft.isStruct() && rht.isStruct()) {
            return TypedValue(jited::ast::BoolType::name(), lft.as<PackedStruct*>() != rht.as<PackedStruct*>());
        } else if (lft.type != rht.type) {
            return TypedValue(jited::ast::BoolType::name(), true);
        } else if (lft.type == jited::ast::IntType::name()) {
            return TypedValue(jited::ast::BoolType::name(), lft.asInt() != rht.asInt());
        } else if (lft.type == jited::ast::BoolType::name()) {
            return TypedValue(jited::ast::BoolType::name(), lft.asBool() != rht.asBool());
        } else {
            return TypedValue(jited::ast::BoolType::name(), true); // ??
        }
    }

    if (lft.type == jited::ast::IntType::name() && rht.type == jited::ast::IntType::name()) {
        int lftVal = lft.asInt(), rhtVal = rht.asInt();
        switch (expression->op) {
            case jited::ast::BinaryExpression::Operator::TIMES:
                return TypedValue(jited::ast::IntType::name(), lftVal * rhtVal);
            case jited::ast::BinaryExpression::Operator::DIVIDE:
                return TypedValue(jited::ast::IntType::name(), lftVal / rhtVal);
            case jited::ast::BinaryExpression::Operator::PLUS:
                return TypedValue(jited::ast::IntType::name(), lftVal + rhtVal);
            case jited::ast::BinaryExpression::Operator::MINUS:
                return TypedValue(jited::ast::IntType::name(), lftVal - rhtVal);
            case jited::ast::BinaryExpression::Operator::LT:
                return TypedValue(jited::ast::BoolType::name(), lftVal < rhtVal);
            case jited::ast::BinaryExpression::Operator::LE:
                return TypedValue(jited::ast::BoolType::name(), lftVal <= rhtVal);
            case jited::ast::BinaryExpression::Operator::GT:
                return TypedValue(jited::ast::BoolType::name(), lftVal > rhtVal);
            case jited::ast::BinaryExpression::Operator::GE:
                return TypedValue(jited::ast::BoolType::name(), lftVal >= rhtVal);
            default:
                break;
        }
    } else if (lft.type == jited::ast::BoolType::name() && rht.type == jited::ast::BoolType::name()) {
        bool lftVal = lft.asBool(), rhtVal = rht.asBool();
        switch (expression->op) {
            case jited::ast::BinaryExpression::Operator::AND:
                return TypedValue(jited::ast::BoolType::name(), lftVal && rhtVal);
            case jited::ast::BinaryExpression::Operator::OR:
                return TypedValue(jited::ast::BoolType::name(), lftVal || rhtVal);
            default:
                break;
        }
    }
    
    return nullptr;
}

Any MiniInterpreter::visit(jited::ast::BlockStatement* statement) {
    for (jited::ast::StatementPtr s : statement->statements) {
        Any result = s->accept(this);
        if (result.is<TypedValue>()) { // add check -- are we in a function?
            return result;
        }
    }
    return nullptr;
}

Any MiniInterpreter::visit(jited::ast::BoolType* type) {
    return jited::ast::BoolType::name();
}

Any MiniInterpreter::visit(jited::ast::ConditionalStatement* statement) {
    TypedValue guard = statement->guard->accept(this);

    if (getBoolState(guard)) {
        return statement->thenBlock->accept(this);
    } else {
        return statement->elseBlock->accept(this);
    }
}

Any MiniInterpreter::visit(jited::ast::Declaration* declaration) {
    std::string typeStr = declaration->type->accept(this);
    if (typeStr == jited::ast::IntType::name()) {
        return std::make_shared<TypedValue>(typeStr, 0, false);
    } else if (typeStr == jited::ast::BoolType::name()) {
        return std::make_shared<TypedValue>(typeStr, false, false);
    } else if (typeStr == jited::ast::VoidType::name()) {
        std::cerr << "error: cannot instantiate a variable with void type\n";
        std::exit(1);
    } else {
        return std::make_shared<TypedValue>(typeStr, (PackedStruct*) nullptr, false);
    }
    return nullptr;
}

Any MiniInterpreter::visit(jited::ast::DeleteStatement* statement) {
    TypedValue toDelete = statement->expression->accept(this);
    delete toDelete.as<PackedStruct*>();
    return nullptr;
}

Any MiniInterpreter::visit(jited::ast::DotExpression* expression) {
    TypedValue left = expression->left->accept(this);
    if (!(left.isStruct())) {
        throw std::runtime_error(std::to_string(expression->line) + ": invalid struct for dot expression");
    } else if (left.isNull()) {
        throw std::runtime_error(std::to_string(expression->line) + ": null used in dot expression");
    } else if (!left.initialized) {
        throw std::runtime_error("use of uninitialized value");
    }
    PackedStruct* ps = left.as<PackedStruct*>();
    // parentStruct = ps; // OR THIS?
    // lastField = expression->id; // DO NOT DO THIS.
    // this should be left raw, no check
    // TODO THIS IS WHERE THINGS ARE BREAKING...
    if (ps->has(expression->id)) {
        return *(ps->at(expression->id));
    } else {
        throw std::runtime_error(std::to_string(expression->line) + ": invalid field in dot expression");
    }
}

Any MiniInterpreter::visit(jited::ast::FalseExpression* expression) {
    return TypedValue(jited::ast::BoolType::name(), false);
}

Any MiniInterpreter::visit(jited::ast::Function* function) {
    for (jited::ast::DeclarationPtr local : function->locals) {
        TypedValuePtr evaluated = local->accept(this);
        // evaluated->initialized = true; //unsure... TODO
        // scopes.back().insert({local->name, evaluated});
        scopes.back()[local->name] = evaluated;
    }
    return function->body->accept(this);
}

Any MiniInterpreter::visit(jited::ast::IdentifierExpression* expression) {
    TypedValuePtr p = lookup(expression->id);
    if (p) {
        return *p;
    }
    return nullptr;
}

Any MiniInterpreter::visit(jited::ast::IntegerExpression* expression) {
    return TypedValue(jited::ast::IntType::name(), expression->value);
}

Any MiniInterpreter::visit(jited::ast::IntType* type) {
    return jited::ast::IntType::name();
}

Any MiniInterpreter::visit(jited::ast::InvocationExpression* expression) {
    /* we may need more in-depth scoping rules than this, fwiw */
    if (funcs.count(expression->name) == 0) {
        throw std::runtime_error(std::to_string(expression->line) + ": call to undefined function");
    }
    jited::ast::FunctionPtr f = funcs.at(expression->name);

    ValueMap s = bindArgs(expression->arguments, f->params);
    scopes.push_back(s);

    // watch on this: shouldn't cause any issues, since it being returned as an Any
    // just ends up being casted later on
    Any retVal = f->accept(this);
    scopes.pop_back();
    return retVal;
}

Any MiniInterpreter::visit(jited::ast::InvocationStatement* statement) {
    // possible that this'll need a check to make sure it's a fn within
    // but the parser should rule that out, probably.
    Any retVal = statement->expression->accept(this);
    return nullptr;
}

Any MiniInterpreter::visit(jited::ast::LvalueDot* lvalue) {
    TypedValue left = lvalue->left->accept(this);
    if (!(left.isStruct())) {
        throw std::runtime_error(std::to_string(lvalue->line) + ": invalid lvalue for dot expression");
    } else if (left.isNull()) {
        throw std::runtime_error(std::to_string(lvalue->line) + ": null lvalue for dot expression");
    } else if (!left.initialized) {
        throw std::runtime_error("use of uninitialized value");
    }

    PackedStruct* ps = left.as<PackedStruct*>();
    // is this just for the lvalue...?
    parentStruct = ps;
    lastField = lvalue->id;
    
    // still feels gross
    if (ps->has(lvalue->id)) {
        return ps->at(lvalue->id);
    } else {
        throw std::runtime_error(std::to_string(lvalue->line) + ": invalid field in dot expression");
    }
}

Any MiniInterpreter::visit(jited::ast::LvalueId* lvalue) {
    return lookup(lvalue->id);
}

Any MiniInterpreter::visit(jited::ast::NewExpression* expression) {
    if (structs.count(expression->id)) {
        // return TypedValue(expression->id, new ValueMap(structs.at(expression->id)), true);
        return TypedValue(expression->id, new PackedStruct(structs.at(expression->id)));
    } else {
        throw std::runtime_error(std::to_string(expression->line) + ": new expression of nonexistent struct");
    }
}

Any MiniInterpreter::visit(jited::ast::NullExpression* expression) {
    return TypedValue(jited::ast::Type::nullName(), (PackedStruct*) NULL);
}

Any MiniInterpreter::visit(jited::ast::PrintLnStatement* statement) {
    TypedValue result = statement->expression->accept(this);
    if (!result.initialized) {
        throw std::runtime_error("use of uninitialized value");
    }
    std::cout << result.toString() << std::endl;
    return nullptr;
}

Any MiniInterpreter::visit(jited::ast::PrintStatement* statement) {
    TypedValue result = statement->expression->accept(this);
    if (!result.initialized) {
        throw std::runtime_error("use of uninitialized value");
    }
    std::cout << result.toString() << ' ';
    return nullptr;
}

Any MiniInterpreter::visit(jited::ast::Program* program) {
    if (program != this->program.get()) {
        std::cerr << "error: interpreter called on different program than initialized with\n";
        return 1;
    }

    // globals.clear();
    // funcs.clear();
    // structs.clear();

    scopes.clear();
    scopes.push_back(ValueMap()); // scope for main
    // for (jited::ast::DeclarationPtr d : program->decls) {
    //     // get the globals
    //     TypedValuePtr decRes = d->accept(this);
    //     globals.insert({d->name, decRes});
    // }

    // for (jited::ast::TypeDeclarationPtr td : program->types) {
    //     // structs.insert({td->name, td->accept(this)});
    //     structs.insert({td->name, td});
    // }

    // // check to see if main actually returned something?
    // jited::ast::FunctionPtr mainFn = nullptr;
    // for (jited::ast::FunctionPtr f : program->funcs) {
    //     if (f->name == "main") {
    //         mainFn = f;
    //     }
    //     funcs.insert({f->name, f});
    // };
    
    if (funcs.count("main") == 0) {
        std::cerr << "error: no main function in program\n";
        return -1;
    }

    jited::ast::FunctionPtr mainFn = funcs.at("main");


    Any result = mainFn->accept(this);
    if (!result.is<TypedValue>()) {
        std::cerr << "error: main did not return a value\n";
        return 1;
    }
    TypedValue retVal = result.as<TypedValue>();
    if (retVal.type != jited::ast::IntType::name()) {
        std::cerr << "error: main did not return an int\n";
        return 1;
    } else {
        return retVal.asInt();
    }
    return 1;
}

Any MiniInterpreter::visit(jited::ast::ReadExpression* expression) {
    int val;
    std::cin >> val;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return TypedValue(jited::ast::IntType::name(), 0);
    } else {
        return TypedValue(jited::ast::IntType::name(), val);
    }
}

Any MiniInterpreter::visit(jited::ast::ReturnEmptyStatement* statement) {
    // feels strange for TypedValue to contain Void, but it's distinct from not returning anything
    // TypedValue is mostly to indicate return values or expression results
    return TypedValue(jited::ast::VoidType::name(), nullptr);
}

Any MiniInterpreter::visit(jited::ast::ReturnStatement* statement) {
    return statement->expression->accept(this);
}

Any MiniInterpreter::visit(jited::ast::StructType* type) {
    return type->name();
}

Any MiniInterpreter::visit(jited::ast::TrueExpression* expression) {
    return TypedValue(jited::ast::BoolType::name(), true);
}

Any MiniInterpreter::visit(jited::ast::TypeDeclaration* typeDeclaration) {
    // ValueMap m;
    // for (auto dec : typeDeclaration->fields) {
    //     m.insert({dec->name, dec->accept(this)});
    // }
    // return m;
    return nullptr;
}

Any MiniInterpreter::visit(jited::ast::UnaryExpression* expression) {
    TypedValue operand = expression->operand->accept(this);
    if (!operand.initialized) {
        throw std::runtime_error("use of uninitialized value");
    }

    if (expression->op == jited::ast::UnaryExpression::NOT && operand.type == jited::ast::BoolType::name()) {
        return TypedValue(operand.type, !operand.asBool());
    } else if (expression->op == jited::ast::UnaryExpression::MINUS && operand.type == jited::ast::IntType::name()) {
        return TypedValue(operand.type, -operand.asInt());
    }
}

Any MiniInterpreter::visit(jited::ast::VoidType* type) {
    return jited::ast::VoidType::name();
}

Any MiniInterpreter::visit(jited::ast::WhileStatement* statement) {
    TypedValue guard = statement->guard->accept(this);
    while (getBoolState(guard)) {
        Any result = statement->body->accept(this);
        if (result.is<TypedValue>()) {
            return result;
        }

        // check for next iteration
        guard = statement->guard->accept(this);
    }
    return nullptr;
}

int MiniInterpreter::run() {
    int exitCode = program->accept(this);
    // PackedStruct* x = lookup("head")->asStruct();
    // uint8_t* nextBuf = x->getPtr("next");
    // uint8_t* offset = nextBuf + 4;
    // // intptr_t nb = *((intptr_t*) offset);
    // uint8_t* nextNext = *(uint8_t**) offset;
    // // std::cout << (void*) nb;
    // // std::cout << nextNext << std::endl;
    // std::cout << ((int32_t*) nextNext)[0] << std::endl;
    return exitCode;
}

}