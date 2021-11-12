# EduJIT

## Requirements

1. `LLVM-10`
2. `CLANG-10`
3. `C++11`
2. `ANTLR4`

## Building

To build, run the makefile in `jit/src` using GNU make. The make command by itself will make executables for the JIT, example compiler, and interpreter. These components can also be compiled individually using `make jit`, `make compiler`, and `make interpreter` respectively.

The files `MainJIT.cpp`, `MainCompiler.cpp`, and `MainInterpreter.cpp` contain the main functions for each of the executable. For students, the file `MainJIT.cpp` should be altered to create their compiler built on top of `ast::ASTVisitor` and supply it to the JIT framework.

## Running

To run the executables, simply supply a .mini file as the only argument.