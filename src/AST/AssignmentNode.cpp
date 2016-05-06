#pragma once

#include "AssignmentNode.hpp"
#include "VariableNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/AsmGenerator.h"


AssignmentNode::AssignmentNode(Node *l, Node *r) : lhs(l), rhs(r) {
	nodeType = nullptr;
}

void AssignmentNode::print(ostream &os) {
    int self = int(this);
    os << self
       << "[label=\"=\"]"
       << endl;
    lhs->print(os);
    if (rhs) rhs->print(os);
    os << self << "->" << int(lhs) << endl;
    os << self << "->" << int(rhs) << endl;
  }
  
  
void AssignmentNode::generate_code()
{
	AsmGenerator::comment("<Assignment Node");

	AsmGenerator::comment("<Binary Operation Left node");
	lhs->generate_code();
	AsmGenerator::comment("Binary Operation Left node/>");

	AsmGenerator::comment("<Binary Operation right node");
	rhs->generate_code();
	AsmGenerator::comment("Binary Operation right node/>");


	if (this->getNodeType()->getTypeId() == INTEGER_TYPE_ID || this->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){
		// int numeric value are stored in data segment and variables values also stored in data segment so life is easy :-)

		string s0 = "s0";
		string s1 = "s1";			 
		AsmGenerator::pop(s1);
		AsmGenerator::pop(s0);
		//s0 should contain the value of int variale we don't want value here we need address to write on it
		//s1 should contain the int vlaue

		if (VariableNode *variable = dynamic_cast<VariableNode*>(lhs)){

			//Get memory address of variable because variable node with primitive type (int,bool,float) push its value on stack
			//but when primitve variable appeare in lhs we need its address to write on it so we have to get the address 

			string variable_mem_address = AsmGenerator::global_int+to_string(variable->variable->getId());
			AsmGenerator::la(s0,variable_mem_address); // Store address in s0 (overwrite the value-reusing the s0 reg)
			AsmGenerator::sw(s1,0,s0);	   //  Store value from rhs in the addresss
		}else{
			throw std::invalid_argument("left hand side should be Variable node");
		}
	}

	if (this->getNodeType()->getTypeId() == STRING_TYPE_ID){
		// String literals are stored in data and in variableNode we pushed the address of this literals

		string s0 = "s0";
		string s1 = "s1";			 
		AsmGenerator::pop(s1);
		AsmGenerator::pop(s0);
		//s0 should contain the address of string variale
		//s1 should contain the address of string literals

		//To store rhs string literal in lhs string variable we should do:
			//1. Get the size of string literal.
			//2. Allocate new memory (in heap) with previous size.
			//3. Change the address of lhs (string variable) to the address of newly created memory.
			//4. Copy the content of rhs (string literal) to the newly created memory.


		string t0 = "t0";
		AsmGenerator::move("a0",s1); // copy address of string literal into a0
		AsmGenerator::jal(AsmGenerator::strlen_functoion_name); // calculate length of string the result will be in $v1

		AsmGenerator::add_instruction("addi $t0,$v1,1"); 
		AsmGenerator::sbrk(t0,t0);						// allcoate memory and store the address of newley crated memory in t0

		AsmGenerator::sw(t0,0,s0);						// store the addrees in variable address

		AsmGenerator::move("a0",s0); // put the address of newly created memoty in a0 (the parameter of strcpy function)
		AsmGenerator::move("a1",s1); // put the address of string literal in a1 (the second parameter of strcpy function)
		AsmGenerator::jal(AsmGenerator::strcpy_function_name);
		

	}

	if (true){//(this->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
		// float numeric values are stored in data like strings but the value of float variable not in heap
		// because it have fixed length not like strings

		//f0 should contain the value of float variale
		//f1 should contain the address of float value in data segment

		string f0 = "f0";
		string f1 = "f1";
	

		AsmGenerator::f_pop(f1);
		AsmGenerator::f_pop(f0);
		if (VariableNode *variable = dynamic_cast<VariableNode*>(lhs)){
			string variable_mem_address = AsmGenerator::global_float+to_string(variable->variable->getId());

			
			AsmGenerator::ss(f1,variable_mem_address);
		}else{
			throw std::invalid_argument("left hand side should be Variable node");
		}
	}


	AsmGenerator::comment("Assignment Node/>");
}

  TypeExpression* AssignmentNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }


  bool AssignmentNode::type_checking() {
	  if (dynamic_cast<VariableNode*>(lhs)->variable->isConst) {
		  this->nodeType = new TypeError("lvalue is not modifiable.");
		  return false;
	  }
	  else {
		  //TODO: should be implemented with coercion, not equivelantTo !!
		  if (lhs->getNodeType()->equivelantTo(rhs->getNodeType()->getTypeId())) {
			  this->nodeType = lhs->getNodeType();
			  return true;
		  }
		  else {
			  this->nodeType = new TypeError("no suitable conversion exists between " + 
				  TypeSystemHelper::getTypeName(lhs->getNodeType()->getTypeId()) + " and " + 
					  TypeSystemHelper::getTypeName(rhs->getNodeType()->getTypeId()));
			  return false;
		  }
	  }
  }