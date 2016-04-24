#include "definitions.h"
#include "grammar.hpp"
#include "generate_dot.hpp"
#include "TypeChecker.h"
#include "AST\ListNode.hpp"
#include "Code Generator/AsmGenerator.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <Windows.h>
#include <unistd.h>

#define SIMULATOR 1

using namespace std;
extern void initSymbolsParser();
extern SymbolsParser * symbolsParser;
extern TypeChecker * typeChecker;
extern ListNode *tree;
extern void initTypeChecker();

void print_ast(Node *root, std::ostream &os);

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

	ofstream ast_dot("ast.dot");
	print_ast(tree, ast_dot);
	ast_dot.close();
	ShellExecute(NULL, NULL, "dot.exe", "-Tsvg ast.dot -o ast.svg", NULL, SW_HIDE);
	
	ofstream dot_file("symbol_table.dot");
	generate_dot(symbolsParser->getRootScope(), dot_file);
	dot_file.close();
	ShellExecute(NULL, NULL, "dot.exe", "-Tsvg symbol_table.dot -o symbol_table.svg", NULL, SW_HIDE);

	typeChecker->checkForwardDeclarations(); cout << "checkForwardDeclarations\n";
	typeChecker->checkDependency(); cout << "checkDependency\n"; // check circular dependency and create dependency graph
	//typeChecker->checkInnerClasses(); cout << "checkInnerClasses\n";

	symbolsParser->printSymbolTables(); cout << "printSymbolTables\n";// log symbol table
	errorRec.printErrQueue(); cout << "printErrQueue\n";

	tree->type_checking(); cout << "TypeChecking Pass\n";



	AsmGenerator::initialize_file();
	tree->generate_code();
	AsmGenerator::write_file();

	if (SIMULATOR == 1){
		system("java -jar ./src/\"Code Generator\"/Mars.jar ./src/\"Code Generator\"/AssemblyCode.asm");
	}

	if (SIMULATOR == 2){
		system("QtSpim.exe -a ./src/\"Code Generator\"/mips1.asm");
	}



	cout << "compilation done" << endl;
	return 0;
}

void print_ast(Node *root, std::ostream &os) {
  cout<< "Printing Ast\n";
  os << "digraph AST {\n";
  root->print(os);
  os << "}\n";
  cout<< "Printing Done\n";
}