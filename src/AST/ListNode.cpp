#pragma once

#include "ListNode.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/AsmGenerator.h"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include "AST_Visitors\CheckerVisitor.hpp"
#include "../TypeSystem/TypesTable.h"

ListNode::ListNode()
{
	nodeType = nullptr;
}

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
	  if(node != nullptr && dynamic_cast<TypeError*>(node->getNodeType()) != nullptr)
			node->type_checking();
    }
	this->nodeType = TypesTable::getInstance()->getType(VOID_TYPE_ID);
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

Node* ListNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void ListNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}

void ListNode::accept(CheckerVisitor* visitor, TypeExpression* context) {
	visitor->visit(this, context);
}