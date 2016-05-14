#include "TypesTable.h"
#include "TypeInteger.hpp"
#include "TypeFloat.hpp"
#include "TypeString.hpp"
#include "TypeVoid.hpp"
#include "TypeBoolean.hpp"
#include "TypeClass.hpp"

//static definition
TypesTable* TypesTable::mTypesTable;


TypesTable* TypesTable::getInstance() {
	if (!mTypesTable) {
		mTypesTable = new TypesTable();
		return mTypesTable;
	}
	return mTypesTable;
}


TypeExpression* TypesTable::getType(int typeId) {
	switch (typeId) {
	case INTEGER_TYPE_ID:
		return TypeInteger::getInstance();
	case FLOAT_TYPE_ID:
		return TypeFloat::getInstance();
	case STRING_TYPE_ID:
		return TypeString::getInstance();
	case VOID_TYPE_ID:
		return TypeVoid::getInstance();
	case BOOLEAN_TYPE_ID:
		return TypeBoolean::getInstance();
	default:
		return nullptr;
	}

}

TypeExpression* TypesTable::getClassType(string name) {
	return TypeClass::getInstance(name);
}

TypeExpression* TypesTable::buildClassType(ClassDefineNode* classNode, Class* classSymbol) {
	return TypeClass::buildClass(classNode, classSymbol);
}

