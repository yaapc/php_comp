#pragma once
#include "ListNode.hpp"
#include "../SymbolTable/Symbol.h"

class ClassMethodNode : public Node {
public:
	Method* methodSym;
	Node* bodySts;          // body statements nodes
	ListNode* paramsList;   // a list of parameter nodes

	ClassMethodNode(Symbol* methodSym, Node* bodySts, Node* paramsList);

	virtual void print(ostream &os);

	virtual bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

};