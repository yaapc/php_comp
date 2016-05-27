#pragma once
#ifndef TYPE_FLOAT_H
#define TYPE_FLOAT_H

#include "TypeExpression.hpp"


/*
 * A Singleton Class of Float Type.
 */
class TypeFloat : public TypeExpression {
public:

	static TypeFloat* getInstance();


	// overrided Operations' Methods that Float Type can be applied to , goes here:

	TypeExpression* opPlus(int secondTypeId);

	TypeExpression* opMinus(int secondTypeId);

	TypeExpression* opMult(int secondTypeId);

	TypeExpression* opDiv(int secondTypeId);

	TypeExpression* opMod(int secondTypeId);

	TypeExpression* opGreaterThan(int secondTypeId);

	TypeExpression* opLessThan(int secondTypeId);

	TypeExpression* opGreaterOrEq(int secondTypeId);

	TypeExpression* opLessOrEq(int secondTypeId);

	TypeExpression* opEqual(int secondTypeId);

	bool equivelantTo(int secondTypeId);

	//overriding virtual method
	int getTypeId();

protected:
	static TypeFloat* mInstance;

	TypeFloat() {
		this->size = 4;
	}
};

#endif
