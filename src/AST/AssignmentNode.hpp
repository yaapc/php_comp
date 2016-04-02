#pragma once

#include "NOde.hpp"
#include <iostream>

class AssignmentNode : public Node {
public:
  Node *lhs, *rhs;
  AssignmentNode(Node *l, Node *r) : lhs(l), rhs(r) {}

  virtual void print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"=\"]"
       << endl;
    lhs->print(os);
    if (rhs) rhs->print(os);
    os << self << "->" << int(lhs) << endl;
    os << self << "->" << int(rhs) << endl;
  }
};
