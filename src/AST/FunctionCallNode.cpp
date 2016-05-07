#pragma once

#include "FunctionCallNode.hpp"
#include "../TypeSystem/TypeError.hpp"

FunctionCallNode::FunctionCallNode(string name, Node* argsList) {
	this->nodeType = nullptr;
	this->name = name;
	this->argumentsList = dynamic_cast<ListNode*>(argsList);
}

void FunctionCallNode::print(ostream &os) {
	int self = int(this);
	os << self << "[label = \""
		<< name << "() Call"
		<< "\"]"
		<< endl;
	os << self << "->" << int(argumentsList) << endl;
	if (argumentsList) argumentsList->print(os);
}

bool FunctionCallNode::type_checking() {
	//TODO:
	this->nodeType = new TypeError("Not implmented yet.");
	return false;
}

void FunctionCallNode::generate_code() {
	//TODO:
}