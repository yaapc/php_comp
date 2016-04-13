#pragma once
#ifndef TYPE_ERROR_H
#define TYPE_ERROR_H

#include"TypeExpression.hpp"


/* this class represents an Error-Type to signal errors out of the type checker context.
*/
class TypeError : public TypeExpression {
public:

	TypeError(string msg);
	int getTypeId();

	string msg;
};


#endif
