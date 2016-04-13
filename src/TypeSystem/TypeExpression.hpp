#pragma once
#ifndef TYPEEXPRESSION_HPP
#define TYPEEXPRESSION_HPP


#include <string>
//#include "TypeDefinitions.hpp"
#include "TypeSystemHelper.hpp"
//#include "TypeError.hpp"
using namespace std;


/*
 * this class will be the interface for all the possible types ( simple and compound ) and defining thier uniform processing.
 */


class TypeExpression {
public:

	TypeExpression() {
	}

	virtual int getTypeId() = 0;

	string typeExpression() {
		return "";
	}

    /* here are all the operators that can be applied to a type in our language
     * the naming convention of the methods is: op[Operation Name]
	 */

	// .
	TypeExpression* opDot();

    // + - * /
	TypeExpression* opAggregate(int secondTypeId);

	// >
	TypeExpression* opGreaterThan(int secondTypeId);

	// <
	TypeExpression* opLessThan(int secondTypeId);

	// >=
	TypeExpression* opGreaterOrEq(int secondTypeId);

	// <=
	TypeExpression* opLessOrEq(int secondTypeId);

	// logical and
	TypeExpression* opAnd(int secondTypeId);

	// logical or
	TypeExpression* opOr(int secondTypeId);

	// ==
	TypeExpression* opEqual(int secondTypeId);

    // []
	TypeExpression* opSqrBrackets(int secondTypeId);

};




#endif
