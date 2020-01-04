OBJECTS = scanner.o lox.o token.o parser.o main.o interpreter.o
CXXFLAGS= -std=c++17 -Wall -lstdc++
CXX = g++

all: $(OBJECTS)
	$(CXX) -o cpplox $(OBJECTS)

scanner.o: scanner.h lox.h

parser.o: parser.h

token.o: token.h

interpreter.o: interpreter.h lox.h stmt.h

lox.o: lox.h scanner.h

main.o: lox.h

.PHONY : clean
clean:
	rm $(OBJECTS)