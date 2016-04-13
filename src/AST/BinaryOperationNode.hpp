#pragma once

#include "Node.hpp"
#include <iostream>
//#include "../TypeSystem/TypeExpression.hpp"


class BinaryOperationNode : public Node {
public:
  Node *left, *right;
  char* op_type;

  BinaryOperationNode(char* op, Node *lft, Node *rgt) : left(lft), right(rgt), op_type(op) {
	  nodeType = nullptr;
  }

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

  TypeExpression* getType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }


  bool type_checking() {
	  TypeExpression* binType = this->left->getNodeType()->opAggregate(this->right->getNodeType()->getTypeId());
	  return true;
  }
};
