#pragma once 
#include "ElseNode.hpp"
#include "../Code Generator/AsmGenerator.h"

ElseNode::ElseNode(Node *node){
	this->body = node;
}

 void ElseNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"else\"]"
       << endl;
    body->print(os);
    os << self << "->" << int(body) << endl;
  }

 void ElseNode::generate_code(){
	AsmGenerator::comment("<Else Node");
	body->generate_code();
	AsmGenerator::comment("Else Node/>");
  }