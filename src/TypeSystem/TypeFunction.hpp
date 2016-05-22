#pragma once
#ifndef TYPE_FUNCTION_H
#define TYPE_FUNCTION_H

#include "TypeExpression.hpp"
#include "../AST/FunctionDefineNode.hpp"
#include "../AST/ClassMethodNode.hpp"
#include "../SymbolTable/Symbol.h"
#include <vector>


class TypeFunction : public TypeExpression {
public:

	/*
	* this method is responsable for instantiating and building a Type of function.
	*
	* it should first look for an already defined FunctionType with the same signature
	* if there is a FunctionType with the same Signature then It won't build it.
	* if there isn't, then it will parse the @FunctionDefineNode and the @Function symbol for information about the 
	* type it should build.
	* if there was an error during parsing node and symbol, it will return a TypeError with a msg describing the error.
	*
	* This implementation can be enhanced easily to implement more functionality on FunctionType, like Functions Overloading.
	*/
	static TypeExpression* buildFunction(FunctionDefineNode* functionNode, Function* functionSym);
	
	static TypeExpression* buildMethod(ClassMethodNode* classMethodNode, Method* methodSym);


	/*
	 * This method is responsable for looking for a given signature of @FunctionType in the @functionInstances.
	 * if none is found, it will return a TypeError as not defined.
	 */
	static TypeExpression* getInstance(string signature);

	TypeExpression* getReturnTypeExpression(); // get the TypeExpression of the return value of the function

	vector<TypeExpression*> getParamsTEs(); // get params Type Expressions

	int getSize(); // get the size of params

	vector<string> getSignatures();

	int getTypeId();

	string getFunctionName();

	/*
		unique name is structred as " name+id " see Function Symbol @getUniqueName for more info
	*/
	string getUniqueName();

	static bool compareSignatures(vector<string> signature, vector<string> otherSignature);

private:
	TypeFunction(vector<string> functionSign, string functionName, string uniqueName);

	/*
	 * searches the @functionInstances for an already declared function
	 */
	static bool isDeclared(string name);

	vector<TypeExpression*> paramsTE;
	TypeExpression* returnType;
	
	/*
		A vector of @TypeFunction that contains the functions defined through out the code being compiled.
	*/
	static vector<TypeFunction*> functionInstances;


	void addToParams(TypeExpression* paramTE);

	int paramsSize;
	
	void resize();

	/*
	  a TypeFunction may have multiple signatures, see @Function Symbol for more info. 
	*/
	vector<string> signatures;

	string functionName;

	string uniqueName;
};

#endif
