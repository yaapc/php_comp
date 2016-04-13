#pragma once

#include "Node.hpp"
#include <iostream>
#include "../SymbolTable/Symbol.h"

class VariableNode : public Node {
public:
  Variable *variable;
  VariableNode(Symbol *var) {
    variable = dynamic_cast<Variable*>(var);
  //  if (!variable) throw "Bad variable";
  }

  virtual void print(ostream &os) {
    os << int(this)
       << "[label = \""
       << (variable ? variable->getName() : "!!!")
       <<"\"]"
       << endl;
  }

  bool type_checking() {
	  if (this->variable->getVariableType() == "int") {
		  this->nodeType = TypesTable::getInstance()->getType(INTEGER_TYPE_ID);
	  }
	  else {
		  this->nodeType = TypesTable::getInstance()->getType(ERROR_TYPE_ID);
	  }
	  return true;
  }
};
