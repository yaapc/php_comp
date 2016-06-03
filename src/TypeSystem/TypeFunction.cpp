#pragma once

#include "TypeFunction.hpp"
#include "TypesTable.h"
#include "TypeError.hpp"
#include <string>
#include "../AST/FunctionCallNode.hpp"

//static declaration
vector<TypeFunction*> TypeFunction::functionInstances;
vector<FunctionCallNode*> TypeFunction::errorFunctionCalls;

TypeExpression* TypeFunction::buildFunction(FunctionDefineNode* functionNode, Function* functionSymbol) {


	//try to find a TypeFunction from the first signature of the given function symbol.
	//the @TypeFunction::isDeclared() returns false if a name of the function already found, not just unmatched signature.
	//this behaviour can be changed, so we can obtain the functionality of Function Overloading.
	bool isDeclared = TypeFunction::isDeclared(functionNode->functionSym->functionSignatures.at(0));
								   
	
	//if isDeclared, throw a TypeError with already defined
	if (isDeclared)
		return new TypeError(string(functionSymbol->getName())  + "already defined.");

	//let's build the FunctionType:
	TypeFunction* typeFunction = new TypeFunction(functionSymbol->functionSignatures, functionSymbol->getName()
												  ,functionSymbol->getUniqueName());

	//extract params TypeExpressions and add them to @paramsTE
	for (auto &paramNode : functionNode->paramsList->nodes) {
		typeFunction->addToParams(paramNode->getNodeType());
	}   

	//extract return type
	typeFunction->returnType = TypesTable::getInstance()->getType(functionSymbol->getReturnType());
	
	//if return type not found, return a TypeError
	TypeError* errorReturnType = dynamic_cast<TypeError*>(typeFunction->returnType);
	if (errorReturnType != nullptr)
		return new TypeError("return type: " + string(functionSymbol->getReturnType()) + " is undefied.");


	//everything is ok
	//now let's resize according to the new params
	typeFunction->resize();

	//and finally, add it to the FunctionType's we have:
	TypeFunction::functionInstances.push_back(typeFunction);

	return typeFunction;
}

TypeExpression* TypeFunction::buildMethod(ClassMethodNode* methodNode, Method* methodSymbol) {

	//let's build the FunctionType:
	TypeFunction* typeFunction = new TypeFunction(methodSymbol->functionSignatures, methodSymbol->getName()
		, methodSymbol->getUniqueName());

	//extract params TypeExpressions and add them to @paramsTE
	for (auto &paramNode : methodNode->paramsList->nodes) {
		typeFunction->addToParams(paramNode->getNodeType());
	}

	//extract return type: 
	//check if a returnType exists in the symbol (might be a constructor)
	if (methodSymbol->getReturnType() != nullptr) {
		typeFunction->returnType = TypesTable::getInstance()->getType(methodSymbol->getReturnType());

		//if return type not found, return a TypeError
		TypeError* errorReturnType = dynamic_cast<TypeError*>(typeFunction->returnType);
		if (errorReturnType != nullptr)
			return new TypeError("return type: " + string(methodSymbol->getReturnType()) + " is undefied.");
	}

	//everything is ok
	//now let's resize according to the new params
	typeFunction->resize();

	return typeFunction;
}

/* this builds default constructor:
*/
TypeExpression* TypeFunction::buildConstructor(Method* methodSym) {
	//let's build the FunctionType:
	TypeFunction* typeFunction = new TypeFunction(methodSym->functionSignatures, methodSym->getName()
		, methodSym->getUniqueName());

	//no params, so not adding any

	//return type should be the TypeClass the constructor is for, so setting the returnType is moved
	//to caller
	
	//everything is ok
	//now let's resize according to the new params
	typeFunction->resize();

	return typeFunction;
}


TypeExpression* TypeFunction::getInstance(string signature, FunctionCallNode* funcCallNode) {
	for (auto functionType : TypeFunction::functionInstances) {
		for (auto &functionSign : functionType->getSignatures()) {
			if(functionSign == signature)
				return functionType;
		}	
	}
	TypeFunction::errorFunctionCalls.push_back(funcCallNode);
	return new TypeError("function with signature: " + signature + " is undefined.");
}

bool TypeFunction::isDeclared(string name) {
	for (auto functionType : TypeFunction::functionInstances) {
		if (functionType->getFunctionName() == name)
			return true;
	}
	return false;
}

TypeFunction::TypeFunction(vector<string> functionSignatures, string functionName, string uniqueName) {
	this->signatures = functionSignatures;
	this->size = 0;
	this->functionName = functionName;
	this->uniqueName = uniqueName;
}

TypeExpression* TypeFunction::getReturnTypeExpression() {
	return this->returnType;
}

void TypeFunction::setReturnTypeExpression(TypeExpression* returnType) {
	this->returnType = returnType;
}

vector<TypeExpression*> TypeFunction::getParamsTEs() {
	return this->paramsTE;
}

int TypeFunction::getSize() {
	return this->size;
}

vector<string> TypeFunction::getSignatures() {
	return this->signatures;
}

int TypeFunction::getTypeId() {
	return FUNCTION_TYPE_ID;
}

void TypeFunction::addToParams(TypeExpression* paramTE) {
	this->paramsTE.push_back(paramTE);
}

void TypeFunction::resize() {
	int size = 0;
	for (auto &param : this->paramsTE)
		size += param->getSize();
	this->size = size;
}

string TypeFunction::getFunctionName() {
	return this->functionName;
}

string TypeFunction::getUniqueName() {
	return this->uniqueName;
}

bool TypeFunction::compareSignatures(vector<string> sign, vector<string> otherSign) {
	//TODO:
	string aSign = sign.at(0);
	std::size_t pos = aSign.find("_func");
	string cleanedString = aSign.erase(0, pos + 1); //+1 for the '_'
	for (auto &otherASign : otherSign) {
		std::size_t pos = otherASign.find("_func");
		string otherCleaned = otherASign.erase(0, pos + 1); //+1 for the '_'
		if (cleanedString == otherCleaned)
			return true;
	}
	return false;
}

bool TypeFunction::tryReDefine() {
	int first, later;
	first = later = TypeFunction::errorFunctionCalls.size();
	while (true) {
		later = first;
		for (int i = 0; i < TypeFunction::errorFunctionCalls.size(); i++) {
			auto& classCallNode = TypeFunction::errorFunctionCalls.at(i);
			if (classCallNode->type_checking()) {
				TypeFunction::errorFunctionCalls.erase(TypeFunction::errorFunctionCalls.begin() + i);
				first--;
			}
		}
		if (first == later)
			break;
	}
	if (first == 0)
		return true;
	return false;
}
