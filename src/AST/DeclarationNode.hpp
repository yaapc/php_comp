#pragma once

#include "Node.hpp"
#include <iostream>
#include "../SymbolTable/Symbol.h"

class DeclarationNode : public Node {
public:
  Variable *variable;
  DeclarationNode(Symbol *v, int line, int col);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);
	virtual void accept(CheckerVisitor*, TypeExpression*);

	virtual TypeExpression* getNodeType();

	virtual bool type_checking();
};
