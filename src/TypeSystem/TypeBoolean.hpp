#pragma once
#ifndef TYPE_BOOLEAN_HPP
#define TYPE_BOOLEAN_HPP

#include "TypeExpression.hpp"
/*
 * A Singleton Class of Boolean Type.
 */
class TypeBoolean : public TypeExpression {

public:
	static TypeBoolean* getInstance();

	int getTypeId();

protected:
    static TypeBoolean* mInstance;
    TypeBoolean(){}

};

#endif
