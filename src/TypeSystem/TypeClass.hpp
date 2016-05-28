#pragma once
#ifndef TYPE_CLASS_H
#define TYPE_CLASS_H

#include "TypeExpression.hpp"
#include <vector>
#include "../AST/ClassDefineNode.hpp"
#include "../SymbolTable/Symbol.h"
#include "../definitions.h"
#include "TypeFunction.hpp"

class MemberWrapper;//forward declaration
class MethodWrapper;
class PropertyWrapper;

class TypeClass : public TypeExpression {
public:

	/*
	   This method is responsable for instantiating and building a Type of Class.
	   It should first look for a class already created with the same name.
	   If there is already a class with the same name, then It will return TypeError indecating that It's already defined.
	   If there isn't, it should parse the @ClassDefineNode and @Class Symbol for information about the type it should create.

	   This will raise a problem with Inner Classes, where two unique classes may exist as inner classes of different
	   outer classes... 
	   At first thought the problem can be solved by uniquely naming each class, which depends on the context
	   it was created in... :TODO

	*/
	static TypeExpression* buildClass(ClassDefineNode* classNode, Class* classSymbol);


	/*
	 * here we build the God of Classes:	
	 */
	static TypeClass* buildObjectClass();
	
	/*
		This method is responsable for looking for a given id or name of @ClassType in the @classInstances.
	    if none is found, it will return a TypeError indecating that It's undefined.
		TODO: look for isntance using Unique Name, INNER CLASSES!!

	*/
	static TypeExpression* getInstance(string name);

	TypeExpression* opDot(string propertyStr, bool isMethod, string methodSign, MemberWrapper*& memWrapper);

	int getSize();
	vector<MemberWrapper*> getMembers();


	//TODO: bool equivelantTo(int secondTypeId);

	//overriding virtual method
	int getTypeId();

	string getName();

	bool equivelantTo(int secondTypeId);

	PropertyWrapper* lookupMembers(string memberStr);

	MethodWrapper* lookupMembers(string memberStr, string methodSign);

private:
	
	/* a vector of properties that this TypeClass has*/
	vector<PropertyWrapper*> props; // properties
	/* a vector of methods that this TypeClass has*/
	vector<MethodWrapper*> methods;
	/* a vector of methods and properties of this TypeClass*/
	vector<MemberWrapper*> members;
	/* a TypeClass of the parent of this TypeClass*/
	TypeExpression* parentClass;

	/*
		A vector of @TypeClass that contains the classes defined through out the code being compiled.
	*/
	static vector<TypeClass*> classInstances;

	static bool isDefined(string name);

	void addToProps(PropertyWrapper* prop);

	string name;

	/* a pointer to super class */
	TypeClass* super;


	TypeClass(string name);

	void addToMembers(MemberWrapper* memberType);

	void makeSize();

};


class MemberWrapper {
public:
	virtual string getUniqueName() = 0;
	virtual string getName() = 0;
	virtual int getWrapperType() = 0;
	static const int PROPERTY_WRAPPER = 0;
	static const int METHOD_WRAPPER = 1;
	int getAccessModifier();
	TypeExpression* getTypeExpr();
	void setTypeExpr(TypeExpression* te);
	int getSize();
private:
	TypeExpression* type;
	int accessModifier;
};

class PropertyWrapper : public MemberWrapper {
public:
	DataMember* memberSymbol;
	PropertyWrapper(TypeExpression* te, DataMember* memberSymbol);
	string getUniqueName();
	string getName();
	int getWrapperType();
};


class MethodWrapper : public MemberWrapper {
private:
		TypeExpression* returnType;
    	Method* methodSymbol;	
public:
	MethodWrapper(TypeExpression* type, Method* methodSymbol);

	string getLabel();
	
	string getUniqueName();
	
	string getName();
	
	int getWrapperType();

	bool isDefaultContructor();

	TypeFunction* getMethodType();
};


#endif
