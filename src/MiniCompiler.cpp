#include "MiniParser.h"
#include "MiniLexer.h"

#include "antlr4-common.h"
#include "antlr4-runtime.h"

#include "ast/program.h"
#include "ast/expressions.h"
#include "ast/statements.h"
#include "ast/visitors.h"

#include "lib/proxy.h"

#include "tl/values.h"
#include "tl/block.h"
#include "tl/instructions.h"

#include <sstream>
#include <iostream>
#include <map>


#include "MiniCompiler.h"

// #include "tl/values.h"
// #include "tl/block.h"
using antlrcpp::Any;
using mini::proxy;
namespace minic {

template <typename T>
Any ExpressionToValueVisitor::proxy_val(std::shared_ptr<T> from) {
    auto proxied = std::static_pointer_cast<Value>(from);
    return antlrcpp::Any(proxied);
}

BinaryInstruction::Operator ExpressionToValueVisitor::convertBop(ast::BinaryExpression::Operator op) {
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

ExpressionToValueVisitor::ExpressionToValueVisitor() : current(nullptr) {};
ExpressionToValueVisitor::ExpressionToValueVisitor(BlockPtr current, std::map<std::string, StructInfoTable>* structTable,
                        std::map<std::string, std::string>* locals, std::map<std::string, std::string>* globals,
                        std::map<std::string, ast::FunctionPtr>* functions) :
    current(current), structTable(structTable), locals(locals), globals(globals), functions(functions) {}; 

ValuePtr ExpressionToValueVisitor::zext(ValuePtr v) {
    if (v->typeStr() == "i1") {
        RegisterPtr zexted_r = Register::get("i32");
        InstructionPtr zext = std::make_shared<CastInstruction>(CastInstruction::Operator::ZEXT, zexted_r, v);
        current->addInstruction(zext);
        return zexted_r;
    } else {
        return v;
    }
}

ValuePtr ExpressionToValueVisitor::trunc(ValuePtr v) {
    if (v->typeStr() == "i32") {
        RegisterPtr trunc_r = Register::get("i32");
        InstructionPtr trunc = std::make_shared<CastInstruction>(CastInstruction::Operator::TRUNC, trunc_r, v);
        current->addInstruction(trunc);
        return trunc_r;
    } else if (v->typeStr() == "i1") {
        return v;
    } else {
        return nullptr;
    }
}

void ExpressionToValueVisitor::setCurrent(BlockPtr newCurrent) {
    this->current = newCurrent;
}

std::string ExpressionToValueVisitor::structName(std::string structType) {
    int prefixLen = std::string("%struct.").length();
    if (structType.length() < prefixLen) {
        // TODO (if there's time) that check could be much more robust
        return structType;
    } else {
        std::string suffix = structType.substr(prefixLen);
        return suffix.substr(0, suffix.length() - 1);
    }
}

Any ExpressionToValueVisitor::visit(ast::BinaryExpression* expression) {
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
            res = Register::get("i32");
            break;

        case ast::BinaryExpression::Operator::LT:
        case ast::BinaryExpression::Operator::GT:
        case ast::BinaryExpression::Operator::LE:
        case ast::BinaryExpression::Operator::GE:
        case ast::BinaryExpression::Operator::EQ:
        case ast::BinaryExpression::Operator::NE:
            res = Register::get("i1");
            break;
    }

    inst = std::make_shared<BinaryInstruction>(convertBop(expression->op), res, left, right);
    this->current->addInstruction(inst);
    return proxy_val<Register>(res);
}

Any ExpressionToValueVisitor::visit(ast::DotExpression* expression) {
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

    return proxy_val<Register>(reg2);
}

Any ExpressionToValueVisitor::visit(ast::FalseExpression* expression) {
    return proxy_val<Immediate>(Immediate::get(0));
}

Any ExpressionToValueVisitor::visit(ast::IdentifierExpression* expression) {
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
    } else { 
        throw std::runtime_error("nonexistent id\n");
    }
    return proxy_val<Value>(reg);
}

Any ExpressionToValueVisitor::visit(ast::IntegerExpression* expression) {
    return proxy_val<Immediate>(Immediate::get(expression->value));
}

Any ExpressionToValueVisitor::visit(ast::InvocationExpression* expression) {
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
    return proxy_val<Register>(reg);
}

Any ExpressionToValueVisitor::visit(ast::LvalueDot* lvalue) {
    ValuePtr left = lvalue->left->accept(this);
    auto& structRecord = structTable->at(structName(left->typeStr())).at(lvalue->id);
    int offset = structRecord.second;
    RegisterPtr reg = Register::get(structRecord.first);
    InstructionPtr getInstr = std::make_shared<LoadStoreInstruction>(
        LoadStoreInstruction::Operator::GETPTR,
        reg, left, offset
    );
    this->current->addInstruction(getInstr);
    return proxy_val<Register>(reg);
}

Any ExpressionToValueVisitor::visit(ast::LvalueId* lvalue) {
    std::string idType;
    if (locals->count(lvalue->id)) {
        idType = locals->at(lvalue->id);
    } else {
        idType = globals->at(lvalue->id);
    }
    return proxy_val<Register>(Register::get(idType));
}

Any ExpressionToValueVisitor::visit(ast::NewExpression* expression) {
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
    return proxy_val<Register>(cast);
}

Any ExpressionToValueVisitor::visit(ast::NullExpression* expression) {
    return proxy_val<NullValue>(NullValue::get());
}

Any ExpressionToValueVisitor::visit(ast::ReadExpression* expression) {
    RegisterPtr reg = Register::get("i32");
    InstructionPtr call = std::make_shared<InvocationInstruction>(reg, Values(), "i32", "readInt");
    this->current->addInstruction(call);
    return proxy_val<Register>(reg);
}

Any ExpressionToValueVisitor::visit(ast::TrueExpression* expression) {
    return proxy_val<Immediate>(Immediate::get(1));
}

Any ExpressionToValueVisitor::visit(ast::UnaryExpression* expression) {
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
    return proxy_val<Register>(reg);
}

















void StatementToBlockVisitor::syncExpVisitor() { 
    expVisitor.structTable = &structTable;
    expVisitor.locals = &locals;
    expVisitor.globals = &globals;
    expVisitor.functions = &functions;
}

void StatementToBlockVisitor::setCurrent(BlockPtr newCurrent) {
    expVisitor.setCurrent(newCurrent);
    this->current = newCurrent;
}

bool StatementToBlockVisitor::isStruct(ast::TypePtr type) {
    return type->toString().at(0) == '%';
}

void StatementToBlockVisitor::processTypedec(ast::TypeDeclarationPtr typeDec) {
    StructInfoTable structMap;
    int i = 0;
    std::ostringstream oss;
    #ifdef PRINTMETA
    oss << "%struct." << typeDec->name << " = type {";
    #endif
    for (ast::DeclarationPtr field : typeDec->fields) {
        structMap.insert({field->name, {field->type->toString(), i++}});
        oss << field->type->toString() << ", ";
    }

    std::string text = oss.str();
    // TODO CHANGE THIS.
    #ifdef PRINTMETA
    std::cout << text.substr(0, text.length() - 2) + "}\n";
    #endif
    structTable.insert({typeDec->name, structMap});
}

void StatementToBlockVisitor::processGlobals(ast::DeclarationPtr declaration) {
    std::string initialValue = "i32 0";
    std::string type = declaration->type->toString();
    if (isStruct(declaration->type)) {
        ast::StructTypePtr structType = std::static_pointer_cast<ast::StructType>(declaration->type);
        initialValue = structType->toString() + " null";
    }
    globals.insert({declaration->name, type});

    // std::cout << "@" << declaration->name << " = common global " << initialValue << ", align 4\n";
}

void StatementToBlockVisitor::processFunctionHeader(ast::Function* function) {
    returnType = function->retType->toString();
    std::string header = "define " + returnType + " @" + function->name + "(";
    for (ast::DeclarationPtr param : function->params) {
        header += param->type->toString() + " %" + param->name + ", ";
    }
    // #ifdef PRINTMETA
    if (function->params.size() > 0) {
        this->current->header = header.substr(0, header.length() - 2) + ")\n{\n";
    } else {
        this->current->header = header + ")\n{\n";
    }
    // #endif
}

void StatementToBlockVisitor::processRetval() {
    if (returnType != "void") {
        returnReg = Register::get(returnType);
        returnReg->label = "_retval_";
        current->assignType("_retval_", returnType);
    }
    // why did i put haha here before?
}

BlockPtr StatementToBlockVisitor::newBlock() {
    return std::make_shared<Block>(currentFunction);
}

StatementToBlockVisitor::StatementToBlockVisitor(ast::ProgramPtr program) : program(program) {
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
};
    
Any StatementToBlockVisitor::visit(ast::AssignmentStatement* statement) {
    ValuePtr lhs = statement->target->accept(&expVisitor);
    ValuePtr rhs = statement->source->accept(&expVisitor);
    rhs = expVisitor.zext(rhs);

    // this needs to be fixed to include OFFSETS
    if (statement->target->isDot()) {
        current->addInstruction(
            std::make_shared<LoadStoreInstruction>(
                LoadStoreInstruction::Operator::STORE, lhs, rhs
            )
        );
    } else {
        ast::LvalueIdPtr lhsV = std::dynamic_pointer_cast<ast::LvalueId>(statement->target);
        if (!locals.count(lhsV->id)) {
            GlobalPtr reg = Global::get(lhsV->id, globals.at(lhsV->id));
            current->addInstruction(
                std::make_shared<LoadStoreInstruction>(
                    LoadStoreInstruction::Operator::STORE, reg, rhs
                )
            );
        } else {
            current->writeVariable(lhsV->id, rhs);
        }
    }
    return current;
}

Any StatementToBlockVisitor::visit(ast::BlockStatement* statement) {
    for (ast::StatementPtr bodyStmt : statement->statements) {
        setCurrent(bodyStmt->accept(this));
    }
    return current;
}

Any StatementToBlockVisitor::visit(ast::ConditionalStatement* statement) {
    ValuePtr guard = statement->guard->accept(&expVisitor);
    guard = expVisitor.trunc(guard);
    BlockPtr thenEntry = newBlock(), elseEntry = newBlock(), join = newBlock();
    thenEntry->assignLabel();
    thenEntry->seal();
    elseEntry->assignLabel();
    elseEntry->seal();

    blocks.push_back(thenEntry);
    blocks.push_back(elseEntry);
    InstructionPtr branch = std::make_shared<BranchInstruction>(guard, thenEntry, elseEntry);
    current->addInstruction(branch);

    current->connectTo(thenEntry);
    current->connectTo(elseEntry);

    setCurrent(thenEntry);
    BlockPtr thenExit = statement->thenBlock->accept(this);

    setCurrent(elseEntry);
    BlockPtr elseExit = statement->elseBlock->accept(this);

    join->assignLabel();

    if (thenExit->toNodes.size() == 0 && thenExit != retBlock) {
        InstructionPtr inst = std::make_shared<BranchInstruction>(join);
        thenExit->addInstruction(inst);
        thenExit->connectTo(join);
    }
    if (elseExit->toNodes.size() == 0 && elseExit != retBlock) {
        InstructionPtr inst = std::make_shared<BranchInstruction>(join);
        elseExit->addInstruction(inst);
        elseExit->connectTo(join);
    }
    join->seal();

    if (join->fromNodes.size() > 0) {
        blocks.push_back(join);
        return join;
    } else {
        return retBlock;
    }
}

Any StatementToBlockVisitor::visit(ast::Declaration* declaration) {
    RegisterPtr alloc = Register::get(declaration->type->toString());
    current->writeVariable(declaration->name, alloc);
    return current;
}

Any StatementToBlockVisitor::visit(ast::DeleteStatement* statement) {
    ValuePtr reg = statement->expression->accept(&expVisitor);
    RegisterPtr cast = Register::get("i8*");
    InstructionPtr castInstr = std::make_shared<CastInstruction>(
        CastInstruction::Operator::BITCAST, cast, reg, reg->typeStr(), "i8*"
    );
    current->addInstruction(castInstr);

    Values args;
    args.push_back(cast);
    InstructionPtr call = std::make_shared<InvocationInstruction>(args, "void", "free");
    current->addInstruction(call);
    return current;
}

Any StatementToBlockVisitor::visit(ast::Function* function) {
    currentFunction = function;

    locals.clear();
    setCurrent(newBlock());

    retBlock = newBlock();
    current->assignLabel();
    retBlock->assignLabel();
    processFunctionHeader(function);

    for (ast::DeclarationPtr param : function->params) {
        RegisterPtr local = Register::get(param->type->toString());
        local->label = param->name;
        // locals.insert({param->name, param->type->toString()});
        locals[param->name] = param->type->toString();
        this->current->writeVariable(param->name, local);
    }

    for (ast::DeclarationPtr loc : function->locals) {
        // locals.insert({loc->name, loc->type->toString()});
        locals[loc->name] = loc->type->toString();
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

    std::ostringstream oss;
    for (BlockPtr b : this->blocks) {
        oss << b->toString();
    }
    blocks.clear();
    return oss.str();
}

Any StatementToBlockVisitor::visit(ast::InvocationStatement* statement) {
    statement->expression->accept(&expVisitor);
    return current;
}

Any StatementToBlockVisitor::visit(ast::PrintLnStatement* statement) {
    ValuePtr reg = statement->expression->accept(&expVisitor);
    Values args;
    args.push_back(reg);
    InstructionPtr inst = std::make_shared<InvocationInstruction>(args, "void", "printIntEndl");
    current->addInstruction(inst);
    return current;
}

Any StatementToBlockVisitor::visit(ast::PrintStatement* statement) {
    ValuePtr reg = statement->expression->accept(&expVisitor);
    Values args;
    args.push_back(reg);
    InstructionPtr inst = std::make_shared<InvocationInstruction>(args, "void", "printInt");
    current->addInstruction(inst);
    return current;
}

Any StatementToBlockVisitor::visit(ast::Program* program) {
    throw std::runtime_error("error: program compilation not supported");
}

Any StatementToBlockVisitor::visit(ast::ReturnEmptyStatement* statement) {
    InstructionPtr inst = std::make_shared<BranchInstruction>(retBlock);
    current->addInstruction(inst);
    current->connectTo(retBlock);
    return current;
}

Any StatementToBlockVisitor::visit(ast::ReturnStatement* statement) {
    ValuePtr retReg = statement->expression->accept(&expVisitor);
    if (current != retBlock) {
        current->writeVariable("_retval_", retReg); // ?
        InstructionPtr inst = std::make_shared<BranchInstruction>(retBlock);
        current->addInstruction(inst);
        current->connectTo(retBlock);
    }
    return current;
}

Any StatementToBlockVisitor::visit(ast::WhileStatement* statement) {
    ValuePtr guard = statement->guard->accept(&expVisitor);
    guard = expVisitor.trunc(guard);

    BlockPtr bodyEntry = newBlock(), join = newBlock();
    bodyEntry->assignLabel();
    join->assignLabel();

    current->connectTo(bodyEntry);
    blocks.push_back(bodyEntry);
    current->connectTo(join);

    InstructionPtr branch = std::make_shared<BranchInstruction>(guard, bodyEntry, join);
    current->addInstruction(branch);

    setCurrent(bodyEntry);
    BlockPtr whileExit = statement->body->accept(this);
    // expVisitor.setCurrent(whileExit);

    expVisitor.setCurrent(whileExit);
    guard = statement->guard->accept(&expVisitor); // with new current block

    guard = expVisitor.trunc(guard);

    branch = std::make_shared<BranchInstruction>(guard, bodyEntry, join);
    whileExit->addInstruction(branch);

    whileExit->connectTo(bodyEntry);
    whileExit->connectTo(join);
    join->seal();
    bodyEntry->seal();
    blocks.push_back(join);
    
    setCurrent(bodyEntry);
    // setCurrent(bodyEntry); // ???
    return join;
}

// int main(int argc, char** argv) {
//     if (argc == 1) {
//         return 0;
//     }
//     antlr4::ANTLRFileStream input(argv[1]);
//     MiniLexer lexer(&input);
//     antlr4::CommonTokenStream tokens(&lexer);
//     MiniParser parser(&tokens);
//     antlr4::tree::ParseTree* tree = parser.program();

//     if (parser.getNumberOfSyntaxErrors() == 0) {
//         MiniToAstProgramVisitor programVisitor;
//         ast::ProgramPtr p = programVisitor.visit(tree);
//         StatementToBlockVisitor c(p);
//         c.run();
//     } else {
//         std::cout << "could not parse input file\n";
//         return 1;
//     }
// }

}