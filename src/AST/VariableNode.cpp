#pragma once
#include "VariableNode.hpp"
#include "../Code Generator/AsmGenerator.h"
#include "../TypeSystem/TypesTable.h"

VariableNode::VariableNode(Symbol *var) {
    variable = dynamic_cast<Variable*>(var);
	nodeType = nullptr;
	 //  if (!variable) throw "Bad variable";
  }

void VariableNode::print(ostream &os) {
    os << int(this)
       << "[label = \""
       << (variable ? variable->getName() : "!!!")
       <<"\"]"
       << endl;
  }

  bool VariableNode::type_checking() {
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

    TypeExpression* VariableNode::getNodeType() {
	  if (this->nodeType == nullptr)
		  this->type_checking();
	  return this->nodeType;
  }


void VariableNode::generate_code(){
	AsmGenerator::comment("<Variable Node");
	string s0 = "s0";

	// In primitive types we care about value so we have to load it
	if (this->nodeType->getTypeId() == INTEGER_TYPE_ID	|| nodeType->getTypeId() == BOOLEAN_TYPE_ID){
		string variable_mem_address = AsmGenerator::global_int+to_string(variable->getId());
		AsmGenerator::lw(s0,variable_mem_address); 		//Get memory address and put in s0
		AsmGenerator::push(s0);
	}

	// String and float literals are stored in data so we only load the address of those literals
	if (nodeType->getTypeId()== STRING_TYPE_ID){
		string variable_mem_address = AsmGenerator::gloabl_string+to_string(variable->getId());
		AsmGenerator::la(s0,variable_mem_address); 		//Get memory address and put in s0
		AsmGenerator::push(s0);
	}
		
	if (nodeType->getTypeId() == FLOAT_TYPE_ID){
		string variable_mem_address = AsmGenerator::global_float+to_string(variable->getId());
		AsmGenerator::la(s0,variable_mem_address); 	//Get memory address and put in s0
		AsmGenerator::push(s0);
	}
	AsmGenerator::comment("Variable Node/>");
    
}