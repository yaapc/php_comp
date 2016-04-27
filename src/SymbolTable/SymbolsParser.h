#pragma once
#ifndef SYMBOLSPARSER_H
#define SYMBOLSPARSER_H

#include "../ErrorRecovery/ErrorRecovery.h"
#include "Scope.h"
#include "../definitions.h"
#include <stack>
extern ErrorRecovery errorRec;//using one unified ErrorRecovery defined in yacc
class Class;
class SymbolsParser {
public :
	SymbolsParser();
	~SymbolsParser();

	//methods:
	void setCurrentScope(Scope * scope);
	Scope* getCurrentScope();

	void setCurrentClassSym(Class* classSym);
	Class* getCurrentClassSym();

	Scope* getRootScope();

	//insert symbol in @currScope current scope:
	Symbol* insertSymbol(Symbol* symbol);
	//insert symbol in given scope
	Symbol* insertSymbol(Symbol* symbol, Scope* scope);
	//look up the symbol in current scope and up the hierarchy :
	Symbol* lookUpSymbol(char* name, int lineNo, int colNo);
	//look up a symbol starting from a given scope
	Symbol* lookUpSymbol(Scope* scope, char* name, int lineNo, int colNo);
	Symbol* lookUpSymbol(Scope* scope, char* name);


	void goUp(); // go up the hierarchy of scopes

	void printSymbolTables();

	/* this method inserts the function symbol in its appropriate scope */
	Symbol* insertFunctionSymbol(char* name, char* returnType, int colNo, int lineNo, Scope* scope, Symbol* params);
	Scope* insertParams(Symbol* symbol, Scope* scope);


	/* this method finishes the class declaration and adds additional information about the class symbol */
	Symbol* finishClassInsertion(char* inhertedFrom, Class* classSymbol, Scope* scope);

	/* this method finishes data member declarations */
	Symbol* finishDataMembersDeclaration(DataMember* dataMem, int *modifiers, int modCount, char* type);

	Symbol* insertMethodSymbol(char* name, int colNo, int lineNo, int* modifiers, int modsCount, char* returnType, Scope* bodyScope, Symbol* paramSymbol);
	Symbol* finishMethodDeclaration(Method* method, char* returnType,Scope* bodyScope, Symbol* paramSymbol);

	static Symbol* joinSymbolsLists(Symbol* firstList, Symbol* secondList);
	static Scope* joinScopes(Scope* firstScope, Scope* secondScope);

	Scope* createNewScope(bool &flag);
	Scope* createNewScope();

	void pushToClassesStack(Class* classSym);
	void popFromClassesStack();

	void checkModifiersAndSet(DataMember *mem, int* mods, int count);
	void checkModifiersAndSet(Method *method, int* mods, int count);

	int getStaticsCounter();
	int addToStaticsCounter();

	//sets a variable counter id
	void setVariableId(Variable* var, Scope* scope);

private :
	ErrorRecovery *errRecovery;
	Scope * currScope;
	Scope * rootScope;

	Class* currClassSym; // a class symbol used to bind data members and methods to thier class
	stack<Class*> *classesStack;

	string buildTableString(Scope* scope);
};

void reverse_list(Symbol**);

#endif
