
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

using antlrcpp::Any;
using mini::proxy;

// wtf? why is this necessary
Any::~Any() {};

/*
    Container class for both return values (or lack thereof) and expression evaluation results
*/
class TypedValue {
public:

    Any value;
    std::string type;

    TypedValue(const std::string type, Any value) : type(type), value(value) {};

    template <typename T>
    T get() {
        // UNSAFE
        return value.as<T>();
    }

    std::string toString() {
        if (type == ast::IntType::name()) {
            return std::to_string(value.as<int>());
        } else if (type == ast::BoolType::name()) {
            return (value.as<bool>()) ? "true" : "false";
        } else if (type == ast::VoidType::name()) {
            return "void"; // ???
        } else if (type == ast::Type::name()) {
            return "null";
        } else {
            return "struct_TODO";
        }
    }
};
typedef std::shared_ptr<TypedValue> TypedValuePtr;


class MiniInterpreter : ast::ASTVisitor {
private:
    bool isBool(std::string eType) {
        return eType == typeid(ast::TrueExpression).name() || eType == typeid(ast::FalseExpression).name();
    }

    bool getBoolState(TypedValue& tv) {
        if (tv.type == ast::IntType::name()) {
            return tv.get<int>();
        } else if (tv.type == ast::BoolType::name()) {
            return tv.get<bool>();
        } else {
            throw std::runtime_error("error: cannot extract bool state of non-int/bool");
        }
    }

    typedef std::map<std::string, TypedValuePtr> Scope;

    Scope globals;
    std::vector<Scope> scopes;
    std::map<std::string, ast::FunctionPtr> funcs;
    ast::ProgramPtr program;

    TypedValuePtr lookup(std::string name) {
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

    Scope bindArgs(std::vector<ast::ExpressionPtr>& args, std::vector<ast::DeclarationPtr>& params) {
        Scope newScope;
        if (params.size() != args.size()) {
            throw std::runtime_error("arg and param list were not the same length");
        }
        for (int i = 0; i < args.size(); ++i) {
            TypedValue tv = args.at(i)->accept(this);
            TypedValuePtr dec = params.at(i)->accept(this);
            if (tv.type != dec->type) {
                throw std::runtime_error("incorrect type for arg-param binding");
            }
            dec->value = tv.value;
            newScope.insert({params.at(i)->name, dec});
        }
        return newScope;
    }
public:
    MiniInterpreter(ast::ProgramPtr program) : program(program) {};

    Any visit(ast::AssignmentStatement* statement) override {
        TypedValuePtr target = statement->target->accept(this);
        if (!target) {
            // error ...?
            return nullptr;
        }
        TypedValue source = statement->source->accept(this);
        // TODO: CHANGE FALSE FOR STRUCT CONDITIONS
        if (target->type == source.type || false) {
            target->value = source.value;
        } else {
            // error
        }
        return nullptr;
    }

    // this is gonna be a monster
    Any visit(ast::BinaryExpression* expression) override {
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
            if (lft.type != rht.type) {
                return TypedValue(ast::BoolType::name(), false);
            } else if (lft.type == ast::IntType::name()) {
                return TypedValue(ast::BoolType::name(), lft.get<int>() == rht.get<int>());
            } else if (lft.type == ast::BoolType::name()) {
                return TypedValue(ast::BoolType::name(), lft.get<bool>() == rht.get<bool>());
            } else {
                throw std::runtime_error("struct comparison unimplemented");
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

    Any visit(ast::BlockStatement* statement) override {
        for (ast::StatementPtr s : statement->statements) {
            Any result = s->accept(this);
            if (result.is<TypedValue>()) { // add check -- are we in a function?
                return result;
            }
        }
        return nullptr;
    }

    Any visit(ast::BoolType* type) override {
        return ast::BoolType::name();
    }

    Any visit(ast::ConditionalStatement* statement) override {
        TypedValue guard = statement->guard->accept(this);

        if (getBoolState(guard)) {
            return statement->thenBlock->accept(this);
        } else {
            return statement->elseBlock->accept(this);
        }
    }

    Any visit(ast::Declaration* declaration) override {
        std::string typeStr = declaration->type->accept(this);
        if (typeStr == ast::IntType::name()) {
            return std::make_shared<TypedValue>(typeStr, 0);
        } else if (typeStr == ast::BoolType::name()) {
            return std::make_shared<TypedValue>(typeStr, false);
        } else if (typeStr == ast::VoidType::name()) {
            std::cerr << "error: cannot instantiate a variable with void type\n";
            std::exit(1);
        } else {
            throw std::runtime_error("struct variables not implemented");
        }
        return nullptr;
    }

    // Any visit(ast::DeleteStatement* statement) override
    // Any visit(ast::DotExpression* expression) override

    Any visit(ast::FalseExpression* expression) override {
        return TypedValue(ast::BoolType::name(), false);
    }

    Any visit(ast::Function* function) override {
        for (ast::DeclarationPtr local : function->locals) {
            TypedValuePtr evaluated = local->accept(this);
            scopes.back().insert({local->name, evaluated});
        }
        return function->body->accept(this);
    }

    Any visit(ast::IdentifierExpression* expression) override {
        TypedValuePtr p = lookup(expression->id);
        if (p) {
            return *p;
        }
        return nullptr;
    }

    Any visit(ast::IntegerExpression* expression) override {
        return TypedValue(ast::IntType::name(), expression->value);
    }

    Any visit(ast::IntType* type) override {
        return ast::IntType::name();
    }

    Any visit(ast::InvocationExpression* expression) override {
        /* we may need more in-depth scoping rules than this, fwiw */
        if (funcs.count(expression->name) == 0) {
            throw std::runtime_error("call to undefined function");
        }
        ast::FunctionPtr f = funcs.at(expression->name);

        Scope s = bindArgs(expression->arguments, f->params);
        scopes.push_back(s);

        TypedValue retVal = f->accept(this);
        scopes.pop_back();
        return retVal;
    }

    Any visit(ast::InvocationStatement* statement) override {
        // possible that this'll need a check to make sure it's a fn within
        // but the parser should rule that out, probably.
        return statement->expression->accept(this);
    }
    // Any visit(ast::LvalueDot* lvalue) override

    Any visit(ast::LvalueId* lvalue) override {
        // should return a TypedValuePtr
        return lookup(lvalue->id);
    }

    // Any visit(ast::NewExpression* expression) override

    Any visit(ast::NullExpression* expression) override {
        // currently Type::name() is null; perhaps unintuitive?
        return TypedValue(ast::Type::name(), nullptr);
    }

    Any visit(ast::PrintLnStatement* statement) override {
        // is it safe to assume ~~Some~~ TypedVal? this is getting a bit cumbersome
        TypedValue result = statement->expression->accept(this);
        std::cout << result.toString() << std::endl;
        return nullptr;
    }

    Any visit(ast::PrintStatement* statement) override {
        TypedValue result = statement->expression->accept(this);
        std::cout << result.toString() << ' ';
        return nullptr;
    }

    Any visit(ast::Program* program) {
        if (program != this->program.get()) {
            std::cerr << "error: interpreter called on different program than initialized with\n";
            return 1;
        }

        globals.clear();
        scopes.clear();
        funcs.clear();
        scopes.push_back(Scope()); // scope for main
        for (ast::DeclarationPtr d : program->decls) {
            // get the globals
            TypedValuePtr decRes = d->accept(this);
            globals.insert({d->name, decRes});
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

        TypedValue result = mainFn->accept(this);
        if (result.type != ast::IntType::name()) {
            std::cerr << "error: main did not return an int\n";
            return 1;
        } else {
            return result.get<int>();
        }
        return 1;
    }

    Any visit(ast::ReadExpression* expression) override {
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

    Any visit(ast::ReturnEmptyStatement* statement) override {
        // feels strange for TypedValue to contain Void, but it's distinct from not returning anything
        // TypedValue is mostly to indicate return values or expression results
        return TypedValue(ast::VoidType::name(), nullptr);
    }

    Any visit(ast::ReturnStatement* statement) override {
        return statement->expression->accept(this);
    }

    Any visit(ast::StructType* type) override {
        return type->name();
    }

    Any visit(ast::TrueExpression* expression) override {
        return TypedValue(ast::BoolType::name(), true);
    }

    // Any visit(ast::TypeDeclaration* typeDeclaration) override

    Any visit(ast::UnaryExpression* expression) override {
        TypedValue operand = expression->operand->accept(this);
        if (expression->op == ast::UnaryExpression::NOT && operand.type == ast::BoolType::name()) {
            return TypedValue(operand.type, !operand.get<bool>());
        } else if (expression->op == ast::UnaryExpression::MINUS && operand.type == ast::IntType::name()) {
            return TypedValue(operand.type, -operand.get<int>());
        }
    }

    Any visit(ast::VoidType* type) override {
        return ast::VoidType::name();
    }

    Any visit(ast::WhileStatement* statement) override {
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

    int run() {
        return program->accept(this);
    }
};

int main(int argc, char** argv) {
    if (argc == 1) {
        return 0;
    }
    antlr4::ANTLRFileStream input(argv[1]);
    MiniLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    MiniParser parser(&tokens);
    antlr4::tree::ParseTree* tree = parser.program();

    if (parser.getNumberOfSyntaxErrors() == 0) {
        MiniToAstProgramVisitor programVisitor;
        ast::ProgramPtr p = programVisitor.visit(tree);
        MiniInterpreter interpreter(p);
        return interpreter.run();
    } else {
        std::cout << "could not parse input file\n";
        return 1;
    }
}
