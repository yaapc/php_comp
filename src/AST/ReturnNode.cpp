#pragma once
#include "ReturnNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "AST_Visitors\CheckerVisitor.hpp"

ReturnNode::ReturnNode(Node *exp, int line, int col) {
	this->returned_node = exp;
	this->nodeType = nullptr;
	this->line = line;
	this->col = col;
}

void ReturnNode::print(ostream &os) {
    auto self = int(this);
    os << self
       << "[label=\"return\"]"
       <<endl;
    if (returned_node) {
		returned_node->print(os);
      os << self << "->" << int(returned_node) << endl;
    }
  }

 void ReturnNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* ReturnNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

bool ReturnNode::type_checking() {
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		this->returned_node->type_checking();
		return true; // pass it this time
	}

	
	if (this->returned_node == nullptr) { // void return
		this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
		return true;
	}
	
	// not nullptr, re check it for second passes
	this->returned_node->type_checking();

	//TODO: check if return TypeExpr is the same as the Return Type of the function defined
	if (returned_node){
		this->nodeType = this->returned_node->getNodeType();
	}
	return true;
}


void ReturnNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}

void ReturnNode::accept(CheckerVisitor* visitor, TypeExpression* context) {
	visitor->visit(this, context);
}