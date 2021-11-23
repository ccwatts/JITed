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
#include <cstdint>
#include <vector>
#include <tuple>
#include <limits>

/*
    Container class for both return values (or lack thereof) and expression evaluation results
*/
namespace jited {

typedef std::shared_ptr<int> IntPtr;
typedef std::shared_ptr<bool> BoolPtr;

class PackedStruct;

class TypedValue {
private:
public:
    typedef std::map<std::string, std::shared_ptr<TypedValue>>* _RawValueMapPtr;

    antlrcpp::Any value;
    std::string type;
    bool initialized;

    TypedValue(const std::string type, antlrcpp::Any value, bool initialized=true);

    template <typename T>
    T as();

    bool isStruct();

    void setValue(antlrcpp::Any val, PackedStruct* parentStruct=NULL, std::string lastField="");

    int asInt();
    bool asBool();
    int32_t asI32();
    PackedStruct* asStruct();

    // bool isStruct();
    bool isNull();
    std::string toString();
};
typedef std::shared_ptr<TypedValue> TypedValuePtr;
typedef std::map<std::string, TypedValuePtr> ValueMap;
typedef std::map<std::string, std::pair<std::string, int>> StructDescription;










// TODO
class PackedStruct {
private:
    std::map<std::string, int> offsets;
    std::map<std::string, std::string> types;
    std::map<std::string, TypedValuePtr> accessors; // field name -> int offset
    // int is the offset
    // vector is of the same type as the constructor, used to make another of itself
    // std::map<int, std::pair<int, std::vector<std::pair<std::string, std::string>>>> childStructs;
    // std::map<std::string, StructDescription>* fullStructTable;
    // size_t size;
    template <typename T>
    T get(std::string fieldName);
public:
    static std::map<uint8_t*, PackedStruct*> lookupTable;

    size_t totalBytes;
    uint8_t* buf;
    // vector is <field name, field type>
    // iterate and add the size of each type
    // i1, i8, i32, structs
    PackedStruct(jited::ast::TypeDeclarationPtr fieldInfo, uint8_t* existingBuf=NULL);
    ~PackedStruct();

    bool has(std::string fieldName);
    TypedValuePtr at(std::string fieldName);

    int32_t getInt(std::string fieldName);
    uint8_t* getPtr(std::string fieldName);

    void setMember(std::string fieldName, PackedStruct* member);
};










class MiniInterpreter : public jited::ast::ASTVisitor {
protected:
    bool isBool(std::string eType);

    bool getBoolState(TypedValue& tv);

    ValueMap globals;
    PackedStruct* parentStruct;
    std::string lastField;
    std::vector<ValueMap> scopes;
    std::map<std::string, jited::ast::FunctionPtr> funcs;
    // std::map<std::string, StructDescription> structInfo;
    std::map<std::string, jited::ast::TypeDeclarationPtr> structs; // instances of structs to copy from
    jited::ast::ProgramPtr program;

    TypedValuePtr lookup(std::string name);

    ValueMap bindArgs(std::vector<jited::ast::ExpressionPtr>& args, std::vector<jited::ast::DeclarationPtr>& params);

    void resetStruct();
public:
    MiniInterpreter(jited::ast::ProgramPtr program);
    ~MiniInterpreter();

    antlrcpp::Any visit(jited::ast::AssignmentStatement* statement) override;

    antlrcpp::Any visit(jited::ast::BinaryExpression* expression) override;

    antlrcpp::Any visit(jited::ast::BlockStatement* statement) override;

    antlrcpp::Any visit(jited::ast::BoolType* type) override;

    antlrcpp::Any visit(jited::ast::ConditionalStatement* statement) override;

    antlrcpp::Any visit(jited::ast::Declaration* declaration) override;

    antlrcpp::Any visit(jited::ast::DeleteStatement* statement) override;

    antlrcpp::Any visit(jited::ast::DotExpression* expression) override;

    antlrcpp::Any visit(jited::ast::FalseExpression* expression) override;

    antlrcpp::Any visit(jited::ast::Function* function) override;

    antlrcpp::Any visit(jited::ast::IdentifierExpression* expression) override;

    antlrcpp::Any visit(jited::ast::IntegerExpression* expression) override;

    antlrcpp::Any visit(jited::ast::IntType* type) override;

    antlrcpp::Any visit(jited::ast::InvocationExpression* expression) override;

    antlrcpp::Any visit(jited::ast::InvocationStatement* statement) override;

    antlrcpp::Any visit(jited::ast::LvalueDot* lvalue) override;

    antlrcpp::Any visit(jited::ast::LvalueId* lvalue) override;

    antlrcpp::Any visit(jited::ast::NewExpression* expression) override;

    antlrcpp::Any visit(jited::ast::NullExpression* expression) override;

    antlrcpp::Any visit(jited::ast::PrintLnStatement* statement) override;

    antlrcpp::Any visit(jited::ast::PrintStatement* statement) override;

    antlrcpp::Any visit(jited::ast::Program* program) override;

    antlrcpp::Any visit(jited::ast::ReadExpression* expression) override;

    antlrcpp::Any visit(jited::ast::ReturnEmptyStatement* statement) override;

    antlrcpp::Any visit(jited::ast::ReturnStatement* statement) override;

    antlrcpp::Any visit(jited::ast::StructType* type) override;

    antlrcpp::Any visit(jited::ast::TrueExpression* expression) override;

    antlrcpp::Any visit(jited::ast::TypeDeclaration* typeDeclaration) override;

    antlrcpp::Any visit(jited::ast::UnaryExpression* expression) override;

    antlrcpp::Any visit(jited::ast::VoidType* type) override;

    antlrcpp::Any visit(jited::ast::WhileStatement* statement) override;

    int run();
};

}
