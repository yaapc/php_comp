#pragma once

#include "Node.hpp"
#include <iostream>

class ForNode : public Node {
public:
	Node *initializer, *condition, *post_statement, *body;

	ForNode(Node *initializer, Node *condition, Node *post_statement, Node *body, int line, int col);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	bool type_checking();

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);
	virtual void accept(CheckerVisitor*, TypeExpression*);

};
