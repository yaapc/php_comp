#include "TypesTable.h"
#include "TypeInteger.hpp"
#include "TypeFloat.hpp"
#include "TypeString.hpp"

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
	default:
		return nullptr;
	}

}