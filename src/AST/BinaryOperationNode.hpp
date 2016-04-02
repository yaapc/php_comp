#pragma once

#include "Node.hpp"
#include <iostream>

class BinaryOperationNode : public Node {
public:
  Node *left, *right;
  char* op_type;
  BinaryOperationNode(char* op, Node *lft, Node *rgt) : left(lft), right(rgt), op_type(op) {}

  virtual void print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\""
       << op_type
       <<"\"]"
       <<endl;
    if (left) left->print(os);
    if (right) right->print(os);
    os << self << "->" << int(left)<<endl;
    os << self << "->" << int(right)<<endl;
  }
};
