#pragma once 
#include "Node.hpp"
#include "../SymbolTable/Symbol.h"

class ClassMemNode : public Node {
public:
	ClassMemNode(Symbol* memSym);

	virtual void print(ostream &os);

	virtual bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	//Symbol interfacing methods:
	bool isStatic();
	bool isInited();
	bool isConst();
	string getName();
	DataMember* getMemSymbol();

private:
	DataMember* memberSym;
};
