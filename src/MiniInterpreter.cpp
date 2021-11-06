
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
using mini::proxy;

namespace mini {

/*
    Container class for both return values (or lack thereof) and expression evaluation results
*/
TypedValue::TypedValue(const std::string type, Any value) : type(type), value(value) {};

template <typename T>
T TypedValue::get() {
    // UNSAFE
    return value.as<T>();
}

bool TypedValue::isStruct() {
    return value.is<_RawValueMapPtr>();
}

bool TypedValue::isNull() {
    return isStruct() && value.as<_RawValueMapPtr>() == nullptr;
}

std::string TypedValue::toString() {
    if (type == ast::IntType::name()) {
        return std::to_string(value.as<int>());
    } else if (type == ast::BoolType::name()) {
        return (value.as<bool>()) ? "true" : "false";
    } else if (type == ast::VoidType::name()) {
        return "void"; // should not be returned from anything...
    } else if (isNull()) {
        return "null";
    } else {
        std::ostringstream oss;
        oss << value.as<_RawValueMapPtr>();
        return oss.str();
    }
}

















bool MiniInterpreter::isBool(std::string eType) {
    return eType == typeid(ast::TrueExpression).name() || eType == typeid(ast::FalseExpression).name();
}

bool MiniInterpreter::getBoolState(TypedValue& tv) {
    if (tv.type == ast::IntType::name()) {
        return tv.get<int>();
    } else if (tv.type == ast::BoolType::name()) {
        return tv.get<bool>();
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

ValueMap MiniInterpreter::bindArgs(std::vector<ast::ExpressionPtr>& args, std::vector<ast::DeclarationPtr>& params) {
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
        dec->value = tv.value;
        // newScope.insert({params.at(i)->name, dec});
        newScope[params.at(i)->name] = dec;
    }
    return newScope;
}
MiniInterpreter::MiniInterpreter(ast::ProgramPtr program) : program(program) {};

Any MiniInterpreter::visit(ast::AssignmentStatement* statement) {
    TypedValuePtr target = statement->target->accept(this);
    if (!target) {
        // error ...?
        return nullptr;
    }
    TypedValue source = statement->source->accept(this);
    if (target->type == source.type || (target->isStruct() && source.isStruct())) {
        target->value = source.value;
    } else {
        // error
    }
    return nullptr;
}

// this is gonna be a monster
Any MiniInterpreter::visit(ast::BinaryExpression* expression) {
    // holy shit this is a mess...
    TypedValue lft = expression->left->accept(this), rht = expression->right->accept(this);
    /*
    TIMES,DIVIDE,PLUS,MINUS,
    LT,LE,GT,GE,
    EQ,NE,
    AND,OR
    */

    // these are done in advance since the operand types aren't constrained
    if (expression->op == ast::BinaryExpression::Operator::EQ) {
        if (lft.isStruct() && rht.isStruct()) {
            return TypedValue(ast::BoolType::name(), lft.get<ValueMap*>() == rht.get<ValueMap*>());
        } else if (lft.type != rht.type) {
            return TypedValue(ast::BoolType::name(), false);
        } else if (lft.type == ast::IntType::name()) {
            return TypedValue(ast::BoolType::name(), lft.get<int>() == rht.get<int>());
        } else if (lft.type == ast::BoolType::name()) {
            return TypedValue(ast::BoolType::name(), lft.get<bool>() == rht.get<bool>());
        } else {
            return TypedValue(ast::BoolType::name(), false);
            // throw std::runtime_error("struct comparison unimplemented");
        }
    } else if (expression->op == ast::BinaryExpression::Operator::NE) {
        if (lft.type != rht.type) {
            return TypedValue(ast::BoolType::name(), true);
        }
    }

    if (lft.type == ast::IntType::name() && rht.type == ast::IntType::name()) {
        int lftVal = lft.get<int>(), rhtVal = rht.get<int>();
        switch (expression->op) {
            case ast::BinaryExpression::Operator::TIMES:
                return TypedValue(ast::IntType::name(), lftVal * rhtVal);
            case ast::BinaryExpression::Operator::DIVIDE:
                return TypedValue(ast::IntType::name(), lftVal / rhtVal);
            case ast::BinaryExpression::Operator::PLUS:
                return TypedValue(ast::IntType::name(), lftVal + rhtVal);
            case ast::BinaryExpression::Operator::MINUS:
                return TypedValue(ast::IntType::name(), lftVal - rhtVal);
            case ast::BinaryExpression::Operator::LT:
                return TypedValue(ast::BoolType::name(), lftVal < rhtVal);
            case ast::BinaryExpression::Operator::LE:
                return TypedValue(ast::BoolType::name(), lftVal <= rhtVal);
            case ast::BinaryExpression::Operator::GT:
                return TypedValue(ast::BoolType::name(), lftVal > rhtVal);
            case ast::BinaryExpression::Operator::GE:
                return TypedValue(ast::BoolType::name(), lftVal >= rhtVal);
            default:
                break;
        }
    } else if (lft.type == ast::BoolType::name() && rht.type == ast::BoolType::name()) {
        bool lftVal = lft.get<bool>(), rhtVal = rht.get<bool>();
        switch (expression->op) {
            case ast::BinaryExpression::Operator::AND:
                return TypedValue(ast::BoolType::name(), lftVal && rhtVal);
            case ast::BinaryExpression::Operator::OR:
                return TypedValue(ast::BoolType::name(), lftVal || rhtVal);
            default:
                break;
        }
    }
    
    return nullptr;
}

Any MiniInterpreter::visit(ast::BlockStatement* statement) {
    for (ast::StatementPtr s : statement->statements) {
        Any result = s->accept(this);
        if (result.is<TypedValue>()) { // add check -- are we in a function?
            return result;
        }
    }
    return nullptr;
}

Any MiniInterpreter::visit(ast::BoolType* type) {
    return ast::BoolType::name();
}

Any MiniInterpreter::visit(ast::ConditionalStatement* statement) {
    TypedValue guard = statement->guard->accept(this);

    if (getBoolState(guard)) {
        return statement->thenBlock->accept(this);
    } else {
        return statement->elseBlock->accept(this);
    }
}

Any MiniInterpreter::visit(ast::Declaration* declaration) {
    std::string typeStr = declaration->type->accept(this);
    if (typeStr == ast::IntType::name()) {
        return std::make_shared<TypedValue>(typeStr, 0);
    } else if (typeStr == ast::BoolType::name()) {
        return std::make_shared<TypedValue>(typeStr, false);
    } else if (typeStr == ast::VoidType::name()) {
        std::cerr << "error: cannot instantiate a variable with void type\n";
        std::exit(1);
    } else {
        return std::make_shared<TypedValue>(typeStr, static_cast<ValueMap*>(nullptr));
    }
    return nullptr;
}

Any MiniInterpreter::visit(ast::DeleteStatement* statement) {
    TypedValue toDelete = statement->expression->accept(this);
    delete toDelete.get<ValueMap*>();
    return nullptr;
}

Any MiniInterpreter::visit(ast::DotExpression* expression) {
    TypedValue left = expression->left->accept(this);
    if (!(left.isStruct())) {
        throw std::runtime_error(std::to_string(expression->line) + ": invalid struct for dot expression");
    } else if (left.isNull()) {
        throw std::runtime_error(std::to_string(expression->line) + ": null used in dot expression");
    }
    ValueMap* vm = left.get<ValueMap*>();
    // this should be left raw, no check
    if (vm->count(expression->id)) {
        return *(vm->at(expression->id));
    } else {
        throw std::runtime_error(std::to_string(expression->line) + ": invalid field in dot expression");
    }
}

Any MiniInterpreter::visit(ast::FalseExpression* expression) {
    return TypedValue(ast::BoolType::name(), false);
}

Any MiniInterpreter::visit(ast::Function* function) {
    for (ast::DeclarationPtr local : function->locals) {
        TypedValuePtr evaluated = local->accept(this);
        // scopes.back().insert({local->name, evaluated});
        scopes.back()[local->name] = evaluated;
    }
    return function->body->accept(this);
}

Any MiniInterpreter::visit(ast::IdentifierExpression* expression) {
    TypedValuePtr p = lookup(expression->id);
    if (p) {
        return *p;
    }
    return nullptr;
}

Any MiniInterpreter::visit(ast::IntegerExpression* expression) {
    return TypedValue(ast::IntType::name(), expression->value);
}

Any MiniInterpreter::visit(ast::IntType* type) {
    return ast::IntType::name();
}

Any MiniInterpreter::visit(ast::InvocationExpression* expression) {
    /* we may need more in-depth scoping rules than this, fwiw */
    if (funcs.count(expression->name) == 0) {
        throw std::runtime_error(std::to_string(expression->line) + ": call to undefined function");
    }
    ast::FunctionPtr f = funcs.at(expression->name);

    ValueMap s = bindArgs(expression->arguments, f->params);
    scopes.push_back(s);

    // watch on this: shouldn't cause any issues, since it being returned as an Any
    // just ends up being casted later on
    Any retVal = f->accept(this);
    scopes.pop_back();
    return retVal;
}

Any MiniInterpreter::visit(ast::InvocationStatement* statement) {
    // possible that this'll need a check to make sure it's a fn within
    // but the parser should rule that out, probably.
    Any retVal = statement->expression->accept(this);
    return nullptr;
}

Any MiniInterpreter::visit(ast::LvalueDot* lvalue) {
    TypedValue left = lvalue->left->accept(this);
    if (!(left.isStruct())) {
        throw std::runtime_error(std::to_string(lvalue->line) + ": invalid lvalue for dot expression");
    } else if (left.isNull()) {
        throw std::runtime_error(std::to_string(lvalue->line) + ": null lvalue for dot expression");
    }
    ValueMap* vm = left.get<ValueMap*>();
    // still feels gross
    if (vm->count(lvalue->id)) {
        return vm->at(lvalue->id);
    } else {
        throw std::runtime_error(std::to_string(lvalue->line) + ": invalid field in dot expression");
    }
}

Any MiniInterpreter::visit(ast::LvalueId* lvalue) {
    return lookup(lvalue->id);
}

Any MiniInterpreter::visit(ast::NewExpression* expression) {
    if (structs.count(expression->id)) {
        return TypedValue(expression->id, new ValueMap(structs.at(expression->id)));
    } else {
        throw std::runtime_error(std::to_string(expression->line) + ": new expression of nonexistent struct");
    }
}

Any MiniInterpreter::visit(ast::NullExpression* expression) {
    return TypedValue(ast::Type::nullName(), static_cast<ValueMap*>(nullptr));
}

Any MiniInterpreter::visit(ast::PrintLnStatement* statement) {
    TypedValue result = statement->expression->accept(this);
    std::cout << result.toString() << std::endl;
    return nullptr;
}

Any MiniInterpreter::visit(ast::PrintStatement* statement) {
    TypedValue result = statement->expression->accept(this);
    std::cout << result.toString() << ' ';
    return nullptr;
}

Any MiniInterpreter::visit(ast::Program* program) {
    if (program != this->program.get()) {
        std::cerr << "error: interpreter called on different program than initialized with\n";
        return 1;
    }

    globals.clear();
    scopes.clear();
    funcs.clear();
    structs.clear();

    scopes.push_back(ValueMap()); // scope for main
    for (ast::DeclarationPtr d : program->decls) {
        // get the globals
        TypedValuePtr decRes = d->accept(this);
        globals.insert({d->name, decRes});
    }

    for (ast::TypeDeclarationPtr td : program->types) {
        structs.insert({td->name, td->accept(this)});
    }

    // check to see if main actually returned something?
    ast::FunctionPtr mainFn = nullptr;
    for (ast::FunctionPtr f : program->funcs) {
        if (f->name == "main") {
            mainFn = f;
        }
        funcs.insert({f->name, f});
    };

    if (mainFn == nullptr) {
        std::cerr << "error: no main function in program\n";
        return 1;
    }

    Any result = mainFn->accept(this);
    if (!result.is<TypedValue>()) {
        std::cerr << "error: main did not return a value\n";
        return 1;
    }
    TypedValue retVal = result.as<TypedValue>();
    if (retVal.type != ast::IntType::name()) {
        std::cerr << "error: main did not return an int\n";
        return 1;
    } else {
        return retVal.get<int>();
    }
    return 1;
}

Any MiniInterpreter::visit(ast::ReadExpression* expression) {
    int val;
    std::cin >> val;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return TypedValue(ast::IntType::name(), 0);
    } else {
        return TypedValue(ast::IntType::name(), val);
    }
}

Any MiniInterpreter::visit(ast::ReturnEmptyStatement* statement) {
    // feels strange for TypedValue to contain Void, but it's distinct from not returning anything
    // TypedValue is mostly to indicate return values or expression results
    return TypedValue(ast::VoidType::name(), nullptr);
}

Any MiniInterpreter::visit(ast::ReturnStatement* statement) {
    return statement->expression->accept(this);
}

Any MiniInterpreter::visit(ast::StructType* type) {
    return type->name();
}

Any MiniInterpreter::visit(ast::TrueExpression* expression) {
    return TypedValue(ast::BoolType::name(), true);
}

Any MiniInterpreter::visit(ast::TypeDeclaration* typeDeclaration) {
    ValueMap m;
    for (auto dec : typeDeclaration->fields) {
        m.insert({dec->name, dec->accept(this)});
    }
    return m;
}

Any MiniInterpreter::visit(ast::UnaryExpression* expression) {
    TypedValue operand = expression->operand->accept(this);
    if (expression->op == ast::UnaryExpression::NOT && operand.type == ast::BoolType::name()) {
        return TypedValue(operand.type, !operand.get<bool>());
    } else if (expression->op == ast::UnaryExpression::MINUS && operand.type == ast::IntType::name()) {
        return TypedValue(operand.type, -operand.get<int>());
    }
}

Any MiniInterpreter::visit(ast::VoidType* type) {
    return ast::VoidType::name();
}

Any MiniInterpreter::visit(ast::WhileStatement* statement) {
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
    return program->accept(this);
}

}