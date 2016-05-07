#pragma once
#include "BinaryOperationNode.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"

BinaryOperationNode::BinaryOperationNode(char* op, Node *lft, Node *rgt) : left(lft), right(rgt), op_type(op) {
	  nodeType = nullptr;
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

 TypeExpression* BinaryOperationNode::getType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
}

 bool BinaryOperationNode::type_checking() {
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
				
		this->nodeType = TypesTable::getInstance()->getType(ERROR_TYPE_ID);
		return false;
  }

 int BinaryOperationNode::temp_label_count = 0;