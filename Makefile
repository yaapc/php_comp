CXX = g++
CXXLFLAGS = -Wl,--enable-auto-import
CXXFLAGS= -ggdb -std=c++11 -Wall
LEX = flex
LFLAGS=-osrc/lexer.cpp
YACC= bison
YFLAGS= -d -v -o src/grammar.cpp

phpc: grammar.o lexer.o main.o Scope.o Symbol.o SymbolTable.o SymbolsParser.o generate_dot.o TypeChecker.o DependencyGraph.o
	${CXX} ${CXXLFLAGS} ${CXXFLAGS} Scope.o Symbol.o SymbolTable.o SymbolsParser.o TypeChecker.o DependencyGraph.o generate_dot.o lexer.o grammar.o main.o -o $@

main.o: src/main.cpp src/definitions.h
	${CXX} ${CXXFLAGS} -c $<

grammar.o: src/grammar.y src/definitions.h src/Logger.hpp AST.o
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

generate_dot.o: src/generate_dot.cpp src/generate_dot.hpp
	${CXX} ${CXXFLAGS} -c $<

TypeChecker.o: src/TypeChecker.cpp src/TypeChecker.h
	${CXX} ${CXXFLAGS} -c $<

AST.o: src/AST/*
	touch AST.o

DependencyGraph.o: src/DependencyGraph/DependencyGraph.cpp src/DependencyGraph/DependencyGraph.h
	${CXX} ${CXXFLAGS} -c $<

symbol_table: symbol_table.dot
	dot -Tsvg -o symbol_table.svg symbol_table.dot

dependency_graph_dot: dependency_graph.dot
	dot -Tsvg -o dependency_graph.svg $<

ast_dot: ast.dot
	dot -Tsvg -o ast.svg $<

clean:
	rm *.o
	rm *.txt
	rm *.dot

run:
	make phpc
	./phpc
	make symbol_table
	make dependency_graph_dot
	make ast_dot
