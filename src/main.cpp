#include "definitions.h"
#include "grammar.hpp"
#include "generate_dot.hpp"
#include "TypeChecker.h"
#include "AST\ListNode.hpp"
#include "Code Generator/AsmGenerator.h"
#include "Code Generator\CodeGeneratorVistor.hpp"
#include "Code Generator\OptimizationVistor.hpp"
#include "TypeSystem\TypesTable.h"

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

CodeGneratorVistor codeGeneratorVistor;
OptimizationVistor optimizationVistor;

void print_ast(Node *root, std::ostream &os,string name);

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

	//Draw Complate AST
	ofstream ast_dot("ast.dot");
	print_ast(tree, ast_dot,"Complate");
	ast_dot.close();
	ShellExecute(NULL, NULL, "dot.exe", "-Tsvg ast.dot -o ast.svg", NULL, SW_HIDE);
	
	//Draw Symbol Table
	ofstream dot_file("symbol_table.dot");
	generate_dot(symbolsParser->getRootScope(), dot_file);
	dot_file.close();
	ShellExecute(NULL, NULL, "dot.exe", "-Tsvg symbol_table.dot -o symbol_table.svg", NULL, SW_HIDE);

	//Check types in symbol tables
	typeChecker->checkForwardDeclarations(); cout << "checkForwardDeclarations\n";
	typeChecker->checkDependency(); cout << "checkDependency\n";

	//Print errors and symbol table in files
	symbolsParser->printSymbolTables(); cout << "printSymbolTables\n";
	errorRec.printErrQueue(); cout << "printErrQueue\n";

	TypesTable::buildObjectClass();

	//Check AST types 
	tree->type_checking(); cout << "TypeChecking Pass\n";

	//Optimize AST
	optimizationVistor.optmize(tree);

	//Draw Optimized AST
	ofstream ast_dot_optmized("ast_optmized.dot");
	print_ast(tree, ast_dot_optmized,"Optimized");
	ast_dot_optmized.close();
	ShellExecute(NULL, NULL, "dot.exe", "-Tsvg ast_optmized.dot -o ast_optmized.svg", NULL, SW_HIDE);

	//Generate code AST
	codeGeneratorVistor.generate(tree);

	if (SIMULATOR == 1){
		system("java -jar ./src/\"Code Generator\"/Mars.jar ./src/\"Code Generator\"/AssemblyCode.asm");
	}

	if (SIMULATOR == 2){
		system("QtSpim.exe -a ./src/\"Code Generator\"/mips1.asm");
	}

	cout << "compilation done" << endl;
	return 0;
}

void print_ast(Node *root, std::ostream &os,string name) {
  os << "digraph \""+ name + " AST\" {\n";
  root->print(os);
  os << "}\n";
}