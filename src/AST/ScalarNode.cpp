#pragma once
#include "ScalarNode.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/AsmGenerator.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"

ScalarNode::ScalarNode(int i) {
	this->nodeType = nullptr;
	value.type = 1;
	value.int_val = i;
  }

 ScalarNode::ScalarNode(float f){
	this->nodeType = nullptr;
	value.type = 2;
	value.float_val = f;
  }

ScalarNode::ScalarNode(bool b) {
	this->nodeType = nullptr;
	value.type = 3;
	value.bool_val = b;
  }

ScalarNode::ScalarNode(string s){
	this->nodeType = nullptr;
	value.type = 5;
	value.string_val = s;
  }


void ScalarNode::print(ostream &os) {
    os << int(this)
       << "[label=\"";
	value.print(os);
       os<<"\"]"
       << endl;
  }

bool ScalarNode::type_checking() {
	  switch (value.type){
			case 1:
				this->nodeType = TypesTable::getInstance()->getType(INTEGER_TYPE_ID);
				break;
			case 2:
				this->nodeType = TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
				break;
			case 3:
				this->nodeType = TypesTable::getInstance()->getType(BOOLEAN_TYPE_ID);
				break;
			case 5:
				this->nodeType = TypesTable::getInstance()->getType(STRING_TYPE_ID);
				break;
			default:
				this->nodeType = TypesTable::getInstance()->getType(ERROR_TYPE_ID);
				break;
		}	  
    return false;
  }

TypeExpression* ScalarNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
      
}

void ScalarNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}