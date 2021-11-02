CC=g++
LDLIBS=-lantlr4-runtime
CFLAGS=-c

compiler: MiniCompiler.o MiniParser.o MiniLexer.o tl-instructions.o tl-block.o
	$(CC) -o compiler MiniCompiler.o MiniParser.o MiniLexer.o tl-instructions.o tl-block.o $(LDLIBS)

MiniCompiler.o: MiniCompiler.cpp
	$(CC) -o MiniCompiler.o MiniCompiler.cpp $(CFLAGS)


MiniParser.o: MiniParser.cpp MiniParser.h
	$(CC) -o MiniParser.o MiniParser.cpp $(CFLAGS)

MiniLexer.o: MiniLexer.cpp MiniLexer.h
	$(CC) -o MiniLexer.o MiniLexer.cpp $(CFLAGS)

tl-instructions.o: tl/instructions.cpp tl/instructions.h
	$(CC) -o tl-instructions.o tl/instructions.cpp $(CFLAGS)

tl-block.o: tl/block.cpp tl/block.h
	$(CC) -o tl-block.o tl/block.cpp $(CFLAGS)

clean:
	rm -f *.o