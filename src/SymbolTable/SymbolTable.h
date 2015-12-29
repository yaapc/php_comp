#pragma once
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include"Symbol.h"
#define MAX_LENGTH 71 // symbols hash map "M"

//file headers
#include<iostream>
#include<fstream>
#include<string>
#include <vector>

class Scope;

class SymbolTable {
public:
	SymbolTable();
	~SymbolTable();

	Symbol* insert(Symbol* symbol);
	Symbol* lookup(char* name);

	string toString();

	void setOwnerScope(Scope* owner);
	Scope* getOwnerScope();

	vector<Symbol*> symbols();
private:

	Symbol **map;
	int hash(char * name); // the hash function
	Symbol* get(int index);
	Symbol* put(int index, Symbol* symbol);

	Scope* ownerScope; //the scope which owns the SymbolTable
};

#endif
