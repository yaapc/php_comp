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

bool TypeString::equivelantTo(int secondTypeId) {
	if (secondTypeId == STRING_TYPE_ID || 
		secondTypeId == INTEGER_TYPE_ID	||
		secondTypeId == FLOAT_TYPE_ID)
		return true;

	return false;
}
