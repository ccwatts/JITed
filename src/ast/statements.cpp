#include <string>
#include <vector>
#include <memory>
#include "types.h"
#include "visitors.h"
#include "statements.h"
#include "expressions.h"

#include "antlr4-runtime.h"
using antlrcpp::Any;

namespace jited {
namespace ast {

Statement::Statement(int line) : line(line) {};
Any Statement::accept(ASTVisitor* v) {
    throw std::runtime_error("Error: Visit function not implemented.");
}
Any Statement::accept(StatementVisitor* v) {
    throw std::runtime_error("Error: Visit function not implemented.");
}

// unsure if these need to be Statements for Any to not blow up
Declaration::Declaration(int line, TypePtr type, std::string name) : line(line), type(type), name(name) {};
Any Declaration::accept(ASTVisitor* v) { return v->visit(this); }
Any Declaration::accept(StatementVisitor* v) { return v->visit(this); }


// this too
Function::Function(int line, std::string name, std::vector<DeclarationPtr> params, TypePtr retType, std::vector<DeclarationPtr> locals, StatementPtr body) :
    line(line), name(name), retType(retType), params(params), locals(locals), body(body) {};
Any Function::accept(ASTVisitor* v) { return v->visit(this); }
Any Function::accept(StatementVisitor* v) { return v->visit(this); }



TypeDeclaration::TypeDeclaration(int line, std::string name, std::vector<DeclarationPtr> fields) : line(line), name(name), fields(fields) {};
Any TypeDeclaration::accept(ASTVisitor* v) { return v->visit(this); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

AssignmentStatement::AssignmentStatement(int line, LvaluePtr target, ExpressionPtr source) : Statement(line), target(target), source(source) {};
Any AssignmentStatement::accept(ASTVisitor* v) { return v->visit(this); }
Any AssignmentStatement::accept(StatementVisitor* v) { return v->visit(this); }



BlockStatement::BlockStatement(int line, std::vector<StatementPtr> statements) : Statement(line), statements(statements) {};
std::shared_ptr<BlockStatement> BlockStatement::emptyBlock() {
    return std::make_shared<BlockStatement>(-1, std::vector<StatementPtr>());
}
Any BlockStatement::accept(ASTVisitor* v) { return v->visit(this); }
Any BlockStatement::accept(StatementVisitor* v) { return v->visit(this); }



ConditionalStatement::ConditionalStatement(int line, ExpressionPtr guard, StatementPtr thenBlock, StatementPtr elseBlock) :
    Statement(line), guard(guard), thenBlock(thenBlock), elseBlock(elseBlock) {};
Any ConditionalStatement::accept(ASTVisitor* v) { return v->visit(this); }
Any ConditionalStatement::accept(StatementVisitor* v) { return v->visit(this); }



DeleteStatement::DeleteStatement(int line, ExpressionPtr expression) : Statement(line), expression(expression) {};
Any DeleteStatement::accept(ASTVisitor* v) { return v->visit(this); }
Any DeleteStatement::accept(StatementVisitor* v) { return v->visit(this); }



InvocationStatement::InvocationStatement(int line, ExpressionPtr expression) : Statement(line), expression(expression) {};
Any InvocationStatement::accept(ASTVisitor* v) { return v->visit(this); }
Any InvocationStatement::accept(StatementVisitor* v) { return v->visit(this); }



PrintLnStatement::PrintLnStatement(int line, ExpressionPtr expression) : Statement(line), expression(expression) {};
Any PrintLnStatement::accept(ASTVisitor* v) { return v->visit(this); }
Any PrintLnStatement::accept(StatementVisitor* v) { return v->visit(this); }



PrintStatement::PrintStatement(int line, ExpressionPtr expression) : Statement(line), expression(expression) {};
Any PrintStatement::accept(ASTVisitor* v) { return v->visit(this); }
Any PrintStatement::accept(StatementVisitor* v) { return v->visit(this); }



ReturnEmptyStatement::ReturnEmptyStatement(int line) : Statement(line) {};
Any ReturnEmptyStatement::accept(ASTVisitor* v) { return v->visit(this); }
Any ReturnEmptyStatement::accept(StatementVisitor* v) { return v->visit(this); }



ReturnStatement::ReturnStatement(int line, ExpressionPtr expression) : Statement(line), expression(expression) {};
Any ReturnStatement::accept(ASTVisitor* v) { return v->visit(this); }
Any ReturnStatement::accept(StatementVisitor* v) { return v->visit(this); }



WhileStatement::WhileStatement(int line, ExpressionPtr guard, StatementPtr body) : Statement(line), guard(guard), body(body) {};
Any WhileStatement::accept(ASTVisitor* v) { return v->visit(this); }
Any WhileStatement::accept(StatementVisitor* v) { return v->visit(this); }

}
}