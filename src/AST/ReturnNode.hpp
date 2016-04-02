#pragma once

#include "Node.hpp"
#include <iostream>

class ReturnNode : public Node {
public:
  Node* returnend_expression;
  ReturnNode(Node *exp) : returnend_expression(exp) {}

  virtual void print(ostream &os) {
    auto self = int(this);
    os << self
       << "[label=\"return\"]"
       <<endl;
    if (returnend_expression) {
      returnend_expression->print(os);
      os << self << "->" << int(returnend_expression) << endl;
    }
  }
};
