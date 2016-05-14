#pragma once

#include "ClassMemNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"


ClassMemNode::ClassMemNode(Symbol* memberSym) {
	this->memberSym = dynamic_cast<DataMember*>(memberSym);
	this->nodeType = nullptr;
}


bool ClassMemNode::type_checking() {
	
	if (strcmp(this->memberSym->getVariableType(), "int") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(INTEGER_TYPE_ID);
		return true;
	}


	if (strcmp(this->memberSym->getVariableType(), "bool") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(BOOLEAN_TYPE_ID);
		return true;
	}


	if (strcmp(this->memberSym->getVariableType(), "float") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
		return true;
	}

	if (strcmp(this->memberSym->getVariableType(), "string") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(STRING_TYPE_ID);
		return true;
	}

	string type = this->memberSym->getVariableType(); // converting char* to string
	this->nodeType = TypesTable::getInstance()->getClassType(type);

	if (this->nodeType == nullptr) { // no type found
		this->nodeType = new TypeError("Undefined");
		return false;
	}

	return true;
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
