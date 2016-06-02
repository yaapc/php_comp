#pragma once
#include "BinaryOperationNode.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"

BinaryOperationNode::BinaryOperationNode(char* op, Node *lft, Node *rgt, int line, int col) : left(lft), right(rgt), op_type(op) {
	  nodeType = nullptr;
	  this->line = line;
	  this->col = col;
}

void BinaryOperationNode::print(ostream &os) {
	int self = int(this);
	os << self
		<< "[label=\""
		<< op_type
		<<"\"]"
		<<endl;
	if (left) left->print(os);
	if (right) right->print(os);
	os << self << "->" << int(left)<<endl;
	os << self << "->" << int(right)<<endl;
}

void BinaryOperationNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* BinaryOperationNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

 TypeExpression* BinaryOperationNode::getType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
}

 bool BinaryOperationNode::type_checking() {
		if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
			//this for second passes, if the current node is free of TypeError no need to re type_check it
			this->right->type_checking();
			this->left->type_checking();
			return true; // pass it this time
		}
	 
		this->right->type_checking();
		this->left->type_checking();

	    if (strcmp(op_type, "==") == 0){
			this->nodeType = left->getNodeType()->opEqual(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "!=") == 0){
			this->nodeType = left->getNodeType()->opEqual(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "<") == 0){
			this->nodeType = left->getNodeType()->opLessThan(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, ">") == 0){
			this->nodeType = left->getNodeType()->opGreaterThan(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, ">=") == 0){
			this->nodeType = left->getNodeType()->opGreaterOrEq(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "<=") == 0){
			this->nodeType = left->getNodeType()->opLessOrEq(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "+") == 0) {
			this->nodeType = left->getNodeType()->opPlus(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "-") == 0) {
			this->nodeType = left->getNodeType()->opMinus(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "*") == 0) {
			this->nodeType = left->getNodeType()->opMult(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "/") == 0) {
			this->nodeType = left->getNodeType()->opDiv(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "%") == 0) {
			this->nodeType = left->getNodeType()->opMod(this->right->getNodeType()->getTypeId());
			return true;
		}
				
		this->nodeType = new TypeError("Given operator is Undefind" + string("line:") + to_string(this->line) + string(", col : ") + to_string(this->col));
		return false;
  }


 void BinaryOperationNode::accept(TypeErrorVisitor* typeVisitor) {
	 typeVisitor->visit(this);
 }