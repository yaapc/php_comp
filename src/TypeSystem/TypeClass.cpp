#pragma once

#include "TypeClass.hpp"
#include "TypeError.hpp"


//static defination
vector<TypeClass*> TypeClass::classInstances;


TypeClass* TypeClass::buildClass(ClassDefineNode* classNode, Class* classSymbol) {
	
	TypeClass* typeClass = TypeClass::getInstance(classSymbol->getName());

	if (typeClass != nullptr) // a TypeClass already defined with the same name.
		return typeClass;

	
	
	//***  build the type:
	typeClass = new TypeClass(classSymbol->getName());

	//extract members' types and symbols from @classNode and append them to the TypeClass members, as a @MemberHolder
	for (int i = 0; i < classNode->classMemNodes.size(); i++) {
		typeClass->addToMembers(
			new MemberHolder(classNode->classMemNodes.at(i)->getNodeType(),
							 classNode->classMemNodes.at(i)->getMemSymbol())
			);
	}
	
	//all members added, tell typeClass to resize itself:
	typeClass->makeSize();	

	//and finally, add it to the TypeClasses we have:
	TypeClass::classInstances.push_back(typeClass);
}



TypeClass* TypeClass::buildObjectClass() {
	TypeClass* object = new TypeClass("Object");

	TypeClass::classInstances.push_back(object);

	return object;
}


//search @classInstances for a decalred class with name @name
TypeClass* TypeClass::getInstance(string name) {
	for (auto classType : TypeClass::classInstances) {
		if (classType->getName() == name)
			return classType;
	}
	return nullptr;
}

TypeClass::TypeClass(string name) {
	this->name = name;
}


//TODO: I think opDot should have a string not a TypeId. 
//ex: obj.x  x is the name of the property we want to access, it doesn't have
//an id, and we don't know what type it is.
//additionally, it should have an access context.
TypeExpression* TypeClass::opDot(string propertyStr) {

	//properties have '$' in their symbol's name,
	//but when called from a reference of an object, they're called without the '$'...
	//PHP MY ASS..
	MemberHolder* member = this->lookupMembers("$"+ propertyStr);
	if(member == nullptr) // property not found
		return new TypeError(this->getName() + " doesn't have property: " + propertyStr);
	else {
		//TODO: check access context and what so ever....
		return member->type;
	}	
}

int TypeClass::getSize() {
	return this->size; 
}

int TypeClass::getTypeId() {
	return CLASS_TYPE_ID;
}


string TypeClass::getName() {
	return this->name;
}

void TypeClass::addToMembers(MemberHolder* memberType) {
	this->members.push_back(memberType);
}

MemberHolder* TypeClass::lookupMembers(string memberStr) {
	for (auto member : this->members) {
		if (strcmp(member->memberSymbol->getName(), memberStr.c_str()) == 0) {
			return member;
		}
	}
	return nullptr;
}

void TypeClass::makeSize() {
	int size = 0;
	for (auto mem : this->members) {
		size += mem->type->getSize();
	}
	this->size = size;
}

bool TypeClass::equivelantTo(int secondTypeId){
	//TODO
	if(this->getTypeId() == secondTypeId)
		return true;
	return false;	
}