#pragma once

#include "Node.hpp"
#include <iostream>


typedef struct Values
{
	int		type;
	int		int_val;
	float	float_val;
	bool	bool_val;
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

	ScalarNode(int i);
	ScalarNode(float f);
	ScalarNode(bool b);
	ScalarNode(string s);

  virtual void print(ostream &os);

  virtual void generate_code();

  virtual bool type_checking();

  virtual TypeExpression* getNodeType();

};
