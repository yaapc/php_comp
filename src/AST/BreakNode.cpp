#pragma once 
#include "BreakNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"

BreakNode::BreakNode(int line, int col){
	this->line = line;
	this->col = col;
}

void BreakNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"Break\"]"
       << endl;
  }

void BreakNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* BreakNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void BreakNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}