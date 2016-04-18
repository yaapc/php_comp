#pragma once

#include "Node.hpp"
#include <iostream>
#include "../Code Generator/AsmGenerator.h"

typedef struct Values
{
	int		type;
	int		int_val;
	float	float_val;
	bool	bool_val;
	char	char_val;
	string  string_val;

	void print(ostream &os){
	switch (type)
		{
		case 1:
			os << int_val;
			break;
		case 2:
			os << float_val;
			break;
		case 3:
			os << bool_val;
			break;
		case 4:
			os << char_val;
			break;
		case 5:
			os << string_val;
			break;			
		}
	}
	string to_string(){
	switch (type)
		{
		case 1:
			return std::to_string(int_val);
			break;
		case 2:
			return std::to_string(float_val);
			break;
		case 3:
			return std::to_string(bool_val);
			break;
		case 4:
			return std::to_string(char_val);
			break;
		case 5:
			return string_val;
			break;
		}
	}
};

class ScalarNode : public Node {

protected:
	Values  value;
public:
  ScalarNode(int i) {
	value.type = 1;
	value.int_val = i;
  }
  ScalarNode(float f) {
	value.type = 2;
	value.float_val = f;
  }

   ScalarNode(bool b) {
	value.type = 3;
	value.bool_val = b;
  }
   ScalarNode(char c) {
	value.type = 4;
	value.char_val = c;
  }
   ScalarNode(string s) {
	value.type = 5;
	value.string_val = s;
  }


  virtual void print(ostream &os) {
    os << int(this)
       << "[label=\"";
	value.print(os);
       os<<"\"]"
       << endl;
  }
  void generate_code(){
	astLog.log("generate_code ScalarNode ("+value.to_string()+")");
	AsmGenerator::comment("Scaler Node:");
	switch (value.type)
		{
		case 1:
			AsmGenerator::li("t0",value.int_val);
			AsmGenerator::push("t0");
			break;
		case 2:
			AsmGenerator::f_li("f0",value.float_val);
			AsmGenerator::f_push("f0");
			break;
		case 3:
			AsmGenerator::li("t0",value.bool_val);
			AsmGenerator::push("t0");
			break;
		case 4:
			AsmGenerator::li("t0",value.char_val);
			AsmGenerator::push("t0");
			break;
		case 5:
			AsmGenerator::la("t0",AsmGenerator::store_string(value.string_val));
			AsmGenerator::push("t0");
			break;
		}
	AsmGenerator::comment("Scaler Node.");
	}



};
