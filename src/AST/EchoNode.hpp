#pragma once

#include "Node.hpp"
#include <iostream>

using namespace std;

class EchoNode : public Node {
public:
  char* text;
  Node* node;
  EchoNode(char* str) : text(str) {}
  EchoNode(Node* node) : node(node), text(nullptr) {}
  virtual bool type_checking() {
    return true;
  }

  virtual void print(ostream& os) {
    os << int(this) << "[label = \"Echo";
    if (text) os << text;
    os << "\"]" <<endl;
  }
};
