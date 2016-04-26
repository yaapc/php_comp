#pragma once
#ifndef TYPE_VOID_H
#define TYPE_VOID_H

#include "TypeExpression.hpp"

class TypeVoid : public TypeExpression {
public:
	static TypeVoid* getInstance();
	int getTypeId();

protected:
	static TypeVoid* mInstance;
	TypeVoid(){}

};

#endif
