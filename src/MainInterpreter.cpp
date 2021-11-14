#include "MiniFrontend.h"
#include "MiniInterpreter.h"

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
    jited::MiniInterpreter interpreter(p);
    return interpreter.run();
}