#pragma once 
#include "CodeGeneratorVistor.hpp"
#include "AsmGenerator.h"
#include "../AST/all.hpp"
#include "Frame.hpp"
#include "../TypeSystem/TypeClass.hpp"
#include "../TypeSystem/TypeFunction.hpp"


void CodeGneratorVistor::generate(ListNode *ast)
{
	currentFrame	= new GlobalFrame();
	AsmGenerator::initialize_file();
	fillesClassesFrames();
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

		if (ClassCallNode *classCallNode = dynamic_cast<ClassCallNode*>(assignmentNode->lhs))
		{
			string probertyAddress = getClassMemberAddress(classCallNode,s0);
			AsmGenerator::sw(s1,probertyAddress);
		}else{
			cout << "left hand side should be Variable node or Member of object" << endl;
		}
	}

	if (assignmentNode->getNodeType()->getTypeId() == STRING_TYPE_ID)
	{
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
		}
		else
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

		if (GC){

			AsmGenerator::move("a1","a0");
			AsmGenerator::jal(AsmGenerator::increase_rc_function_name);
		}
	}

	if (assignmentNode->getNodeType()->getTypeId() == CLASS_TYPE_ID)
	{
		string s0 = "s0";
		string s1 = "s1";			 
		AsmGenerator::pop(s1);
		AsmGenerator::pop(s0);
		//s0 should contain the address of object variale
		//s1 should contain the address of object in heap
	
		if (VariableNode *variableNode = dynamic_cast<VariableNode*>(assignmentNode->lhs))
		{
			string variableAddress = currentFrame->getAddress(variableNode->variable->getNameWithout());
			AsmGenerator::sw(s1,variableAddress);
			if (GC)
			{

				// If we are in scoopeFrame and i have an assigment
				ScopeFrame *scopeFrame = dynamic_cast<ScopeFrame*>(currentFrame);
				if (scopeFrame){
					scopeFrame->objectsLocals.push_back(variableNode->variable->getNameWithout());
				}

				string endEditingRC = "GC_Finish_RC_Editing_"	+ to_string(AsmGenerator::temp_label_count);
				string tempLabel	= "GC_Finish_RC_Editing_temp"	    + to_string(AsmGenerator::temp_label_count++);
				AsmGenerator::beq(s0,s1,endEditingRC);

		
				AsmGenerator::add_instruction("beq $s0,$0,"+tempLabel); // after decrease if v0 != 0 ( I know that object is freed)
				AsmGenerator::lw(s0,variableAddress);
				AsmGenerator::move("a1",s0);
				AsmGenerator::jal(AsmGenerator::decrease_rc_function_name);


				AsmGenerator::add_instruction("beq $v0,$0,"+tempLabel); // after decrease if v0 != 0 ( I know that object is freed)
				AsmGenerator::sw("0",variableAddress);						// so i put null in its variable


				AsmGenerator::add_label(tempLabel);

				AsmGenerator::move("a1",s1);

				AsmGenerator::jal(AsmGenerator::increase_rc_function_name);

				AsmGenerator::add_label(endEditingRC);
			}
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


	binaryOperationNode->left->generate_code(this);

	binaryOperationNode->right->generate_code(this);

	AsmGenerator::comment("<Binary Operation Calculation");
	if (binaryOperationNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID){ //Integer

		AsmGenerator::pop(t1); 
		AsmGenerator::pop(t0); 
		
		if (*(binaryOperationNode->op_type) == '+'){
			AsmGenerator::binary_operation(t0, t0, t1, 1); 
			AsmGenerator::push(t0);
		}

		if (*(binaryOperationNode->op_type) == '-'){
			AsmGenerator::binary_operation(t0, t0, t1, 2);
			AsmGenerator::push(t0); 
		}

		if (*(binaryOperationNode->op_type) == '*'){
			AsmGenerator::binary_operation(t0, t0, t1, 3); 
			AsmGenerator::push(t0); 
		}

		if (*(binaryOperationNode->op_type) == '/'){
			AsmGenerator::binary_operation(t0, t0, t1, 4); 
			AsmGenerator::push(t0); 
		}

		if (*(binaryOperationNode->op_type) == '%'){
			AsmGenerator::binary_operation(t0, t0, t1, 5); 
			AsmGenerator::push(t0); 
		}

		
	}

	if (binaryOperationNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){ //Boolean

		if (binaryOperationNode->left->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID && 
			binaryOperationNode->right->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID ){
		
			AsmGenerator::pop(t1);
			AsmGenerator::pop(t0); 

			if(strcmp(binaryOperationNode->op_type, "&&") == 0){
				AsmGenerator::binary_operation(t0, t0, t1, 6); 
				AsmGenerator::push(t0); 
			}

			if (strcmp(binaryOperationNode->op_type, "||") == 0){
				AsmGenerator::binary_operation(t0, t0, t1, 7); 
				AsmGenerator::push(t0);
			}
		}



		if (binaryOperationNode->left->getNodeType()->getTypeId() == INTEGER_TYPE_ID && 
			binaryOperationNode->right->getNodeType()->getTypeId() == INTEGER_TYPE_ID ){

			AsmGenerator::pop(t1);
			AsmGenerator::pop(t0); 


			if (strcmp(binaryOperationNode->op_type, "==") == 0){
				AsmGenerator::equal_operation(t2,t0,t1,false); 
				AsmGenerator::push(t2); 
			}

			if (strcmp(binaryOperationNode->op_type, "!=") == 0){
				AsmGenerator::equal_operation(t2,t0,t1,true);
				AsmGenerator::push(t2);
			}

			if (strcmp(binaryOperationNode->op_type, "<") == 0){
				AsmGenerator::less_than_operation(t2, t0, t1);  
				AsmGenerator::push(t2);						

			}

			if (strcmp(binaryOperationNode->op_type, ">") == 0){
				AsmGenerator::less_than_operation(t2, t1, t0); 
				AsmGenerator::push(t2);						
			}

			if (strcmp(binaryOperationNode->op_type, ">=") == 0){
				AsmGenerator::greater_or_equal_operation(t2, t0, t1); 
				AsmGenerator::push(t2);	
			}

			if (strcmp(binaryOperationNode->op_type, "<=") == 0){
				AsmGenerator::greater_or_equal_operation(t2, t1, t0); 
				AsmGenerator::push(t2);	
			}

		}

		if (binaryOperationNode->left->getNodeType()->getTypeId() == FLOAT_TYPE_ID ||  
			binaryOperationNode->right->getNodeType()->getTypeId() == FLOAT_TYPE_ID ){
		
			string f0 = "f0";
			string f1 = "f1";


			if (binaryOperationNode->left->getNodeType()->getTypeId() == INTEGER_TYPE_ID){ // convert int to float
				AsmGenerator::f_pop(f1);
				AsmGenerator::pop(t0);
				AsmGenerator::add_instruction("mtc1 $t0, $f0");
				AsmGenerator::add_instruction("cvt.s.w $f0, $f0");
			}else
			if(binaryOperationNode->right->getNodeType()->getTypeId() == INTEGER_TYPE_ID ){// convert int to float
				AsmGenerator::pop(t0);
				AsmGenerator::add_instruction("mtc1 $t0, $f1");
				AsmGenerator::add_instruction("cvt.s.w $f1, $f1");
				AsmGenerator::f_pop(f0);
			}else{
				AsmGenerator::f_pop(f1);
				AsmGenerator::f_pop(f0);
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

		if (binaryOperationNode->left->getNodeType()->getTypeId() == CLASS_TYPE_ID ||  
			binaryOperationNode->right->getNodeType()->getTypeId() == CLASS_TYPE_ID ){
			string t0 = "t0";
			string t1 = "t1";

			AsmGenerator::pop(t1); 
			AsmGenerator::pop(t0); 

			AsmGenerator::equal_operation(t2,t0,t1,false); 
			AsmGenerator::push(t2); 
		}
		
	
	}

	if (binaryOperationNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){ //Float
		string f0 = "f0";
		string f1 = "f1";

		if (binaryOperationNode->left->getNodeType()->getTypeId() == INTEGER_TYPE_ID){ // convert int to float
			AsmGenerator::f_pop(f1);
			AsmGenerator::pop(t0);
			AsmGenerator::add_instruction("mtc1 $t0, $f0");
			AsmGenerator::add_instruction("cvt.s.w $f0, $f0");
		}else
		if(binaryOperationNode->right->getNodeType()->getTypeId() == INTEGER_TYPE_ID ){// convert int to float
			AsmGenerator::pop(t0);
			AsmGenerator::add_instruction("mtc1 $t0, $f1");
			AsmGenerator::add_instruction("cvt.s.w $f1, $f1");
			AsmGenerator::f_pop(f0);
		}else{
			AsmGenerator::f_pop(f1);
			AsmGenerator::f_pop(f0);
		}

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

			string begin_loop_label		= "concat_begin" + to_string(AsmGenerator::if_temp_label_count);
			string finish_loop_leble	= "concat_end"   + to_string(AsmGenerator::if_temp_label_count);
			AsmGenerator::if_temp_label_count++;

	
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


		if (binaryOperationNode->left->getNodeType()->getTypeId() == INTEGER_TYPE_ID)
		{
			string s0 = "s0";
			string s1 = "s1";
			string t0 = "t0";
			string t1 = "t1";
			string t3 = "t3";
			string begin_loop_label		= "concat_begin" + to_string(AsmGenerator::if_temp_label_count);
			string finish_loop_leble	= "concat_end"   + to_string(AsmGenerator::if_temp_label_count);
			AsmGenerator::if_temp_label_count++;

			AsmGenerator::pop(s0); //string 
			AsmGenerator::pop(s1); // integer

			AsmGenerator::move("a0",s0);
			AsmGenerator::jal(AsmGenerator::strlen_functoion_name);
			AsmGenerator::add_instruction("addi $t0,$v1,5");
			AsmGenerator::sbrk(t0,t1);

			AsmGenerator::move(t3,"v0");

			AsmGenerator::move("a0",s1);
			AsmGenerator::move("a1","t1");
			AsmGenerator::jal(AsmGenerator::int_to_asci_functoion_name);

			AsmGenerator::add_instruction("addi $t1,$t1,1");

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
	AsmGenerator::comment("<Variable Node "+variableNode->variable->getNameWithout());
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
	if (assignmentNode->body)
		assignmentNode->body->generate_code(this);
	AsmGenerator::comment("Else Node/>");
}

void CodeGneratorVistor::visit(ForNode *forNode)
{
	AsmGenerator::comment("<ForNode");
	string beginFor		= "for_begin_label_"	+ to_string(AsmGenerator::if_temp_label_count);
	string endFor		= "for_end_label_"		+ to_string(AsmGenerator::if_temp_label_count);
	string contineFor	= "for_continue_label_" + to_string(AsmGenerator::if_temp_label_count);
	AsmGenerator::if_temp_label_count++;

	string s0 = "s0";
	string prevReturnLabel		= returnLabel;
	
	returnLabel		= endFor;

	string prevContinueLabel	= continueLabel;
	continueLabel				= contineFor;

	currentFrame = new ScopeFrame(currentFrame,isFunction);

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

	if (GC){
		collectRefVariablesGarbage(currentFrame);
	}

	AsmGenerator::add_label(continueLabel);

	AsmGenerator::comment("<For Increment");
	if (forNode->post_statement){
		forNode->post_statement->generate_code(this);
	}
	AsmGenerator::comment("For Increment/>");


	AsmGenerator::add_instruction("b " + beginFor);

	AsmGenerator::add_label(endFor);

	int size				= currentFrame->frameSize;
	currentFrame			= currentFrame->parentFrame;
	currentFrame->frameSize = size;

	returnLabel		= prevReturnLabel;
	continueLabel	= prevContinueLabel;

	AsmGenerator::comment("ForNode/>");
}

void CodeGneratorVistor::visit(IfNode *ifNode)
{
	AsmGenerator::comment("<If Statment");
	string t0 = "t0";
	string else_label	= "else_label_" + to_string(AsmGenerator::if_temp_label_count);
	string endIf		= "end_if_"     + to_string(AsmGenerator::if_temp_label_count);
	AsmGenerator::if_temp_label_count++;
	
	AsmGenerator::comment("<If Statment Condition Node");
	if (ifNode->condition)
		ifNode->condition->generate_code(this);
	AsmGenerator::comment("<If Statment Condition Node/>");

	AsmGenerator::pop(t0);
	AsmGenerator::beq(t0,"0",else_label); // if t0 (condition) equal 0 ==> control go to else node

	if (ifNode->body)					// else t0 (condition) equal 1 ==> control got to body node
	{
		AsmGenerator::comment("<If Statment Body Node");
		ifNode->body->generate_code(this);
		AsmGenerator::comment("If Statment Body Node/>");
	}
	AsmGenerator::jump_label(endIf);	 // body completed got to finish label
	AsmGenerator::add_label(else_label);
	if (ifNode->else_node){
		AsmGenerator::comment("<If Statment Else Node");
		ifNode->else_node->generate_code(this);
		AsmGenerator::comment("If Statment Else Node/>");
	}
	AsmGenerator::add_label(endIf);

	AsmGenerator::comment("If Statment/>");
}

void CodeGneratorVistor::visit(ListNode *listNode)
{
	if (listNode->hasScopeFrame){
		currentFrame = new ScopeFrame(currentFrame,isFunction);
	}
	for (auto &node : listNode->nodes) {
		if (node == nullptr) continue;
		node->generate_code(this);
	}
	if (listNode->hasScopeFrame){
		if (GC){
			collectRefVariablesGarbage(currentFrame);
		}
		int size				= currentFrame->frameSize;
		currentFrame			= currentFrame->parentFrame;
		currentFrame->frameSize = size;
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
	string beginWhile	= "while_begin_label_"		+ to_string(AsmGenerator::if_temp_label_count);
	string endWhile		= "while_end_label_"		+ to_string(AsmGenerator::if_temp_label_count);
	string contineWhile	= "while_continue_label_"	+ to_string(AsmGenerator::if_temp_label_count);
	AsmGenerator::if_temp_label_count++;

	string prevReturnLabel		= returnLabel;
	string prevContinueLabel	= continueLabel;
	returnLabel		= endWhile;
	continueLabel	= beginWhile;

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
	if (whileNode->body){
		whileNode->body->generate_code(this);
	}
	AsmGenerator::comment("While Body/>");


	AsmGenerator::add_instruction("b " + beginWhile);
  
	AsmGenerator::add_label(endWhile);

	

	returnLabel		= prevReturnLabel;
	continueLabel	= prevContinueLabel;

	AsmGenerator::comment("WhileNode/>");
}

void CodeGneratorVistor::visit(FunctionCallNode *functionCallNode)
{
	AsmGenerator::comment("<FunctionCallNode");

	TypeFunction *functionType = functionCallNode->functionType;

	prepareArguments(functionCallNode->argumentsList,functionType);

	string functionName = functionType->getUniqueName();

	// :( first 4 bytes is used by methods, here we don't care about so it'z zero
	AsmGenerator::push("0");

	AsmGenerator::jal(functionName);

	AsmGenerator::pop("0");


	clearArguments(functionCallNode->argumentsList);

	if (functionType->getReturnTypeExpression()->getTypeId() != VOID_TYPE_ID){
		if (functionType->getReturnTypeExpression()->getTypeId() == FLOAT_TYPE_ID){
			AsmGenerator::f_push("f1");
		}
		else{
			AsmGenerator::push("v1");
		}
	}
	AsmGenerator::comment("FunctionCallNode/>");
}

void CodeGneratorVistor::visit(FunctionDefineNode *functionDefineNode)
{
	isFunction = true;
	AsmGenerator::comment("<FunctionDefineNode");
	TypeFunction* functionType = dynamic_cast<TypeFunction*>(functionDefineNode->getNodeType());

	string functionName = functionType->getUniqueName();
	string prevReturnLabel = returnLabel;
	returnLabel = functionName+"_ret";

	AsmGenerator::comment("Look below to see function "+functionName);

	AsmGenerator::initialize_function(functionName); 


	currentFrame = new FunctionFrame(currentFrame,functionDefineNode->paramsList);

	FunctionFrame* functionFrame = dynamic_cast<FunctionFrame*>(currentFrame);

	stringstream *temp = new stringstream();
	stringstream *prev;

	prev = AsmGenerator::current_stream;

	AsmGenerator::current_stream = temp;

	for(auto &node : functionDefineNode->paramsList->nodes)
	{
		ParameterNode* paramterNode = dynamic_cast<ParameterNode*>(node);
		if (!paramterNode)
			cout << "DAMN this shoudn't happen" << endl;
		paramterNode->generate_code(this);
	}

	if (functionDefineNode->bodySts)
		functionDefineNode->bodySts->generate_code(this);

	AsmGenerator::current_stream = prev;

	AsmGenerator::function_prologue(functionFrame->frameSize);

	*AsmGenerator::current_stream << temp->str();

	AsmGenerator::add_label(returnLabel);

	if (functionType->getReturnTypeExpression()->getTypeId() != VOID_TYPE_ID) {
		AsmGenerator::comment("Returning Value");
		if (functionType->getReturnTypeExpression()->getTypeId() == FLOAT_TYPE_ID)
			AsmGenerator::f_pop("f1");
		else
			AsmGenerator::pop("v1");
	}


	AsmGenerator::function_epilogue(functionFrame->frameSize);
	AsmGenerator::write_function();
	AsmGenerator::comment("FunctionDefineNode/>");

	currentFrame = functionFrame->parentFrame;
	returnLabel = prevReturnLabel;

	isFunction = false;
}

void CodeGneratorVistor::visit(ParameterNode *parameterNode)
{
	AsmGenerator::comment("<ParameterNode " + parameterNode->parSym->getNameWithout());

	// if paramter is default we have to load it value and store it in proper address
	// but some default paramters will have a value when function get called
	// so we have to differentiate between default paramter with value in function call and 
	// default paramter without value in funcion (value should be taken from function signature)
	// by using special value that we stored in function call

	if (parameterNode->isDefault){
		// to contimue_without_load_default_parameter_value
		string label = "cont_without_load_def_" + to_string(AsmGenerator::if_temp_label_count);
		AsmGenerator::if_temp_label_count++;

		string parameterAddress = currentFrame->getAddress(parameterNode->parSym->getNameWithout());


		AsmGenerator::lw("s1",parameterAddress);
		
		AsmGenerator::li("s2",-1005);

		// if paramters address contian -1005 value 
		// we know that the caller didn't pass the value of defult paramter
		AsmGenerator::add_instruction("bne $s1, $s2, "+label);

		if (parameterNode->defaultValueNode)
			parameterNode->defaultValueNode->generate_code(this);
		
		AsmGenerator::pop("s0");
		
		AsmGenerator::sw("s0",parameterAddress);

		AsmGenerator::add_label(label);
	}

	AsmGenerator::comment("ParameterNode/>");
}

void CodeGneratorVistor::visit(ReturnNode *returnNode)
{
	AsmGenerator::comment("<ReturnNode");
	if (returnNode->returned_node){
		returnNode->returned_node->generate_code(this);
	}
	AsmGenerator::add_instruction("b "+returnLabel);
	AsmGenerator::comment("ReturnNode/>");
}

void CodeGneratorVistor::visit(ClassDefineNode	*classDefineNode)
{
	string className = classDefineNode->classSymbol->getName();
	AsmGenerator::comment("<Class Define Node "+className);

	TypeClass *typeClass		= dynamic_cast<TypeClass*>(classDefineNode->getNodeType());

	currentFrame = objectsFrames[typeClass->getName()];

	if (classDefineNode->body)
			classDefineNode->body->generate_code(this);

	currentFrame = currentFrame->parentFrame;
	AsmGenerator::comment("Class Define Node/>");
}

void CodeGneratorVistor::visit(ClassMemNode	*classMemNode)
{
	string memberName = classMemNode->getMemSymbol()->getName();
	AsmGenerator::comment("<Declare member "+memberName+" />");

	if (classMemNode->getMemSymbol()->isStatic){
		currentFrame->addStatic(classMemNode);
	}
	// non static will be processed when object is created
}

void CodeGneratorVistor::visit(ClassMethodNode *classMethodNode)
{
	isFunction = true;
	AsmGenerator::comment("<Class Method Node");

	TypeFunction* functionType	= dynamic_cast<TypeFunction*>(classMethodNode->getNodeType());

	Method *methodSymbol		= classMethodNode->methodSym;

	string methodName			= methodSymbol->getLabel();
	string prevReturnLabel		= returnLabel;
	returnLabel					= methodName+"_ret";

	AsmGenerator::comment("Look below to see method "+methodName);

	AsmGenerator::initialize_function(methodName); 

	currentFrame				= new FunctionFrame(currentFrame,classMethodNode->paramsList);
	FunctionFrame* methodFrame  = dynamic_cast<FunctionFrame*>(currentFrame);

	stringstream *temp = new stringstream();
	stringstream *prev;

	prev = AsmGenerator::current_stream;

	AsmGenerator::current_stream = temp;
		
	for(auto &node : classMethodNode->paramsList->nodes)
	{
		ParameterNode* paramterNode = dynamic_cast<ParameterNode*>(node);
		if (!paramterNode)
			cout << "DAMN this shoudn't happen" << endl;
		paramterNode->generate_code(this);
	}

	if (classMethodNode->bodySts)
		classMethodNode->bodySts->generate_code(this);


	AsmGenerator::current_stream = prev;

	AsmGenerator::function_prologue(methodFrame->frameSize);

	*AsmGenerator::current_stream << temp->str();

	AsmGenerator::add_label(returnLabel);

	if (!methodSymbol->isConstructor){
		if (functionType->getReturnTypeExpression()->getTypeId() != VOID_TYPE_ID) {
			AsmGenerator::comment("Returning Value");
			if (functionType->getReturnTypeExpression()->getTypeId() == FLOAT_TYPE_ID)
				AsmGenerator::f_pop("f1");
			else
				AsmGenerator::pop("v1");
		}
	}


	AsmGenerator::function_epilogue(methodFrame->frameSize);

	AsmGenerator::write_function();

	currentFrame	= methodFrame->parentFrame;
	returnLabel		= prevReturnLabel;
	
	AsmGenerator::comment("Class Method Node/>");
	
	isFunction = false;
}

void CodeGneratorVistor::visit(ClassCallNode *classCallNode)
{
	AsmGenerator::comment("<Class Call Node "+classCallNode->propertyString);
	string thisReg = "s0";
	string s1 = "s1";
	string probertyAddress	= getClassMemberAddress(classCallNode,thisReg);

	//if thisReg = null go to null ptre exeption handler and stop the program.
	AsmGenerator::add_instruction("beq $"+thisReg+",$0,"+AsmGenerator::nullptr_exception_function_name);

	if (!classCallNode->isMethodCall){
		AsmGenerator::comment("<MemberProperty");

		int propertyTypeID		= classCallNode->member->getTypeExpr()->getTypeId();

		if (propertyTypeID == INTEGER_TYPE_ID	|| propertyTypeID == BOOLEAN_TYPE_ID){
			AsmGenerator::lw(s1,probertyAddress);
			AsmGenerator::push(s1);
		}

		if (propertyTypeID == STRING_TYPE_ID){
			AsmGenerator::lw(s1,probertyAddress);
			AsmGenerator::push(s1);
		}
		
		if (propertyTypeID == FLOAT_TYPE_ID){
			AsmGenerator::ls("f0",probertyAddress);
 			AsmGenerator::f_push("f0");
		}

		if (propertyTypeID == CLASS_TYPE_ID){ 
			AsmGenerator::lw(s1,probertyAddress);
			AsmGenerator::push(s1);
		}
		AsmGenerator::comment("Member Property/>");
	}else{

		AsmGenerator::comment("<Method Property");

		TypeFunction *functionType = dynamic_cast<TypeFunction*>(classCallNode->member->getTypeExpr());

		prepareArguments(classCallNode->argumentsList,functionType);

		string thisReg = "s0";
		string probertyAddress	= getClassMemberAddress(classCallNode,thisReg);
		AsmGenerator::push("s0"); // Push this reg

		AsmGenerator::lw(s1,probertyAddress);
		AsmGenerator::add_instruction("jalr $"+s1);

		AsmGenerator::pop("s0"); // Pop this reg

		clearArguments(classCallNode->argumentsList);
	
		if (functionType->getReturnTypeExpression()->getTypeId() != VOID_TYPE_ID){
			if (functionType->getReturnTypeExpression()->getTypeId() == FLOAT_TYPE_ID){
				AsmGenerator::f_push("f1");
			}
			else{
				AsmGenerator::push("v1");
			}
		}

		AsmGenerator::comment("Method Property/>");
	}
	AsmGenerator::comment("Class Call Node/>");
}

void CodeGneratorVistor::visit(NewNode *newNode)
{
	AsmGenerator::comment("<NewNode");

	string s0 = "s0",s1 = "s1",s2 = "s2";

	TypeClass *classType = static_cast<TypeClass*>(newNode->getNodeType());

	ObjectFrame* objectFrame = objectsFrames[newNode->className];

	// load the size of oject in $s0
	AsmGenerator::li(s0,classType->getSize());

	//syscall to allocate space for object, the address of new allocated memory $s1 (address of object)
	AsmGenerator::sbrk(s0,s1);

	AsmGenerator::comment("<Fill Object Table");
	for(auto &memberWrapper :classType->getMembers())
	{
		if (memberWrapper->getWrapperType() == MemberWrapper::PROPERTY_WRAPPER){

			PropertyWrapper* propertyWrapper	= dynamic_cast<PropertyWrapper*>(memberWrapper);
			DataMember* dataMember				= propertyWrapper->memberSymbol;
			int propertyOffset					= objectFrame->locals[dataMember->getNameWithout()];
			string propertyAddress				= to_string(propertyOffset)+"($" + s1 + ")"; // address in object

			int propertyTypeID = propertyWrapper->getTypeExpr()->getTypeId();

			if (strcmp(dataMember->getName(),"$id")==0){
				AsmGenerator::li(s2,objectFrame->objectsCount++);
				AsmGenerator::sw(s2,propertyAddress);
				continue;
			}

			if (strcmp(dataMember->getName(),"$tag")==0){
				AsmGenerator::la(s2,objectFrame->classTagAddress);
				AsmGenerator::sw(s2,propertyAddress);
				continue;
			}

			if (dataMember->isInit()){
				if (propertyTypeID == INTEGER_TYPE_ID){
					AsmGenerator::li(s2,dataMember->getInitialValue().int_val);
					AsmGenerator::sw(s2,propertyAddress);
					continue;
				}

				if (propertyTypeID == BOOLEAN_TYPE_ID){
					AsmGenerator::li(s2,dataMember->getInitialValue().bool_val);
					AsmGenerator::sw(s2,propertyAddress);
					continue;
				}

				if (propertyTypeID == FLOAT_TYPE_ID){
					AsmGenerator::f_li("f0",dataMember->getInitialValue().float_val);
					AsmGenerator::ss("f0",propertyAddress);
					continue;
				}

				if (propertyTypeID == STRING_TYPE_ID){
					string t0 = "t0",t1 = "t1";

					string stringAddress = AsmGenerator::store_string_literal(dataMember->getInitialValue().string_val);

					AsmGenerator::la(t0,stringAddress);
					
					AsmGenerator::move("a0",t0); // copy address of string literal into a0
					AsmGenerator::jal(AsmGenerator::strlen_functoion_name); // calculate length of string the result will be in $v1
					AsmGenerator::add_instruction("addi $t0,$v1,1"); // length++;
					AsmGenerator::sbrk(t0,s2);						// allcoate memory and store the address of newley crated memory in t0
					AsmGenerator::sw(s2,propertyAddress);

					AsmGenerator::la(t0,stringAddress);
					AsmGenerator::move("a0",s2); // put the address of newly created memory in a0 (the parameter of strcpy function)
					AsmGenerator::move("a1",t0); // put the address of string literal in a1 (the second parameter of strcpy function)
					AsmGenerator::jal(AsmGenerator::strcpy_function_name);
					continue;
				}
			}else{
					
				if (propertyTypeID == STRING_TYPE_ID){
					// load the address of empty string in s0
					AsmGenerator::la(s2,AsmGenerator::empty_string_address);
					// store the address of empty string in variable address
					AsmGenerator::sw(s2,propertyAddress);
					continue;
				}
			}
		}

		if (memberWrapper->getWrapperType() == MemberWrapper::METHOD_WRAPPER){
			MethodWrapper* methodWrapper	= dynamic_cast<MethodWrapper*>(memberWrapper);

			int methodOffset				= objectFrame->locals[methodWrapper->getName()];
			string methodLabel				= methodWrapper->getLabel();
			string methodAddress			= to_string(methodOffset)+"($" + s1 + ")"; // address in object

			AsmGenerator::la(s2,methodLabel);	//load the address of method
			AsmGenerator::sw(s2,methodAddress); //store the address of method in object
		}
	}
	AsmGenerator::comment("Fill Object Table/>");

	AsmGenerator::comment("<Calling Constructor");

	MethodWrapper * constructorWr	= static_cast<MethodWrapper*>(newNode->constructorWr);
	TypeFunction *functionType		= static_cast<TypeFunction*>(constructorWr->getTypeExpr());

	prepareArguments(newNode->argumentsList,functionType);

	string constractorName = constructorWr->getLabel();


	AsmGenerator::push(s1);	// Push this reg

	AsmGenerator::jal(constractorName);

	AsmGenerator::pop(s1);	// pop this reg

	clearArguments(newNode->argumentsList);

	AsmGenerator::comment("Calling Constructor/>");

	AsmGenerator::push(s1);

	AsmGenerator::comment("NewNode/>");	
}

void CodeGneratorVistor::visit(BreakNode *breakNode)
{
	AsmGenerator::comment("<BreakNode");
	AsmGenerator::add_instruction("b "+returnLabel);
	AsmGenerator::comment("BreakNode/>");
}

void CodeGneratorVistor::visit(ContinueNode *continueNode)
{
	AsmGenerator::comment("<ContinueNode");
	AsmGenerator::add_instruction("b "+continueLabel);
	AsmGenerator::comment("ContinueNode/>");
}

void CodeGneratorVistor::visit(StaticCallNode 	*staticCallNode)
{
	MemberWrapper *memberWrapper = staticCallNode->memberWrapper;
	int propertyTypeID			  = memberWrapper->getTypeExpr()->getTypeId();
	AsmGenerator::comment("<Static Variable Node "+memberWrapper->getName());
	
	if (staticCallNode->isMethodCall){
	
		TypeFunction *functionType = dynamic_cast<TypeFunction*>(memberWrapper->getTypeExpr());

		prepareArguments(staticCallNode->argumentsList,functionType);


		MethodWrapper* methodWrapper = dynamic_cast<MethodWrapper*>(memberWrapper);

		AsmGenerator::push("0");

		AsmGenerator::jal(methodWrapper->getLabel());

		AsmGenerator::pop("0"); 

		clearArguments(staticCallNode->argumentsList);
	
		if (functionType->getReturnTypeExpression()->getTypeId() != VOID_TYPE_ID){
			if (functionType->getReturnTypeExpression()->getTypeId() == FLOAT_TYPE_ID){
				AsmGenerator::f_push("f1");
			}
			else{
				AsmGenerator::push("v1");
			}
		}
	}else{
		string s0 = "s0";
		string variableAddress = currentFrame->getAddress(memberWrapper->getUniqueName());

		if (propertyTypeID == INTEGER_TYPE_ID	|| 
			propertyTypeID  == BOOLEAN_TYPE_ID){
			AsmGenerator::lw(s0,variableAddress); 		
			AsmGenerator::push(s0);
		}

		if (propertyTypeID  == STRING_TYPE_ID){
			AsmGenerator::lw(s0,variableAddress); 	
			AsmGenerator::push(s0);
		}
		
		if (propertyTypeID  == FLOAT_TYPE_ID){
			AsmGenerator::ls("f0",variableAddress);	
			AsmGenerator::f_push("f0");
		}

		if (propertyTypeID == CLASS_TYPE_ID){
			AsmGenerator::lw(s0,variableAddress); 		
			AsmGenerator::push(s0);
		}
		
	}

	AsmGenerator::comment("Static Variable Node/>");
}

string CodeGneratorVistor::getClassMemberAddress(ClassCallNode*  classCallNode,string thisReg)
{
	TypeClass *typeClass		= dynamic_cast<TypeClass*>(classCallNode->object->getNodeType());

	string objectAddress		= currentFrame->getAddress(classCallNode->object->variable->getNameWithout());

	AsmGenerator::lw(thisReg,objectAddress);

	ObjectFrame* objectFrame	= objectsFrames[typeClass->getName()];
	objectFrame->thisReg		= thisReg;
	return objectFrame->getAddress(classCallNode->propertyString);
}

void CodeGneratorVistor::collectRefVariablesGarbage(Frame *frame)
{
	//Loop over ref variables in this frame and performe decrease
	for (string varName : frame->objectsLocals) 
	{
		string varAddress = frame->getAddress(varName);
		AsmGenerator::lw("a1",varAddress); //store address of ojbect in a0
		AsmGenerator::jal(AsmGenerator::decrease_rc_function_name);

		string endLebel = "GC_Finish_decress_"	+ to_string(AsmGenerator::temp_label_count++);
		AsmGenerator::add_instruction("beq $v0,$0,"+endLebel);		// after decrease if v0 != 0 ( I know that object is freed)
		AsmGenerator::sw("0",varAddress);						// so i put null in its variabless
		AsmGenerator::add_label(endLebel);
	}
}

void CodeGneratorVistor::fillesClassesFrames()
{
	for(auto classType : TypeClass::classInstances){
		currentFrame				= new ObjectFrame(currentFrame,classType);
		ObjectFrame* objectFrame	= dynamic_cast<ObjectFrame*>(currentFrame);
		objectsFrames[classType->getName()] = objectFrame;
		currentFrame = objectFrame->parentFrame;
	}
}

void CodeGneratorVistor::prepareArguments(Node *argumentsList,TypeFunction *typeFunctions)
{
	AsmGenerator::comment("<ArgumentsList");
	if (argumentsList)
		argumentsList->generate_code(this);
	AsmGenerator::comment("ArgumentsList/>");

	ListNode *argsList = static_cast<ListNode*>(argumentsList);

	int arguemtnsSize;
	if (argsList){
		arguemtnsSize = argsList->nodes.size();
	}else{
		arguemtnsSize = 0;
	}

	int parameterSize	= typeFunctions->getParamsTEs().size();
	int diffSize		= parameterSize - arguemtnsSize;	// number of enabeld default parameters

	//Make some space for enabeld default parameters
	for(int i = 0 ; i < diffSize ; i++)
	{
		string t0 = "t0";
		//we will store special value -1005 (maybe special) to know that this parameter is default and it's value should be
		//taken from function decleration
		AsmGenerator::li(t0,-1005);
		AsmGenerator::push(t0);
	}
}

void CodeGneratorVistor::clearArguments(Node *argumentsList)
{
	ListNode *argsList = static_cast<ListNode*>(argumentsList);
	if (argsList){
		AsmGenerator::comment("<Clear Arguments");
		int argumentsSize = 0; // in bytes 
		for(auto &node : argsList->nodes){
			//TODO replace fixed size with size from type System
			int argSize = 4;
			argumentsSize+= argSize;
		}
		AsmGenerator::add_instruction("addi $sp, $sp, " + to_string(argumentsSize));
		AsmGenerator::comment("Clear Arguments/>");
	}
}
