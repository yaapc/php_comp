#pragma once
#include "EchoNode.hpp"
#include "ScalarNode.hpp"
#include "VariableNode.hpp"
#include "../Code Generator/AsmGenerator.h"
#include "ListNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "../TypeSystem/TypeError.hpp"

EchoNode::EchoNode(Node* node) : expression(node) {
}

bool EchoNode::type_checking() {
	//this for second passes, if the current node is free of TypeError no need to re type_check it
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		return true; // pass it this time
	}

	return this->expression->type_checking();
}


 TypeExpression* EchoNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }

void EchoNode::print(ostream& os) {
        int self = int(this);
        os << self << "[label = \"Echo\"]" <<endl;
        expression->print(os);
        os << self << "->" << int(expression) << endl;
}

void EchoNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* EchoNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}