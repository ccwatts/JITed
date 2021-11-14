#pragma once

#include "MiniToAstVisitors.h"
#include "ast/program.h"
namespace jited {
class MiniFrontend {
private:
    MiniToAstProgramVisitor programVisitor;
public:
    MiniFrontend();
    
    jited::ast::ProgramPtr parseFile(char* fname);
};
}