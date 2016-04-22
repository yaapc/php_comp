#include "TypeSystemHelper.hpp"

string TypeSystemHelper::getTypeName(int typeId) {
	switch (typeId) {
	case INTEGER_TYPE_ID: return "Int";
	case VOID_TYPE_ID: return "Void";
	case BOOLEAN_TYPE_ID: return "Boolean";
	case FLOAT_TYPE_ID: return "Float";
	case CHARACTER_TYPE_ID: return "Char";
	case STRING_TYPE_ID: return "String";
	default: return "TS_HELPER_ERROR";
	}
}
