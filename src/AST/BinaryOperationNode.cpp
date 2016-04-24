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

	if (getNodeType()->getTypeId() == STRING_TYPE_ID){

		if (right->getNodeType()->getTypeId() == STRING_TYPE_ID && 
			left-> getNodeType()->getTypeId() == STRING_TYPE_ID)
		{
			string s0 = "s0";
			string s1 = "s1";
			string s2 = "s2";
			string s3 = "s3";

			string t0 = "t0";
			string t1 = "t1";
			string t2 = "t2";

			AsmGenerator::pop(s1);
			AsmGenerator::pop(s0);


			//Calculate lenght of fisrt String
			AsmGenerator::move("a0",s0); 
			AsmGenerator::jal(AsmGenerator::strlen_functoion_name);
			AsmGenerator::move(s2,"v1"); //store returned length in t0
		
	
		

			//Calculate lenght of second String
			AsmGenerator::move("a0",s1); 
			AsmGenerator::jal(AsmGenerator::strlen_functoion_name);
			AsmGenerator::move(s3,"v1"); //store returned length in t1

		
			AsmGenerator::binary_operation(t2,s2,s3,1);
			AsmGenerator::add_instruction("addi $t2,$t2,1"); // add another extra byte for null terminator
		
			AsmGenerator::sbrk(t2,s2);	 // allocate memory for new string s2 contian the address of allocated memeory

			AsmGenerator::move	("a0",s0);
			AsmGenerator::move	("a1",s1);
			AsmGenerator::move	("a2",s2);
			AsmGenerator::jal(AsmGenerator::strcpy_functoion_name);
			AsmGenerator::push(s2);
		}

		if (right->getNodeType()->getTypeId() == INTEGER_TYPE_ID)
		{
			string s0 = "s0";
			string s1 = "s1";
			string t0 = "t0";
			string t1 = "t1";
			string t3 = "t3";

			string begin_loop_label		= "concat_begin" + temp_label_count;
			string finish_loop_leble	= "concat_end" + temp_label_count;
			temp_label_count++;

	
			AsmGenerator::pop(s1); //integer type
			AsmGenerator::pop(s0);// string type
			AsmGenerator::move("a0",s0); // copy address of string into a0
			AsmGenerator::jal(AsmGenerator::strlen_functoion_name); // calculate length of string - result in $v1
			AsmGenerator::add_instruction("addi $t0,$v1,5"); // calculate new string length N = stringlenth + 4 (integer) + 1 (null terminator) 
			AsmGenerator::sbrk(t0,t1);						// allcoate memory
			AsmGenerator::move(t1,"v0");					// save memory address in t1
			AsmGenerator::move(t3,"v0");					//save memory address in t3

			// loop over string and store it in newly created memory 
			AsmGenerator::add_label(begin_loop_label);
			AsmGenerator::add_instruction("lb $t0 0($s0)");
			AsmGenerator::add_instruction("beq  $t0 $0 "+finish_loop_leble);
			AsmGenerator::add_instruction("sb   $t0 0($t1)");
			AsmGenerator::add_instruction("addi $s0 $s0 1");
			AsmGenerator::add_instruction("addi $t1 $t1 1");
			AsmGenerator::add_instruction("b "+begin_loop_label);
			AsmGenerator::add_label(finish_loop_leble);

			
			AsmGenerator::move("a0",s1);	//copy integer value into a0
			AsmGenerator::move("a1",t1);	//copy memory address (in the place where we left off loop) 
			AsmGenerator::jal(AsmGenerator::int_to_asci_functoion_name); // use predefine function to store integer as asci in memory
			AsmGenerator::add_instruction("sb $0 4($t1)");	// null terminator into string 
			AsmGenerator::push(t3); //push address of newly created string into stack
		}		


		//Todo Ask Bassel
		if (left->getNodeType()->getTypeId() == INTEGER_TYPE_ID)
		{
			string s0 = "s0";
			string s1 = "s1";
			string t0 = "t0";
			string t1 = "t1";
			string t3 = "t3";
			string begin_loop_label		= "concat_begin" + temp_label_count;
			string finish_loop_leble	= "concat_end" + temp_label_count;
			temp_label_count++;
			AsmGenerator::pop(s0);
			AsmGenerator::pop(s1);
			AsmGenerator::move("a0",s0);
			AsmGenerator::jal(AsmGenerator::strlen_functoion_name);
			AsmGenerator::add_instruction("addi $t0,$v1,5");
			AsmGenerator::sbrk(t0,t1);
			AsmGenerator::move(t1,"v0");
			AsmGenerator::move(t3,"v0");
			AsmGenerator::move("a0",s1);
			AsmGenerator::move("a1","v0");
			AsmGenerator::jal(AsmGenerator::int_to_asci_functoion_name);
			AsmGenerator::add_instruction("addi $t1,$t1,4");
			AsmGenerator::add_label(begin_loop_label);
			AsmGenerator::add_instruction("lb $t0 0($s0)");
			AsmGenerator::add_instruction("beq  $t0 $0 "+finish_loop_leble);
			AsmGenerator::add_instruction("sb   $t0 0($t1)");
			AsmGenerator::add_instruction("addi $s0 $s0 1");
			AsmGenerator::add_instruction("addi $t1 $t1 1");
			AsmGenerator::add_instruction("b "+begin_loop_label);
			AsmGenerator::add_label(finish_loop_leble);
			AsmGenerator::add_instruction("sb $0 0($t1)");
			AsmGenerator::push(t3);
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

 int BinaryOperationNode::temp_label_count = 0;