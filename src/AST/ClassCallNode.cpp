#pragma once

#include "ClassCallNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include <sstream>


ClassCallNode::ClassCallNode(Node* objectNode, string propertyString, int line, int col) {
	this->object = dynamic_cast<VariableNode*>(objectNode);
	this->propertyString = propertyString;
	this->nodeType = nullptr;
	this->isMethodCall = false;
	this->line = line;
}

ClassCallNode::ClassCallNode(Node* objectNode, string propertyString, Node* argumentsList, int line, int col) {
	this->object = dynamic_cast<VariableNode*>(objectNode);
	this->propertyString = propertyString;
	this->nodeType = nullptr;
	this->isMethodCall = true;
	this->argumentsList = dynamic_cast<ListNode*>(argumentsList);
	this->line = line;
	this->col = col;
}


void ClassCallNode::generate_code(CodeGneratorVistor *codeGneratorVistor) {
	codeGneratorVistor->visit(this);
}

Node* ClassCallNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

bool ClassCallNode::type_checking() {
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		return true; // pass it this time
	}

	this->object->type_checking();
	if (this->isMethodCall) {
		this->argumentsList->type_checking();
		this->nodeType = this->object->getNodeType()->opDot(this->propertyString, true, this->generateCallSignature(), member, this);
		if(this->nodeType == nullptr)
			this->nodeType = new TypeError(TypeSystemHelper::getTypeName(this->object->getNodeType()) + " doesn't have method " + propertyString +
				" with signature " + this->generateCallSignature() + " line:" + to_string(this->line) + ",col:" + to_string(this->col));
	}
	else {
		this->nodeType = this->object->getNodeType()->opDot(this->propertyString, false, "", member, this);
		if(this->nodeType == nullptr)
			this->nodeType = new TypeError(TypeSystemHelper::getTypeName(this->object->getNodeType()) + " doesn't have property " + propertyString +
				", remember properties don't need $ to access them." + " line:" + to_string(this->line) + ",col:" + to_string(this->col));
	}
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
		if (param->getNodeType()->getTypeId() < CLASS_TYPE_ID)
			os << TypeSystemHelper::getTypeName(param->getNodeType()->getTypeId());
		else {
			os << dynamic_cast<TypeClass*>(param->getNodeType())->getName();
	}
		firstParamFlag = false;
	}
	os << ")";
	return os.str();
}


void ClassCallNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}