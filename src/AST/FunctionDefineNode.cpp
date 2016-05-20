#pragma once
#include "FunctionDefineNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"


FunctionDefineNode::FunctionDefineNode(Symbol* func, Node* bod, Node* paramsList) {
    functionSym = dynamic_cast<Function*>(func);
    bodySts = dynamic_cast<ListNode*>(bod);
	this->paramsList = dynamic_cast<ListNode*>(paramsList);
	this->nodeType = nullptr;
	
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
	this->nodeType = TypesTable::getInstance()->buildFunctionType(this, this->functionSym);
	return true;
}

void FunctionDefineNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}
