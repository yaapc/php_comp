#pragma once
#include "VariableNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../TypeSystem/TypeError.hpp"


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
	  	  
	  //check if a class is available:
	  string type = this->variable->getVariableType(); // converting char* to string
	  this->nodeType = TypesTable::getInstance()->getClassType(type);

	  if (this->nodeType == nullptr) { // no type found
		  this->nodeType = new TypeError("Undefined");
		  return false;
	  }

	  return true;
  }

    TypeExpression* VariableNode::getNodeType() {
	  if (this->nodeType == nullptr)
		  this->type_checking();
	  return this->nodeType;
  }


void VariableNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}