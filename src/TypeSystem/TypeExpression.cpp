#include "TypeExpression.hpp"
#include "TypeError.hpp"
#include "../AST/ClassCallNode.hpp"


TypeExpression::TypeExpression() {
	this->size = 0;
}

TypeExpression* TypeExpression::opDot(string propertyStr, bool isMethod, string methodSign, MemberWrapper*& memWrapper, ClassCallNode* classCallNode) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + " Type doesn't support -> operation");
}

TypeExpression* TypeExpression::opPlus(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support aggregate operation");
}


TypeExpression* TypeExpression::opMinus(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support aggregate operation");
}


TypeExpression* TypeExpression::opMult(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support aggregate operation");
}


TypeExpression* TypeExpression::opDiv(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support aggregate operation");
}


TypeExpression* TypeExpression::opMod(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support aggregate operation");
}


TypeExpression* TypeExpression::opGreaterThan(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support > operation");
}

TypeExpression* TypeExpression::opLessThan(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support < operation");
}

TypeExpression* TypeExpression::opGreaterOrEq(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support >= operation");
}

TypeExpression* TypeExpression::opLessOrEq(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support <= operation");
}

TypeExpression* TypeExpression::opAnd(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support Logical_And operation");
}

TypeExpression* TypeExpression::opOr(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support Logical_Or operation");
}

TypeExpression* TypeExpression::opEqual(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support == operation");
}

TypeExpression* TypeExpression::opSqrBrackets(int secondTypeId) {
	return new TypeError(TypeSystemHelper::getTypeName(this->getTypeId()) + "  Type doesn't support [] operation");
}

int TypeExpression::equivelantTo(int secondTypeId) {
	return ERROR_TYPE_ID;
}

int TypeExpression::getSize() {
	return this->size;
}
