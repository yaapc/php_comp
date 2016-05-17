#pragma once 
#include "CodeGeneratorVistor.hpp"
#include "AsmGenerator.h"
#include "../AST/ListNode.hpp"
#include "../AST/all.hpp"
#include "FunctionFrame.hpp"
#include "../TypeSystem/TypeClass.hpp"


void CodeGneratorVistor::generate(ListNode *ast)
{
	currentFrame	= new GlobalFrame();
	symbolIDS		= 0;
	AsmGenerator::initialize_file();
	ast->generate_code(this);
	AsmGenerator::write_file();
}

void CodeGneratorVistor::visit(AssignmentNode *assignmentNode)
{
	AsmGenerator::comment("<Assignment Node");

	//AsmGenerator::comment("<Binary Operation Left node");
	assignmentNode->lhs->generate_code(this);
	//AsmGenerator::comment("Binary Operation Left node/>");

	//AsmGenerator::comment("<Binary Operation right node");
	assignmentNode->rhs->generate_code(this);
	//AsmGenerator::comment("Binary Operation right node/>");


	if (assignmentNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID || assignmentNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){
		// int numeric value are stored in data segment and variables values also stored in data segment so life is easy :-)

		string s0 = "s0";
		string s1 = "s1";			 
		AsmGenerator::pop(s1);
		AsmGenerator::pop(s0);
		//s0 should contain the value of int variale we don't want value here we need address to write on it
		//s1 should contain the int vlaue

		if (VariableNode *variableNode = dynamic_cast<VariableNode*>(assignmentNode->lhs)){
			//Get memory address of variable because variable node with primitive type (int,bool,float) push its value on stack
			//but when primitve variable appeare in lhs we need its address to write on it so we have to get the address 
			string variableAddress = currentFrame->getAddress(variableNode->variable->getNameWithout());
			AsmGenerator::sw(s1,variableAddress);	   //  Store value from rhs in the addresss
		}else

		if (ClassCallNode *classCallNode = dynamic_cast<ClassCallNode*>(assignmentNode->lhs)){
			string probertyAddress = getClassMemberAddress(classCallNode,s0);
			AsmGenerator::sw(s1,probertyAddress);
		}else{
			cout << "left hand side should be Variable node or Member of object" << endl;
		}
	}

	if (assignmentNode->getNodeType()->getTypeId() == STRING_TYPE_ID){
		// String literals are stored in data and in variableNode we pushed the address of this literals
		string s0 = "s0";
		string s1 = "s1";			 
		AsmGenerator::pop(s1);
		AsmGenerator::pop(s0);
		//s0 should contain the address of string variable
		//s1 should contain the address of string literals

		//To store rhs string literal in lhs string variable we should do:
			//1. Get the size of string literal.
			//2. Allocate new memory (in heap) with previous size.
			//3. Change the address of lhs (string variable) to the address of newly created memory.
			//4. Copy the content of rhs (string literal) to the newly created memory.


		string t0 = "t0";
		AsmGenerator::move("a0",s1); // copy address of string literal into a0
		AsmGenerator::jal(AsmGenerator::strlen_functoion_name); // calculate length of string the result will be in $v1
		AsmGenerator::add_instruction("addi $t0,$v1,1"); // length++;
		AsmGenerator::sbrk(t0,t0);						// allcoate memory and store the address of newley crated memory in t0



		if (VariableNode *variableNode = dynamic_cast<VariableNode*>(assignmentNode->lhs))
		{
			string variableAddress = currentFrame->getAddress(variableNode->variable->getNameWithout());
			AsmGenerator::sw(t0,variableAddress);	   // store the addrees in variable address
		}else
		if (ClassCallNode *classCallNode = dynamic_cast<ClassCallNode*>(assignmentNode->lhs)){
			string probertyAddress = getClassMemberAddress(classCallNode,s0);
			AsmGenerator::sw(t0,probertyAddress);
		}
		else{
			cout << "left hand side should be Variable node or Member of object" << endl;
		}
		
		AsmGenerator::move("a0",t0); // put the address of newly created memoty in a0 (the parameter of strcpy function)
		AsmGenerator::move("a1",s1); // put the address of string literal in a1 (the second parameter of strcpy function)
		AsmGenerator::jal(AsmGenerator::strcpy_function_name);
	}

	if (assignmentNode->getNodeType()->getTypeId() == CLASS_TYPE_ID){
		string s0 = "s0";
		string s1 = "s1";			 
		AsmGenerator::pop(s1);
		AsmGenerator::pop(s0);
		//s0 should contain the address of object variale
		//s1 should contain the address of object in heap
	
		if (VariableNode *variableNode = dynamic_cast<VariableNode*>(assignmentNode->lhs)){
			string variableAddress = currentFrame->getAddress(variableNode->variable->getNameWithout());
			AsmGenerator::sw(s1,variableAddress);
		}else{
			cout << "left hand side should be Variable node" << endl;
		}
	}


	if (assignmentNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
		// float numeric values are stored in data like strings but the value of float variable not in heap
		// because it have fixed length not like strings

		//f0 should contain the value of float variale
		//f1 should contain the address of float value in data segment

		string f0 = "f0";
		string f1 = "f1";

		AsmGenerator::f_pop(f1);
		AsmGenerator::f_pop(f0);
		if (VariableNode *variableNode = dynamic_cast<VariableNode*>(assignmentNode->lhs)){
			string variableAddress = currentFrame->getAddress(variableNode->variable->getNameWithout());
			AsmGenerator::ss(f1,variableAddress);
		}else
		if (ClassCallNode *classCallNode = dynamic_cast<ClassCallNode*>(assignmentNode->lhs)){
			string s0 = "s0";
			string probertyAddress = getClassMemberAddress(classCallNode,s0);
			AsmGenerator::ss(f1,probertyAddress);
		}
		else{
			cout << "left hand side should be Variable node or Member of object" << endl;
		}
	}

	AsmGenerator::comment("Assignment Node/>");
}

void CodeGneratorVistor::visit(BinaryOperationNode *binaryOperationNode)
{
	AsmGenerator::comment("<Binary Operation ");

	string t0 = "t0";
	string t1 = "t1";
	string t2 = "t2";

	//AsmGenerator::comment("<Binary Operation Left node");
	binaryOperationNode->left->generate_code(this);
	//AsmGenerator::comment("Binary Operation Left node/>");

	//AsmGenerator::comment("<Binary Operation right node");
	binaryOperationNode->right->generate_code(this);
	//AsmGenerator::comment("Binary Operation right node/>");


	AsmGenerator::comment("<Binary Operation Calculation");
	if (binaryOperationNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID){ //Integer

		AsmGenerator::pop(t1); //get the result of right and put it in reg t1
		AsmGenerator::pop(t0); //get the result of left and put it in reg t0

		
		if (*(binaryOperationNode->op_type) == '+'){
			AsmGenerator::binary_operation(t0, t0, t1, 1); //perform add and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}

		if (*(binaryOperationNode->op_type) == '-'){
			AsmGenerator::binary_operation(t0, t0, t1, 2); //perform minus and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}

		if (*(binaryOperationNode->op_type) == '*'){
			AsmGenerator::binary_operation(t0, t0, t1, 3); //perform mult and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}

		if (*(binaryOperationNode->op_type) == '/'){
			AsmGenerator::binary_operation(t0, t0, t1, 4); //perform div and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}

		if (*(binaryOperationNode->op_type) == '%'){
			AsmGenerator::binary_operation(t0, t0, t1, 5); //perform reminder and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}

		
	}

	if (binaryOperationNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){ //Boolean

		AsmGenerator::pop(t1); //get the result of right and put it in reg t1
		AsmGenerator::pop(t0); //get the result of left and put it in reg t0

		if(strcmp(binaryOperationNode->op_type, "&&") == 0){
			AsmGenerator::binary_operation(t0, t0, t1, 6); //perform and operation and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}

		if (strcmp(binaryOperationNode->op_type, "||") == 0){
			AsmGenerator::binary_operation(t0, t0, t1, 7); // perform in operation and put the result in t0
			AsmGenerator::push(t0); //push t0 (the result) into stack
		}


	if (strcmp(binaryOperationNode->op_type, "==") == 0){
			AsmGenerator::equal_operation(t2,t0,t1,false); //perform bne and put the result in t2 (true 1 or false 0)
			AsmGenerator::push(t2); //push t2 (the result) into stack
		}

		if (strcmp(binaryOperationNode->op_type, "!=") == 0){
			AsmGenerator::equal_operation(t2,t0,t1,true); //perform bne and put the result in t2 (true 1 or false 0)
			AsmGenerator::push(t2);//push t2 (the result) into stack
		}

		if (strcmp(binaryOperationNode->op_type, "<") == 0){
			AsmGenerator::less_than_operation(t2, t0, t1);  //perform slt and put the result in t2 (true 1 or false 0)
			AsmGenerator::push(t2);						//push t2 (the result) into stack

		}

		if (strcmp(binaryOperationNode->op_type, ">") == 0){
			AsmGenerator::less_than_operation(t2, t1, t0); //perform slt and put the result in t2 (true 1 or false 0)
			AsmGenerator::push(t2);						//push t2 (the result) into stack
		}

		if (strcmp(binaryOperationNode->op_type, ">=") == 0){
			AsmGenerator::greater_or_equal_operation(t2, t0, t1); //perform greater or equal and put the result in t2 (true 1 or false 0)
			AsmGenerator::push(t2);						//push t2 (the result) into stack
		}

		if (strcmp(binaryOperationNode->op_type, "<=") == 0){
			AsmGenerator::greater_or_equal_operation(t2, t1, t0); //perform slt and put the result in t2
			AsmGenerator::push(t2);						//push t2 (the result) into stack
		}
	
	}

	if (binaryOperationNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){ //Float
		string f0 = "f0";
		string f1 = "f1";
		string t2 = "t2";
		AsmGenerator::f_pop(f1);
		AsmGenerator::f_pop(f0);

		if (*(binaryOperationNode->op_type) == '+'){
			AsmGenerator::f_binary_operation(f0,f0,f1,1);
			AsmGenerator::f_push(f0);
		}

		if (*(binaryOperationNode->op_type) == '-'){
			AsmGenerator::f_binary_operation(f0,f0,f1,2);
			AsmGenerator::f_push(f0);
		}

		if (*(binaryOperationNode->op_type) == '*'){
			AsmGenerator::f_binary_operation(f0,f0,f1,3);
			AsmGenerator::f_push(f0);
		}

		if (*(binaryOperationNode->op_type) == '/'){
			AsmGenerator::f_binary_operation(f0,f0,f1,4);
			AsmGenerator::f_push(f0);
		}

		if (strcmp(binaryOperationNode->op_type, "==") == 0){
			AsmGenerator::f_equal_operation(t2,f0,f1,false);
			AsmGenerator::push(t2);
		}

		if (strcmp(binaryOperationNode->op_type, "!=") == 0){
			AsmGenerator::f_equal_operation(t2,f0,f1,true);
			AsmGenerator::push(t2);
		}

		if (strcmp(binaryOperationNode->op_type, "<") == 0){
			AsmGenerator::f_less_than_operation(t2, f0, f1);
			AsmGenerator::push(t2);
		}

		if (strcmp(binaryOperationNode->op_type, ">") == 0){
			AsmGenerator::f_greater_than_operation(t2, f0, f1);
			AsmGenerator::push(t2);
		}

		if (strcmp(binaryOperationNode->op_type, ">=") == 0){
			AsmGenerator::f_greater_or_equal_operation(t2, f0, f1);
			AsmGenerator::push(t2);
		}

		if (strcmp(binaryOperationNode->op_type, "<=") == 0){
			AsmGenerator::f_less_or_equal_operation(t2, f0, f1);
			AsmGenerator::push(t2);
		}
	}

	if (binaryOperationNode->getNodeType()->getTypeId() == STRING_TYPE_ID){

		if (binaryOperationNode->right->getNodeType()->getTypeId() == STRING_TYPE_ID && 
			binaryOperationNode->left-> getNodeType()->getTypeId() == STRING_TYPE_ID)
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
			AsmGenerator::jal(AsmGenerator::strconcat_functoion_name);
			AsmGenerator::push(s2);
		}

		if (binaryOperationNode->right->getNodeType()->getTypeId() == INTEGER_TYPE_ID)
		{
			string s0 = "s0";
			string s1 = "s1";
			string t0 = "t0";
			string t1 = "t1";
			string t3 = "t3";

			string begin_loop_label		= "concat_begin" + binaryOperationNode->temp_label_count;
			string finish_loop_leble	= "concat_end"   + binaryOperationNode->temp_label_count;
			binaryOperationNode->temp_label_count++;

	
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
		if (binaryOperationNode->left->getNodeType()->getTypeId() == INTEGER_TYPE_ID)
		{
			string s0 = "s0";
			string s1 = "s1";
			string t0 = "t0";
			string t1 = "t1";
			string t3 = "t3";
			string begin_loop_label		= "concat_begin" + binaryOperationNode->temp_label_count;
			string finish_loop_leble	= "concat_end"	+binaryOperationNode-> temp_label_count;
			binaryOperationNode->temp_label_count++;
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

void CodeGneratorVistor::visit(DeclarationNode *declarationNode)
{
	
	AsmGenerator::comment("<Declaration Node");
	currentFrame->addLocal(declarationNode);
	AsmGenerator::comment("Declaration Node/>");
}

void CodeGneratorVistor::visit(VariableNode *variableNode)
{
	AsmGenerator::comment("<Variable Node");
	string s0 = "s0";

	string variableAddress = currentFrame->getAddress(variableNode->variable->getNameWithout());

	if (variableNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID	|| variableNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){
		// In primitive types we care about value so we have to load it
		AsmGenerator::lw(s0,variableAddress); 		//Get value from memory address and put the value in s0
		AsmGenerator::push(s0);
	}

	if (variableNode->getNodeType()->getTypeId()== STRING_TYPE_ID){
		AsmGenerator::lw(s0,variableAddress); 		//Get memory address and put in s0
		AsmGenerator::push(s0);

	}
		
	if (variableNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
		//float literals are stored in data so we only load the address of those literals
		AsmGenerator::ls("f0",variableAddress);	//Get value from memory address and put the value in s0	
		AsmGenerator::f_push("f0");
	}

	if (variableNode->getNodeType()->getTypeId()== CLASS_TYPE_ID){
		AsmGenerator::lw(s0,variableAddress); 		//Get memory address and put in s0
		AsmGenerator::push(s0);
	}

	AsmGenerator::comment("Variable Node/>");
}

void CodeGneratorVistor::visit(EchoNode *echoNode)
{
    AsmGenerator::comment("<Echo Node");
    if (echoNode->expression) {
		string t0 = "t0";
		if (ListNode* listNode = dynamic_cast<ListNode*>(echoNode->expression)) {
				AsmGenerator::comment("<List Node");
				for (auto &node : listNode->nodes) {
						if (node == nullptr) continue;
							
						node->generate_code(this);

						if (node->getNodeType()->getTypeId() == INTEGER_TYPE_ID) { //Integer
							AsmGenerator::pop(t0);
							AsmGenerator::print_reg(t0);
						}
						if (node->getNodeType()->getTypeId() == STRING_TYPE_ID) { //String
							AsmGenerator::pop(t0);
							AsmGenerator::print_string(t0);
						}

						if (node->getNodeType()->getTypeId() == FLOAT_TYPE_ID){ // float
							//AsmGenerator::pop("t0");
							//AsmGenerator::ls("f1",0,"t0");
							//AsmGenerator::f_print_reg("f1");

							AsmGenerator::f_pop("f0");
							AsmGenerator::f_print_reg("f0");
						}

						if (node->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){
							AsmGenerator::pop(t0);
							AsmGenerator::print_reg(t0);
							
						}
				}
				AsmGenerator::la(t0,AsmGenerator::new_line_address);
				AsmGenerator::print_string(t0);
				AsmGenerator::comment("List Node/>");
		}else{
				AsmGenerator::comment("<Experesion Node");
				echoNode->expression->generate_code(this);
				AsmGenerator::pop(t0);
				AsmGenerator::print_string(t0);
				AsmGenerator::comment("Experesion Node/>");
		}

    }
    AsmGenerator::comment("Echo Node/>");	
}

void CodeGneratorVistor::visit(ElseNode *assignmentNode)
{
	AsmGenerator::comment("<Else Node");
	assignmentNode->body->generate_code(this);
	AsmGenerator::comment("Else Node/>");
}

void CodeGneratorVistor::visit(ForNode *forNode)
{
	AsmGenerator::comment("<ForNode");
	string beginFor		= "for_begin_label_" + to_string(AsmGenerator::if_temp_label_count);
	string endFor		= "for_end_label"  + to_string(AsmGenerator::if_temp_label_count);

	string s0 = "s0";

	AsmGenerator::comment("<For Initializer");
	if (forNode->initializer){
		forNode->initializer->generate_code(this);
	}else{
		cout << "For without initializer" << endl;
	}
	AsmGenerator::comment("For Initializer/>");


	AsmGenerator::add_label(beginFor);
	AsmGenerator::comment("<For Condition");
	if (forNode->condition){
		forNode->condition->generate_code(this);
		AsmGenerator::pop(s0);
		AsmGenerator::add_instruction("beqz $"+s0+", "+endFor);
	}
	AsmGenerator::comment("For Condition/>");


	AsmGenerator::comment("<For Body");
	if (forNode->body){
		forNode->body->generate_code(this);
	}
	AsmGenerator::comment("For Body/>");


	AsmGenerator::comment("<For Increment");
	if (forNode->post_statement){
		forNode->post_statement->generate_code(this);
	}
	AsmGenerator::comment("For Increment/>");


	AsmGenerator::add_instruction("b " + beginFor);

	AsmGenerator::add_label(endFor);
	AsmGenerator::if_temp_label_count++;
	AsmGenerator::comment("ForNode/>");
}

void CodeGneratorVistor::visit(IfNode *ifNode)
{
	AsmGenerator::comment("<If Statment");
	string t0 = "t0";
	string else_label	= "else_label_" + to_string(AsmGenerator::if_temp_label_count);
	string endIf		= "end_if_"     + to_string(AsmGenerator::if_temp_label_count);
	
	AsmGenerator::comment("<If Statment Condition Node");
	ifNode->condition->generate_code(this);
	AsmGenerator::comment("<If Statment Condition Node/>");

	AsmGenerator::pop(t0);
	AsmGenerator::beq(t0,"0",else_label); // if t0 (condition) equal 0 ==> control go to else node

	if (ifNode->body != NULL)					// else t0 (condition) equal 1 ==> control got to body node
	{
		AsmGenerator::comment("<If Statment Body Node");
		ifNode->body->generate_code(this);
		AsmGenerator::comment("If Statment Body Node/>");
	}
	AsmGenerator::jump_label(endIf);	 // body completed got to finish label
	AsmGenerator::add_label(else_label);
	if (ifNode->else_node != NULL){
		AsmGenerator::comment("<If Statment Else Node");
		ifNode->else_node->generate_code(this);
		AsmGenerator::comment("If Statment Else Node/>");
	}
	AsmGenerator::add_label(endIf);
	AsmGenerator::if_temp_label_count++;
	AsmGenerator::comment("If Statment/>");
}

void CodeGneratorVistor::visit(ListNode *listNode)
{
	for (auto &node : listNode->nodes) {
		if (node == nullptr) continue;
		node->generate_code(this);
	}
}

void CodeGneratorVistor::visit(ScalarNode *scalarNode)
{
	AsmGenerator::comment("<Scaler Node");
	string s0 = "s0";
	string f0 = "f0";
	switch (scalarNode->value.type)
		{
		case 1:
			AsmGenerator::li(s0,scalarNode->value.int_val);
			AsmGenerator::push(s0);
			break;
		case 2:
			AsmGenerator::f_li(f0,scalarNode->value.float_val);
			AsmGenerator::f_push(f0);
			break;
		case 3:
			AsmGenerator::li(s0,scalarNode->value.bool_val);
			AsmGenerator::push(s0);
			break;
		case 5:
			AsmGenerator::la(s0,AsmGenerator::store_string_literal(scalarNode->value.string_val));
			AsmGenerator::push(s0);
			break;
		}
	AsmGenerator::comment("Scaler Node/>");
}

void CodeGneratorVistor::visit(WhileNode *whileNode)
{
	AsmGenerator::comment("<WhileNode");
	string beginWhile	= "while_begin_label_" + to_string(AsmGenerator::if_temp_label_count);
	string endWhile		= "while_end_label"  + to_string(AsmGenerator::if_temp_label_count);


	string s0 = "s0";
	AsmGenerator::add_label(beginWhile);
	AsmGenerator::comment("<While Condition");
	if (whileNode->condition){
		whileNode->condition->generate_code(this);
		AsmGenerator::pop(s0);
		AsmGenerator::add_instruction("beqz $"+s0+", "+endWhile);
	}
	AsmGenerator::comment("While Condition/>");


	AsmGenerator::comment("<While Body");
	whileNode->body->generate_code(this);
	AsmGenerator::comment("While Body/>");


	AsmGenerator::add_instruction("b " + beginWhile);
  
	AsmGenerator::add_label(endWhile);
	AsmGenerator::if_temp_label_count++;
	AsmGenerator::comment("WhileNode/>");
}

void CodeGneratorVistor::visit(FunctionCallNode *functionCallNode)
{
	AsmGenerator::comment("<FunctionCallNode");

	AsmGenerator::comment("<ArgumentList");
	functionCallNode->argumentsList->generate_code(this);
	AsmGenerator::comment("ArgumentList/>");

	//TODO replace this with unique name of function
	string functionName = functionCallNode->name;

	AsmGenerator::jal(functionName);

	if (functionCallNode->argumentsList){
		AsmGenerator::comment("<Clear Arguments");
		int argumentsSize = 0; // in bytes 
		for(auto &node : functionCallNode->argumentsList->nodes){
			//TODO replace fixed size with size from type System
			int argSize = 4;
			argumentsSize+= argSize;
		}
		AsmGenerator::add_instruction("addi $sp, $sp, " + to_string(argumentsSize));
		AsmGenerator::comment("Clear Arguments/>");
	}

	
	if (functionCallNode->getNodeType()->getTypeId() != VOID_TYPE_ID){
		if (functionCallNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID)
			AsmGenerator::f_push("v1");
		else
			AsmGenerator::push("v1");
	}

	AsmGenerator::comment("FunctionCallNode/>");
}

void CodeGneratorVistor::visit(FunctionDefineNode *functionDefineNode)
{
	Function *functionSymbol = functionDefineNode->functionSym;

	functionSymbol->setId(symbolIDS++);

	AsmGenerator::comment("<FunctionDefineNode");



	string functionName = functionSymbol->getName();
	returnLabel = functionName+"_ret";

	AsmGenerator::comment("Look below to see function "+functionName);

	AsmGenerator::initialize_function(functionName); 

	currentFrame = new FunctionFrame(currentFrame,functionDefineNode->paramsList);

	FunctionFrame* functionFrame = dynamic_cast<FunctionFrame*>(currentFrame);

	AsmGenerator::function_prologue(functionFrame->stackSize);

	for(auto &node : functionDefineNode->paramsList->nodes)
	{
		ParameterNode* paramterNode = dynamic_cast<ParameterNode*>(node);
		if (paramterNode == nullptr)
			cout << "DAMN this shoudn't happen" << endl;

		paramterNode->generate_code(this);
	}

	functionDefineNode->bodySts->generate_code(this);

	AsmGenerator::add_label(returnLabel);

	/*if (functionDefineNode->getNodeType()->getTypeId() != VOID_TYPE_ID) {
		AsmGenerator::comment("Returning Value");
		if (functionDefineNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID)
			AsmGenerator::f_pop("v1");
		else
			AsmGenerator::pop("v1");
	}*/
	AsmGenerator::function_epilogue(functionFrame->stackSize);
	AsmGenerator::write_function();
	AsmGenerator::comment("FunctionDefineNode/>");

	currentFrame = functionFrame->parentFrame;
}

void CodeGneratorVistor::visit(ParameterNode *parameterNode)
{
	parameterNode->parSym->setId(symbolIDS++);
	AsmGenerator::comment("<ParameterNode");

	AsmGenerator::comment("ParameterNode/>");
}

void CodeGneratorVistor::visit(ReturnNode *returnNode)
{
	AsmGenerator::comment("<ReturnNode");
	if (returnNode->returnend_expression){
		returnNode->returnend_expression->generate_code(this);
	}
	AsmGenerator::add_instruction("b "+returnLabel);
	AsmGenerator::comment("ReturnNode/>");
}

void CodeGneratorVistor::visit(ClassDefineNode	*classDefineNode)
{
	AsmGenerator::comment("<ClassDefineNode");
	currentFrame = new ObjectFrame(currentFrame,classDefineNode);

	ObjectFrame* objectFrame = dynamic_cast<ObjectFrame*>(currentFrame);
	objectsFrames[classDefineNode->classSymbol->getName()] = objectFrame;
	classDefineNode->body->generate_code(this);


	AsmGenerator::comment("ClassDefineNode/>");

	currentFrame = objectFrame->parentFrame;
}

void CodeGneratorVistor::visit(ClassMemNode	*classMemNode)
{
	ObjectFrame* objectFrame = dynamic_cast<ObjectFrame*>(currentFrame);
	if (objectFrame != nullptr)
	{
		objectFrame->addLocal(classMemNode);
	}else{
		cout << "CurrentFrame Should be objectFrame" << endl;
	}
	string memberName = classMemNode->getMemSymbol()->getName();
	AsmGenerator::comment("<Declare member "+memberName+" />");
}

void CodeGneratorVistor::visit(ClassMethodNode *classMethodNode)
{
	AsmGenerator::comment("<ClassMethodNode");
	ObjectFrame* objectFrame = dynamic_cast<ObjectFrame*>(currentFrame);
	if (objectFrame != nullptr){
		objectFrame->addFunction(classMethodNode);

		Method *methodSymbol = classMethodNode->methodSym;

		string methodName = methodSymbol->getName();
		returnLabel = methodName+"_ret";

		AsmGenerator::comment("Look below to see method "+methodName);

		AsmGenerator::initialize_function(methodName); 

		currentFrame = new FunctionFrame(currentFrame,nullptr);

		FunctionFrame* functionFrame = dynamic_cast<FunctionFrame*>(currentFrame);

		AsmGenerator::function_prologue(functionFrame->stackSize);
		
		//TODO
		/*for(auto &node : )
		{
			ParameterNode* paramterNode = dynamic_cast<ParameterNode*>(node);
			if (paramterNode == nullptr)
				cout << "DAMN this shoudn't happen" << endl;

			paramterNode->generate_code(this);
		}*/




	}else{
		cout << "CurrentFrame Should be objectFrame" << endl;
	}
	AsmGenerator::comment("ClassMethodNode/>");
}

void CodeGneratorVistor::visit(ClassCallNode *classCallNode)
{
	AsmGenerator::comment("<ClassCallNode");
	string s0 = "s0";
	
	string probertyAddress = getClassMemberAddress(classCallNode,s0);

	if (!classCallNode->isMethodCall){
		TypeClass* classType = dynamic_cast<TypeClass*>(classCallNode->object->getNodeType());
		int propertyTypeID = classType->lookupMembers("$"+classCallNode->propertyString)->type->getTypeId();


		if (propertyTypeID == INTEGER_TYPE_ID	|| propertyTypeID == BOOLEAN_TYPE_ID){
			AsmGenerator::lw(s0,probertyAddress);
			AsmGenerator::push(s0);
		}

		if (propertyTypeID == STRING_TYPE_ID){
			AsmGenerator::lw(s0,probertyAddress); 
			AsmGenerator::push(s0);

		}
		
		if (propertyTypeID == FLOAT_TYPE_ID){
			AsmGenerator::ls("f0",probertyAddress);
			AsmGenerator::f_push("f0");
		}

		if (propertyTypeID == CLASS_TYPE_ID){
		AsmGenerator::lw(s0,probertyAddress); 
		AsmGenerator::push(s0);
	}
	}else{
	
	
	}

	AsmGenerator::comment("ClassCallNode/>");
}


void CodeGneratorVistor::visit(NewNode *newNode)
{
	AsmGenerator::comment("<NewNode");
	
	if (objectsFrames.find(newNode->className) != objectsFrames.end()) {
		ObjectFrame *objectsFrame = objectsFrames[newNode->className];
		int objectSize = objectsFrame->membersOffset;
		string s0 = "s0",s1 = "s1";
		AsmGenerator::li(s0,objectSize);
		AsmGenerator::sbrk(s0,s1);
		AsmGenerator::push(s1);
	}else{
		cout << "UnDeclared Class" << endl;
	}
	AsmGenerator::comment("NewNode/>");

	
}

string CodeGneratorVistor::getClassMemberAddress(ClassCallNode *classCallNode,string reg)
{
	TypeClass* classType = dynamic_cast<TypeClass*>(classCallNode->object->getNodeType());

	if (classType != nullptr){
		string className = classType->getName();
		if (objectsFrames.find(className) != objectsFrames.end()){
			ObjectFrame* objectFrame = objectsFrames[className];

	

			string propertyOffset = objectFrame->getAddress(classCallNode->propertyString);
			string objectAddress  = currentFrame->getAddress(classCallNode->object->variable->getNameWithout());


			AsmGenerator::lw(reg,objectAddress); //load the addrees of object
			string probertyAddress = propertyOffset+"($"+reg+")";


			return probertyAddress;
		}else{
			cout << "UnDeclared Class" << endl;
			return nullptr;
		}
	}else{
		cout << "UnDeclared Class" << endl;
		return nullptr;
	}

}