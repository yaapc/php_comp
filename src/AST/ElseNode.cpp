#pragma once 
#include "ElseNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "../TypeSystem/TypesTable.h"

ElseNode::ElseNode(Node *node){
	this->body = node;
	this->body->hasScopeFrame = true;
}

 void ElseNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"else\"]"
       << endl;
    body->print(os);
    os << self << "->" << int(body) << endl;
  }

void ElseNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* ElseNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

bool ElseNode::type_checking() {
	this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
	this->body->type_checking();
	return true;
}
