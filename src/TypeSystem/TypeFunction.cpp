#pragma once

#include "TypeFunction.hpp"
#include "TypesTable.h"
#include "TypeError.hpp"

//static declaration
vector<TypeFunction*> TypeFunction::functionInstances;

TypeExpression* TypeFunction::buildFunction(FunctionDefineNode* functionNode, Function* functionSymbol) {


	//try to find a TypeFunction from the first signature of the given function symbol.
	//the @TypeFunction::isDeclared() returns TypeError if a name of the function already found, not just unmatched signature.
	//this behaviour can be changed, so we can obtain the functionality of Function Overloading.
	bool isDeclared = TypeFunction::isDeclared(functionNode->functionSym->functionSignatures.at(0));
								   
	
	//if isDeclared, throw a TypeError with already defined
	if (isDeclared)
		return new TypeError(string(functionSymbol->getName())  + "already defined.");

	//let's build the FunctionType:
	TypeFunction* typeFunction = new TypeFunction(functionSymbol->functionSignatures, functionSymbol->getName());

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

TypeExpression* TypeFunction::getInstance(string signature) {
	for (auto functionType : TypeFunction::functionInstances) {
		for (auto &functionSign : functionType->getSignatures()) {
			if(functionSign == signature)
				return functionType;
		}	
	}
	return new TypeError("function with signature: " + signature + " is undefined.");
}

bool TypeFunction::isDeclared(string name) {
	for (auto functionType : TypeFunction::functionInstances) {
		if (functionType->getFunctionName() == name)
			return true;
	}
	return false;
}

TypeFunction::TypeFunction(vector<string> functionSignatures, string functionName) {
	this->signatures = functionSignatures;
	this->size = 0;
	this->functionName = functionName;
}

TypeExpression* TypeFunction::getReturnTypeExpression() {
	return this->returnType;
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