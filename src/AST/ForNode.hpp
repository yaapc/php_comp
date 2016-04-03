#pragma once

#include "Node.hpp"
#include <iostream>

class ForNode : public Node {
public:
  Node *initializer, *condition, *post_statement, *body;
  ForNode(Node *initializer, Node *condition, Node *post_statement, Node *body) :
    initializer(initializer),
    condition(condition),
    post_statement(post_statement),
    body(body) {}

  virtual void print(ostream &os) {
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
};
