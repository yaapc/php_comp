#pragma once

#include "ClassCallNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"

ClassCallNode::ClassCallNode(Node* objectNode, string propertyString) {
	this->object = dynamic_cast<VariableNode*>(objectNode);
	this->propertyString = propertyString;
	this->nodeType = nullptr;
	this->isMethodCall = false; //TODO
}



void ClassCallNode::generate_code(CodeGneratorVistor *codeGneratorVistor) {
	codeGneratorVistor->visit(this);
}

Node* ClassCallNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

bool ClassCallNode::type_checking() {
	this->nodeType = this->object->getNodeType()->opDot(this->propertyString);
	return true;
}


void ClassCallNode::print(ostream &os) {
	int self = int(this);
	os << self
		<< "[label=\""
		<< "Object Call"
		<< "\"]"
		<< endl;
	if (object) object->print(os);
	int propertyInt = int(propertyString.c_str());
	os << propertyInt
		<< "[label=\""
		<< propertyString
		<< "\"]"
		<< endl;
	os << self << "->" << int(object) << endl;
	os << self << "->" << propertyInt << endl;
}