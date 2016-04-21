#pragma once
#include "WhileNode.hpp"


WhileNode::WhileNode(Node *condition, Node *body) : condition(condition), body(body) {}

 void WhileNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"while\"]"
       << endl;
    condition->print(os);
    body->print(os);
    os << self << "->" << int(condition) << endl;
    os << self << "->" << int(body) << endl;
  }