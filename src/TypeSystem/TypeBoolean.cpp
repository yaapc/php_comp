#pragma once

#include "TypeBoolean.hpp"

//static defifintion
TypeBoolean* TypeBoolean::mInstance;


TypeBoolean* TypeBoolean::getInstance() {
	if (!mInstance) {
		mInstance = new TypeBoolean();
	}
	return mInstance;
}

TypeExpression* TypeBoolean::opAnd(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(BOOLEAN_TYPE_ID);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeBoolean::opOr(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(BOOLEAN_TYPE_ID);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}


int TypeBoolean::equivelantTo(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == BOOLEAN_TYPE_ID)
		return BOOLEAN_TYPE_ID;
	return ERROR_TYPE_ID;
}
int TypeBoolean::getTypeId() {
	return BOOLEAN_TYPE_ID;
}