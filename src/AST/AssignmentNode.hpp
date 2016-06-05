#pragma once

#include "Node.hpp"
#include <iostream>

class AssignmentNode : public Node {
public:
  Node *lhs, *rhs;
  AssignmentNode(Node *l, Node *r, int line , int col);

  virtual void print(ostream &os);

  virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

  virtual Node* optmize(OptimizationVistor *);

  virtual void accept(TypeErrorVisitor *);

  virtual void accept(CheckerVisitor*, TypeExpression*);

  virtual bool type_checking();

  virtual TypeExpression* getNodeType();
};
