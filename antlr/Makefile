CC=g++
LDLIBS=-lantlr4-runtime
CFLAGS=-c

compiler: MiniCompiler.o MiniParser.o MiniLexer.o tl-instructions.o tl-block.o tl-values.o
	$(CC) -o compiler MiniCompiler.o MiniParser.o MiniLexer.o tl-instructions.o tl-block.o tl-values.o $(LDLIBS)


interpreter: MiniInterpreter.o MiniParser.o MiniLexer.o
	$(CC) -o interpreter MiniInterpreter.o MiniParser.o MiniLexer.o $(LDLIBS)


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

clean:
	rm -f *.o *.gch