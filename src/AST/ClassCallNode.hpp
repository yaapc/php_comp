#pragma once

#include "Node.hpp"
#include "VariableNode.hpp"
#include "../TypeSystem/TypeClass.hpp"


class ClassCallNode : public Node {
public:
	VariableNode* object;
	string propertyString;
	bool isMethodCall;
	MemberWrapper* member;
	ListNode* argumentsList;

	ClassCallNode(Node* objectNode, string propertyString);

	ClassCallNode(Node* objectNode, string propertyString, Node* argumentsList);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual bool type_checking();

	/*
	*	generates a signature of the function called.
	*	the default structure of our Function Signature is as follow:
	*  func_$function_name$($type_name$,$type_name$...)
	*/
	string generateCallSignature();

};
