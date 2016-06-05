#pragma once

#include "AssignmentNode.hpp"
#include "VariableNode.hpp"
#include "ClassCallNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "AST_Visitors\CheckerVisitor.hpp"


AssignmentNode::AssignmentNode(Node *l, Node *r, int line, int col) : lhs(l), rhs(r) {
	nodeType = nullptr;
	this->line = line;
	this->col = col;
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
	codeGneratorVistor->visit(this);
}

Node* AssignmentNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

  TypeExpression* AssignmentNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }


  bool AssignmentNode::type_checking() {
	  if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		  //this for second passes, if the current node is free of TypeError no need to re type_check it
		  return true; // pass it this time
	  }

	  this->lhs->type_checking();
	  this->rhs->type_checking();
	  //check lhs and rhs for TypeErrors
	  TypeError* typeError = dynamic_cast<TypeError*>(this->lhs->getNodeType());
	  if (typeError != nullptr) {
		  this->nodeType = typeError;
		  return false;
	  }
	  typeError = dynamic_cast<TypeError*>(this->rhs->getNodeType());
	  if (typeError != nullptr) {
		  this->nodeType = typeError;
		  return false;
	  }
	  
	  VariableNode* leftVar = dynamic_cast<VariableNode*>(lhs);
	  if(leftVar != nullptr){
		  if (leftVar->variable->isConst) {
			  this->nodeType = new TypeError("lvalue is not modifiable. line:" + this->line + string(",col:") + to_string(this->col));
			  return false;
		  }
	  }
	  else {
		  ClassCallNode* leftObj = dynamic_cast<ClassCallNode*>(lhs);
		  //double check
		  if (leftObj != nullptr) {
			  if (leftObj->isMethodCall) {
				  this->nodeType = new TypeError("lvalue is not modifiable. line:" + this->line + string(",col:") + to_string(this->col));
				  return false;
			  }
		  }
	  }

	  //TODO: should be implemented with coercion, not equivelantTo !!
	  if (rhs->getNodeType()->equivelantTo(lhs->getNodeType()->getTypeId())) {
		  this->nodeType = lhs->getNodeType();
		  return true;
	  }
	  else {
		  this->nodeType = new TypeError("no suitable conversion exists between " + 
			  TypeSystemHelper::getTypeName(lhs->getNodeType()) + " and " + 
				  TypeSystemHelper::getTypeName(rhs->getNodeType()) + string(". line:") + to_string(this->line) + string(", col : ") + to_string(this->col));
		  return false;
	  }
  }


void AssignmentNode::accept(TypeErrorVisitor* typeVisitor) {
	  typeVisitor->visit(this);
}

void AssignmentNode::accept(CheckerVisitor* visitor, TypeExpression* context) {
	visitor->visit(this, context);
}
