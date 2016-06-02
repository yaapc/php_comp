#pragma once 
#include "ForNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"


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