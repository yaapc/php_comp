#pragma once
#include "FunctionNode.hpp"
  

FunctionNode::FunctionNode(Symbol* func, Node* bod) {
    function = dynamic_cast<Function*>(func);
    body = dynamic_cast<ListNode*>(bod);
}

void FunctionNode::print(ostream &os) {
    int self = int(this);
    os << self << "[label = \""
       << function->getName()
       << "\"]"
       <<endl;
    os << self << "->" << int(body) << endl;
    if (body) body->print(os);
}

TypeExpression* FunctionNode::getNodeType() {
	return nullptr;
}

