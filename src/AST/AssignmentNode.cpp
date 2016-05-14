#pragma once

#include "AssignmentNode.hpp"
#include "VariableNode.hpp"
#include "ClassCallNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"


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
  
  
void AssignmentNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	//sleep for now : codeGneratorVistor->visit(this);
}

  TypeExpression* AssignmentNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }


  bool AssignmentNode::type_checking() {
	  VariableNode* leftVar = dynamic_cast<VariableNode*>(lhs);
	  if(leftVar != nullptr){
		  if (leftVar->variable->isConst) {
			  this->nodeType = new TypeError("lvalue is not modifiable.");
			  return false;
		  }
	  }
	  else {
		  ClassCallNode* leftObj = dynamic_cast<ClassCallNode*>(lhs);
		  //double check
		  if (leftObj != nullptr) {
			  if (leftObj->isMethodCall) {
				  this->nodeType = new TypeError("lvalue is not modifiable.");
				  return false;
			  }
		  }
	  }
	  
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