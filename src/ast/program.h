#pragma once

#include <vector>
#include <memory>

#include "statements.h"
#include "visitors.h"
#include "program.h"

#include "antlr4-runtime.h"
using antlrcpp::Any;

namespace jited {
namespace ast {

class Program {
public:
    std::vector<TypeDeclarationPtr> types;
    std::vector<DeclarationPtr> decls;
    std::vector<FunctionPtr> funcs;

    Program(std::vector<TypeDeclarationPtr> types, std::vector<DeclarationPtr> decls, std::vector<FunctionPtr> funcs);
    Any accept(ASTVisitor* v);
    Any accept(StatementVisitor* v);
};
typedef std::shared_ptr<Program> ProgramPtr;

}
}