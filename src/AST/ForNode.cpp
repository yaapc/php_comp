#pragma once 
#include "ForNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"


 ForNode::ForNode(Node *initializer, Node *condition, Node *post_statement, Node *body) :
    initializer(initializer),
    condition(condition),
    post_statement(post_statement),
    body(body) {}

 void ForNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"for\"]"
       << endl;
    initializer->print(os);
    condition->print(os);
    post_statement->print(os);
    body->print(os);
    os << self << "->" << int(initializer) << endl;
    os << self << "->" << int(condition) << endl;
    os << self << "->" << int(post_statement) << endl;
    os << self << "->" << int(body) << endl;
  }

void ForNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}