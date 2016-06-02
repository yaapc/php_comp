#pragma once
#include "ClassMethodNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "../TypeSystem/TypesTable.h"

ClassMethodNode::ClassMethodNode(Symbol* methodSym, Node* bodySts, Node* params, int line, int col) {
	this->nodeType = nullptr;
	this->methodSym = dynamic_cast<Method*>(methodSym);
	this->bodySts = bodySts;
	this->paramsList = dynamic_cast<ListNode*>(params);
	this->line = line;
	this->col = col;
}

bool ClassMethodNode::type_checking() {
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		this->bodySts->type_checking();
		return true; // pass it this time
	}

	//TODO
	//build the TypeFunction although the body of the function might have errors.
	this->nodeType = TypesTable::getInstance()->buildFunctionType(this, this->methodSym);
	this->bodySts->type_checking();
	return true;
}


void ClassMethodNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* ClassMethodNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void ClassMethodNode::print(ostream &os) {
	int self = int(this);

	string methodLabel = methodSym ? methodSym->getName() : "!!!";

	os << int(this)
		<< "[label = \""
		<< methodLabel
		<< "\"]"
		<< endl;
}

void ClassMethodNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}





