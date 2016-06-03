#pragma once

#include "ClassMemNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"


ClassMemNode::ClassMemNode(Symbol* memberSym, int line, int col) {
	this->memberSym = dynamic_cast<DataMember*>(memberSym);
	this->nodeType = nullptr;
	this->line = line;
	this->col = col;
}

bool ClassMemNode::type_checking() {
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		return true; // pass it this time
	}
	
	if (strcmp(this->memberSym->getVariableType(), "int") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(INTEGER_TYPE_ID);
		return true;
	}


	if (strcmp(this->memberSym->getVariableType(), "bool") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(BOOLEAN_TYPE_ID);
		return true;
	}


	if (strcmp(this->memberSym->getVariableType(), "float") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
		return true;
	}

	if (strcmp(this->memberSym->getVariableType(), "string") == 0) {
		this->nodeType = TypesTable::getInstance()->getType(STRING_TYPE_ID);
		return true;
	}

	string type = this->memberSym->getVariableType(); // converting char* to string
	this->nodeType = TypesTable::getInstance()->getClassType(type);

	if (this->nodeType == nullptr) { // no type found
		this->nodeType = new TypeError("Class " + type + " is Undefined," + string(" line:") + to_string(this->line) + string(", col : ") + to_string(this->col));
		return false;
	}
	return true;
}

void ClassMemNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* ClassMemNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void ClassMemNode::print(ostream &os) {
	int self = int(this);
	
	string memLabel = memberSym ? memberSym->getName() : "!!!";

	os << int(this)
		<< "[label = \""
		<< memLabel
		<< "\"]"
		<< endl;
}

bool ClassMemNode::isInited() {
	return this->memberSym->isInit();
}

bool ClassMemNode::isStatic() {
	return this->memberSym->isStatic;
}

bool ClassMemNode::isConst() {
	return this->memberSym->isConst;
}

string ClassMemNode::getName() {
	return this->memberSym->getName();
}

DataMember* ClassMemNode::getMemSymbol() {
	return this->memberSym;
}


void ClassMemNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}