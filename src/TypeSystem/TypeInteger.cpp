#include "TypeInteger.hpp"
#include "TypeBoolean.hpp"
#include "TypeError.hpp"

//static definition
TypeInteger* TypeInteger::mInstance;

TypeInteger* TypeInteger::getInstance() {
	if (!TypeInteger::mInstance) {
		TypeInteger::mInstance = new TypeInteger();
	}
	return TypeInteger::mInstance;
}

// overrided Operations' Methods that Integer Type can be applied to , goes here:
// + - * /
TypeExpression* TypeInteger::opPlus(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(secondTypeId);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeInteger::opMinus(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(secondTypeId);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeInteger::opMult(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(secondTypeId);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeInteger::opDiv(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(secondTypeId);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeInteger::opMod(int secondTypeId) {
	if (this->equivelantTo(secondTypeId))
		return TypesTable::getInstance()->getType(secondTypeId);
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeInteger::opGreaterThan(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support > operation");
}

// <
TypeExpression* TypeInteger::opLessThan(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support < operation");
}

// >=
TypeExpression* TypeInteger::opGreaterOrEq(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support >= operation");
}

// <=
TypeExpression* TypeInteger::opLessOrEq(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support <= operation");
}

TypeExpression* TypeInteger::opEqual(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID || secondTypeId == FLOAT_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + " Type doesn't support == operation");
}

int TypeInteger::equivelantTo(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID)
		return INTEGER_TYPE_ID;
	if (secondTypeId == FLOAT_TYPE_ID)
		return FLOAT_TYPE_ID;
	if (secondTypeId == STRING_TYPE_ID)
		return STRING_TYPE_ID;
	return ERROR_TYPE_ID;
}


//overriding virtual method
int TypeInteger::getTypeId() {
	return INTEGER_TYPE_ID;
}