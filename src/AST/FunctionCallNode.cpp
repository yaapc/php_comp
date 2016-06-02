#pragma once

#include "FunctionCallNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../Code Generator/OptimizationVistor.hpp"
//#include "../TypeSystem/TypeFunction.hpp"
#include <sstream>

FunctionCallNode::FunctionCallNode(string name, Node* argsList) {
	this->nodeType = nullptr;
	this->functionType = nullptr;
	this->name = name;
	this->argumentsList = dynamic_cast<ListNode*>(argsList);
}

void FunctionCallNode::print(ostream &os) {
	int self = int(this);
	os << self << "[label = \""
		<< name << "() Call"
		<< "\"]"
		<< endl;
	os << self << "->" << int(argumentsList) << endl;
	if (argumentsList) argumentsList->print(os);
}

bool FunctionCallNode::type_checking() {
	if (this->nodeType != nullptr && dynamic_cast<TypeError*>(this->nodeType) == nullptr) {
		//this for second passes, if the current node is free of TypeError no need to re type_check it
		return true; // pass it this time
	}

	TypeExpression* type = TypeFunction::getInstance(this->generateCallSignature(), this);
	TypeFunction* functionType = dynamic_cast<TypeFunction*>(type);
	if (functionType != nullptr) {
		this->nodeType = functionType->getReturnTypeExpression();
		this->functionType = functionType;
		return true;
	}
	this->nodeType = type;// TypeError indecating undefind.
	return false;
}

void FunctionCallNode::generate_code(CodeGneratorVistor *codeGneratorVistor)
{
	codeGneratorVistor->visit(this);
}
string FunctionCallNode::generateCallSignature() {
	std::ostringstream os;
	bool firstParamFlag = true;
	os << "func_" << this->name << "(";
	for (auto &param : this->argumentsList->nodes) {
		if (!firstParamFlag)
			os << ",";		
		os << TypeSystemHelper::getTypeName(param->getNodeType()->getTypeId());
		firstParamFlag = false;
	}
	os << ")";
	return os.str();
}

Node* FunctionCallNode::optmize(OptimizationVistor *optimizationVistor)
{
	return optimizationVistor->visit(this);
}
