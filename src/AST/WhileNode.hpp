#pragma once

#include "Node.hpp"
#include <iostream>

class WhileNode : public Node {
public:
  Node *condition;
  Node *body;

  WhileNode(Node *condition, Node *body) : condition(condition), body(body) {}

  virtual void print(ostream &os) override {
    int self = int(this);
    os << self
       << "[label=\"while\"]"
       << endl;
    condition->print(os);
    body->print(os);
    os << self << "->" << int(condition) << endl;
    os << self << "->" << int(body) << endl;
  }
};
