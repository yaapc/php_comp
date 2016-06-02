#pragma once
#include "WhileNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"

WhileNode::WhileNode(Node *condition, Node *body, int line, int col) : condition(condition) {
	this->body = body;
	this->body->hasScopeFrame = true;
	this->line = line;
	this->col = col;
}

 void WhileNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"while\"]"
       << endl;
	if(condition) condition->print(os);
    if(body) body->print(os);
    os << self << "->" << int(condition) << endl;
    os << self << "->" << int(body) << endl;
  }

 void WhileNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* WhileNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void WhileNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}