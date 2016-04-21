#pragma once
#include "VariableNode.hpp"
#include "../TypeSystem/TypesTable.h"

VariableNode::VariableNode(Symbol *var) {
    variable = dynamic_cast<Variable*>(var);
  //  if (!variable) throw "Bad variable";
  }

void VariableNode::print(ostream &os) {
    os << int(this)
       << "[label = \""
       << (variable ? variable->getName() : "!!!")
       <<"\"]"
       << endl;
  }

  bool VariableNode::type_checking() {
	  if (this->variable->getVariableType() == "int") {
		  this->nodeType = TypesTable::getInstance()->getType(INTEGER_TYPE_ID);
	  }
	  else {
		  this->nodeType = TypesTable::getInstance()->getType(ERROR_TYPE_ID);
	  }
	  return true;
  }