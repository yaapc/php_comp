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

int TypeBoolean::getTypeId() {
	return BOOLEAN_TYPE_ID;
}