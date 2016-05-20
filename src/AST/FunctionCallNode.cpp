#pragma once

#include "FunctionCallNode.hpp"
#include "../TypeSystem/TypeError.hpp"
#include "../Code Generator/CodeGeneratorVistor.hpp"
#include "../TypeSystem/TypeFunction.hpp"
#include <sstream>

FunctionCallNode::FunctionCallNode(string name, Node* argsList) {
	this->nodeType = nullptr;
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
	TypeExpression* type = TypeFunction::getInstance(this->generateCallSignature());
	TypeFunction* functionType = dynamic_cast<TypeFunction*>(type);
	if (functionType != nullptr) {
		this->nodeType = functionType->getReturnTypeExpression();
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

