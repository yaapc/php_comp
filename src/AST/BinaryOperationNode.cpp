#pragma once
#include "BinaryOperationNode.hpp"
#include "../TypeSystem/TypesTable.h"

BinaryOperationNode::BinaryOperationNode(char* op, Node *lft, Node *rgt) : left(lft), right(rgt), op_type(op) {
	  nodeType = nullptr;
}

void BinaryOperationNode::print(ostream &os) {
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

 void BinaryOperationNode::generate_code(){
	AsmGenerator::comment("<Binary Operation");


	string t0 = "t0";
	string t1 = "t1";
	string t2 = "t2";

	AsmGenerator::comment("<Binary Operation Left node");
	left->generate_code();
	AsmGenerator::comment("Binary Operation Left node/>");

	AsmGenerator::comment("<Binary Operation right node");
	right->generate_code();
	AsmGenerator::comment("Binary Operation right node/>");


	AsmGenerator::comment("<Binary Operation Calculation");
	if (this->getNodeType()->getTypeId() == INTEGER_TYPE_ID){ //Integer

		AsmGenerator::pop(t1); //get the result of right and put it in reg t1
		AsmGenerator::pop(t0); //get the result of left and put it in reg t0

		if (*op_type == '+'){
			AsmGenerator::binary_operation(t0, t0, t1, 1); //perform add and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
			//AsmGenerator::print_reg(t0);
		}

		if (*op_type == '-'){
			AsmGenerator::binary_operation(t0, t0, t1, 2); //perform minus and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}

		if (*op_type == '*'){
			AsmGenerator::binary_operation(t0, t0, t1, 3); //perform mult and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}

		if (*op_type == '/'){
			AsmGenerator::binary_operation(t0, t0, t1, 4); //perform div and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}

		if (*op_type == '%'){
			AsmGenerator::binary_operation(t0, t0, t1, 5); //perform reminder and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}

		if (strcmp(op_type, "==") == 0){
			AsmGenerator::equal_operation(t2,t0,t1,false); //perform bne and put the result in t2 (true 1 or false 0)
			AsmGenerator::push(t2); //push t2 (the result) into stack
		}

		if (strcmp(op_type, "!=") == 0){
			AsmGenerator::equal_operation(t2,t0,t1,true); //perform bne and put the result in t2 (true 1 or false 0)
			AsmGenerator::push(t2);//push t2 (the result) into stack
		}

		if (strcmp(op_type, "<") == 0){
			AsmGenerator::less_than_operation(t2, t0, t1);  //perform slt and put the result in t2 (true 1 or false 0)
			AsmGenerator::push(t2);						//push t2 (the result) into stack
			AsmGenerator::print_reg(t2);
		}

		if (strcmp(op_type, ">") == 0){
			AsmGenerator::less_than_operation(t2, t1, t0); //perform slt and put the result in t2 (true 1 or false 0)
			AsmGenerator::push(t2);						//push t2 (the result) into stack
			AsmGenerator::print_reg(t2);
		}

		if (strcmp(op_type, ">=") == 0){
			AsmGenerator::greater_or_equal_operation(t2, t0, t1); //perform greater or equal and put the result in t2 (true 1 or false 0)
			AsmGenerator::push(t2);						//push t2 (the result) into stack
		}

		if (strcmp(op_type, "<=") == 0){
			AsmGenerator::greater_or_equal_operation(t2, t1, t0); //perform slt and put the result in t2
			AsmGenerator::push(t2);						//push t2 (the result) into stack
		}
	}

	if (this->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){ //Boolean

		AsmGenerator::pop(t1); //get the result of right and put it in reg t1
		AsmGenerator::pop(t0); //get the result of left and put it in reg t0

		if (op_type == "&&"){
			AsmGenerator::binary_operation(t0, t0, t1, 6); //perform and operation and put the result in t0
		}

		if (op_type == "||"){
			AsmGenerator::binary_operation(t0, t0, t1, 7); // perform in operation and put the result in t0
		}
		AsmGenerator::push(t0); //push t0 (the result) into stack
	}

	if (this->getNodeType()->getTypeId() == FLOAT_TYPE_ID){ //Float
		string f0 = "f0";
		string f1 = "f1";
		string t2 = "t2";
		AsmGenerator::f_pop(f1);
		AsmGenerator::f_pop(f0);

		if (*op_type == '+'){
			AsmGenerator::f_binary_operation(f0,f0,f1,1);
			AsmGenerator::f_push(f0);
		}

		if (*op_type == '-'){
			AsmGenerator::f_binary_operation(f0,f0,f1,2);
			AsmGenerator::f_push(f0);
		}

		if (*op_type == '*'){
			AsmGenerator::f_binary_operation(f0,f0,f1,3);
			AsmGenerator::f_push(f0);
		}

		if (*op_type == '/'){
			AsmGenerator::f_binary_operation(f0,f0,f1,4);
			AsmGenerator::f_push(f0);
		}

		if (strcmp(op_type, "==") == 0){
			AsmGenerator::f_equal_operation(t2,f0,f1,false);
			AsmGenerator::push(t2);
		}

		if (strcmp(op_type, "!=") == 0){
			AsmGenerator::f_equal_operation(t2,f0,f1,true);
			AsmGenerator::push(t2);
		}

		if (strcmp(op_type, "<") == 0){
			AsmGenerator::f_less_than_operation(t2, f0, f1);
			AsmGenerator::push(t2);
			AsmGenerator::print_reg(t2);
		}

		if (strcmp(op_type, ">") == 0){
			AsmGenerator::f_greater_than_operation(t2, f0, f1);
			AsmGenerator::push(t2);
			AsmGenerator::print_reg(t2);
		}

		if (strcmp(op_type, ">=") == 0){
			AsmGenerator::f_greater_or_equal_operation(t2, f0, f1);
			AsmGenerator::push(t2);
			AsmGenerator::print_reg(t2);
		}

		if (strcmp(op_type, "<=") == 0){
			AsmGenerator::f_less_or_equal_operation(t2, f0, f1);
			AsmGenerator::push(t2);
			AsmGenerator::print_reg(t2);
		}
	}
	AsmGenerator::comment("Binary Operation Calculation/>");
	AsmGenerator::comment("Binary Operation/>");
}

 TypeExpression* BinaryOperationNode::getType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
}

 bool BinaryOperationNode::type_checking() {
		if (strcmp(op_type, "==") == 0){
			this->nodeType = left->getNodeType()->opEqual(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "!=") == 0){
			this->nodeType = left->getNodeType()->opEqual(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "<") == 0){
			this->nodeType = left->getNodeType()->opLessThan(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, ">") == 0){
			this->nodeType = left->getNodeType()->opGreaterThan(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, ">=") == 0){
			this->nodeType = left->getNodeType()->opGreaterOrEq(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "<=") == 0){
			this->nodeType = left->getNodeType()->opLessOrEq(this->right->getNodeType()->getTypeId());
		}

		if (strcmp(op_type, "+") == 0) {
			this->nodeType = left->getNodeType()->opPlus(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "-") == 0) {
			this->nodeType = left->getNodeType()->opMinus(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "*") == 0) {
			this->nodeType = left->getNodeType()->opMult(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "/") == 0) {
			this->nodeType = left->getNodeType()->opDiv(this->right->getNodeType()->getTypeId());
			return true;
		}

		if (strcmp(op_type, "%") == 0) {
			this->nodeType = left->getNodeType()->opMod(this->right->getNodeType()->getTypeId());
			return true;
		}
				
		this->nodeType = TypesTable::getInstance()->getType(ERROR_TYPE_ID);
		return false;
  }