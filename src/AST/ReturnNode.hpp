#pragma once

#include "Node.hpp"
#include <iostream>

class ReturnNode : public Node {
public:
  Node* returnend_expression;

  ReturnNode(Node *exp);

  virtual void print(ostream &os);

  virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);
};
