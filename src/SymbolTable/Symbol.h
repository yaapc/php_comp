#pragma once
#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <vector>
using namespace std;

/*
=========
 SYMBOL:
=========
*/
class Symbol {
public:
	Symbol(char* name, int symbolType, int colNo, int lineNo);
	~Symbol();

	virtual string toString() = 0;
	virtual int getSymbolType() = 0;
	void setSymbolType(int type);


	void setName(string name);
	char* getName();

	Symbol* getNext();
	Symbol* setNext(Symbol* symbol);

	int getColNo();
	int getLineNo();

	Symbol* node;// TODO: document it
private :
	char * name;
	int symbolType;
	Symbol* next; // to chain symbols in buckets.

	int colNo;
	int lineNo;

	//long memoryLocation; // mem location at runtime
};

/*
=========
VARIABLE:
=========
*/
class Variable : public Symbol {
public:
	//it takes symbolType cuz it's a base class for DataMember and Parameter
	//TODO : Overload it without symbolType
	Variable(char * name, int symbolType, bool isInit, int colNo, int lineNo);
	int getSymbolType();

	bool isInit();

	char* getVariableType();
	void setVariableType(char* type);

	string toString();
private:
	char* variableType;
	bool isInited;
};


/*
=========
FUNCTION:
=========
*/
class Scope; // FORWARD DECLERATION
class Parameter;

class Function : public Symbol {
public:

	Function(char* name, char* returnType, int colNo, int lineNo, Scope* bodyScope);
	~Function();
	int getSymbolType();

	char* getReturnType();
	void setReturnType(char* returnType);

	Scope* getBodyScope();
	void setBodyScope(Scope* scope);

	string toString();

	Symbol* addToParams(Symbol* sym);
	vector<Parameter*> parameters();
private:
	char* returnType;
	Scope* bodyScope;

	Symbol* params;
};

/*
=========
CLASS:
=========
*/
class DataMember;
class Method;

class Class : public Symbol {
public:

	Class(char* name, int colNo, int lineNo, Scope* bodyScope);
	Class(int colNo, int lineNo, bool isFinal, bool isAbstract);
	~Class();

	bool isFinal;
	bool isAbstract;

	void setAsAbstract();
	void setAsFinal();

	void setInhertedFrom(string inhertedFrom);

	//overrided virtuals
	string toString();
	int getSymbolType();

	void setBodyScope(Scope* bodyScope);
	Scope* getBodyScope();

	Symbol* addToDataMembers(DataMember* dataMem);
	Symbol* addToMethodMembers(Method* methodMem);
private:
	string inhertedFrom; // string to remove char* overhead when setting it.
	Scope* bodyScope;
	DataMember* dataMembers;
	Method* methodMembers;
};

/*
=========
DATA MEMBER:
=========
*/
class DataMember : public Variable {
public:

	DataMember(char * name, bool isInit, int colNo, int lineNo);
	~DataMember();

	int getAccessModifier();
	void setAccessModifier(int accessModifier);

	int getStorageModifier();
	void setStorageModifier(int storageModifier);

	//overrided methods
	int getSymbolType();
	string toString();
private:
	int accessModifier;
	int storageModifier;

};
/*
=========================
METHOD:
=========================
*/
class Method : public Function {
public:
	Method(char* name, char* returnType, int colNo, int lineNo, Scope* bodyScope, int accessModifier, int storageModifier);
	~Method();

	int getAccessModifier();
	void setAccessModifier(int accessModifier);

	int getStorageModifier();
	void setStorageModifier(int storageModifier);

	//overrided methods
	int getSymbolType();
	string toString();

	bool isConstructor;
	bool isAbstract;
	bool isDefaultConstr;
private:
	int accessModifier;
	int storageModifier;
};
#endif

/*
=========================
PARAMETER:
=========================
*/
class Parameter : public Variable {
public :
	Parameter(char * name, int colNo, int lineNo, bool isDefault);
	~Parameter();

	bool isDefault;
private:

};
