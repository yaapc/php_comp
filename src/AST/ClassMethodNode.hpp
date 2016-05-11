#pragma once
#include "Node.hpp"
#include "../SymbolTable/Symbol.h"

class ClassMethodNode : public Node {
public:
	Method* methodSym;

	ClassMethodNode(Symbol* methodSym);

	void print(ostream &os);
	bool type_checking();
	void generate_code();

};