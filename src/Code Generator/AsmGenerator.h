#pragma once
#ifndef GODEGENERATOR_H
#define GODEGENERATOR_H
#include <iostream>
#include <cstring>

#include <fstream>
#include <sstream>


using namespace std;

class AsmGenerator{
private:
	static ofstream assembly_code_file;
	static stringstream text;
	static stringstream data;

	static int temp_label_count;
	static int temp_float_count;

public:
	static void initialize_file(); 
	static void write_data();
    static void write_text();
	static void generate_code_file();

	static string store_float				(float value);

	static void li							(string reg,int value);

	static void f_li						(string reg,float value);

	static void add_label					(string label_name);

	static void jump_label					(string label_name);

	static void push						(string source_register);
	static void f_push						(string source_register);

	static void pop							(string destination_register);
	static void f_pop						(string destination_register);

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

	static void comment						(string comment_meesage);

	static void system_call					(int systam_call_code);

	static void print_sring					(int string_label);

	static void print_int					(int printed_int);

	static void print_reg					(string reg);
	static void f_print_reg					(string reg);

	static void move						(string dest_reg ,string source_reg);
	static void f_move						(string dest_reg ,string source_reg);
};



#endif