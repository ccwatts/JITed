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
#include "tl/block.h"
#include "tl/instructions.h"

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <map>
#include <limits>

using antlrcpp::Any;
using mini::proxy;
using namespace minic;

// #include "tl/values.h"
// #include "tl/block.h"

typedef std::map<std::string, std::pair<std::string, int>> StructInfoTable;
class ExpressionToValueVisitor : public ast::ExpressionVisitor {
private:
    ast::ProgramPtr program;
    BlockPtr current;

    template <typename T>
    Any proxy_val(std::shared_ptr<T> from) {
        auto proxied = std::static_pointer_cast<Value>(from);
        return antlrcpp::Any(proxied);
    }

    ValuePtr zext(ValuePtr v) {
        if (v->typeStr() == "i1") {
            RegisterPtr zexted_r = std::make_shared<Register>("i32");
            InstructionPtr zext = std::make_shared<CastInstruction>(CastInstruction::Operator::ZEXT, zexted_r, v);
            current->addInstruction(zext);
            return zexted_r;
        } else {
            return v;
        }
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
    std::map<std::string, StructInfoTable>* structTable;
    std::map<std::string, std::string>* locals;
    std::map<std::string, std::string>* globals;
    std::map<std::string, ast::FunctionPtr>* functions;

    ExpressionToValueVisitor() : current(nullptr) {};
    ExpressionToValueVisitor(BlockPtr current, std::map<std::string, StructInfoTable>* structTable,
                            std::map<std::string, std::string>* locals, std::map<std::string, std::string>* globals,
                            std::map<std::string, ast::FunctionPtr>* functions) :
        current(current), structTable(structTable), locals(locals), globals(globals), functions(functions) {}; 

    void setCurrent(BlockPtr newCurrent) {
        this->current = newCurrent;
    }

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

        left = zext(left);
        right = zext(right);

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
        std::tie(finalType, offset) = structTable->at(structName(left->typeStr())).at(expression->id);

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
    
    Any visit(ast::IdentifierExpression* expression) override {
        ValuePtr reg;
        if (locals->count(expression->id)) {
            reg = this->current->readVar(expression->id);
        } else if (globals->count(expression->id)) {
            std::string idType = globals->at(expression->id);
            reg = Register::get(idType);
            ValuePtr global = Global::get(expression->id, idType);
            InstructionPtr inst = std::make_shared<LoadStoreInstruction>(
                LoadStoreInstruction::LOAD, reg, global
            );
            this->current->addInstruction(inst);
        }
        return reg;
    }

    Any visit(ast::IntegerExpression* expression) override {
        return Immediate::get(expression->value);
    }

    Any visit(ast::InvocationExpression* expression) override {
        std::vector<std::string> args;
        Values argVals;

        ast::FunctionPtr fn = functions->at(expression->name);
        std::string retType = globals->at(expression->name);

        Values values;
        for (int i = 0; i < expression->arguments.size(); ++i) {
            ast::ExpressionPtr arg = expression->arguments.at(i);
            ast::TypePtr paramType = fn->params.at(i)->type;
            ValuePtr u = arg->accept(this);
            values.push_back(u);
            if (u->isNull()) {
                NullValuePtr nul = std::static_pointer_cast<NullValue>(u);
                nul->structType = paramType->toString();
                u = nul;
            }
            argVals.push_back(u);
            args.push_back(paramType->toString() + " " + u->toString());
        }
        RegisterPtr reg;
        InstructionPtr inst;
        if (retType == "void") {
            inst = std::make_shared<InvocationInstruction>(argVals, retType, expression->name);
        } else {
            reg = Register::get(retType);
            inst = std::make_shared<InvocationInstruction>(reg, argVals, retType, expression->name);
        }
        this->current->addInstruction(inst);
        return reg;
    }

    Any visit(ast::LvalueDot* lvalue) override {
        ValuePtr left = lvalue->left->accept(this);
        auto& structRecord = structTable->at(structName(left->typeStr())).at(lvalue->id);
        int offset = structRecord.second;
        RegisterPtr reg = Register::get(structRecord.first);
        InstructionPtr getInstr = std::make_shared<LoadStoreInstruction>(
            LoadStoreInstruction::Operator::GETPTR,
            reg, left, offset
        );
        this->current->addInstruction(getInstr);
        return reg;
    }

    Any visit(ast::LvalueId* lvalue) override {
        std::string idType;
        if (locals->count(lvalue->id)) {
            idType = locals->at(lvalue->id);
        } else {
            idType = globals->at(lvalue->id);
        }
        return Register::get(idType);
    }

    Any visit(ast::NewExpression* expression) override {
        RegisterPtr call = Register::get();
        RegisterPtr cast = Register::get("%struct." + expression->id + "*");
        InstructionPtr callInstr, castInstr;
        int size = 4 * structTable->at(expression->id).size();

        Values arg = Values({Immediate::get(size)});
        callInstr = std::make_shared<InvocationInstruction>(call, arg, "i8*", "malloc");
        castInstr = std::make_shared<CastInstruction>(CastInstruction::Operator::BITCAST, cast, call,
                        "i8*", "%struct." + expression->id + "*");
        this->current->addInstruction(callInstr);
        this->current->addInstruction(castInstr);
        return cast;
    }
    
    Any visit(ast::NullExpression* expression) override {
        return NullValue::get();
    }
    
    Any visit(ast::ReadExpression* expression) override {
        RegisterPtr reg = Register::get("i32");
        InstructionPtr call = std::make_shared<InvocationInstruction>(reg, Values(), "i32", "readInt");
        this->current->addInstruction(call);
        return reg;
    }
    
    Any visit(ast::TrueExpression* expression) override {
        return Immediate::get(1);
    }
    
    Any visit(ast::UnaryExpression* expression) override {
        ValuePtr opVal = expression->operand->accept(this);
        opVal = zext(opVal);

        InstructionPtr inst;
        RegisterPtr reg = Register::get("i32");
        switch (expression->op) {
            case ast::UnaryExpression::Operator::NOT:
                inst = std::make_shared<BinaryInstruction>(BinaryInstruction::Operator::XOR, reg, Immediate::get(1), opVal);
                break;
            case ast::UnaryExpression::Operator::MINUS:
                inst = std::make_shared<BinaryInstruction>(BinaryInstruction::Operator::SUB, reg, Immediate::get(0), opVal);
                break;
        }
        this->current->addInstruction(inst);
        return reg;
    }
};

class StatementToBlockVisitor : public ast::ASTVisitor {
private:
    ast::ProgramPtr program;
    ExpressionToValueVisitor expVisitor;
    Blocks blocks;
    BlockPtr current, retBlock;
    RegisterPtr returnReg;
    std::string returnType;
    
    std::map<std::string, StructInfoTable> structTable;
    std::map<std::string, std::string> locals;
    std::map<std::string, std::string> globals;
    std::map<std::string, ast::FunctionPtr> functions;

    void syncExpVisitor() { 
        expVisitor.structTable = &structTable;
        expVisitor.locals = &locals;
        expVisitor.globals = &globals;
        expVisitor.functions = &functions;
    }

    void setCurrent(BlockPtr newCurrent) {
        expVisitor.setCurrent(newCurrent);
        this->current = newCurrent;
    }

    bool isStruct(ast::TypePtr type) {
        return type->toString().at(0) == '%';
    }

    void processTypedec(ast::TypeDeclarationPtr typeDec) {
        StructInfoTable structMap;
        int i = 0;
        std::ostringstream oss;
        oss << "%struct." << typeDec->name << " = type {";
        for (ast::DeclarationPtr field : typeDec->fields) {
            structMap.insert({field->name, {field->type->toString(), i}});
            oss << field->type->toString() << ", ";
        }

        std::string text = oss.str();
        // TODO CHANGE THIS.
        std::cout << text.substr(0, text.length() - 2) + "}\n";
        structTable.insert({typeDec->name, structMap});
    }

    void processGlobals(ast::DeclarationPtr declaration) {
        std::string initialValue = "i32 0";
        std::string type = declaration->type->toString();
        if (isStruct(declaration->type)) {
            ast::StructTypePtr structType = std::static_pointer_cast<ast::StructType>(declaration->type);
            initialValue = structType->toString() + " null";
        }
        globals.insert({declaration->name, type});

        std::cout << "@" << declaration->name << " = common global " << initialValue << ", align 4\n";
    }

    void processFunctionHeader(ast::Function* function) {
        returnType = function->retType->toString();
        std::string header = "define " + returnType + " @" + function->name + "(";
        for (ast::DeclarationPtr param : function->params) {
            header += param->type->toString() + " %" + param->name + ", ";
        }
        if (function->params.size() > 0) {
            this->current->header = header.substr(0, header.length() - 2) + ")\n{\n";
        } else {
            this->current->header = header + ")\n{\n";
        }
    }

    void processRetval() {
        if (returnType != "void") {
            returnReg = Register::get(returnType);
            returnReg->label = "_retval_";
            current->assignType("_retval_", returnType);
        }
        // why did i put haha here before?
    }
public:
    StatementToBlockVisitor(ast::ProgramPtr program) : program(program) {
        structTable = std::map<std::string, StructInfoTable>();
        globals = std::map<std::string, std::string>();
        functions = std::map<std::string, ast::FunctionPtr>();
        syncExpVisitor();

        for (ast::TypeDeclarationPtr struc : program->types) {
            processTypedec(struc);
        }
        for (ast::DeclarationPtr dec : program->decls) {
            processGlobals(dec);
        }
        for (ast::FunctionPtr fun : program->funcs) {
            globals.insert({fun->name, fun->retType->toString()});
            functions.insert({fun->name, fun});
        }
        // for (ast::FunctionPtr fun : program->funcs) {
        // think this is where there'd be the compilation of functions
        // }
    };

    // valueType is the same as just Type::toString
        
    // Any visit(ast::AssignmentStatement* statement) override;

    // Any visit(ast::BinaryExpression* expression) override;

    Any visit(ast::BlockStatement* statement) override {
        for (ast::StatementPtr bodyStmt : statement->statements) {
            setCurrent(bodyStmt->accept(this));
        }
        return current;
    }
    
    // Any visit(ast::BoolType* type) override;

    // Any visit(ast::ConditionalStatement* statement) override;
    
    // Any visit(ast::Declaration* declaration) override;
    
    // Any visit(ast::DeleteStatement* statement) override;
    
    // Any visit(ast::DotExpression* expression) override;
    
    // Any visit(ast::FalseExpression* expression) override;
    
    Any visit(ast::Function* function) override {
        locals.clear();
        setCurrent(std::make_shared<Block>(function));

        retBlock = std::make_shared<Block>(function);
        current->assignLabel();
        retBlock->assignLabel();
        processFunctionHeader(function);

        for (ast::DeclarationPtr param : function->params) {
            RegisterPtr local = Register::get(param->type->toString());
            local->label = param->name;
            locals.insert({param->name, param->type->toString()});
            this->current->writeVariable(param->name, local);
        }

        for (ast::DeclarationPtr loc : function->locals) {
            locals.insert({loc->name, loc->type->toString()});
            loc->accept(this);
        }

        processRetval();

        blocks.push_back(current);
        BlockPtr exit = function->body->accept(this);
        if (exit->toNodes.size() == 0 && exit != retBlock) {
            BranchInstructionPtr branch = std::make_shared<BranchInstruction>(retBlock);
            exit->addInstruction(branch);
            exit->connectTo(retBlock);
        }
        blocks.push_back(retBlock);

        if (returnType == "void") {
            retBlock->addInstruction(std::make_shared<ReturnInstruction>());
        } else {
            ValuePtr retVal = retBlock->readVar("_retval_");
            retBlock->addInstruction(std::make_shared<ReturnInstruction>(retVal));
        }
        retBlock->footer = "}\n";

        current->seal();
        retBlock->seal();
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
    
    Any visit(ast::Program* program) override {
        throw std::runtime_error("error: program compilation not supported");
    }
    
    // Any visit(ast::ReadExpression* expression) override;

    Any visit(ast::ReturnEmptyStatement* statement) override {
        InstructionPtr inst = std::make_shared<BranchInstruction>(retBlock);
        current->addInstruction(inst);
        current->connectTo(retBlock);
        return current;
    }

    Any visit(ast::ReturnStatement* statement) override {
        ValuePtr retReg = statement->expression->accept(&expVisitor);
        if (current != retBlock) {
            current->writeVariable("_retval_", retReg); // ?
            InstructionPtr inst = std::make_shared<BranchInstruction>(retBlock);
            current->addInstruction(inst);
            current->connectTo(retBlock);
        }
        return current;
    }
    
    // Any visit(ast::StructType* type) override;
    
    // Any visit(ast::TrueExpression* expression) override;
    
    // Any visit(ast::TypeDeclaration* typeDeclaration) override;
    
    // Any visit(ast::UnaryExpression* expression) override;
    
    // Any visit(ast::VoidType* type) override;

    // Any visit(ast::WhileStatement* statement) override;

    Any run() {
        Any ret = program->funcs.at(0)->accept(this);
        for (BlockPtr b : blocks) {
            b->print();
        }
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