#pragma once

#include "TypeString.hpp"
#include "TypeError.hpp"

//static definition
TypeString* TypeString::mInstance;


TypeString* TypeString::getInstance() {
	if (!TypeString::mInstance) {
		TypeString::mInstance = new TypeString();
	}
	return TypeString::mInstance;
}

TypeExpression* TypeString::opPlus(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypeString::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeString::opEqual(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypeString::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

int TypeString::getTypeId() {
	return STRING_TYPE_ID;
}

int TypeString::equivelantTo(int secondTypeId) {
	if (secondTypeId == STRING_TYPE_ID)
		return STRING_TYPE_ID;
	if (secondTypeId == INTEGER_TYPE_ID)
		return STRING_TYPE_ID;
	return ERROR_TYPE_ID;
}
