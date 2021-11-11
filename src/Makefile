CC=g++
CFLAGS=-c -g -fPIC -I/usr/lib/llvm-10/include

LDLIBS=-lantlr4-runtime -L/usr/lib/llvm-10/lib -lLLVM-10 -lLLVM

all: compiler interpreter jit

jit: any.o MiniJIT.o MiniInterpreter.o MiniCompiler.o MiniFrontend.o MiniToAstVisitors.o MiniParser.o MiniLexer.o tl-instructions.o tl-block.o tl-values.o  ast-expressions.o ast-program.o ast-statements.o ast-types.o ast-visitors.o
	$(CC) -o jit MiniJIT.o MiniInterpreter.o MiniCompiler.o MiniFrontend.o MiniToAstVisitors.o MiniParser.o MiniLexer.o tl-instructions.o tl-block.o tl-values.o  ast-expressions.o ast-program.o ast-statements.o ast-types.o ast-visitors.o $(LDLIBS)

# CLEAN THIS UP, HOLY HELL.
compiler: any.o MainCompiler.o MiniCompiler.o MiniFrontend.o MiniToAstVisitors.o MiniParser.o MiniLexer.o tl-instructions.o tl-block.o tl-values.o  ast-expressions.o ast-program.o ast-statements.o ast-types.o ast-visitors.o
	$(CC) -o compiler MainCompiler.o MiniCompiler.o MiniFrontend.o MiniToAstVisitors.o MiniParser.o MiniLexer.o tl-instructions.o tl-block.o tl-values.o  ast-expressions.o ast-program.o ast-statements.o ast-types.o ast-visitors.o $(LDLIBS)

interpreter: any.o MainInterpreter.o MiniInterpreter.o MiniFrontend.o MiniToAstVisitors.o MiniParser.o MiniLexer.o ast-expressions.o ast-program.o ast-statements.o ast-types.o ast-visitors.o
	$(CC) -o interpreter MainInterpreter.o MiniInterpreter.o MiniFrontend.o MiniToAstVisitors.o MiniParser.o MiniLexer.o ast-expressions.o ast-program.o ast-statements.o ast-types.o ast-visitors.o $(LDLIBS)

# the big one
MiniJIT.o: MiniJIT.cpp
	$(CC) -o MiniJIT.o MiniJIT.cpp $(CFLAGS)

# files that contain the main functions
MainCompiler.o: MainCompiler.cpp
	$(CC) -o MainCompiler.o MainCompiler.cpp $(CFLAGS)

MainInterpreter.o: MainInterpreter.cpp
	$(CC) -o MainInterpreter.o MainInterpreter.cpp $(CFLAGS)

# major component files
MiniCompiler.o: MiniCompiler.cpp MiniCompiler.h
	$(CC) -o MiniCompiler.o MiniCompiler.cpp $(CFLAGS)

MiniInterpreter.o: MiniInterpreter.cpp MiniInterpreter.h
	$(CC) -o MiniInterpreter.o MiniInterpreter.cpp $(CFLAGS)

# lib files
any.o: lib/any.cpp
	$(CC) -o any.o lib/any.cpp $(CFLAGS)

# antlr parsing files
MiniParser.o: MiniParser.cpp MiniParser.h
	$(CC) -o MiniParser.o MiniParser.cpp $(CFLAGS)

MiniLexer.o: MiniLexer.cpp MiniLexer.h
	$(CC) -o MiniLexer.o MiniLexer.cpp $(CFLAGS)

MiniToAstVisitors.o: MiniToAstVisitors.cpp MiniToAstVisitors.h
	$(CC) -o MiniToAstVisitors.o MiniToAstVisitors.cpp $(CFLAGS)

MiniFrontend.o: MiniFrontend.cpp MiniFrontend.h
	$(CC) -o MiniFrontend.o MiniFrontend.cpp $(CFLAGS)

# translation files
tl-instructions.o: tl/instructions.cpp tl/instructions.h
	$(CC) -o tl-instructions.o tl/instructions.cpp $(CFLAGS)

tl-block.o: tl/block.cpp tl/block.h
	$(CC) -o tl-block.o tl/block.cpp $(CFLAGS)

tl-values.o: tl/values.cpp tl/values.h
	$(CC) -o tl-values.o tl/values.cpp $(CFLAGS)

# ast files
ast-expressions.o: ast/expressions.cpp ast/expressions.h
	$(CC) -o ast-expressions.o ast/expressions.cpp $(CFLAGS)

ast-program.o: ast/program.cpp ast/program.h
	$(CC) -o ast-program.o ast/program.cpp $(CFLAGS)

ast-statements.o: ast/statements.cpp ast/statements.h
	$(CC) -o ast-statements.o ast/statements.cpp $(CFLAGS)

ast-types.o: ast/types.cpp ast/types.h
	$(CC) -o ast-types.o ast/types.cpp $(CFLAGS)

ast-visitors.o: ast/visitors.cpp ast/visitors.h
	$(CC) -o ast-visitors.o ast/visitors.cpp $(CFLAGS)

clean:
	rm -f *.o *.gch