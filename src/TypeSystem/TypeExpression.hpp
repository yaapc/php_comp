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

	virtual TypeExpression* opDot();

	virtual TypeExpression* opPlus(int secondTypeId);

	virtual TypeExpression* opMinus(int secondTypeId);

	virtual TypeExpression* opMult(int secondTypeId);

	virtual TypeExpression* opDiv(int secondTypeId);

	virtual TypeExpression* opMod(int secondTypeId);

	virtual TypeExpression* opGreaterThan(int secondTypeId);

	virtual TypeExpression* opLessThan(int secondTypeId);

	virtual TypeExpression* opGreaterOrEq(int secondTypeId);

	virtual TypeExpression* opLessOrEq(int secondTypeId);

	virtual TypeExpression* opAnd(int secondTypeId);

	virtual TypeExpression* opOr(int secondTypeId);

	virtual TypeExpression* opEqual(int secondTypeId);

	virtual TypeExpression* opSqrBrackets(int secondTypeId);

};




#endif
