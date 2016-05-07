#pragma once

#include "ListNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"

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
	  //TODO: remove the condition when all nodes has implemented type_check()
	  if(node != nullptr)
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

 ListNode* ListNode::joinNodeLists(Node* f, Node* s) {
	 ListNode* fl = dynamic_cast<ListNode*>(f);
	 ListNode* sl = dynamic_cast<ListNode*>(s);
	 for (auto &node : sl->nodes) {
		 if (node == nullptr) continue;
		 fl->nodes.push_back(node);
	 }
	 return fl;
 }

 void ListNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}