#pragma once
#ifndef TYPE_BOOLEAN_HPP
#define TYPE_BOOLEAN_HPP

#include "TypeExpression.hpp"
#include "TypesTable.h"
#include "TypeError.hpp"
/*
 * A Singleton Class of Boolean Type.
 */
class TypeBoolean : public TypeExpression {

public:
	static TypeBoolean* getInstance();

	TypeExpression* opAnd(int secondTypeId);

	TypeExpression* opOr(int secondTypeId);

	int equivelantTo(int secondTypeId);

	int getTypeId();

protected:
    static TypeBoolean* mInstance;
    TypeBoolean(){
		this->size = 4;
	}

};

#endif
