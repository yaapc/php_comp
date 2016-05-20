#pragma once
#include "Node.hpp"
#include "../SymbolTable/Symbol.h"

class ClassMethodNode : public Node {
public:
	Method* methodSym;

	ClassMethodNode(Symbol* methodSym);

	virtual void print(ostream &os);

	virtual bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

};