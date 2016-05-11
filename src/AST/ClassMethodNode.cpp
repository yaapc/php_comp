#pragma once
#include "ClassMethodNode.hpp"
#include "../TypeSystem/TypeError.hpp"


ClassMethodNode::ClassMethodNode(Symbol* methodSym) {
	this->nodeType = nullptr;
	this->methodSym = dynamic_cast<Method*>(methodSym);
}

bool ClassMethodNode::type_checking() {
	//TODO
	this->nodeType = new TypeError("Not Implemented, yet");
	return false;
}


void ClassMethodNode::generate_code() {
	//TODO
}

void ClassMethodNode::print(ostream &os) {
	int self = int(this);

	string methodLabel = methodSym ? methodSym->getName() : "!!!";

	os << int(this)
		<< "[label = \""
		<< methodLabel
		<< "\"]"
		<< endl;
}







