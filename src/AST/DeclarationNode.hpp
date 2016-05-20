#pragma once

#include "Node.hpp"
#include <iostream>
#include "../SymbolTable/Symbol.h"

class DeclarationNode : public Node {
public:
  Variable *variable;
  DeclarationNode(Symbol *v);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual TypeExpression* getNodeType();

	virtual bool type_checking();
};
