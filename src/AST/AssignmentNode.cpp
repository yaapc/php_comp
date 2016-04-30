#pragma once

#include "AssignmentNode.hpp"
#include "VariableNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/AsmGenerator.h"


AssignmentNode::AssignmentNode(Node *l, Node *r) : lhs(l), rhs(r) {
	nodeType = nullptr;
}

void AssignmentNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"=\"]"
       << endl;
    lhs->print(os);
    if (rhs) rhs->print(os);
    os << self << "->" << int(lhs) << endl;
    os << self << "->" << int(rhs) << endl;
  }
  
  
void AssignmentNode::generate_code()
{
	AsmGenerator::comment("<Assignment Node");
	string s0 = "s0";
	string s1 = "s1";
	AsmGenerator::comment("<Binary Operation Left node");
	lhs->generate_code();
	AsmGenerator::comment("Binary Operation Left node/>");

	AsmGenerator::comment("<Binary Operation right node");
	rhs->generate_code();
	AsmGenerator::comment("Binary Operation right node/>");
	 
	AsmGenerator::pop(s1);
	AsmGenerator::pop(s0);

	AsmGenerator::sw(s1,0,s0);
	AsmGenerator::comment("Assignment Node/>");
}

  TypeExpression* AssignmentNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }


  bool AssignmentNode::type_checking() {
	  if (dynamic_cast<VariableNode*>(lhs)->variable->isConst) {
		  this->nodeType = new TypeError("lvalue is not modifiable.");
		  return false;
	  }
	  else {
		  //TODO: should be implemented with coercion, not equivelantTo !!
		  if (lhs->getNodeType()->equivelantTo(rhs->getNodeType()->getTypeId())) {
			  this->nodeType = lhs->getNodeType();
			  return true;
		  }
		  else {
			  this->nodeType = new TypeError("no suitable conversion exists between " + 
				  TypeSystemHelper::getTypeName(lhs->getNodeType()->getTypeId()) + " and " + 
					  TypeSystemHelper::getTypeName(rhs->getNodeType()->getTypeId()));
			  return false;
		  }
	  }
  }