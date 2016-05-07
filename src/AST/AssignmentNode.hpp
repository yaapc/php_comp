#pragma once

#include "Node.hpp"
#include <iostream>

class AssignmentNode : public Node {
public:
  Node *lhs, *rhs;
  AssignmentNode(Node *l, Node *r);

  virtual void print(ostream &os);

  virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

  virtual bool type_checking();

  virtual TypeExpression* getNodeType();
};
