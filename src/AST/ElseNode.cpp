#pragma once 
#include "ElseNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "../TypeSystem/TypesTable.h"
#include "AST_Visitors\CheckerVisitor.hpp"

ElseNode::ElseNode(Node *node, int line, int col){
	this->body = node;
	this->body->hasScopeFrame = true;
	this->line = line;
	this->col = col;
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


void ElseNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}

void ElseNode::accept(CheckerVisitor* visitor, TypeExpression* context) {
	visitor->visit(this, context);
}