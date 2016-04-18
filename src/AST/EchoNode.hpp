#pragma once

#include "Node.hpp"
#include <iostream>

using namespace std;

class EchoNode : public Node {
public:
  Node* expression;
  EchoNode(Node* node) : expression(node) {}
  virtual bool type_checking() {
    return true;
  }

  virtual void print(ostream& os) {
	int self = int(this);
    os << self << "[label = \"Echo\"]" <<endl;
	expression->print(os);
	os << self << "->" << int(expression) << endl;
  }

  virtual void generate_code(){
	astLog.log("generate_code EchoNode");

	AsmGenerator::comment("<Echo Node");

	if (expression){
		string t0 = "t0";
		AsmGenerator::comment("<Experesions Node");
		expression->generate_code();
		AsmGenerator::pop(t0);
		AsmGenerator::print_reg(t0);
		AsmGenerator::comment("Experesions Node/>");
	}

	AsmGenerator::comment("Echo Node/>");

  }
};
