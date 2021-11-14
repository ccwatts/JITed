#include "MiniFrontend.h"
#include "MiniCompiler.h"

#include <string>
#include <iostream>


int main(int argc, char** argv) {
    if (argc == 1) {
        return 0;
    }
    jited::MiniFrontend fe;
    jited::ast::ProgramPtr p = fe.parseFile(argv[1]);
    if (!p) {
        return 1;
    }
    minic::StatementToBlockVisitor c(p);
    // c.run();
    // jited::ast::FunctionPtr f = p->funcs.at(0);
    // std::string out = f->accept(&c);
    // std::cout << out;
    for (auto f : p->funcs) {
        std::string s = f->accept(&c);
        std::cout << s << std::endl;
    }
    return 0;
}