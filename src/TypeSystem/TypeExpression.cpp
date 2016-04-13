#include "TypeExpression.hpp"
#include "TypeError.hpp"


// .
TypeExpression* TypeExpression::opDot() {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + " Type doesn't support . operation");
}

// + - * /
TypeExpression* TypeExpression::opAggregate(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support aggregate operation");
}

// >
TypeExpression* TypeExpression::opGreaterThan(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support > operation");
}

// <
TypeExpression* TypeExpression::opLessThan(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support < operation");
}

// >=
TypeExpression* TypeExpression::opGreaterOrEq(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support >= operation");
}

// <=
TypeExpression* TypeExpression::opLessOrEq(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support <= operation");
}

// logical and
TypeExpression* TypeExpression::opAnd(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support Logical_And operation");
}

// logical or
TypeExpression* TypeExpression::opOr(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support Logical_Or operation");
}

// ==
TypeExpression* TypeExpression::opEqual(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support == operation");
}

// []
TypeExpression* TypeExpression::opSqrBrackets(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support [] operation");
}