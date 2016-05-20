#pragma once 
#include "Node.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/OptimizationVistor.hpp"

void Node::print(ostream& os) {
	os << int(this) << "[label = \"Node\"]" << endl;
	}

  bool Node::type_checking() {
	  this->nodeType = TypesTable::getInstance()->getType(ERROR_TYPE_ID);
    return false;
  }


  TypeExpression* Node::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }

  void Node::generate_code(CodeGneratorVistor *codeGneratorVistor){

  }

Node* Node::optmize(OptimizationVistor *optimizationVistor)
{
	return this;
}