#pragma once
#include "StaticCallNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "AST_Visitors\CheckerVisitor.hpp"

StaticCallNode::StaticCallNode(string className, string varNode, int line, int col) {
	this->className		= className;
	this->memberName		= varNode;
	this->line			= line;
	this->col			= col;
	this->nodeType		= nullptr;
	this->isMethodCall	= false;
}

StaticCallNode::StaticCallNode(string className, string varNode,Node *argsList, int line, int col) {
	this->className		= className;
	this->memberName	= varNode;
	this->argumentsList = argsList;
	this->line			= line;
	this->col			= col;
	this->nodeType		= nullptr;
	this->isMethodCall = true;
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

	int propInt = int(this->memberName.c_str());
	os << propInt
		<< "[label=\"" + this->memberName + "\"]"
		<< endl;
	
	os << self << "->" << classNameInt << endl;
	os << self << "->" << propInt << endl;	
}

bool StaticCallNode::type_checking() {
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		return true; // pass it this time
	}

	this->memberWrapper = TypeClass::getStaticProperty(this->className, this->memberName);
	if (this->memberWrapper == nullptr) {
		this->nodeType = new TypeError("Either " + this->className + " or " + this->memberName + " is undefined, " + string(" line:") + to_string(this->line) + string(",col:") + to_string(this->col));
		return false;
	}
	if (this->isMethodCall)
		this->nodeType = dynamic_cast<MethodWrapper*>(this->memberWrapper)->getReturnType();
	else
		this->nodeType = this->memberWrapper->getTypeExpr();
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

void StaticCallNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}

void StaticCallNode::accept(CheckerVisitor* visitor, TypeExpression* context) {
	visitor->visit(this, context);
}