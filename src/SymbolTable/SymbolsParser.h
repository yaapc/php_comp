#pragma once
#ifndef SYMBOLSPARSER_H
#define SYMBOLSPARSER_H

#include "../ErrorRecovery/ErrorRecovery.h"
#include "Scope.h"

extern ErrorRecovery errorRec;//using one unified ErrorRecovery defined in yacc

class SymbolsParser {
public :
	SymbolsParser();
	~SymbolsParser();

	//methods:
	void setCurrentScope(Scope * scope);
	Scope* getCurrentScope();

	Scope* getRootScope();

	//insert symbol in @currScope current scope:
	Symbol* insertSymbol(Symbol* symbol);
	//insert symbol in given scope
	Symbol* insertSymbol(Symbol* symbol, Scope* scope);
	//look up the symbol in current scope and up the hierarchy :
	Symbol* lookUpSymbol(char* name, int lineNo, int colNo);
	
	void goUp(); // go up the hierarchy of scopes

	void printSymbolTables();

	/* this method inserts the function symbol in its appropriate scope */
	Symbol* insertFunctionSymbol(char* name, char* returnType, int colNo, int lineNo, Scope* scope, Symbol* params);
	Scope* insertParams(Symbol* symbol, Scope* scope);


	/* this method finishes the class declaration and adds additional information about the class symbol */
	Symbol* finishClassInsertion(char* name, char* inhertedFrom, Class* classSymbol, Scope* scope);

	/* this method finishes data member declarations */
	Symbol* finishDataMembersDeclaration(DataMember* dataMem, int accessMod, int storageMod, char* type);

	Symbol* insertMethodSymbol(char* name, int colNo, int lineNo, int accessModifier, int storageModifier);
	Symbol* finishMethodDeclaration(Method* method, char* returnType,Scope* bodyScope, Symbol* paramSymbol);

	static Symbol* joinSymbolsLists(Symbol* firstList, Symbol* secondList);
	static Scope* joinScopes(Scope* firstScope, Scope* secondScope);
private :
	ErrorRecovery *errRecovery;
	Scope * currScope;
	Scope * rootScope;	

	string buildTableString(Scope* scope);
};


#endif
