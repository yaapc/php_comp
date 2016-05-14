#pragma once
#include "DeclarationNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "..\TypeSystem\TypesTable.h"
#include "../TypeSystem/TypeError.hpp"

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

void DeclarationNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

TypeExpression* DeclarationNode::getNodeType() {
	if (!this->nodeType)
		this->type_checking();
	return this->nodeType;
}

 bool DeclarationNode::type_checking() {
	if (strcmp(this->variable->getVariableType() ,"int")==0) {
		this->nodeType = TypesTable::getInstance()->getType(INTEGER_TYPE_ID);
		return true;
	}


	if (strcmp(this->variable->getVariableType() ,"bool")==0) {
		this->nodeType = TypesTable::getInstance()->getType(BOOLEAN_TYPE_ID);
		return true;
	}

	  
	if (strcmp(this->variable->getVariableType() ,"float")==0) {
		this->nodeType = TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
		return true;
	}

	if (strcmp(this->variable->getVariableType() ,"string")==0) {
		this->nodeType = TypesTable::getInstance()->getType(STRING_TYPE_ID);
		return true;
	}

	//check if a class is available:
	string type = this->variable->getVariableType(); // converting char* to string
	this->nodeType = TypesTable::getInstance()->getClassType(type);

	if (this->nodeType == nullptr) { // no type found
		this->nodeType = new TypeError("Undefined");
		return false;
	}

	return true;
  }