#pragma once
#ifndef TYPESYSTEM_HELPER_HPP
#define TYPESYSTEM_HELPER_HPP

#include "TypeDefinitions.hpp"
#include <string>
using namespace std;

class TypeExpression;

class TypeSystemHelper {
public:
	static string getTypeName(int typeId);
	static string getTypeName(TypeExpression* type);
};


#endif
