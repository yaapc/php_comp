#pragma once

#include "ClassCallNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include <sstream>


ClassCallNode::ClassCallNode(Node* objectNode, string propertyString) {
	this->object = dynamic_cast<VariableNode*>(objectNode);
	this->propertyString = propertyString;
	this->nodeType = nullptr;
	this->isMethodCall = false;
}

ClassCallNode::ClassCallNode(Node* objectNode, string propertyString, Node* argumentsList) {
	this->object = dynamic_cast<VariableNode*>(objectNode);
	this->propertyString = propertyString;
	this->nodeType = nullptr;
	this->isMethodCall = true;
	this->argumentsList = dynamic_cast<ListNode*>(argumentsList);
}


void ClassCallNode::generate_code(CodeGneratorVistor *codeGneratorVistor) {
	codeGneratorVistor->visit(this);
}

Node* ClassCallNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

bool ClassCallNode::type_checking() {
	if(this->isMethodCall)
		this->nodeType = this->object->getNodeType()->opDot(this->propertyString, true, this->generateCallSignature(), member);
	else
		this->nodeType = this->object->getNodeType()->opDot(this->propertyString, false, "", member);
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

string ClassCallNode::generateCallSignature() {
	std::ostringstream os;
	bool firstParamFlag = true;
	os << "func_" << this->propertyString << "(";
	for (auto &param : this->argumentsList->nodes) {
		if (!firstParamFlag)
			os << ",";
		os << TypeSystemHelper::getTypeName(param->getNodeType()->getTypeId());
		firstParamFlag = false;
	}
	os << ")";
	return os.str();
}
