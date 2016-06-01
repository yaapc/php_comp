#pragma once

#include "TypeFloat.hpp"
#include "TypeBoolean.hpp"
#include "TypeError.hpp"
#include "TypesTable.h"

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
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeFloat::opMinus(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeFloat::opMult(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeFloat::opDiv(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeFloat::opMod(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeFloat::opGreaterThan(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support > operation");
}

// <
TypeExpression* TypeFloat::opLessThan(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support < operation");
}

// >=
TypeExpression* TypeFloat::opGreaterOrEq(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support >= operation");
}

// <=
TypeExpression* TypeFloat::opLessOrEq(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support <= operation");
}

TypeExpression* TypeFloat::opEqual(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + " Type doesn't support == operation");
}

int TypeFloat::equivelantTo(int secondTypeId) {
	if (secondTypeId == FLOAT_TYPE_ID || secondTypeId == INTEGER_TYPE_ID)
		return FLOAT_TYPE_ID;
	return ERROR_TYPE_ID;
}


//overriding virtual method
int TypeFloat::getTypeId() {
	return FLOAT_TYPE_ID;
}




