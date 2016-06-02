#pragma once

#include "Node.hpp"
#include <iostream>
#include "../SymbolTable/Symbol.h"
#include <string>

class Scope;
class VariableNode : public Node {
public:
	Variable *variable;
	string variableName;
	Scope* variableScope;

	VariableNode(Symbol *var);

	VariableNode(Scope* scope, string varName);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual bool type_checking();

	virtual TypeExpression* getNodeType();
};
