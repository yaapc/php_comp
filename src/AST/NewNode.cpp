#include "NewNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"


NewNode::NewNode(Node* args,string className) {
	this->className		= className;
	this->argumentsList	= dynamic_cast<ListNode*>(args);
	this->nodeType	= nullptr;
 }

void NewNode::print(ostream &os) {
    int self = int(this);
    os << self << "[label = \""
       << "New " << className 
       << "\"]"
       <<endl;
	os << self << "->" << int(argumentsList) << endl;
	//if (this->paramsList) paramsList->print(os);
	//overriding vertual ListNode print method:
	self = (int)argumentsList;
	os << self << "[label = \"Params\"]" << endl;
	for (auto &node : argumentsList->nodes) {
		if (node == nullptr) continue;
		node->print(os);
		os << self << "->" << (int)node << endl;
	}

 }

bool NewNode::type_checking() {	
	//TODO:
	this->nodeType = new TypeError("Not implemented yet.");
	return false;
}

void NewNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

 TypeExpression* NewNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
}