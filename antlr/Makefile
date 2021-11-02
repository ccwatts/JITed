CC=g++
LDLIBS=-lantlr4-runtime
CFLAGS=-c

all: compiler interpreter

compiler: MiniCompiler.o MiniParser.o MiniLexer.o tl-instructions.o tl-block.o tl-values.o  ast-expressions.o ast-program.o ast-statements.o ast-types.o ast-visitors.o
	$(CC) -o compiler MiniCompiler.o MiniParser.o MiniLexer.o tl-instructions.o tl-block.o tl-values.o  ast-expressions.o ast-program.o ast-statements.o ast-types.o ast-visitors.o $(LDLIBS)

interpreter: MiniInterpreter.o MiniParser.o MiniLexer.o ast-expressions.o ast-program.o ast-statements.o ast-types.o ast-visitors.o
	$(CC) -o interpreter MiniInterpreter.o MiniParser.o MiniLexer.o ast-expressions.o ast-program.o ast-statements.o ast-types.o ast-visitors.o $(LDLIBS)


MiniCompiler.o: MiniCompiler.cpp
	$(CC) -o MiniCompiler.o MiniCompiler.cpp $(CFLAGS)

MiniInterpreter.o: MiniInterpreter.cpp
	$(CC) -o MiniInterpreter.o MiniInterpreter.cpp $(CFLAGS)

MiniParser.o: MiniParser.cpp MiniParser.h
	$(CC) -o MiniParser.o MiniParser.cpp $(CFLAGS)

MiniLexer.o: MiniLexer.cpp MiniLexer.h
	$(CC) -o MiniLexer.o MiniLexer.cpp $(CFLAGS)


tl-instructions.o: tl/instructions.cpp tl/instructions.h
	$(CC) -o tl-instructions.o tl/instructions.cpp $(CFLAGS)

tl-block.o: tl/block.cpp tl/block.h
	$(CC) -o tl-block.o tl/block.cpp $(CFLAGS)

tl-values.o: tl/values.cpp tl/values.h
	$(CC) -o tl-values.o tl/values.cpp $(CFLAGS)


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