#pragma once

#include "Node.hpp"
#include <iostream>
#include "../SymbolTable/Symbol.h"

class VariableNode : public Node {
public:
	Variable *variable;

	VariableNode(Symbol *var);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual bool type_checking();

	virtual TypeExpression* getNodeType();
};
