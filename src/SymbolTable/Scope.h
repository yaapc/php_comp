#pragma once
#ifndef SCOPE_H
#define SCOPE_H

#include "SymbolTable.h"

class Scope {
public :
	Scope(Scope* parentScope);
	~Scope();

	//methods:
	void setParentScope(Scope* scope);
	Scope* getParentScope();

	void addToInnerScopes(Scope * scope);
	Scope * getInnerScope();

	SymbolTable* getSymbolTable();
	
	void setNextScope(Scope * symbol);
	Scope * getNextScope();

	void setOwnerSymbol(Symbol* owner);
	Symbol* getOwnerSymbol();
	
	int addToVarCounter();
	int addToStaticsCounter();

	int getStaticsCounter();
	int getVarCounter();

private :
	//members;
	Scope * parentScope;
	Scope * innerScope;
	SymbolTable* symTable;

	Scope* nextScope; // to chain scopes at the same level.(brothers)

	Symbol* ownerSymbol;

	int variablesCounter;
	int staticsCounter;
	int functionsCounter;
};


#endif
