#include "TypesTable.h"
#include "TypeInteger.hpp"
#include "TypeFloat.hpp"
#include "TypeString.hpp"
#include "TypeVoid.hpp"
#include "TypeBoolean.hpp"
#include "TypeClass.hpp"
#include "TypeError.hpp"
#include "TypeFunction.hpp"

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

//get the TypeExpression from the type name
TypeExpression* TypesTable::getType(string type) {

	if (strcmp(type.c_str(), "int") == 0) {
		return TypesTable::getInstance()->getType(INTEGER_TYPE_ID);
	}


	if (strcmp(type.c_str(), "bool") == 0) {
		return TypesTable::getInstance()->getType(BOOLEAN_TYPE_ID);
	}

	if (strcmp(type.c_str(), "float") == 0) {
		return TypesTable::getInstance()->getType(FLOAT_TYPE_ID);
	}

	if (strcmp(type.c_str(), "string") == 0) {
		return TypesTable::getInstance()->getType(STRING_TYPE_ID);
	}

	if (strcmp(type.c_str(), "void") == 0) {
		return TypesTable::getInstance()->getType(VOID_TYPE_ID);
	}
	//check if a class is available:
	TypeExpression* typeClass = TypesTable::getInstance()->getClassType(type);

	if (typeClass == nullptr) { // no type found
		return new TypeError("Undefined");
	}
	return typeClass;
}

TypeExpression* TypesTable::getClassType(string name) {
	return TypeClass::getInstance(name);
}

TypeExpression* TypesTable::buildClassType(ClassDefineNode* classNode, Class* classSymbol) {
	return TypeClass::buildClass(classNode, classSymbol);
}

TypeExpression* TypesTable::buildFunctionType(FunctionDefineNode* functionNode, Function* functionSymbol) {
	return TypeFunction::buildFunction(functionNode, functionSymbol);
}

