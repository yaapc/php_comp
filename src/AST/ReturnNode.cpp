#pragma once
#include "ReturnNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"


ReturnNode::ReturnNode(Node *exp) : returnend_expression(exp) {}

void ReturnNode::print(ostream &os) {
    auto self = int(this);
    os << self
       << "[label=\"return\"]"
       <<endl;
    if (returnend_expression) {
      returnend_expression->print(os);
      os << self << "->" << int(returnend_expression) << endl;
    }
  }

 void ReturnNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}
