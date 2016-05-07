#pragma once
#include "EchoNode.hpp"
#include "ScalarNode.hpp"
#include "VariableNode.hpp"
#include "../Code Generator/AsmGenerator.h"
#include "ListNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"

EchoNode::EchoNode(Node* node) : expression(node) {
}

bool EchoNode::type_checking() {
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