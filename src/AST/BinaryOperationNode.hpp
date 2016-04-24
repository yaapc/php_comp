#pragma once

#include "Node.hpp"
#include <iostream>

#include "../Code Generator/AsmGenerator.h"

class BinaryOperationNode : public Node {
public:
  Node *left, *right;
  char* op_type;
  static int temp_label_count ;

  BinaryOperationNode(char* op, Node *lft, Node *rgt);

  virtual void print(ostream &os);

  virtual void generate_code();

  TypeExpression* getType();

  bool type_checking();
};
