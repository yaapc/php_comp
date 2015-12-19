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
	
private :
	//members;
	Scope * parentScope;
	Scope * innerScope;
	SymbolTable* symTable;

	Scope* nextScope; // to chain scopes at the same level.(brothers)

	Symbol* ownerSymbol;
};


#endif
