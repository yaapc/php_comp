#pragma once
#include "VariableNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "../SymbolTable/Scope.h"
#include "../SymbolTable/SymbolsParser.h"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "AST_Visitors\CheckerVisitor.hpp"

VariableNode::VariableNode(Symbol *var, int line, int col) {
    variable = dynamic_cast<Variable*>(var);
	nodeType = nullptr;
	this->variableScope = nullptr;
	this->variableName = variable->getName();
	this->line = line;
	this->col = col;
	 //  if (!variable) throw "Bad variable";
  }

VariableNode::VariableNode(Scope* scope, string varName, int line, int col) {
	this->variable = nullptr;
	this->nodeType = nullptr;
	this->variableScope = scope;
	this->variableName = varName;
	this->line = line;
	this->col = col;
}

void VariableNode::print(ostream &os) {
    os << int(this)
       << "[label = \""
       << (variable ? variable->getName() : "!!!")
       <<"\"]"
       << endl;
  }

  bool VariableNode::type_checking() {
	  if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		  //this for second passes, if the current node is free of TypeError no need to re type_check it
		  return true; // pass it this time
	  }

	  if (this->variable == nullptr) {// variable symbol not found in symbol table
		  //try to find it again		  
		  //check scope:
		  if (this->variableScope == nullptr) { 
			  this->nodeType = new TypeError("Variable " + variableName +" is undefined, " + string(" line:") + to_string(this->line) + string(",col:") + to_string(this->col));
			  return false;
		  }		  
		  		  
		  extern SymbolsParser * symbolsParser;
		  this->variable = dynamic_cast<Variable*>(symbolsParser->lookUpSymbol(this->variableScope, &this->variableName[0]));
		  if (this->variable == nullptr) {// it's still not found
			  this->nodeType = new TypeError("Variable " + variableName + " is undefined, " + string(" line:") + to_string(this->line) + string(",col:") + to_string(this->col));
			  return false;
		  }
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
		  this->nodeType = new TypeError("Undefined Variable Type, " + string(" line:") + to_string(this->line) + string(",col:") + to_string(this->col));
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

Node* VariableNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void VariableNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}

void VariableNode::accept(CheckerVisitor* visitor, TypeExpression* context) {
	visitor->visit(this, context);
}