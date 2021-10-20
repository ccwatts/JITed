#pragma once

#include <vector>
#include <memory>

#include "expressions.h"
#include "statements.h"
#include "types.h"
#include "visitors.h"
#include "antlr4-runtime.h"
using antlrcpp::Any;

namespace ast {

class Program {
public:
    std::vector<TypeDeclarationPtr> types;
    std::vector<DeclarationPtr> decls;
    std::vector<FunctionPtr> funcs;

    Program(std::vector<TypeDeclarationPtr> types, std::vector<DeclarationPtr> decls, std::vector<FunctionPtr> funcs) :
        types(types), decls(decls), funcs(funcs) {};
        
    Any accept(ASTVisitor* v) {
        return v->visit(this);
    }

    Any accept(StatementVisitor* v) {
        return v->visit(this);
    }
};
typedef std::shared_ptr<Program> ProgramPtr;

}