#pragma once


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

/*
    Container class for both return values (or lack thereof) and expression evaluation results
*/
namespace mini {
class TypedValue {
public:
    typedef std::map<std::string, std::shared_ptr<TypedValue>>* _RawValueMapPtr;

    antlrcpp::Any value;
    std::string type;
    bool isStruct;

    TypedValue(const std::string type, antlrcpp::Any value, bool isStruct=false);

    template <typename T>
    T get();

    // bool isStruct();
    bool isNull();
    std::string toString();
};
typedef std::shared_ptr<TypedValue> TypedValuePtr;
typedef std::map<std::string, TypedValuePtr> ValueMap;

class MiniInterpreter : public ast::ASTVisitor {
protected:
    bool isBool(std::string eType);

    bool getBoolState(TypedValue& tv);

    ValueMap globals;
    std::vector<ValueMap> scopes;
    std::map<std::string, ast::FunctionPtr> funcs;
    std::map<std::string, ValueMap> structs; // instances of structs to copy from
    ast::ProgramPtr program;

    TypedValuePtr lookup(std::string name);

    ValueMap bindArgs(std::vector<ast::ExpressionPtr>& args, std::vector<ast::DeclarationPtr>& params);
public:
    MiniInterpreter(ast::ProgramPtr program);

    antlrcpp::Any visit(ast::AssignmentStatement* statement);

    antlrcpp::Any visit(ast::BinaryExpression* expression) override;

    antlrcpp::Any visit(ast::BlockStatement* statement) override;

    antlrcpp::Any visit(ast::BoolType* type) override;

    antlrcpp::Any visit(ast::ConditionalStatement* statement) override;

    antlrcpp::Any visit(ast::Declaration* declaration) override;

    antlrcpp::Any visit(ast::DeleteStatement* statement) override;

    antlrcpp::Any visit(ast::DotExpression* expression) override;

    antlrcpp::Any visit(ast::FalseExpression* expression) override;

    antlrcpp::Any visit(ast::Function* function) override;

    antlrcpp::Any visit(ast::IdentifierExpression* expression) override;

    antlrcpp::Any visit(ast::IntegerExpression* expression) override;

    antlrcpp::Any visit(ast::IntType* type) override;

    antlrcpp::Any visit(ast::InvocationExpression* expression) override;

    antlrcpp::Any visit(ast::InvocationStatement* statement) override;

    antlrcpp::Any visit(ast::LvalueDot* lvalue) override;

    antlrcpp::Any visit(ast::LvalueId* lvalue) override;

    antlrcpp::Any visit(ast::NewExpression* expression) override;

    antlrcpp::Any visit(ast::NullExpression* expression) override;

    antlrcpp::Any visit(ast::PrintLnStatement* statement) override;

    antlrcpp::Any visit(ast::PrintStatement* statement) override;

    antlrcpp::Any visit(ast::Program* program);

    antlrcpp::Any visit(ast::ReadExpression* expression) override;

    antlrcpp::Any visit(ast::ReturnEmptyStatement* statement) override;

    antlrcpp::Any visit(ast::ReturnStatement* statement) override;

    antlrcpp::Any visit(ast::StructType* type) override;

    antlrcpp::Any visit(ast::TrueExpression* expression) override;

    antlrcpp::Any visit(ast::TypeDeclaration* typeDeclaration) override;

    antlrcpp::Any visit(ast::UnaryExpression* expression) override;

    antlrcpp::Any visit(ast::VoidType* type) override;

    antlrcpp::Any visit(ast::WhileStatement* statement) override;

    int run();
};

}
