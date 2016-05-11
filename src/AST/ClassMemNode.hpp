#pragma once 
#include "Node.hpp"
#include "../SymbolTable/Symbol.h"

class ClassMemNode : public Node {
public:
	DataMember* memberSym;

	ClassMemNode(Symbol* memSym);

	void print(ostream &os);
	bool type_checking();
	void generate_code();

};