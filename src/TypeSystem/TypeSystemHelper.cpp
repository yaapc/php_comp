#include "TypeSystemHelper.hpp"
#include "TypeExpression.hpp"
#include "TypeClass.hpp"

string TypeSystemHelper::getTypeName(int typeId) {
	switch (typeId) {
	case ERROR_TYPE_ID: return "error_type";
	case INTEGER_TYPE_ID: return "int";
	case VOID_TYPE_ID: return "void";
	case BOOLEAN_TYPE_ID: return "boolean";//TODO: check if lexer scans boolean or bool
	case FLOAT_TYPE_ID: return "float";
	case STRING_TYPE_ID: return "string";
	case CLASS_TYPE_ID: return "class";//TODO: get class name
	default: return "Undefined";
	}
}

string TypeSystemHelper::getTypeName(TypeExpression* type) {
	if (type->getTypeId() > CLASS_TYPE_ID)
		return dynamic_cast<TypeClass*>(type)->getName();
	return TypeSystemHelper::getTypeName(type->getTypeId());
}



