#pragma once

#include "TypeFloat.hpp"
#include "TypeBoolean.hpp"
#include "TypeError.hpp"

//static definition
TypeFloat* TypeFloat::mInstance;

TypeFloat* TypeFloat::getInstance() {
	if (!TypeFloat::mInstance) {
		TypeFloat::mInstance = new TypeFloat();
	}
	return TypeFloat::mInstance;
}

// overrided Operations' Methods that Integer Type can be applied to , goes here:
// + - * /
TypeExpression* TypeFloat::opPlus(int secondTypeId) {
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == FLOAT_TYPE_ID)
		return TypeFloat::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeFloat::opMinus(int secondTypeId) {
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == FLOAT_TYPE_ID)
		return TypeFloat::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeFloat::opMult(int secondTypeId) {
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == FLOAT_TYPE_ID)
		return TypeFloat::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeFloat::opDiv(int secondTypeId) {
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == FLOAT_TYPE_ID)
		return TypeFloat::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeFloat::opMod(int secondTypeId) {
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == FLOAT_TYPE_ID)
		return TypeFloat::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeFloat::opGreaterThan(int secondTypeId) {
	if (secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support > operation");
}

// <
TypeExpression* TypeFloat::opLessThan(int secondTypeId) {
	if (secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support < operation");
}

// >=
TypeExpression* TypeFloat::opGreaterOrEq(int secondTypeId) {
	if (secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support >= operation");
}

// <=
TypeExpression* TypeFloat::opLessOrEq(int secondTypeId) {
	if (secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support <= operation");
}

TypeExpression* TypeFloat::opEqual(int secondTypeId) {
	if (secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + " Type doesn't support == operation");
}

bool TypeFloat::equivelantTo(int secondTypeId) {
	if (secondTypeId == FLOAT_TYPE_ID)
		return true;

	return false;
}


//overriding virtual method
int TypeFloat::getTypeId() {
	return FLOAT_TYPE_ID;
}




