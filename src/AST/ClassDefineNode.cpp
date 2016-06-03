#pragma once 

#include "ClassDefineNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"

ClassDefineNode::ClassDefineNode(Symbol* classSym, Node* classBody, int line, int col) {
	this->classSymbol = dynamic_cast<Class*>(classSym);
	this->body = dynamic_cast<ListNode*>(classBody);	
	this->nodeType = nullptr;
	this->line = line;
	this->col = col;
}

/*
 * this method is responsable for extracting class information from its methods and members
 */
void ClassDefineNode::extractInfo() {
	//TODO
	//for now let's extract members types and nodes

	for (auto node : this->body->nodes) {
		ClassMemNode* memNode = dynamic_cast<ClassMemNode*>(node);
		if (memNode != nullptr)
			this->classMemNodes.push_back(memNode);
		else {
			ClassMethodNode* methodNode = dynamic_cast<ClassMethodNode*>(node);
			if (methodNode != nullptr)
				this->classMethodsNodes.push_back(methodNode);
		}
	}

	for (auto memberNode : this->classMemNodes) {
		this->classMemTE.push_back(memberNode->getNodeType());
	}
}

bool ClassDefineNode::type_checking() {
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		this->body->type_checking();
		return true; // pass it this time
	}
	//if this is the first time to type_checking
	if(this->nodeType == nullptr)
		this->extractInfo();//extract info

	this->nodeType = TypesTable::getInstance()->buildClassType(this, this->classSymbol);
	this->body->type_checking();//called implicity in buildClassType
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


void ClassDefineNode::accept(TypeErrorVisitor* typeVisitor) {
	typeVisitor->visit(this);
}