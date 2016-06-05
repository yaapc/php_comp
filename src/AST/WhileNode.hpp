#pragma once

#include "Node.hpp"
#include <iostream>

class WhileNode : public Node {
public:
	Node *condition;
	Node *body;

	WhileNode(Node *condition, Node *body, int line, int col);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);
	virtual void accept(CheckerVisitor*, TypeExpression*);

	virtual bool type_checking();
};
