#include "MiniCompiler.h"
#include "MiniFrontend.h"
#include "MiniInterpreter.h"
#include "MiniJIT.h"

#include <cstdint>
#include <iostream>
#include <regex>

// parse assembly string...?

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "error: no input file specified" << std::endl
                  << "usage: jit [input .mini file]" << std::endl;
        return 1;
    }

    mini::MiniFrontend fe;
    ast::ProgramPtr p = fe.parseFile(argv[1]);
    if (!p) {
        return 1;
    }
    std::shared_ptr<ast::ASTVisitor> compiler = std::make_shared<minic::StatementToBlockVisitor>(p);
    jited::JIT::initialize();
    jited::JIT j(p, compiler);
    return j.run();
}


