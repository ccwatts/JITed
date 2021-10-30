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
#include "tl/values.h"

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <map>
#include <limits>

using antlrcpp::Any;
using mini::proxy;
using namespace minic;

#include "tl/values.h"
#include "tl/block.h"
#include "tl/instruction.h"

class ExpressionToValueVisitor : ast::ExpressionVisitor {
private:
    ast::ProgramPtr program;
    BlockPtr current;
    std::map<std::string, std::map<std::string, std::pair<std::string, int>>> structTable;
    std::map<std::string, std::string> locals, globals;
    std::map<std::string, ast::FunctionPtr> functions;

    template <typename T>
    Any proxy_val(std::shared_ptr<T> from) {
        auto proxied = std::static_pointer_cast<Value>(from);
        return antlrcpp::Any(proxied);
    }

    ValuePtr zext(ValuePtr v) {
        RegisterPtr zexted_r = std::make_shared<Register>("i32");
        InstructionPtr zext = std::make_shared<CastInstruction>(CastInstruction::Operator::ZEXT, zexted_r, v);
        current->addInstruction(zext);
        return zexted_r;
    }

    BinaryInstruction::Operator convertBop(ast::BinaryExpression::Operator op) {
        switch (op) {
            case ast::BinaryExpression::Operator::TIMES:
                return BinaryInstruction::Operator::MUL;
            case ast::BinaryExpression::Operator::DIVIDE:
                return BinaryInstruction::Operator::DIV;
            case ast::BinaryExpression::Operator::PLUS:
                return BinaryInstruction::Operator::ADD;
            case ast::BinaryExpression::Operator::MINUS:
                return BinaryInstruction::Operator::SUB;
            case ast::BinaryExpression::Operator::LT:
                return BinaryInstruction::Operator::LT;
            case ast::BinaryExpression::Operator::GT:
                return BinaryInstruction::Operator::GT;
            case ast::BinaryExpression::Operator::LE:
                return BinaryInstruction::Operator::LE;
            case ast::BinaryExpression::Operator::GE:
                return BinaryInstruction::Operator::GE;
            case ast::BinaryExpression::Operator::EQ:
                return BinaryInstruction::Operator::EQ;
            case ast::BinaryExpression::Operator::NE:
                return BinaryInstruction::Operator::NE;
            case ast::BinaryExpression::Operator::AND:
                return BinaryInstruction::Operator::AND;
            case ast::BinaryExpression::Operator::OR:
                return BinaryInstruction::Operator::OR;
            default:
                throw std::runtime_error("invalid bop in convertBop");
        }
    }
public:
    ExpressionToValueVisitor() : current(std::make_shared<Block>()) {};
    ExpressionToValueVisitor(BlockPtr current, std::map<std::string, std::map<std::string, std::pair<std::string, int>>> structTable,
                            std::map<std::string, std::string> locals, std::map<std::string, std::string> globals,
                            std::map<std::string, ast::FunctionPtr> functions) :
        current(current), structTable(structTable), locals(locals), globals(globals), functions(functions) {}; 

    std::string structName(std::string structType) {
        int prefixLen = std::string("struct ").length();
        if (structType.length() < prefixLen) {
            // TODO (if there's time) that check could be much more robust
            return structType;
        } else {
            return structType.substr(prefixLen);
        }
    }

    Any visit(ast::BinaryExpression* expression) override {
        ValuePtr left = expression->left->accept(this);
        ValuePtr right = expression->right->accept(this);

        if (left->typeStr() == "i1") {
            left = zext(left);
        }
        if (right->typeStr() == "i1") {
            right = zext(right);
        }

        RegisterPtr res;
        InstructionPtr inst;
        switch (expression->op) {
            case ast::BinaryExpression::Operator::TIMES:
            case ast::BinaryExpression::Operator::DIVIDE:
            case ast::BinaryExpression::Operator::PLUS:
            case ast::BinaryExpression::Operator::MINUS:
            case ast::BinaryExpression::Operator::AND:
            case ast::BinaryExpression::Operator::OR:
                res = std::make_shared<Register>("i32");
                break;

            case ast::BinaryExpression::Operator::LT:
            case ast::BinaryExpression::Operator::GT:
            case ast::BinaryExpression::Operator::LE:
            case ast::BinaryExpression::Operator::GE:
            case ast::BinaryExpression::Operator::EQ:
            case ast::BinaryExpression::Operator::NE:
                res = std::make_shared<Register>("i1");
                break;
        }

        inst = std::make_shared<BinaryInstruction>(convertBop(expression->op), res, left, right);
        this->current->addInstruction(inst);
        return res;
    }
    
    Any visit(ast::DotExpression* expression) override {
        ValuePtr left;
        RegisterPtr reg1, reg2;
        InstructionPtr getInstr, loadInstr;
        std::string finalType; // don't remember why this was named like this.
        int offset;

        left = expression->left->accept(this);
        std::tie(finalType, offset) = structTable.at(structName(left->typeStr())).at(expression->id);

        reg1 = Register::get(finalType);
        getInstr = std::make_shared<LoadStoreInstruction>(LoadStoreInstruction::Operator::GETPTR, reg1, left, offset);
        this->current->addInstruction(getInstr);

        reg2 = Register::get(finalType);
        loadInstr = std::make_shared<LoadStoreInstruction>(LoadStoreInstruction::Operator::LOAD, reg2, reg1);
        this->current->addInstruction(loadInstr);

        return reg2;
    }
    
    Any visit(ast::FalseExpression* expression) override {
        return Immediate::get(0);
    }
    
    Any visit(ast::IdentifierExpression* expression) override;
    
    Any visit(ast::IntegerExpression* expression) override;
    
    Any visit(ast::InvocationExpression* expression) override;
    
    Any visit(ast::LvalueDot* lvalue) override;

    Any visit(ast::LvalueId* lvalue) override;
    
    Any visit(ast::NewExpression* expression) override;
    
    Any visit(ast::NullExpression* expression) override {
        return NullValue::get();
    }
    
    Any visit(ast::ReadExpression* expression) override;
    
    Any visit(ast::TrueExpression* expression) override {
        return Immediate::get(1);
    }
    
    Any visit(ast::UnaryExpression* expression) override;
};

class StatementToBlockVisitor : ast::ASTVisitor {
private:
    ast::ProgramPtr program;
    Blocks blocks;

public:
    StatementToBlockVisitor(ast::ProgramPtr program) : program(program) {};
        
    // Any visit(ast::AssignmentStatement* statement) override;

    // Any visit(ast::BinaryExpression* expression) override;

    // Any visit(ast::BlockStatement* statement) override;
    
    // Any visit(ast::BoolType* type) override;

    // Any visit(ast::ConditionalStatement* statement) override;
    
    // Any visit(ast::Declaration* declaration) override;
    
    // Any visit(ast::DeleteStatement* statement) override;
    
    // Any visit(ast::DotExpression* expression) override;
    
    // Any visit(ast::FalseExpression* expression) override;
    
    Any visit(ast::Function* function) override {

        return nullptr;
    }
    
    // Any visit(ast::IdentifierExpression* expression) override;
    
    // Any visit(ast::IntegerExpression* expression) override;
    
    // Any visit(ast::IntType* type) override;
    
    // Any visit(ast::InvocationExpression* expression) override;
    
    // Any visit(ast::InvocationStatement* statement) override;
    
    // Any visit(ast::LvalueDot* lvalue) override;

    // Any visit(ast::LvalueId* lvalue) override;
    
    // Any visit(ast::NewExpression* expression) override;
    
    // Any visit(ast::NullExpression* expression) override;
    
    // Any visit(ast::PrintLnStatement* statement) override;
    
    // Any visit(ast::PrintStatement* statement) override;
    
    // // Any visit(ast::Program* program) override;
    
    // Any visit(ast::ReadExpression* expression) override;

    // Any visit(ast::ReturnEmptyStatement* statement) override;

    // Any visit(ast::ReturnStatement* statement) override;
    
    // Any visit(ast::StructType* type) override;
    
    // Any visit(ast::TrueExpression* expression) override;
    
    // Any visit(ast::TypeDeclaration* typeDeclaration) override;
    
    // Any visit(ast::UnaryExpression* expression) override;
    
    // Any visit(ast::VoidType* type) override;

    // Any visit(ast::WhileStatement* statement) override;

    Any run() {
        return program->funcs.at(0)->accept(this);
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
        StatementToBlockVisitor c(p);
        c.run();
    } else {
        std::cout << "could not parse input file\n";
        return 1;
    }
}
