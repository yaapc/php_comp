#pragma once

#include "IfNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/OptimizationVistor.hpp"

IfNode::IfNode(Node *cond, Node *bod, Node *el) : condition(cond), else_node(dynamic_cast<ElseNode*>(el)) 
{
	body = bod;
	body->hasScopeFrame = true;
}

   void IfNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"If\"]"
       << endl;
	if (condition) condition->print(os);
	if (body) body->print(os);
    if (else_node) else_node->print(os);
    os << self << "->" << int(condition) << endl;
    os << self << "->" << int(body) << endl;
    if (else_node) os << self << "->" << int(else_node) << endl;
  }


  void IfNode::generate_code(CodeGneratorVistor *codeGneratorVistor){
	  codeGneratorVistor->visit(this);
	
  }

Node* IfNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}


  bool IfNode::type_checking() {
	  if (this->condition->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID) {
		  this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
		  this->body->type_checking();
		  if (else_node)
			this->else_node->type_checking();
		  return true;
	  }
	  else {
		  this->nodeType = new TypeError("Expected Boolean Expression for the Condition.");
		  this->body->type_checking();
		  this->else_node->type_checking();
		  return false;
	  }
  }


  TypeExpression* IfNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }

