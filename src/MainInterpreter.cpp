#include "MiniFrontend.h"
#include "MiniInterpreter.h"

#include <iostream>


int main(int argc, char** argv) {
    if (argc == 1) {
        return 0;
    }
    mini::MiniFrontend fe;
    ast::ProgramPtr p = fe.parseFile(argv[1]);
    if (!p) {
        return 1;
    }
    mini::MiniInterpreter interpreter(p);
    return interpreter.run();
}