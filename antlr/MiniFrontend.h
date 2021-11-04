#pragma once

#include "MiniToAstVisitors.h"
#include "ast/program.h"

class MiniFrontend {
private:
    MiniToAstProgramVisitor programVisitor;
public:
    MiniFrontend();
    
    ast::ProgramPtr parseFile(char* fname);
};
