#pragma once
#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include "ErrorRecovery\ErrorRecovery.h"
#include "SymbolTable\SymbolsParser.h"

int yylex(void);
int yyparse();
void yyerror(const char *);

#define SIMULATOR 0


enum primitive {
	T_PRIMITIVE_INTEGER = 1, T_PRIMITIVE_FLOAT, T_PRIMITIVE_DOUBLE, T_PRIMITIVE_BOOLEAN, T_PRIMITIVE_STRING, T_PRIMITIVE_CHAR, T_PRIMITIVE_VOID
};

enum SymbolTypes {
	FUNCTION = 1, CLASS, OBJECT, VARIABLE, INTERFACE, TYPE, DATA_MEMBER, METHODS, CASE_LABEL, PARAMETER, ERROR_SYMBOL
};
enum AccessModifiers {
	DEFAULT = 1, PUBLIC_ACCESS, PRIVATE_ACCESS, PROTECTED_ACCESS
};
enum StorageModifier {
	DEFAULT_STORAGE = 5, FINAL_STORAGE, STATIC_STORAGE, FINAL_STATIC_STORAGE
};


enum open_tag_type {
	NORMAL, WITH_ECHO
};

#endif
