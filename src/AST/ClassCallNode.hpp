#pragma once

#include "Node.hpp"
#include "VariableNode.hpp"

class ClassCallNode : public Node {
public:
	VariableNode* object;
	string propertyString;
	bool isMethodCall;

	ClassCallNode(Node* objectNode, string propertyString);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual bool type_checking();
};
