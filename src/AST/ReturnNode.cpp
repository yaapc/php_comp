#pragma once
#include "ReturnNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"

ReturnNode::ReturnNode(Node *exp) {
	this->returned_node = exp;
	this->nodeType = nullptr;
}

void ReturnNode::print(ostream &os) {
    auto self = int(this);
    os << self
       << "[label=\"return\"]"
       <<endl;
    if (returned_node) {
		returned_node->print(os);
      os << self << "->" << int(returned_node) << endl;
    }
  }

 void ReturnNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* ReturnNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

bool ReturnNode::type_checking() {
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		return true; // pass it this time
	}

	if (this->returned_node == nullptr) { // void return
		this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
		return true;
	}
	//TODO: check if return TypeExpr is the same as the Return Type of the function defined
	this->nodeType = this->returned_node->getNodeType();
	return true;
}
