#pragma once

#include "Node.hpp"
#include <iostream>

using namespace std;

class EchoNode : public Node {
public:
  char* text;
  Node* experesions;
  EchoNode(char* str) : text(str), experesions(nullptr){}
  EchoNode(Node* node) : experesions(node), text(nullptr) {}
  virtual bool type_checking() {
    return true;
  }

  virtual void print(ostream& os) {
    os << int(this) << "[label = \"Echo";
    if (text) os << text;
    os << "\"]" <<endl;
  }

  virtual void generate_code(){
	astLog.log("generate_code EchoNode");

	AsmGenerator::comment("Echo Node:");
	if (text){
		AsmGenerator::comment("Text Node:");

	}

	if (experesions){
		string t0 = "t0";
		AsmGenerator::comment("Experesions Node:");
		experesions->generate_code();
		AsmGenerator::pop(t0);
		AsmGenerator::print_reg(t0);
	}

  }
};
