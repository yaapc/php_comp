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



public:
	static void initialize_file(); 
	static void write_data();
    static void write_text();
	static void generate_code_file();

	static void li		(string register,int value);


	static void push	(string source_register);
	static void pop		(string destination_register);

	static void binary_operation(string reg1,string reg2,string reg3,int operation);

	static void add_instruction(string instruction);

	static void comment(string comment_meesage);

	static void system_call(int systam_call_code);

	static void print_sring (int string_label);

	static void print_int (int printed_int);

	static void print_reg (string reg);

	static void move(string dest_reg ,string source_reg);
};



#endif