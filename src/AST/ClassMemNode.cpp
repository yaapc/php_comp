#pragma once

#include "ClassMemNode.hpp"
#include "../TypeSystem/TypeError.hpp"

ClassMemNode::ClassMemNode(Symbol* memberSym) {
	this->memberSym = dynamic_cast<DataMember*>(memberSym);
	this->nodeType = nullptr;
}


bool ClassMemNode::type_checking() {
	//TODO
	this->nodeType = new TypeError("Not Implemented, Yet");
	return false;
}

void ClassMemNode::generate_code() {
	//TODO
}

void ClassMemNode::print(ostream &os) {
	int self = int(this);
	
	string memLabel = memberSym ? memberSym->getName() : "!!!";

	os << int(this)
		<< "[label = \""
		<< memLabel
		<< "\"]"
		<< endl;
}
