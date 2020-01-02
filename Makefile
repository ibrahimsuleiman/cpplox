OBJECTS = scanner.o lox.o token.o parser.o main.o
CXXFLAGS= -std=c++17 -Wall -lstdc++
CXX = g++

all: $(OBJECTS)
	$(CXX) -o loxparse $(OBJECTS)

scanner.o: scanner.h lox.h

parser.o: parser.h

token.o: token.h

lox.o: lox.h scanner.h

main.o: lox.h

.PHONY : clean
clean:
	rm $(OBJECTS)