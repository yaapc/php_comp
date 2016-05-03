#pragma once
#ifndef TYPE_FUNCTION_H
#define TYPE_FUNCTION_H

#include "TypeExpression.hpp"
#include <vector>


class TypeFunction : public TypeExpression {
public:


	/*
	* this method is the only way to create type expressions for functions..
	*/
	static TypeFunction* buildFunction(string te);
	


	TypeExpression* getReturnTypeExpression(); // get the TypeExpression of the return value of the function

	vector<TypeExpression*> getParamsTEs(); // get params Type Expressions

private:
	TypeFunction();

	vector<TypeExpression*> params;
	TypeExpression* returnType;
	
	void addToParams(TypeExpression* paramTE);
};

#endif
