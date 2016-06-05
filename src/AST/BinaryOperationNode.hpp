#pragma once

#include "Node.hpp"
#include <iostream>

#include "../Code Generator/AsmGenerator.h"

class BinaryOperationNode : public Node {
public:
  Node *left, *right;
  char* op_type;

  BinaryOperationNode(char* op, Node *lft, Node *rgt, int line, int col);

  virtual void print(ostream &os);

  virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

  virtual Node* optmize(OptimizationVistor *);

  virtual void accept(TypeErrorVisitor *);
  virtual void accept(CheckerVisitor*, TypeExpression*);

  virtual TypeExpression* getType();

  virtual bool type_checking();
};
