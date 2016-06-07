#include "definitions.h"
#include "grammar.hpp"
#include "generate_dot.hpp"
#include "TypeChecker.h"
#include "AST\ListNode.hpp"
#include "Code Generator/AsmGenerator.h"
#include "Code Generator\CodeGeneratorVistor.hpp"
#include "Code Generator\OptimizationVistor.hpp"
#include "AST/AST_Visitors/TypeErrorVisitor.hpp"
#include "AST\AST_Visitors\CheckerVisitor.hpp"
#include "TypeSystem\TypesTable.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <Windows.h>
#include <unistd.h>

using namespace std;
extern void initSymbolsParser();
extern SymbolsParser * symbolsParser;
extern TypeChecker * typeChecker;
extern ListNode *tree;
extern void initTypeChecker();
extern vector<string> requires; // a vector of require_once strings

CodeGneratorVistor codeGeneratorVistor;
OptimizationVistor optimizationVistor;
TypeErrorVisitor errorsVisitor;
CheckerVisitor checkerVisitor;

void print_ast(Node *root, std::ostream &os,string name);
void print_errors_vector(vector<TypeExpression*>);

bool GC				= 0;
bool withComments	= 1;
bool HTML			= 1;


int main(int argc, char** argv) {
	initSymbolsParser();
	initTypeChecker();
	cout << "php-compiler" << endl << endl;
	
	/** Syntactic Analysis Phase: **/
	extern FILE *yyin;
	if (!(yyin = fopen("Object.php", "r"))) {
		cerr << "Object.php not found" << endl;
		return 1;
	}
	yyparse();


	if (!(yyin = fopen("index.php", "r"))) {
		cerr << "index.php not found" << endl;
		return 1;
	}
	yyparse();

	//parse required files
	for (auto require : requires) {
		if (!(yyin = fopen(require.c_str(), "r"))) {
			cerr << require + " not found" << endl;
			return 1;
		}
		yyparse();
	}


	
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
	//errorRec.printErrQueue(); cout << "printErrQueue\n";

	if (!errorRec.errQ->isEmpty()) {
		cout << "\n*There are some Parse Errors,\n*Compilation Halted.\n\n";
		errorRec.printErrQueue();
		return 0;
	}

	/** Semantic Analysis Phase: **/
	tree->type_checking(); cout << "TypeChecking Pass\n";
	TypeClass::tryReDefine();
	TypeFunction::tryReDefine();
	tree->type_checking(); cout << "Second TypeChecking Pass\n";

	checkerVisitor.visit(tree, nullptr);

	errorsVisitor.visit(tree);
	if (errorsVisitor.errQ.size() > 0) {
		cout << "\n*There are some Semantic Errors,\n*Compilation Halted.\n\n";
		print_errors_vector(errorsVisitor.errQ);
		return 0;
	}	


	//Draw Complate AST
	ofstream ast_dot("ast.dot");
	print_ast(tree, ast_dot,"Complate");
	ast_dot.close();
	ShellExecute(NULL, NULL, "dot.exe", "-Tsvg ast.dot -o ast.svg", NULL, SW_HIDE);

	//Optimize AST
	optimizationVistor.optmize(tree);

	//Draw Optimized AST
	ofstream ast_dot_optmized("ast_optmized.dot");
	print_ast(tree, ast_dot_optmized,"Optimized");
	ast_dot_optmized.close();
	ShellExecute(NULL, NULL, "dot.exe", "-Tsvg ast_optmized.dot -o ast_optmized.svg", NULL, SW_HIDE);

	/** Code Generation Phase: **/
	codeGeneratorVistor.generate(tree);


	if (HTML){
		system("java -jar ./src/\"Code Generator\"/Mars.jar ./src/\"Code Generator\"/AssemblyCode.asm > output_file.html");
	}else{
		system("java -jar ./src/\"Code Generator\"/Mars.jar ./src/\"Code Generator\"/AssemblyCode.asm");
	}


	cout << "\n\ncompilation done" << endl;
	return 0;
}

void print_ast(Node *root, std::ostream &os,string name) {
  os << "digraph \""+ name + " AST\" {\n";
  root->print(os);
  os << "}\n";
}

void print_errors_vector(vector<TypeExpression*> vec) {
	for (auto &typeE : vec) {
		cout << "- "
			<< dynamic_cast<TypeError*>(typeE)->msg
			<< endl;
	}
	cout << endl << endl;
}
