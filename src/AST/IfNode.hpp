#pragma once

#include "ElseNode.hpp"
#include <iostream>

using namespace std;

class IfNode : public Node {
public:
	Node *condition, *body;
	Node *else_node;

	IfNode(Node *cond, Node *bod, Node *el);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);


	virtual bool type_checking();

	virtual TypeExpression* getNodeType();
  
  
   static IfNode* find_deepest_if(Node *root) {
	IfNode *deepest_if = dynamic_cast<IfNode*>(root);
		while (deepest_if->else_node) 
		{
			ElseNode* elseNode = dynamic_cast<ElseNode*>(deepest_if->else_node);
			deepest_if = dynamic_cast<IfNode*>(elseNode->body);
		}
	return deepest_if;
	}
};



