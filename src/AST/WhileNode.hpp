#pragma once

#include "Node.hpp"
#include <iostream>

class WhileNode : public Node {
public:
  Node *condition;
  Node *body;

  WhileNode(Node *condition, Node *body);

  virtual void print(ostream &os);

  virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);
};
