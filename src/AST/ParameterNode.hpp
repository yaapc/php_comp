#pragma once

#include "Node.hpp"
#include "../SymbolTable/Symbol.h"


class ParameterNode : public Node {
public : 
	bool isDefault;
	Parameter* parSym; // parameter symbol

	ParameterNode(Symbol* parSym, bool isDefault);

	bool type_checking();
	void generate_code();
	void print(ostream &os);


};