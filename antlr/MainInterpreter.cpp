#include "MiniFrontend.h"
#include "MiniInterpreter.h"

#include <iostream>


int main(int argc, char** argv) {
    if (argc == 1) {
        return 0;
    }
    MiniFrontend fe;
    ast::ProgramPtr p = fe.parseFile(argv[1]);
    if (!p) {
        return 1;
    }
    MiniInterpreter interpreter(p);
    return interpreter.run();
}