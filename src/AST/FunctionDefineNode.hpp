#pragma once

#include "Node.hpp"
#include "../SymbolTable/Symbol.h"
#include "ListNode.hpp"
#include "ReturnNode.hpp"

/*
	A node that represents a Function Defination.
*/
class FunctionDefineNode : public Node {
public:
  Function* functionSym;      // a pointer to the corresponding symbol in symbol table
  Node* bodySts;          // body statements nodes
  ListNode* paramsList;       // a list of parameter nodes

	FunctionDefineNode(Symbol* func, Node* bod, Node* paramsList);

	virtual void print(ostream &os);

	virtual bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

private:
	TypeExpression* createReturnType(); // create a TypeExpression represents the return type.
};
