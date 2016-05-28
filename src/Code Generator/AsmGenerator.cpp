#include "../definitions.h"
#include "AsmGenerator.h"
#include <string> 


void AsmGenerator::initialize_file()
{
	assembly_code_file.open("src/Code Generator/AssemblyCode.asm");
	initialize_data();
	initialize_main();
}

void AsmGenerator::write_file()
{
	write_data();
	write_main();
	write_functions();
	assembly_code_file.close();
}

void AsmGenerator::initialize_data()
{
	data_stream		<< ".data\n";
	data_stream		<< new_line_address	 << ": .asciiz \"\\n\" \n";
	data_stream		<< empty_string_address << ": .asciiz \"\" \n";
	data_stream		<< "\n"; 
}

void AsmGenerator::write_data()
{
	assembly_code_file << data_stream.str();
}

void AsmGenerator::initialize_main()
{
	*current_stream	<< ".text\n" 
					<< ".globl main\n" 
					<< "main:\n\n";
}

void AsmGenerator::write_main()
{
	AsmGenerator::system_call(10);
	AsmGenerator::add_instruction("\n\n\n\n");	
	main_stream << current_stream->str();
	current_stream->str("");
	assembly_code_file << main_stream.str();
}

void AsmGenerator::initialize_function(string function_name)
{
	AsmGenerator::main_stream << AsmGenerator::current_stream->str();
	AsmGenerator::current_stream->str("");
	AsmGenerator::comment("Function:"+function_name+"---------------------");
	AsmGenerator::add_label(function_name);
}

void AsmGenerator::write_function()
{
	AsmGenerator::jr("ra");
	AsmGenerator::functions_stream << AsmGenerator::current_stream->rdbuf();
	AsmGenerator::current_stream->str("");
}

void AsmGenerator::write_functions()
{
	AsmGenerator::strconcat();
	AsmGenerator::strlen();
	AsmGenerator::int_to_asci();
	AsmGenerator::strcpy();
	assembly_code_file << functions_stream.str();
}

void AsmGenerator::switchStream(stringstream *newStream)
{
	current_stream = newStream;
}

string AsmGenerator::store_global_int(string variable_name,int initial_value)
{
	string c="";
	c+= variable_name;
	c+=": .word ";
	c+=to_string(initial_value);
	AsmGenerator::add_data(c);

	return variable_name;
}

string AsmGenerator::store_global_float(string variable_name,float initial_value)
{
	string c="";
	c+= variable_name;
	c+=": .float ";
	c+=to_string(initial_value);
	AsmGenerator::add_data(c);

	return variable_name;
}

string AsmGenerator::store_global_string(string variable_name,int initial_value)
{
	string c="";
	c+= variable_name;
	c+=": .word ";
	c+=to_string(initial_value);
	AsmGenerator::add_data(c);

	return variable_name;
}

string AsmGenerator::store_string_literal(string value)
{
	string data_label;
	if (strings_map.find(value) != strings_map.end()){
		data_label = "str_literal_" + to_string(strings_map[value]);
	}else{
		data_label = "str_literal_" + to_string(strings_count);
		strings_map[value] = strings_count++;
		string c="";
		c+= data_label;
		c+=": .asciiz ";
		c+= "\"";
		// if value contian \n , this will print newline 
		stringstream cmt(value);
		string line;
		getline(cmt,line);
		c+= line;
		while (getline(cmt, line)){
		c+= line + "\\n";
		}
		c+="\"";
		
		AsmGenerator::add_data(c);
	}
	return data_label;
}

string AsmGenerator::store_string_empty()
{
	string data_label = "string_" + to_string(strings_count);
	strings_count++;
	string c="";
	c+= data_label;
	c+=": .asciiz";
	AsmGenerator::add_data(c);
	return data_label;
}

void AsmGenerator::li(string reg,int value)
{
	string c="li $";
	c+=reg;
	c+=",";
	c+=to_string(value);
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::f_li(string reg,float value)
{

	string float_data_name = AsmGenerator::store_float_value(value);
	string c="l.s $";
	c+=reg;
	c+=", ";
	c+=float_data_name;
	AsmGenerator::add_instruction(c);

}

string AsmGenerator::store_float_value(float value)
{
	string data_label = "float_"+to_string(floats_count++);
	string c="";
	c+= data_label;
	c+=": .float ";
	c+=to_string(value);
	AsmGenerator::add_data(c);

	return data_label;
}

void AsmGenerator::la(string reg,string value)
{
	string c="la $";
	c+=reg;
	c+=", ";
	c+=value;
	AsmGenerator::add_instruction(c);
}

/*
*	address may be label and may be offset and register exp -12($sp)
*/
void AsmGenerator::lw(string dest_reg,string address)
{
	string c="lw $";
	c+=dest_reg;
	c+=", ";
	c+=address;
	AsmGenerator::add_instruction(c);
}

/*
*	address may be label and may be offset and register exp -12($sp)
*/
void AsmGenerator::sw(string source_reg,string address)
{
	string c="sw $";
	c+=source_reg;
	c+=", ";
	c+=address;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::ls(string dest_reg,string address)
{
	string c="l.s $";
	c+=dest_reg;
	c+=", ";
	c+=address;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::ls(string dest_reg,int offset,string source_reg)
{
	string c="l.s $";
	c+=dest_reg;
	c+=", ";
	c+=to_string(offset);
	c+="($";
	c+=source_reg;
	c+=")";
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::ss(string source_reg,string address)
{
	string c="s.s $";
	c+=source_reg;
	c+=", ";
	c+=address;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::ss(string source_reg,int offset,string dest_reg)	
{
	string c="s.s $";
	c+=source_reg;
	c+=", ";
	c+=to_string(offset);
	c+="($";
	c+=dest_reg;
	c+=")";
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::add_label (string label_name)
{
	AsmGenerator::add_instruction(label_name+":");
}

void AsmGenerator::jal(string function_name)
{
	string c ="";
	c+= "jal ";
	c+= function_name;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::jump_label (string label_name)
{
	string c ="";
	c+= "j ";
	c+= label_name;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::jr (string reg)
{
	string c ="jr $";
	c+= reg;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::push(string source_register)
{

	AsmGenerator::add_instruction("\tsub $sp,$sp,4");
	string c="sw $";
	c+=source_register;
	c+=", 0($sp)";
	AsmGenerator::add_instruction("\t"+c);
}

void AsmGenerator::pop(string destination_register)
{
	string c = "lw $";
	c += destination_register;
	c += ", 0($sp)";
	AsmGenerator::add_instruction("\t"+c);
	AsmGenerator::add_instruction("\tadd $sp,$sp,4");
}

void AsmGenerator::f_push(string source_register)
{
	AsmGenerator::add_instruction("\tsub $sp,$sp,4");
	string c="s.s $";
	c+=source_register;
	c+=", 0($sp)";
	AsmGenerator::add_instruction("\t"+c);
}

void AsmGenerator::f_pop(string destination_register)
{
	string c="l.s $";
	c+=destination_register;
	c+=", 0($sp)";
	AsmGenerator::add_instruction(c);
	AsmGenerator::add_instruction("add $sp,$sp,4");
}

void AsmGenerator::beq(string reg1,string reg2,string label)
{
	string c ="";
	c+="beq $";
	c+=reg1;
	c+=" ,$";
	c+=reg2;
	c+=" ," + label;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::binary_operation(string dest_reg,string reg1,string reg2,int operation)
{
	//operation values:
	// 1 +
	// 2 -
	// 3 *
	// 4 /
	// 5 %
	// 6 &&
	// 7 ||
	string c="";

	switch (operation)
	{
		case 1:
				c+="add $";
			break;
		case 2:
				c+="sub $";
			break;
		case 3:
				c+="mul $";
			break;
		case 4:
				c+="div $";
			break;
		case 5:
				c+="rem $";
			break;
		case 6:
				c+="and $"; 
			break;
		case 7:
				c+="or $"; 
			break;
		default:
			break;
	}

	c+=dest_reg;
	c+=", $";

	c+=reg1;
	c+=", $";

	c+=reg2;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::f_binary_operation(string dest_reg,string reg1,string reg2,int operation)
{
	//operation values:
	// 1 +
	// 2 -
	// 3 *
	// 4 /
	string c="";

	switch (operation)
	{
		case 1:
				c+="add.s $";
			break;
		case 2:
				c+="sub.s $";
			break;
		case 3:
				c+="mul.s $";
			break;
		case 4:
				c+="div.s $";
			break;
	}

	c+=dest_reg;
	c+=", $";

	c+=reg1;
	c+=", $";

	c+=reg2;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::equal_operation(string dest_reg,string reg1,string reg2,bool is_not_equal_opration)
{
	// dest_reg values are 
	//			1 if reg1 == reg2 
	//			0 if reg1 != reg2  
	string temp_label = "bne_temp" + std::to_string(temp_label_count);

	string c="";
	c = "bne $"; 
	c+=reg1;
	c+=", $";
	c+=reg2;
	c+= " , ";
	c+= temp_label;

	if (is_not_equal_opration){
		AsmGenerator::li(dest_reg,1);		// load true (1) into dest_reg
		AsmGenerator::add_instruction(c);  // if reg1 not equal reg2 go to label with true (1) in dest_reg 
		AsmGenerator::li(dest_reg,0);	  // else reg1 equal reg2 load false (0) into dest_reg
	}else{
		AsmGenerator::li(dest_reg,0);		// load false (0) into dest_reg
		AsmGenerator::add_instruction(c);  // if reg1 not equal reg2 go to label with false (0) in dest_reg 
		AsmGenerator::li(dest_reg,1);	  // else reg1 equal reg2 load true (1) into dest_reg
	}
	AsmGenerator::add_label(temp_label);
	temp_label_count++;
}

void AsmGenerator::f_equal_operation(string dest_reg,string reg1,string reg2,bool is_not_equal_opration)
{
	// dest_reg values are 
	//			1 if reg1 == reg2 
	//			0 if reg1 != reg2  
	string temp_label = "c.eq.s_temp" + std::to_string(temp_label_count);

	string c="";
	c = "c.eq.s $"; 
	c+=reg1;
	c+=", $";
	c+=reg2;

	if (is_not_equal_opration){
		AsmGenerator::li(dest_reg,1);		// load true (1) into dest_reg
		AsmGenerator::add_instruction(c);	// if reg1 equal reg2 condition bit = 1
		AsmGenerator::add_instruction(string("bc1f "+temp_label)); //branch to  label if the coprocessor 1 condition bit is true
		AsmGenerator::li(dest_reg,0);	   // else reg1 not equal reg2 load true (1) into dest_reg
	}else{
		AsmGenerator::li(dest_reg,0);		// load false (0) into dest_reg
		AsmGenerator::add_instruction(c);  // if reg1 equal reg2 condition bit = 1
		AsmGenerator::add_instruction(string("bc1f "+temp_label)); //branch to label if the coprocessor 1 condition bit is true
		AsmGenerator::li(dest_reg,1);	  
	}
	AsmGenerator::add_label(temp_label);
	temp_label_count++;
}

void AsmGenerator::less_than_operation(string dest_reg,string reg1,string reg2){
	string c="slt $";
	c+=dest_reg;
	c+=",$";
	c+=reg1;
	c+=",$";
	c+=reg2;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::f_less_than_operation(string dest_reg,string reg1,string reg2){
	string temp_label = "temp" + std::to_string(temp_label_count);
	AsmGenerator::li(dest_reg,1);		
	string c="";
	c = "c.lt.s $"; 
	c+=reg1;
	c+=", $";
	c+=reg2;
	AsmGenerator::add_instruction(c);	
	AsmGenerator::add_instruction(string("bc1t "+temp_label));
	AsmGenerator::li(dest_reg,0);	
	AsmGenerator::add_label(temp_label);
	temp_label_count++;
}

void AsmGenerator::f_greater_than_operation(string dest_reg,string reg1,string reg2){
	string temp_label = "c.le.s_temp" + std::to_string(temp_label_count);
	AsmGenerator::li(dest_reg,1);		
	string c="";
	c = "c.le.s $"; 
	c+=reg1;
	c+=", $";
	c+=reg2;
	AsmGenerator::add_instruction(c);	
	AsmGenerator::add_instruction(string("bc1f "+temp_label));
	AsmGenerator::li(dest_reg,0);	
	AsmGenerator::add_label(temp_label);
	temp_label_count++;
}

void AsmGenerator::greater_or_equal_operation(string dest_reg,string reg1,string reg2){
	// dest_reg values are 
	//			1 if reg1 >= reg2 
	//			0 if reg1 <= reg2  
	string temp_label				= "begz_temp"	+ std::to_string(temp_label_count);
	string finish_operation_label	= "begz_finish" + std::to_string(temp_label_count);
	AsmGenerator::binary_operation(dest_reg,reg1,reg2,2);	// dest_reg = reg1 - reg2 

	string c =""; 
	c +="bgez $";
	c+= dest_reg; 
	c+= " ,";
	c+= temp_label;
	AsmGenerator::add_instruction(c);						// if dest_reg >= 0 (which mean reg1 >= reg2) go to label x...

	AsmGenerator::li(dest_reg,0);							// else load 0 (false) into dest_reg and go to finish label
	AsmGenerator::jump_label(finish_operation_label);		// go to finish label
	AsmGenerator::add_label(temp_label);					// label x:
	AsmGenerator::li(dest_reg,1);							// if dest_reg >= control will jumb to label x and dest_reg will be 1 (true)
	AsmGenerator::add_label(finish_operation_label);
	temp_label_count++;
}

void AsmGenerator::f_less_or_equal_operation(string dest_reg,string reg1,string reg2){
	string temp_label = "c.lt.s_temp" + std::to_string(temp_label_count);
	AsmGenerator::li(dest_reg,1);		
	string c="";
	c = "c.le.s $"; 
	c+=reg1;
	c+=", $";
	c+=reg2;
	AsmGenerator::add_instruction(c);	
	AsmGenerator::add_instruction(string("bc1t "+temp_label));
	AsmGenerator::li(dest_reg,0);	
	AsmGenerator::add_label(temp_label);
	temp_label_count++;
}

void AsmGenerator::f_greater_or_equal_operation(string dest_reg,string reg1,string reg2){
	string temp_label = "c.lt.s_temp" + std::to_string(temp_label_count);
	AsmGenerator::li(dest_reg,0);		
	string c="";
	c = "c.lt.s $";
	c+=reg1;
	c+=", $";
	c+=reg2;
	AsmGenerator::add_instruction(c);	
	AsmGenerator::add_instruction(string("bc1t "+temp_label));
	AsmGenerator::li(dest_reg,1);	
	AsmGenerator::add_label(temp_label);
	temp_label_count++;
}

void AsmGenerator::add_instruction(string instruction)
{
	instruction += "\n";
	*current_stream << instruction;	
}

void AsmGenerator::add_data(string data_instruction)
{
	data_instruction += "\n";
	data_stream << data_instruction;
}

void AsmGenerator::comment(string comment_message)
{
	stringstream cmt(comment_message);
	string line;
	while (getline(cmt, line))
	{
		string c = "#-----------";
		c+= line;
		AsmGenerator::add_instruction(c);
	}
}

void AsmGenerator::system_call(int system_call_code)
{
	AsmGenerator::li("v0",system_call_code);
	*current_stream << "syscall\n";
}

void AsmGenerator::sbrk (string amount_reg,string returned_address_memory)
{
	AsmGenerator::move("a0",amount_reg);
	AsmGenerator::system_call(9);
	AsmGenerator::move(returned_address_memory,"v0");
}

void AsmGenerator::print_string(string reg_string_address)
{
	AsmGenerator::move("a0",reg_string_address);
	AsmGenerator::system_call(4);
}

void AsmGenerator::print_int(int printed_int)
{
	AsmGenerator::li("a0",printed_int);
	AsmGenerator::system_call(1);
}

void AsmGenerator::print_reg(string reg)
{
	AsmGenerator::move("a0",reg);
	AsmGenerator::system_call(1);
}

void AsmGenerator::f_print_reg(string reg)
{
	AsmGenerator::f_move("f12",reg);
	AsmGenerator::system_call(2);
}

void AsmGenerator::move(string dest_reg,string source_reg)
{
	string c = "move $";
	c+=dest_reg;
	c+=",$";
	c+=source_reg;
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::f_move(string dest_reg,string source_reg)
{
	string c = "mov.s $";
	c+=dest_reg;
	c+=",$";
	c+=source_reg;
	AsmGenerator::add_instruction(c);
}

/*
*	save $fp
*	save $ra
*	reserve space for local variables
*/
void AsmGenerator::function_prologue (int frame_size)
{
	AsmGenerator::comment("Callee Prologue:");
	AsmGenerator::add_instruction("\tsubu $sp, $sp, 12");
	AsmGenerator::add_instruction("\tsw $fp, 0($sp)");
	AsmGenerator::add_instruction("\tsw $ra, 4($sp)");
	AsmGenerator::add_instruction("\tsw $a3, 8($sp)");
	AsmGenerator::add_instruction("\taddi $fp, $sp, 12");
	if (frame_size > 0 ) {
		AsmGenerator::comment("# Reserve space (" +  to_string(frame_size) + "b) for function local vars.");
		AsmGenerator::add_instruction("\tsubu $sp, $sp, " + to_string(frame_size));
	}
	AsmGenerator::add_instruction("");

	
}

void AsmGenerator::function_epilogue(int frame_size)
{
	AsmGenerator::comment("Callee Epilogue:");
	if (frame_size > 0) {
		AsmGenerator::add_instruction("\taddu $sp, $sp, "+to_string(frame_size));
	}
	AsmGenerator::add_instruction("\tlw $a3, 8($sp)");
	AsmGenerator::add_instruction("\tlw $ra, 4($sp)");
	AsmGenerator::add_instruction("\tlw $fp, 0($sp)");
	AsmGenerator::add_instruction("\taddu $sp, $sp, 12" );
	AsmGenerator::add_instruction("");
}

void AsmGenerator::strconcat()
{
	AsmGenerator::initialize_function(strconcat_functoion_name);
		//arguments: $a0 address of first string
		//			 $a1 address of second string
		//			 $a2 address of third string to write the result on it
		//return nothing
		AsmGenerator::add_label("strconcat_copyFirst");
		AsmGenerator::add_instruction("lb $t0 0($a0)");
		AsmGenerator::add_instruction("beq  $t0 $0 strconcat_copySecond");
		AsmGenerator::add_instruction("sb   $t0 0($a2)");
		AsmGenerator::add_instruction("addi $a0 $a0 1");
		AsmGenerator::add_instruction("addi $a2 $a2 1");
		AsmGenerator::add_instruction("b strconcat_copyFirst");
		AsmGenerator::add_label("strconcat_copySecond");
		AsmGenerator::add_instruction("lb   $t0 0($a1)");
		AsmGenerator::add_instruction("beq  $t0 $0 exit_strcpy");
		AsmGenerator::add_instruction("sb   $t0 0($a2)");
		AsmGenerator::add_instruction("addi $a1 $a1 1");
		AsmGenerator::add_instruction("addi $a2 $a2 1");
		AsmGenerator::add_instruction("b strconcat_copySecond");
		AsmGenerator::add_label("exit_strconcat");
		AsmGenerator::add_instruction("sb $0 0($a2)");
	AsmGenerator::write_function();
}

void AsmGenerator::strlen()
{
	AsmGenerator::initialize_function(strlen_functoion_name);
		//arguments: $a0 address of string
		//return length of string in $v1
		AsmGenerator::add_instruction("li $t0, 0");
		AsmGenerator::add_label("strlen_loop");
		AsmGenerator::add_instruction("lb $t1, 0($a0)");
		AsmGenerator::add_instruction("beqz $t1, exit_strlen");
		AsmGenerator::add_instruction("addi $a0, $a0, 1");
		AsmGenerator::add_instruction("addi $t0, $t0, 1");
		AsmGenerator::jump_label("strlen_loop");
		AsmGenerator::add_label("exit_strlen");
		AsmGenerator::move("v1","t0");
	AsmGenerator::write_function();
}

void AsmGenerator::int_to_asci()
{
	//https://www.daniweb.com/programming/software-development/code/435631/integer-to-string-in-mips-assembly
	// arguments: $a0 - integer to convert $a1 - character buffer to write to
	// return: number of characters in converted string
	AsmGenerator::initialize_function(int_to_asci_functoion_name);
	AsmGenerator::add_instruction("bnez $a0, ItoA.non_zero");
	AsmGenerator::add_instruction("nop");
	AsmGenerator::add_instruction("li   $t0, '0'");
	AsmGenerator::add_instruction("sb   $t0, 0($a1)");
	AsmGenerator::add_instruction("sb   $zero, 1($a1)");
	AsmGenerator::add_instruction("li   $v0, 1");
	AsmGenerator::add_instruction("jr   $ra");
	AsmGenerator::add_instruction("ItoA.non_zero:");
	AsmGenerator::add_instruction("addi $t0, $zero, 10");
	AsmGenerator::add_instruction("li $v0, 0");
	AsmGenerator::add_instruction("bgtz $a0, ItoA.recurse");
	AsmGenerator::add_instruction("nop");
	AsmGenerator::add_instruction("li   $t1, '-'");
	AsmGenerator::add_instruction("sb   $t1, 0($a1)");
	AsmGenerator::add_instruction("addi $v0, $v0, 1");
	AsmGenerator::add_instruction("neg  $a0, $a0");
	AsmGenerator::add_instruction("ItoA.recurse:");
	AsmGenerator::add_instruction("addi $sp, $sp, -24");
	AsmGenerator::add_instruction("sw   $fp, 8($sp)");
	AsmGenerator::add_instruction("addi $fp, $sp, 8");
	AsmGenerator::add_instruction("sw   $a0, 4($fp)");
	AsmGenerator::add_instruction("sw   $a1, 8($fp)");
	AsmGenerator::add_instruction("sw   $ra, -4($fp)");
	AsmGenerator::add_instruction("sw   $s0, -8($fp)");
	AsmGenerator::add_instruction("sw   $s1, -12($fp)");
	AsmGenerator::add_instruction("div  $a0, $t0");
	AsmGenerator::add_instruction("mflo $s0");
	AsmGenerator::add_instruction("mfhi $s1");
	AsmGenerator::add_instruction("beqz $s0, ItoA.write");
	AsmGenerator::add_instruction("ItoA.continue:");
	AsmGenerator::add_instruction("move $a0, $s0");
	AsmGenerator::add_instruction("jal ItoA.recurse");
	AsmGenerator::add_instruction("nop");
	AsmGenerator::add_instruction("ItoA.write:");
	AsmGenerator::add_instruction("add  $t1, $a1, $v0");
	AsmGenerator::add_instruction("addi $v0, $v0, 1  ");
	AsmGenerator::add_instruction("addi $t2, $s1, 0x30");
	AsmGenerator::add_instruction("sb   $t2, 0($t1)");
	AsmGenerator::add_instruction("sb   $zero, 1($t1)");
	AsmGenerator::add_instruction("ItoA.exit:");
	AsmGenerator::add_instruction("lw   $a1, 8($fp)");
	AsmGenerator::add_instruction("lw   $a0, 4($fp)");
	AsmGenerator::add_instruction("lw   $ra, -4($fp)");
	AsmGenerator::add_instruction("lw   $s0, -8($fp)");
	AsmGenerator::add_instruction("lw   $s1, -12($fp)");
	AsmGenerator::add_instruction("lw   $fp, 8($sp) ");
	AsmGenerator::add_instruction("addi $sp, $sp, 24");
	AsmGenerator::write_function();
}

void AsmGenerator::strcpy()
{
	AsmGenerator::initialize_function(strcpy_function_name);
		//argument $a0 address of dest string
		//		   $a1 address of source string
		// return nothing
		AsmGenerator::add_instruction("add  $t0, $zero, $zero");
		AsmGenerator::add_label("strcpy_loop");
		AsmGenerator::add_instruction("add  $t1, $a1, $t0");
		AsmGenerator::add_instruction("lb   $t2, 0($t1)");
		AsmGenerator::add_instruction("beq  $t2, $zero, exit_strcpy");
		AsmGenerator::add_instruction("add  $t3, $a0, $t0");
		AsmGenerator::add_instruction("sb   $t2, 0($t3)");
		AsmGenerator::add_instruction("addi $t0, $t0, 1");
		AsmGenerator::add_instruction("j strcpy_loop");
		AsmGenerator::add_label("exit_strcpy");
		AsmGenerator::add_instruction("add  $t3, $a0, $t0");
		AsmGenerator::add_instruction("sb   $zero, 0($t3)");
		AsmGenerator::add_instruction("");
	AsmGenerator::write_function();
}

ofstream AsmGenerator::assembly_code_file;
stringstream AsmGenerator::data_stream;
stringstream AsmGenerator::main_stream;
stringstream AsmGenerator::functions_stream;

stringstream *AsmGenerator::current_stream = new stringstream();

map<string,int> AsmGenerator::strings_map;

int AsmGenerator::currentStreamID			= 0;
int AsmGenerator::temp_label_count			= 0;
int AsmGenerator::floats_count				= 0;
int AsmGenerator::strings_count				= 0;
int AsmGenerator::if_temp_label_count		= 0;
int AsmGenerator::else_temp_label_count		= 0;

string AsmGenerator::strconcat_functoion_name		= "strconcat";
string AsmGenerator::strlen_functoion_name			= "strlen";
string AsmGenerator::int_to_asci_functoion_name		= "ItoA";
string AsmGenerator::strcpy_function_name			= "strcpy";

string AsmGenerator::global_label					= "global_";
string AsmGenerator::new_line_address				= "new_line";
string AsmGenerator::empty_string_address			= "empty_string";