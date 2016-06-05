#pragma once
#include "ScalarNode.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/AsmGenerator.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "AST_Visitors\CheckerVisitor.hpp"

ScalarNode::ScalarNode(int i, int line, int col) {
	this->nodeType = nullptr;
	value.type = 1;
	value.int_val = i;
	this->line = line;
	this->col = col;
  }

 ScalarNode::ScalarNode(float f, int line, int col){
	this->nodeType = nullptr;
	value.type = 2;
	value.float_val = f;
	this->line = line;
	this->col = col;
  }

ScalarNode::ScalarNode(bool b, int line, int col) {
	this->nodeType = nullptr;
	value.type = 3;
	value.bool_val = b;
	this->line = line;
	this->col = col;
  }

ScalarNode::ScalarNode(string s, int line, int col){
	this->nodeType = nullptr;
	value.type = 5;
	value.string_val = s;
	this->line = line;
	this->col = col;
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
				this->nodeType = new TypeError("Couldn't recognize Scalar Type, " + string(" line:") + to_string(this->line) + string(",col:") + to_string(this->col));
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


Node* ScalarNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void ScalarNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}


void ScalarNode::accept(CheckerVisitor* visitor, TypeExpression* context) {
	visitor->visit(this, context);
}