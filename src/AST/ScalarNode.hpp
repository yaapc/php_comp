#pragma once

#include "Node.hpp"
#include <iostream>


struct Value
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

public:
	Value  value;
	ScalarNode(int i, int line, int col);
	ScalarNode(float f, int line, int col);
	ScalarNode(bool b, int line, int col);
	ScalarNode(string s, int line, int col);

	virtual void print(ostream &os);

	virtual bool type_checking();

	virtual TypeExpression* getNodeType();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);


};
