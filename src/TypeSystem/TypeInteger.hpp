#pragma once
#ifndef TYPE_INTEGER_H
#define TYPE_INTEGER_H

#include "TypeExpression.hpp"

/*
 * A Singleton Class of Integer Type.
*/
class TypeInteger : public TypeExpression {
public:

	static TypeInteger* getInstance();

	
	// overrided Operations' Methods that Integer Type can be applied to , goes here:
	
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

	//overriding virtual method
	int getTypeId();


protected:
  static TypeInteger* mInstance;

  TypeInteger(){}
};

#endif
