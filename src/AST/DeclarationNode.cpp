#pragma once
#include "DeclarationNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "..\TypeSystem\TypesTable.h"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"

DeclarationNode::DeclarationNode(Symbol *v, int line, int col) : variable(dynamic_cast<Variable*>(v)) {
	nodeType = nullptr;
	this->line = line;
	this->col = col;
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

Node* DeclarationNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

TypeExpression* DeclarationNode::getNodeType() {
	if (!this->nodeType)
		this->type_checking();
	return this->nodeType;
}

 bool DeclarationNode::type_checking() {
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		return true; // pass it this time
	}

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
		this->nodeType = new TypeError("Variable Type is Undefined." + string(" line:") + to_string(this->line) + string(",col:") + to_string(this->col));
		return false;
	}

	return true;
  }


 void DeclarationNode::accept(TypeErrorVisitor* typeVisitor) {
	 typeVisitor->visit(this);
 }