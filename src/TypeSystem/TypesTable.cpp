#include "TypesTable.h"
#include "TypeInteger.hpp"

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
	default:
		return nullptr;
	}

}