#pragma once
#include "DeclarationNode.hpp"
#include "../Code Generator/AsmGenerator.h"
#include "..\TypeSystem\TypesTable.h"

DeclarationNode::DeclarationNode(Symbol *v) : variable(dynamic_cast<Variable*>(v)) {
	nodeType = nullptr;
}

void DeclarationNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"Declare "
       << (variable ? variable->getName() : "!!!")
       << "\"]"
       << endl;
}

void DeclarationNode::generate_code(){
	AsmGenerator::comment("<Declaration Node");

	AsmGenerator::store_int(this->variable->getId(),0);

	AsmGenerator::comment("Declaration Node/>");
}

TypeExpression* DeclarationNode::getNodeType() {
	if (!this->nodeType)
		this->type_checking();
	return this->nodeType;
}

bool DeclarationNode::type_checking() {
	//DeclarationNode is always type-true
	this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
	return true;
}