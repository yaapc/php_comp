#pragma once
#ifndef PAR_NODE_H
#define PAR_NODE_H

#include "ParameterNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"


ParameterNode::ParameterNode(Symbol* parSym, bool isDefault) {
	this->nodeType = nullptr; // VS-2015
	this->isDefault = isDefault;
	this->parSym = dynamic_cast<Parameter*>(parSym);
}


void ParameterNode::print(ostream &os) {
	string parLabel = parSym ? parSym->getName() : "!!!";
	if (isDefault)
		parLabel = parLabel.append(" *");
	os << int(this)
		<< "[label = \""
		<< parLabel
		<< "\"]"
		<< endl;
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