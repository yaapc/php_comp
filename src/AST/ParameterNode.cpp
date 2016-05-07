#pragma once
#ifndef PAR_NODE_H
#define PAR_NODE_H

#include "ParameterNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"


ParameterNode::ParameterNode(Symbol* parSym,Node *defaultValueNode,bool isDefault) {
	this->nodeType = nullptr; // VS-2015
	this->defaultValueNode = defaultValueNode;
	this->isDefault = isDefault;
	this->parSym = dynamic_cast<Parameter*>(parSym);
}


void ParameterNode::print(ostream &os) {
	int self = int(this);
	string parLabel = parSym ? parSym->getName() : "!!!";
	if (isDefault)
		parLabel = parLabel.append(" *");
	os << int(this)
		<< "[label = \""
		<< parLabel
		<< "\"]"
		<< endl;
	if (defaultValueNode){
		defaultValueNode->print(os);
		os << self << "->" << int(defaultValueNode) << endl;
	}
}

 void ParameterNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

bool ParameterNode::type_checking() {
	this->nodeType = new TypeError("Not Implemented yet.");
	return false;
}

#endif