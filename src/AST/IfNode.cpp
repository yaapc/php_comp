#pragma once

#include "IfNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../TypeSystem/TypeError.hpp"

IfNode::IfNode(Node *cond, Node *bod, Node *el) : condition(cond), body(bod), else_node(dynamic_cast<ElseNode*>(el)) {}

   void IfNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"If\"]"
       << endl;
    condition->print(os);
    body->print(os);
    if (else_node) else_node->print(os);
    os << self << "->" << int(condition) << endl;
    os << self << "->" << int(body) << endl;
    if (else_node) os << self << "->" << int(else_node) << endl;
  }


  void IfNode::generate_code(CodeGneratorVistor *codeGneratorVistor){
	  codeGneratorVistor->visit(this);
	
  }


  bool IfNode::type_checking() {
	  if (this->condition->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID) {
		  this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
		  return true;
	  }
	  else {
		  this->nodeType = new TypeError("Expected Boolean Expression for the Condition.");
		  return false;
	  }
  }


  TypeExpression* IfNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }