#pragma once

#include "Node.hpp"
#include "../SymbolTable/Symbol.h"
#include "ListNode.hpp"

/*
	A node that represents a Function Defination.
*/
class FunctionDefineNode : public Node {
public:
  Function* functionSym; // a pointer to the corresponding symbol in symbol table
  ListNode* bodySts; // body statements

  FunctionDefineNode(Symbol* func, Node* bod);

  virtual void print(ostream &os);
 
};
