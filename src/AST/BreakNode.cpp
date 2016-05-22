#pragma once 
#include "BreakNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"

BreakNode::BreakNode(){

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