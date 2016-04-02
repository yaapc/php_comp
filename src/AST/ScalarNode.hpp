#pragma once

#include "Node.hpp"
#include <iostream>

class ScalarNode : public Node {
public:
  int int_value;
  ScalarNode(int i) : int_value(i) {}

  virtual void print(ostream &os) {
    os << int(this)
       << "[label=\""
       << int_value
       <<"\"]"
       << endl;
  }
};
