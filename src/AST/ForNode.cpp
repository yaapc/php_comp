#pragma once 
#include "ForNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "AST_Visitors\CheckerVisitor.hpp"
#include "../TypeSystem/TypesTable.h"


 ForNode::ForNode(Node *initializer, Node *condition, Node *post_statement, Node *body, int line, int col) :
    initializer(initializer),
    condition(condition),
    post_statement(post_statement),
    body(body) {
	 this->line = line;
	 this->col = col; 
 }

 void ForNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"for\"]"
       << endl;
    if(initializer) initializer->print(os);
    if(condition)condition->print(os);
    if(post_statement)post_statement->print(os);
    if(body)body->print(os);
    os << self << "->" << int(initializer) << endl;
    os << self << "->" << int(condition) << endl;
    os << self << "->" << int(post_statement) << endl;
    os << self << "->" << int(body) << endl;
  }

 bool ForNode::type_checking() {
	 if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		 //this for second passes, if the current node is free of TypeError no need to re type_check it
		 this->condition->type_checking();
		 this->initializer->type_checking();
		 this->post_statement->type_checking();
		 return true; // pass it this time
	 }
	 this->condition->type_checking();
	 this->initializer->type_checking();
	 this->post_statement->type_checking();
	 this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
 }


void ForNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* ForNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void ForNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}

void ForNode::accept(CheckerVisitor* visitor, TypeExpression* context) {
	visitor->visit(this, context);
}