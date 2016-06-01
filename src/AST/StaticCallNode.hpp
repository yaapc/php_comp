#pragma once
#include "Node.hpp"
#include <string>
#include <iostream>
#include "../TypeSystem/TypeClass.hpp"

class StaticCallNode : public Node {
public:
	string className;
	string propName;
	PropertyWrapper* propWrapper;

	StaticCallNode(string className, string propName);

	virtual void print(ostream &os);

	virtual bool type_checking();
	
	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

};