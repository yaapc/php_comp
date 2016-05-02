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
		AsmGenerator::store_int(this->variable->getId(),0);
	}

	if (this->getNodeType()->getTypeId() == STRING_TYPE_ID){
		AsmGenerator::store_string(this->variable->getId(),"");
	}

	if (this->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
		AsmGenerator::store_float(this->variable->getId(),0.0f);
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