#pragma once

#include "Node.hpp"
#include <iostream>

class DeclarationNode : public Node {
public:
  Variable *variable;
  DeclarationNode(Symbol *v) : variable(dynamic_cast<Variable*>(v)) {}

  virtual void print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"Declare "
       << (variable ? variable->getName() : "!!!")
       << "\"]"
       << endl;
  }
};
