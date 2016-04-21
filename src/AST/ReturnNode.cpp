#pragma once
#include "ReturnNode.hpp"


ReturnNode::ReturnNode(Node *exp) : returnend_expression(exp) {}

void ReturnNode::print(ostream &os) {
    auto self = int(this);
    os << self
       << "[label=\"return\"]"
       <<endl;
    if (returnend_expression) {
      returnend_expression->print(os);
      os << self << "->" << int(returnend_expression) << endl;
    }
  }