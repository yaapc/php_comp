#pragma once

#include "ElseNode.hpp"
#include <iostream>

using namespace std;

class IfNode : public Node {
public:
  Node *condition, *body;
  ElseNode *else_node;

  IfNode(Node *cond, Node *bod, Node *el);

  virtual void print(ostream &os);


  
  void generate_code();


  
  
   static IfNode* find_deepest_if(Node *root) {
	IfNode *deepest_if = dynamic_cast<IfNode*>(root);
		while (deepest_if->else_node) {
			deepest_if = dynamic_cast<IfNode*>(deepest_if->else_node->body);
		}
	return deepest_if;
	}
};



