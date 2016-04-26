#pragma once

#include "ListNode.hpp"

#include "../Code Generator/AsmGenerator.h"
ListNode* ListNode::add_node(Node* node) {
    nodes.push_back(node);
    return this;
  }

  ListNode* ListNode::add_nodes(const vector<Node*>& nodes_list) {
    for (auto &node : nodes_list) {
      add_node(node);
    }
    return this;
  }

 bool ListNode::type_checking() {
    bool result = true;
    for (auto node : nodes) {
      //result = result &&  node->type_checking();
		node->type_checking();
    }
    return result;
  }

void ListNode::print(ostream& os) {
    int self = (int)this;
    os << self << "[label = \"Statements\"]"<<endl;
    for (auto &node : nodes) {
      if (node == nullptr) continue;
      node->print(os);
      os << self << "->" << (int)node << endl;
    }
  }

 void ListNode::generate_code(){
	for (auto &node : nodes) {
		if (node == nullptr) continue;
		node->generate_code();
	}
  }