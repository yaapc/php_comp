#pragma once 

#include "ClassDefineNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"

ClassDefineNode::ClassDefineNode(Symbol* classSym, Node* classBody) {
	this->classSymbol = dynamic_cast<Class*>(classSym);
	this->body = dynamic_cast<ListNode*>(classBody);	
	this->nodeType = nullptr;
	this->classMemNodes = nullptr;
	this->classMethodsNodes = nullptr;
	//now extract info
	this->extractInfo();
}

/*
 * this method is responsable for extracting class information from its methods and members
 */
void ClassDefineNode::extractInfo() {
	//TODO
}

bool ClassDefineNode::type_checking() {
	//TODO
	this->nodeType = new TypeError("Not Implmeneted, yet");
	return false;
}

void ClassDefineNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
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
