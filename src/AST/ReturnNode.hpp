#pragma once

#include "Node.hpp"
#include <iostream>

class ReturnNode : public Node {
public:
	Node* returned_node;

	ReturnNode(Node *exp);

	virtual void print(ostream &os);

	virtual bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

};
