#pragma once

#include "Node.hpp"
#include <iostream>
#include "../Code Generator/AsmGenerator.h"

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
  void generate_code(){
	astLog.log("generate_code ScalarNode ("+to_string(int_value)+")");
	AsmGenerator::li("t9",int_value);
	AsmGenerator::push("t9");
  }
};
