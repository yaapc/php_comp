#include "definitions.h"
#include "grammar.hpp"
#include "generate_dot.hpp"
#include "TypeChecker.h"

#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;
extern void initSymbolsParser();
extern SymbolsParser * symbolsParser;
extern TypeChecker * typeChecker;
extern void initTypeChecker();

int main(int argc, char** argv) {
	initSymbolsParser();
	initTypeChecker();
	cout << "php-compiler" << endl << endl;
	extern FILE *yyin;
	if (!(yyin = fopen("index.php", "r"))) {
		cerr << "index.php not found" << endl;
		return 1;
	}
	yyparse();

	typeChecker->checkForwardDeclarations();
	typeChecker->checkDependency(); // check circular dependency and create dependency graph
	symbolsParser->printSymbolTables(); // log symbol table	
	errorRec.printErrQueue();

	//Visualizing
	ofstream dot_file("symbol_table.dot");
	generate_dot(symbolsParser->getRootScope(), dot_file);
	dot_file.close();

	cout << "compilation done" << endl;
	return 0;
}
