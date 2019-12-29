OBJECTS = scanner.o lox.o token.o main.o
CXXFLAGS= -std=c++17 -Wall -lstdc++
CXX = g++

all: $(OBJECTS)
	$(CXX) -o loxscan $(OBJECTS)

scanner.o: scanner.h lox.h

token.o: token.h

lox.o: lox.h scanner.h

main.o: lox.h

.PHONY : clean
clean:
	rm $(OBJECTS)