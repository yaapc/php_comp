#pragma once

#include "Node.hpp"
#include <vector>
#include <iostream>

using namespace std;

class ListNode : public Node {
public:
  vector<Node*> nodes;

  void add_node(Node* node) {
    nodes.push_back(node);
  }

  void add_nodes(const vector<Node*>& nodes_list) {
    for (auto &node : nodes_list) {
      add_node(node);
    }
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
};
