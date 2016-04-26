#pragma once

#include "TypeVoid.hpp"

//static definition
TypeVoid* TypeVoid::mInstance;


TypeVoid* TypeVoid::getInstance() {
	if (!TypeVoid::mInstance) {
		TypeVoid::mInstance = new TypeVoid();
	}
	return TypeVoid::mInstance;
}

int TypeVoid::getTypeId() {
	return VOID_TYPE_ID;
}
