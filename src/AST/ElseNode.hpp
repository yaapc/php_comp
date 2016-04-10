#pragma once

#include "Node.hpp"
#include <iostream>

class ElseNode : public Node {
public:
  Node *body;
  ElseNode(Node *body) : body(body) {}

  virtual void print(ostream &os) override {
    int self = int(this);
    os << self
       << "[label=\"else\"]"
       << endl;
    body->print(os);
    os << self << "->" << int(body) << endl;
  }
  virtual void generate_code(){
	astLog.log("generate_code ElseNode");
  }
};
