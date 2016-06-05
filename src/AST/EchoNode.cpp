#pragma once
#include "EchoNode.hpp"
#include "ScalarNode.hpp"
#include "VariableNode.hpp"
#include "../Code Generator/AsmGenerator.h"
#include "ListNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "AST_Visitors\CheckerVisitor.hpp"

EchoNode::EchoNode(Node* node, int line, int col) : expression(node) {
	this->line = line;
	this->col = col;
	nodeType = nullptr;

}

bool EchoNode::type_checking() {
	//this for second passes, if the current node is free of TypeError no need to re type_check it
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		return true; // pass it this time
	}

	TypeExpression* exprTE = this->expression->getNodeType();
	if (exprTE != nullptr && dynamic_cast<TypeError*>(exprTE) != nullptr)
		this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
	else
		this->nodeType = exprTE; // gets the same error type;

	return this->expression->type_checking();
}


 TypeExpression* EchoNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }

void EchoNode::print(ostream& os) {
        int self = int(this);
        os << self << "[label = \"Echo\"]" <<endl;
        expression->print(os);
        os << self << "->" << int(expression) << endl;
}

void EchoNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* EchoNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}


void EchoNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}

void EchoNode::accept(CheckerVisitor* visitor, TypeExpression* context) {
	visitor->visit(this, context);
}