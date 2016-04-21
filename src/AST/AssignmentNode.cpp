#pragma once

#include "AssignmentNode.hpp"

AssignmentNode::AssignmentNode(Node *l, Node *r) : lhs(l), rhs(r) {}

void AssignmentNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"=\"]"
       << endl;
    lhs->print(os);
    if (rhs) rhs->print(os);
    os << self << "->" << int(lhs) << endl;
    os << self << "->" << int(rhs) << endl;
  }
  
  
  void AssignmentNode::generate_code(){
  }