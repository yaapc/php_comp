#pragma once 
#include "Node.hpp"
#include "../SymbolTable/Symbol.h"

class ClassMemNode : public Node {
public:
	DataMember* memberSym;

	ClassMemNode(Symbol* memSym);

	virtual void print(ostream &os);

	virtual bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

};