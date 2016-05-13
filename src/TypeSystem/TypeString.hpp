#pragma once
#ifndef TYPE_STRING_HPP
#define TYPE_STRING_HPP

#include "TypeExpression.hpp"

class TypeString : public TypeExpression {
public:
	static TypeString* getInstance();

	// overrided Operations' Methods that String Type can be applied to , goes here:
    
	TypeExpression* opPlus(int secondTypeId);

	TypeExpression* opEqual(int secondTypeId);

	bool equivelantTo(int secondTypeId);

	int getTypeId();

private:
	static TypeString* mInstance;

	TypeString() {
		this->size = 4;
	}

};



#endif