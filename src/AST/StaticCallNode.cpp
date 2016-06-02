#pragma once
#include "StaticCallNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"

StaticCallNode::StaticCallNode(string className, string varNode) {
	this->className = className;
	this->propName = varNode;
}

void StaticCallNode::print(ostream &os) {
	auto self = int(this);
	os << self
		<< "[label=\"static call\"]"
		<< endl;
	
	int classNameInt = int(this->className.c_str());
	os << classNameInt
		<< "[label=\"" + this->className + "\"]"
		<< endl;

	int propInt = int(this->propName.c_str());
	os << propInt
		<< "[label=\"" + this->propName + "\"]"
		<< endl;
	
	os << self << "->" << classNameInt << endl;
	os << self << "->" << propInt << endl;	
}

bool StaticCallNode::type_checking() {
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		return true; // pass it this time
	}

	this->propWrapper = TypeClass::getStaticProperty(this->className, this->propName);
	if (this->propWrapper == nullptr) {
		this->nodeType = new TypeError("Either " + this->className + " or " + this->propName + " is undefined");
		return false;
	}
	this->nodeType = this->propWrapper->getTypeExpr();
	return true;
}

void StaticCallNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* StaticCallNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}