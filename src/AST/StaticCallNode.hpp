#pragma once
#include "Node.hpp"
#include <string>
#include <iostream>
#include "../TypeSystem/TypeClass.hpp"

class StaticCallNode : public Node {
public:
	Node* argumentsList;
	string className;
	string memberName;
	bool isMethodCall;
	MemberWrapper* memberWrapper;

	StaticCallNode(string className, string propName, int line , int col);

	StaticCallNode(string className, string propName,Node *argsList, int line , int col);

	virtual void print(ostream &os);

	virtual bool type_checking();
	
	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);


};