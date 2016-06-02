#pragma once 
#include "Node.hpp"
#include "ListNode.hpp"
//#include "../TypeSystem/TypeFunction.hpp"


class TypeFunction;
class FunctionCallNode : public Node {
public:
	string name;
	Node* argumentsList;
	TypeFunction* functionType;


	FunctionCallNode(string name, Node* argsList, int line, int col);

	void print(ostream &os);

	bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	/*
	 *	generates a signature of the function called.
	 *	the default structure of our Function Signature is as follow:
	 *  func_$function_name$($type_name$,$type_name$...)
	 */
	string generateCallSignature();

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);

};
