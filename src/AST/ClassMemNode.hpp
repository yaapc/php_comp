#pragma once 
#include "Node.hpp"
#include "../SymbolTable/Symbol.h"

class ClassMemNode : public Node {
public:
	ClassMemNode(Symbol* memSym);

	void print(ostream &os);
	
	bool type_checking();

	void generate_code();

	//Symbol interfacing methods:
	bool isStatic();
	bool isInited();
	bool isConst();
	string getName();
	DataMember* getMemSymbol();

private:
	DataMember* memberSym;
};