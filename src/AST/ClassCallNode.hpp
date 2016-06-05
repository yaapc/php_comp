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

	ClassCallNode(Node* objectNode, string propertyString, int line, int col);

	ClassCallNode(Node* objectNode, string propertyString, Node* argumentsList, int line ,int col);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);
	virtual void accept(CheckerVisitor *, TypeExpression* context);

	virtual bool type_checking();

	/*
	*	generates a signature of the function called.
	*	the default structure of our Function Signature is as follows:
	*  func_$function_name$($type_name$,$type_name$...)
	*/
	string generateCallSignature();

};
