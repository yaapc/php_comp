CXX = g++
CXXLFLAGS = -Wl,--enable-auto-import
CXXFLAGS= -ggdb -std=c++11
LEX = flex
LFLAGS=-osrc/lexer.cpp
YACC= bison
YFLAGS= -d -v -o src/grammar.cpp

phpc: grammar.o lexer.o main.o Scope.o Symbol.o SymbolTable.o SymbolsParser.o
	${CXX} ${CXXLFLAGS} ${CXXFLAGS} Scope.o Symbol.o SymbolTable.o SymbolsParser.o lexer.o grammar.o main.o -o $@

main.o: src/main.cpp src/definitions.h
	${CXX} ${CXXFLAGS} -c $<

grammar.o: src/grammar.y src/definitions.h src/Logger.hpp
	${YACC} ${YFLAGS} $<
	${CXX} ${CXXFLAGS} -c src/grammar.cpp

lexer.o: src/lexer.l src/definitions.h src/Logger.hpp
	${LEX} ${LFLAGS} $<
	${CXX} ${CXXFLAGS} -c src/lexer.cpp

Scope.o: src/SymbolTable/Scope.cpp src/SymbolTable/Scope.h
	${CXX} ${CXXFLAGS} -c $<

Symbol.o: src/SymbolTable/Symbol.cpp src/SymbolTable/Symbol.h
	${CXX} ${CXXFLAGS} -c $<

SymbolTable.o: src/SymbolTable/SymbolTable.cpp src/SymbolTable/SymbolTable.h
	${CXX} ${CXXFLAGS} -c $<

SymbolsParser.o: src/SymbolTable/SymbolsParser.cpp src/SymbolTable/SymbolsParser.h
	${CXX} ${CXXFLAGS} -c $<

clean:
	rm *.o