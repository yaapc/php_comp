#pragma once

#include "IfNode.hpp"
#include "../Code Generator/AsmGenerator.h"

IfNode::IfNode(Node *cond, Node *bod, Node *el) : condition(cond), body(bod), else_node(dynamic_cast<ElseNode*>(el)) {}

   void IfNode::print(ostream &os) {
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


  
  void IfNode::generate_code(){
	AsmGenerator::comment("<If Statment");


	string t0 = "t0";
	string else_label	= "else_label_" + to_string(AsmGenerator::if_temp_label_count);
	string finish		= "finish_if_"  + to_string(AsmGenerator::if_temp_label_count);
	
	AsmGenerator::comment("<If Statment Condition Node");
	condition->generate_code();
	AsmGenerator::comment("<If Statment Condition Node/>");

	AsmGenerator::pop(t0);
	AsmGenerator::beq(t0,"0",else_label); // if t0 (condition) equal 0 ==> control go to else node

	if (body != NULL)					// else t0 (condition) equal 1 ==> control got to body node
	{
		AsmGenerator::comment("<If Statment Body Node");
		body->generate_code();
		AsmGenerator::comment("If Statment Body Node/>");
	}
	AsmGenerator::jump_label(finish);	 // body completed got to finish label
	AsmGenerator::add_label(else_label);
	if (else_node != NULL){
		AsmGenerator::comment("<If Statment Else Node");
		else_node->generate_code();
		AsmGenerator::comment("If Statment Else Node/>");
	}
	AsmGenerator::add_label(finish);
	AsmGenerator::if_temp_label_count++;
	AsmGenerator::comment("If Statment.");
  }


