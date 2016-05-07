#pragma once 
#include "Node.hpp"
#include "ListNode.hpp"

class FunctionCallNode : public Node {
public:
	string name;
	ListNode* argumentsList;
	
	FunctionCallNode(string name, Node* argsList);

	void print(ostream &os);

	bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

};
