#pragma once
#ifndef TYPE_CLASS_H
#define TYPE_CLASS_H

#include "TypeExpression.hpp"
#include <vector>
#include "../AST/ClassDefineNode.hpp"
#include "../SymbolTable/Symbol.h"

class MemberHolder;//forward declaration


class TypeClass : public TypeExpression {
public:

	/*
	   This method is responsable for instantiating and building a Type of Class.
	   It should first look for a class already created with the same name.
	   If there is already a class with the same name, then It wont build the TypeClass.
	   If there isn't it should parse the @ClassDefineNode and @Class Symbol for information about the type it should create.

	   This will raise a problem with Inner Classes, where two unique classes may exist as inner classes of different
	   outer classes... 
	   At first thought the problem can be solved by uniquely naming each class, which depends on the context
	   it was created in... :TODO

	*/
	static TypeClass* buildClass(ClassDefineNode* classNode, Class* classSymbol);


	/*
		here we build the God of Classes:
	
	*/
	static TypeClass* buildObjectClass();
	
	/*
		This method is responsable for looking for a given id or name of @ClassType in the @classInstances.
	    if none is found, it will return nullptr.
		TODO: look for isntance using Unique Name.
	*/
	static TypeClass* getInstance(string name);

	TypeExpression* opDot(string propertyStr);

	int getSize();

	//TODO: bool equivelantTo(int secondTypeId);

	//overriding virtual method
	int getTypeId();

	string getName();

private:
	
	vector<MemberHolder*> members;
	vector<TypeExpression*> methods;
	TypeExpression* parentClass;

	/*
		A vector of @TypeClass that contains the classed defined through out the code being compiled.
	*/
	static vector<TypeClass*> classInstances;

	string name;



	TypeClass(string name);



	void addToMembers(MemberHolder* memberType);
	MemberHolder* lookupMembers(string memberStr);


	void makeSize();
};



class MemberHolder {
public:
	TypeExpression* type;
	DataMember* memberSymbol;
	MemberHolder(TypeExpression* te, DataMember* memberSymbol) {
		this->type = te;
		this->memberSymbol = memberSymbol;
	}
};





#endif
