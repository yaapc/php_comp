#pragma once
#include "FunctionDefineNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"

FunctionDefineNode::FunctionDefineNode(Symbol* func, Node* bod, Node* paramsList, int line, int col) {
    functionSym = dynamic_cast<Function*>(func);
    bodySts = dynamic_cast<ListNode*>(bod);
	this->paramsList = dynamic_cast<ListNode*>(paramsList);
	this->nodeType = nullptr;
	this->line = line;
	this->col = col;
  }

void FunctionDefineNode::print(ostream &os) {
    int self = int(this);
    os << self << "[label = \""
       << functionSym->getName()
       << "\"]"
       <<endl;
    os << self << "->" << int(bodySts) << endl;
	os << self << "->" << int(paramsList) << endl;
    if (bodySts) bodySts->print(os);
	//if (this->paramsList) paramsList->print(os);
	//overriding vertual ListNode print method:
	self = (int)paramsList;
	os << self << "[label = \"Params\"]" << endl;
	for (auto &node : paramsList->nodes) {
		if (node == nullptr) continue;
		node->print(os);
		os << self << "->" << (int)node << endl;
	}

 }

bool FunctionDefineNode::type_checking() {	
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		this->bodySts->type_checking();
		return true; // pass it this time
	}
	
	//build the TypeFunction although the body of the function might have errors.
	this->nodeType = TypesTable::getInstance()->buildFunctionType(this, this->functionSym);
	this->bodySts->type_checking();//perform type checking on function body 
	return true;
}

void FunctionDefineNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* FunctionDefineNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void FunctionDefineNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}