#pragma once
#include "WhileNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "../TypeSystem/TypesTable.h"

WhileNode::WhileNode(Node *condition, Node *body, int line, int col) : condition(condition) {
	this->body = body;
	this->body->hasScopeFrame = true;
	this->line = line;
	this->col = col;
	nodeType = nullptr;
}

 void WhileNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"while\"]"
       << endl;
	if(condition) condition->print(os);
    if(body) body->print(os);
    os << self << "->" << int(condition) << endl;
    os << self << "->" << int(body) << endl;
  }

 void WhileNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* WhileNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void WhileNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}

  bool WhileNode::type_checking() {
	  if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		  //this for second passes, if the current node is free of TypeError no need to re type_check it
		  this->body->type_checking();
		  return true; // pass it this time
	  }

	  if (this->condition->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID) {
		  this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
		  this->body->type_checking();
	  }
	  else {
		  this->nodeType = new TypeError("Expected Boolean Expression for the Condition. " + string(" line:") + to_string(this->line) + string(",col:") + to_string(this->col));
		  this->body->type_checking();
		  return false;
	  }
  }