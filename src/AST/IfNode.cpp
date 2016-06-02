#pragma once

#include "IfNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"

IfNode::IfNode(Node *cond, Node *bod, Node *el, int line, int col) : condition(cond), else_node(dynamic_cast<ElseNode*>(el))
{
	body = bod;
	body->hasScopeFrame = true;
	this->col = col;
	this->line = line;
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
	  if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		  //this for second passes, if the current node is free of TypeError no need to re type_check it
		  this->body->type_checking();
		  return true; // pass it this time
	  }

	  if (this->condition->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID) {
		  this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
		  this->body->type_checking();
		  if (else_node)
			this->else_node->type_checking();
		  return true;
	  }
	  else {
		  this->nodeType = new TypeError("Expected Boolean Expression for the Condition. " + string(" line:") + to_string(this->line) + string(",col:") + to_string(this->col));
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

  void IfNode::accept(TypeErrorVisitor* typeVisitor) {
	  typeVisitor->visit(this);
  }