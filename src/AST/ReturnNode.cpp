#pragma once
#include "ReturnNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"


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
	//TODO: check if return TypeExpr is the same as the Return Type of the function defined
	if (returned_node){
		this->nodeType = this->returned_node->getNodeType();
	}
	return true;
}
