#include "NewNode.hpp"
#include "../TypeSystem/TypesTable.h"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
#include "AST_Visitors\TypeErrorVisitor.hpp"
#include <sstream>

NewNode::NewNode(Node* args,string className, int line, int col) {
	this->className		= className;
	this->argumentsList	= dynamic_cast<ListNode*>(args);
	this->nodeType	= nullptr;
	this->line = line;
	this->col = col;
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
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		return true; // pass it this time
	}

	//check if a class is available:
	this->nodeType = TypesTable::getInstance()->getClassType(className);

	if (this->nodeType == nullptr) { // no type found
		this->nodeType = new TypeError("Undefined Class " + this->className + string(", line:") + to_string(this->line) + string(",col:") + to_string(this->col));
		return false;
	}
	//class found, but does it have the given constructor?
	this->nodeType = this->nodeType->opDot(this->className, true, this->generateCallSignature(), this->constructorWr, nullptr);
	return true;
}

void NewNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}

Node* NewNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}

 TypeExpression* NewNode::getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
}

 string NewNode::generateCallSignature() {
	 std::ostringstream os;
	 bool firstParamFlag = true;
	 os << "func_" << this->className << "(";
	 for (auto &param : this->argumentsList->nodes) {
		 if (!firstParamFlag)
			 os << ",";
		 os << TypeSystemHelper::getTypeName(param->getNodeType()->getTypeId());
		 firstParamFlag = false;
	 }
	 os << ")";
	 return os.str();
 }

 void NewNode::accept(TypeErrorVisitor* typeVisitor) {
	 typeVisitor->visit(this);
 }