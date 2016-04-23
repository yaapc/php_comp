#pragma once

#include "Node.hpp"
#include "../SymbolTable/Symbol.h"
#include "ListNode.hpp"


class FunctionNode : public Node {
public:
  Function* function;
  ListNode* body;

  FunctionNode(Symbol* func, Node* bod);

  virtual void print(ostream &os);
 
  TypeExpression* getNodeType();
};
