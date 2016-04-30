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
	  if (this->variable->getVariableType() == "int") {
		  this->nodeType = TypesTable::getInstance()->getType(INTEGER_TYPE_ID);
	  }
	  else {
		  this->nodeType = TypesTable::getInstance()->getType(ERROR_TYPE_ID);
	  }
	  return true;
  }


void VariableNode::generate_code(){
	AsmGenerator::comment("<Variable Node");
	
	string s0 = "s0";
	string memory_addrees = AsmGenerator::global_label+to_string(variable->getId());
	AsmGenerator::la(s0,memory_addrees);
	AsmGenerator::push(s0);

	AsmGenerator::comment("Variable Node/>");
    
}