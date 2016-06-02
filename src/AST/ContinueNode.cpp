#pragma once 
#include "ContinueNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"

ContinueNode::ContinueNode(int line, int col){
	this->line = line;
	this->col = col;
}

 void ContinueNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"Continue\"]"
       << endl;
  }

void ContinueNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* ContinueNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void ContinueNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}