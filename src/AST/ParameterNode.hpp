#pragma once

#include "Node.hpp"
#include "../SymbolTable/Symbol.h"


class ParameterNode : public Node {
public : 

	Node *defaultValueNode;

	bool isDefault;

	Parameter* parSym; // parameter symbol

	ParameterNode(Symbol* parSym,Node *defaultValueNode,bool isDefault);

	bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	void print(ostream &os);

	virtual Node* optmize(OptimizationVistor *);


};