#pragma once 

#include "ClassDefineNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"

ClassDefineNode::ClassDefineNode(Symbol* classSym, Node* classBody) {
	this->classSymbol = dynamic_cast<Class*>(classSym);
	this->body = dynamic_cast<ListNode*>(classBody);	
	this->nodeType = nullptr;
	this->classMethodsNodes = nullptr;
	//now extract info
	this->extractInfo();
}

/*
 * this method is responsable for extracting class information from its methods and members
 */
void ClassDefineNode::extractInfo() {
	//TODO
	//for now let's extract members types and nodes

	for (auto node : this->body->nodes) {
		ClassMemNode* memNode = dynamic_cast<ClassMemNode*>(node);
		if (node != nullptr)
			this->classMemNodes.push_back(memNode);
	}

	for (auto memberNode : this->classMemNodes) {
		this->classMemTE.push_back(memberNode->getNodeType());
	}
}

bool ClassDefineNode::type_checking() {
	this->nodeType = TypesTable::getInstance()->buildClassType(this, this->classSymbol);
	return true;
}

void ClassDefineNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* ClassDefineNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

void ClassDefineNode::print(ostream &os) {
	int self = int(this);
	string classLabel = "define: ";
	classLabel.append(classSymbol ? classSymbol->getName() : "!!!");

	os << int(this)
		<< "[label = \""
		<< classLabel
		<< "\"]"
		<< endl;

	os << self << "->" << int(body) << endl;
	if (body) body->print(os);

}
