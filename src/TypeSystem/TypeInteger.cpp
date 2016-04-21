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
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == INTEGER_TYPE_ID)
		return TypeInteger::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeInteger::opMinus(int secondTypeId) {
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == INTEGER_TYPE_ID)
		return TypeInteger::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeInteger::opMult(int secondTypeId) {
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == INTEGER_TYPE_ID)
		return TypeInteger::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeInteger::opDiv(int secondTypeId) {
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == INTEGER_TYPE_ID)
		return TypeInteger::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeInteger::opMod(int secondTypeId) {
	//TODO: replace condition with this->equivalentTo(secondTypeId) to enable implicit casting
	if (secondTypeId == INTEGER_TYPE_ID)
		return TypeInteger::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support aggregate operation");
}

TypeExpression* TypeInteger::opGreaterThan(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support > operation");
}

// <
TypeExpression* TypeInteger::opLessThan(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support < operation");
}

// >=
TypeExpression* TypeInteger::opGreaterOrEq(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support >= operation");
}

// <=
TypeExpression* TypeInteger::opLessOrEq(int secondTypeId) {
	if (secondTypeId == INTEGER_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + "  Type doesn't support <= operation");
}

//overriding virtual method
int TypeInteger::getTypeId() {
	return INTEGER_TYPE_ID;
}