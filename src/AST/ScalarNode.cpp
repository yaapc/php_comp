#pragma once
#include "ScalarNode.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/AsmGenerator.h"

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

void ScalarNode::generate_code(){
	AsmGenerator::comment("<Scaler Node");
	string s0 = "s0";
	string f0 = "f0";
	switch (value.type)
		{
		case 1:
			AsmGenerator::li(s0,value.int_val);
			AsmGenerator::push(s0);
			break;
		case 2:
			AsmGenerator::f_li(f0,value.float_val);
			AsmGenerator::f_push(f0);
			break;
		case 3:
			AsmGenerator::li(s0,value.bool_val);
			AsmGenerator::push(s0);
			break;
		case 5:
			AsmGenerator::la(s0,AsmGenerator::store_string_literal(value.string_val));
			AsmGenerator::push(s0);
			break;
		}
	AsmGenerator::comment("Scaler Node/>");
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
			case 4:
				this->nodeType = TypesTable::getInstance()->getType(CHARACTER_TYPE_ID);
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