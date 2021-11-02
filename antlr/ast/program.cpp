#include <vector>
#include <memory>

#include "statements.h"
#include "visitors.h"
#include "program.h"

#include "antlr4-runtime.h"
using antlrcpp::Any;

namespace ast {

Program::Program(std::vector<TypeDeclarationPtr> types, std::vector<DeclarationPtr> decls, std::vector<FunctionPtr> funcs) :
    types(types), decls(decls), funcs(funcs) {};
    
Any Program::accept(ASTVisitor* v) {
    return v->visit(this);
}

Any Program::accept(StatementVisitor* v) {
    return v->visit(this);
}

}