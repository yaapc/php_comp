#pragma once 
#include "ContinueNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"

ContinueNode::ContinueNode(){

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