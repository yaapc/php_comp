#pragma once

#include "Node.hpp"
#include <iostream>

class ElseNode : public Node {
public:
	Node *body;

	ElseNode(Node *body, int line, int col);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);

	
	virtual bool type_checking();
};