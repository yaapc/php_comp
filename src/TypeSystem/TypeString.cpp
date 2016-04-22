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
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == STRING_TYPE_ID)
		return TypeString::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeString::opEqual(int secondTypeId) {
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == STRING_TYPE_ID)
		return TypeString::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

int TypeString::getTypeId() {
	return STRING_TYPE_ID;
}

