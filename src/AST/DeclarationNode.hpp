#pragma once

#include "Node.hpp"
#include <iostream>
#include "../SymbolTable/Symbol.h"

class DeclarationNode : public Node {
public:
  Variable *variable;
  DeclarationNode(Symbol *v);

  virtual void print(ostream &os);
  virtual void generate_code();

  TypeExpression* getNodeType();
  bool type_checking();
};
