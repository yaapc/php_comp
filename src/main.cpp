#include "definitions.h"
#include "grammar.hpp"
#include "generate_dot.hpp"
#include "TypeChecker.h"
#include "AST\ListNode.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;
extern void initSymbolsParser();
extern SymbolsParser * symbolsParser;
extern TypeChecker * typeChecker;
extern void initTypeChecker();
extern ListNode* tree;

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

	typeChecker->checkForwardDeclarations(); cout << "checkForwardDeclarations\n";
	typeChecker->checkDependency(); cout << "checkDependency\n"; // check circular dependency and create dependency graph
	//typeChecker->checkInnerClasses(); cout << "checkInnerClasses\n";

	symbolsParser->printSymbolTables(); cout << "printSymbolTables\n";// log symbol table
	errorRec.printErrQueue(); cout << "printErrQueue\n";

	tree->type_checking(); cout << "TypeChecking Pass";

	//Visualizing
	ofstream dot_file("symbol_table.dot");
	generate_dot(symbolsParser->getRootScope(), dot_file);
	dot_file.close();

	cout << "compilation done" << endl;
	return 0;
}
