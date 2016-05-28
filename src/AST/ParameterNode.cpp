#pragma once
#ifndef PAR_NODE_H
#define PAR_NODE_H

#include "ParameterNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "../TypeSystem/TypesTable.h"


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

 Node* ParameterNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

bool ParameterNode::type_checking() {
	if (strcmp(this->parSym->getVariableType(), "int") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(INTEGER_TYPE_ID);
		return true;
	}


	if (strcmp(this->parSym->getVariableType(), "bool") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(BOOLEAN_TYPE_ID);
		return true;
	}


	if (strcmp(this->parSym->getVariableType(), "float") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
		return true;
	}

	if (strcmp(this->parSym->getVariableType(), "string") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(STRING_TYPE_ID);
		return true;
	}

	//check if a class is available:
	string type = this->parSym->getVariableType(); // converting char* to string
	this->nodeType = TypesTable::getInstance()->getClassType(type);

	if (this->nodeType == nullptr) { // no type found
		this->nodeType = new TypeError("Undefined");
		return false;
	}

	return true;
}

#endif