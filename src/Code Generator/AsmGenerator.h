#pragma once
#ifndef GODEGENERATOR_H
#define GODEGENERATOR_H
#include <iostream>
#include <cstring>

#include <fstream>
#include <sstream>
#include <map>

#define MAIN_STREAM 0
#define FUNCUTION_STREAM 1

using namespace std;

class AsmGenerator{
private:
	static ofstream assembly_code_file;
	static stringstream data_stream;
	static stringstream main_stream;
	static stringstream functions_stream;

	static int current_stream;
	static int temp_label_count;
	static int floats_count;
	static int strings_count;

	static map<string,int> strings_map;

	

public:
	static void initialize_file(); 
	static void write_file();

	static void initialize_data();
	static void write_data();
	
	static void initialize_main();
    static void write_main();
	
	static void initialize_function(string function_name);
	static void write_function();

	static void write_functions();
	

	static string store_float				(float value);
	static string store_string				(string value);
	static string store_string_empty		();

	static void li							(string reg,int value);
	static void f_li						(string reg,float value);

	static void la							(string reg,string value);

	static void add_label					(string label_name);

	static void jump_label					(string label_name);
	
	static void jr							(string reg);

	static void jal							(string function_name);

	static void push						(string source_register);
	static void f_push						(string source_register);

	static void pop							(string destination_register);
	static void f_pop						(string destination_register);

	static void beq							(string reg1,string reg2,string label);

	static void binary_operation			(string dest_reg,string reg1,string reg2,int operation);
	static void f_binary_operation			(string dest_reg,string reg1,string reg2,int operation);

	static void equal_operation				(string dest_reg,string reg1,string reg2,bool is_not_equal_opration);
	static void f_equal_operation			(string dest_reg,string reg1,string reg2,bool is_not_equal_opration);

	static void less_than_operation			(string dest_reg,string reg1,string reg2);
	static void f_less_than_operation		(string dest_reg,string reg1,string reg2);
	static void f_greater_than_operation	(string dest_reg,string reg1,string reg2);

	static void greater_or_equal_operation		(string dest_reg,string reg1,string reg2);
	static void f_greater_or_equal_operation	(string dest_reg,string reg1,string reg2);
	static void f_less_or_equal_operation		(string dest_reg,string reg1,string reg2);

	static void add_instruction				(string instruction);

	static void add_data					(string data_instruction);

	static void add_function				(string function_name,bool is_main);
	
	static void comment						(string comment_meesage);

	// System Calls
	static void system_call					(int systam_call_code);

	static void sbrk						(string amount_reg,string returned_address_memory);

	static void print_string				(string reg_string_address);

	static void print_int					(int printed_int);

	static void print_reg					(string reg);

	static void f_print_reg					(string reg);

	static void move						(string dest_reg ,string source_reg);
	static void f_move						(string dest_reg ,string source_reg);
		
	
	static int if_temp_label_count;
	static int else_temp_label_count;

	//pre defined functions

	static string strcpy_functoion_name;
	static void strcpy				();

	static string strlen_functoion_name;
	static void strlen				();	
};



#endif