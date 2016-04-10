#pragma once

#include "Node.hpp"
#include <iostream>
#include "../Code Generator/AsmGenerator.h"

class BinaryOperationNode : public Node {
public:
  Node *left, *right;
  char* op_type;
  BinaryOperationNode(char* op, Node *lft, Node *rgt) : left(lft), right(rgt), op_type(op) {}

  virtual void print(ostream &os) {
	int self = int(this);
	os << self
		<< "[label=\""
		<< op_type
		<<"\"]"
		<<endl;
	if (left) left->print(os);
	if (right) right->print(os);
	os << self << "->" << int(left)<<endl;
	os << self << "->" << int(right)<<endl;
  }

  virtual void generate_code(){
	astLog.log("generate_code BinaryOperationNode ("+string(op_type)+")");
	string t0 = "t0";
	string t1 = "t1";

	//TODO get the type of right and left node 
	int type = 1;

	left->generate_code();
	right->generate_code();

	AsmGenerator::pop(t1); //get the result of right and put it in reg t1
	AsmGenerator::pop(t0); //get the result of left and put it in reg t0



	if (type == 1){ //Integer 
		if (*op_type == '+'){
			AsmGenerator::binary_operation(t0, t0, t1, 1); //perform add and put the result if t0
		}

		if (*op_type == '-'){
			AsmGenerator::binary_operation(t0, t0, t1, 2); //perform add minus put the result if t0
		}

		if (*op_type == '*'){
			AsmGenerator::binary_operation(t0, t0, t1, 3); //perform add mult put the result if t0
		}

		if (*op_type == '/'){
			AsmGenerator::binary_operation(t0, t0, t1, 4); //perform add div put the result if t0
		}

		if (*op_type == '%'){
			AsmGenerator::binary_operation(t0, t0, t1, 5); //perform add reminder put the result if t0
		}
		AsmGenerator::push(t0); //push t0 (the result) into stack

		AsmGenerator::print_reg(t0);
	}

	if (type == 2){ //Boolean
	
	}

	if (type ==3){ //Float
	
	}

  }
};
