#include "AsmGenerator.h"
#include <string> 
void AsmGenerator::initialize_file(){
	assembly_code_file.open("src/Code Generator/AssemblyCode.asm");
	data << ".data\n";
	text << ".text\n" << ".globl main\n" << "main:\n\n";
}

void AsmGenerator::write_data(){
	assembly_code_file << data.str();
}

void AsmGenerator::write_text(){
	assembly_code_file << text.str();
}

void AsmGenerator::generate_code_file(){
	write_data();
	write_text();
	assembly_code_file.close();
}

void AsmGenerator::add_instruction(string instruction){
	text << instruction;
}

void AsmGenerator::li(string reg,int value){
	string c="li $";
	c+=reg;
	c+=",";
	c+=to_string(value);
	c+="\n";
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::push(string source_register){
	AsmGenerator::add_instruction("sub $sp,$sp,4\n");
	string c="sw $";
	c+=source_register;
	c+=", 0($sp)\n";
	AsmGenerator::add_instruction(c);
}

void AsmGenerator::pop(string destination_register){
	string c = "lw $";
	c += destination_register;
	c += ", 0($sp)\n";
	AsmGenerator::add_instruction(c);
	AsmGenerator::add_instruction("add $sp,$sp,4\n");
}

void AsmGenerator::binary_operation(string reg1,string reg2,string reg3,int operation)
{
	//
	// 1 : +  2:- 3:* 4:/ 5:%
	//
	string c="";

	if(operation == 1)
		c+="add $";
	else if(operation == 2)
		c+="sub $";

	else if(operation == 3)
		c+="mul $";

	else if(operation == 4)
		c+="div $";

	else if(operation == 5)
		c+="rem $";

	c+=reg1;
	c+=",$";

	c+=reg2;
	c+=",$";

	c+=reg3;
	c+="\n";
	AsmGenerator::add_instruction(c);

}

void AsmGenerator::comment(string comment_message)
{
	stringstream cmt(comment_message);
	string line;
	while (getline(cmt, line))
	{
		string c = " #";
		c += line + "\n";
		AsmGenerator::add_instruction(c);
	}
}

void AsmGenerator::system_call(int system_call_code)
{
	AsmGenerator::li("v0",system_call_code);
	text << "syscall\n";
}

void AsmGenerator::print_sring(int string_label)
{
	AsmGenerator::li("a0",string_label);
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


void AsmGenerator::move(string dest_reg,string source_reg)
{
	string c = "move $";
	c+=dest_reg;
	c+=",$";
	c+=source_reg;
	c+="\n";
	AsmGenerator::add_instruction(c);
}

ofstream AsmGenerator::assembly_code_file;
stringstream AsmGenerator::text;
stringstream AsmGenerator::data;

