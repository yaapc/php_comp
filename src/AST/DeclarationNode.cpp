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

	if (this->getNodeType()->getTypeId() == INTEGER_TYPE_ID || this->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){
		AsmGenerator::store_global_int(this->variable->getId(),0);
	}

	// String variables are allocated in heap but the address is stored in data
	// the address is stored as int with .word type 
	if (this->getNodeType()->getTypeId() == STRING_TYPE_ID){
		string variable_mem_address = AsmGenerator::store_global_string(this->variable->getId(),NULL);

		//string variable should be initialize to "" 
		//I stored "" in .data with a name empty_string and all string variables use it

		string s0 = "s0";
		// load the address of empty string in s0
		AsmGenerator::la(s0,AsmGenerator::empty_string_address);
		// store the address of empty string in variable address
		AsmGenerator::sw(variable_mem_address,s0);
	}

	if (this->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
		AsmGenerator::store_global_float(this->variable->getId(),0.0f);
	}

	AsmGenerator::comment("Declaration Node/>");
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

	this->nodeType = TypesTable::getInstance()->getType(ERROR_TYPE_ID);
	return true;
  }