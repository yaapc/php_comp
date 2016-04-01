#pragma once

#include "Node.hpp"
#include "../SymbolTable/Symbol.h"

class FunctionNode : public Node {
public:
  Function* function;
  ListNode* body;

  FunctionNode(Symbol* func, Node* bod) {
    function = dynamic_cast<Function*>(func);
    body = dynamic_cast<ListNode*>(bod);
  }

  virtual void print(ostream &os) {
    int self = int(this);
    os << self << "[label = \""
       << function->getName()
       << "\"]"
       <<endl;
    os << self << "->" << int(body) << endl;
    if (body) body->print(os);
  }
};
