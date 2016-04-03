#pragma once

#include "Node.hpp"
#include "ElseNode.hpp"
#include <iostream>

using namespace std;

class IfNode : public Node {
public:
  Node *condition, *body;
  ElseNode *else_node;

  IfNode(Node *cond, Node *bod, Node *el) : condition(cond), body(bod), else_node(dynamic_cast<ElseNode*>(el)) {}

  virtual void print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"If\"]"
       << endl;
    condition->print(os);
    body->print(os);
    if (else_node) else_node->print(os);
    os << self << "->" << int(condition) << endl;
    os << self << "->" << int(body) << endl;
    if (else_node) os << self << "->" << int(else_node) << endl;
  }
};

IfNode* find_deepest_if(Node *root) {
  IfNode *deepest_if = dynamic_cast<IfNode*>(root);
  while (deepest_if->else_node) {
    deepest_if = dynamic_cast<IfNode*>(deepest_if->else_node->body);
  }
  return deepest_if;
}
