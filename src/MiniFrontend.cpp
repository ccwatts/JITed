#include "MiniToAstVisitors.h"
#include "antlr4-common.h"
#include "antlr4-runtime.h"

#include "MiniParser.h"
#include "MiniLexer.h"
#include "MiniFrontend.h"

#include "ast/program.h"

#include <iostream>
namespace jited {
MiniFrontend::MiniFrontend() {};

jited::ast::ProgramPtr MiniFrontend::parseFile(char* fname) {
    antlr4::ANTLRFileStream input(fname);
    MiniLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    MiniParser parser(&tokens);
    antlr4::tree::ParseTree* tree = parser.program();

    if (parser.getNumberOfSyntaxErrors() == 0) {
        MiniToAstProgramVisitor programVisitor;
        jited::ast::ProgramPtr p = programVisitor.visit(tree);
        return p;
    } else {
        std::cerr << "error: could not parse input file '" << fname << "'";
        return nullptr;
    }
}
}