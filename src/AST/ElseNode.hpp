#pragma once

#include "Node.hpp"
#include <iostream>

class ElseNode : public Node {
public:
  Node *body;

  ElseNode(Node *body);

  virtual void print(ostream &os);

  virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);
};