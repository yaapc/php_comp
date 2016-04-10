#pragma once

#include "Node.hpp"
#include <vector>
#include <iostream>
#include "../Code Generator/AsmGenerator.h"

using namespace std;

class ListNode : public Node {
public:
  vector<Node*> nodes;

  ListNode* add_node(Node* node) {
    nodes.push_back(node);
    return this;
  }

  ListNode* add_nodes(const vector<Node*>& nodes_list) {
    for (auto &node : nodes_list) {
      add_node(node);
    }
    return this;
  }

  virtual bool type_checking() {
    bool result = true;
    for (auto node : nodes) {
      result = result &&  node->type_checking();
    }
    return result;
  }

  virtual void print(ostream& os) {
    int self = (int)this;
    os << self << "[label = \"Statements\"]"<<endl;
    for (auto &node : nodes) {
      if (node == nullptr) continue;
      node->print(os);
      os << self << "->" << (int)node << endl;
    }
  }

   void generate_code(){
	AsmGenerator::initialize_file();
	for (auto &node : nodes) {
		if (node == nullptr) continue;
		node->generate_code();
	}
	AsmGenerator::generate_code_file();
  }
};
