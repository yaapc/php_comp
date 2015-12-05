CXX = g++
CXXLFLAGS = -Wl,--enable-auto-import
CXXFLAGS= -O2
LEX = flex
LFLAGS=-osrc/lexer.cpp
YACC= bison
YFLAGS= -d -v -o src/grammar.cpp

phpc: grammar.o lexer.o main.o
	${CXX} ${CXXLFLAGS} ${CXXFLAGS} lexer.o grammar.o main.o -o $@

main.o: src/main.cpp src/definitions.hpp
	${CXX} ${CXXFLAGS} -c $<

grammar.o: src/grammar.y src/definitions.hpp src/Logger.hpp
	${YACC} ${YFLAGS} $<
	${CXX} ${CXXFLAGS} -c src/grammar.cpp

lexer.o: src/lexer.l src/definitions.hpp src/Logger.hpp
	${LEX} ${LFLAGS} $<
	${CXX} ${CXXFLAGS} -c src/lexer.cpp