#pragma once
#include "ScalarNode.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/AsmGenerator.h"

ScalarNode::ScalarNode(int i) {
	value.type = 1;
	value.int_val = i;
  }

 ScalarNode::ScalarNode(float f) {
	value.type = 2;
	value.float_val = f;
  }

ScalarNode::ScalarNode(bool b) {
	value.type = 3;
	value.bool_val = b;
  }

ScalarNode::ScalarNode(char c) {
	value.type = 4;
	value.char_val = c;
  }

ScalarNode::ScalarNode(string s) {
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
	switch (value.type)
		{
		case 1:
			AsmGenerator::li("t0",value.int_val);
			AsmGenerator::push("t0");
			break;
		case 2:
			AsmGenerator::f_li("f0",value.float_val);
			AsmGenerator::f_push("f0");
			break;
		case 3:
			AsmGenerator::li("t0",value.bool_val);
			AsmGenerator::push("t0");
			break;
		case 4:
			AsmGenerator::li("t0",value.char_val);
			AsmGenerator::push("t0");
			break;
		case 5:
			AsmGenerator::la("t0",AsmGenerator::store_string(value.string_val));
			AsmGenerator::push("t0");
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