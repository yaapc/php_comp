#include "Node.hpp"
#include "ListNode.hpp"
#include <iostream>

using namespace std;

class NewNode : public Node {
public:
	string className;
	ListNode* argumentsList;  // a list of parameter nodes
	MemberWrapper* constructorWr; // a MethodWrapper represents called constructor 

	NewNode(Node* args,string className);

	virtual bool type_checking();

	virtual void print(ostream& os);

	virtual TypeExpression* getNodeType();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	/*
	*	generates a signature of the constructor called.
	*	the default structure of our Function Signature is as follows:
	*  func_$function_name$($type_name$,$type_name$...)
	*/
	string generateCallSignature();

};