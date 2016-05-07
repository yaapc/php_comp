#pragma once

#include "Node.hpp"
#include <vector>
#include <iostream>

using namespace std;

class ListNode : public Node {
public:
  vector<Node*> nodes;

  ListNode* add_node(Node* node);

  ListNode* add_nodes(const vector<Node*>& nodes_list);

  virtual bool type_checking();

  virtual void print(ostream& os);

  virtual void generate_code();

  static ListNode* joinNodeLists(Node* f, Node* s);

  virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);
};
