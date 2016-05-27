#pragma once

#include "TypeClass.hpp"
#include "TypeError.hpp"

//static defination
vector<TypeClass*> TypeClass::classInstances;


TypeExpression* TypeClass::buildClass(ClassDefineNode* classNode, Class* classSymbol) {
	
	bool isDefined = TypeClass::isDefined(classSymbol->getName());
			
	if (isDefined) // a TypeClass already defined with the same name.
		return new TypeError(string(classSymbol->getName()) + " class is already defined.");

	
	//***  build the type:
	TypeClass* typeClass = new TypeClass(classSymbol->getName());

	//get base class TypeClass
	TypeExpression* instance = TypeClass::getInstance(classSymbol->getInhertedFrom());
	//check if base class exists
	if (instance->getTypeId() == ERROR_TYPE_ID) // it's a TypeError
		return instance; //cancel creating of type 

	TypeClass* baseType = dynamic_cast<TypeClass*>(instance);
	typeClass->parentClass = baseType;
	//append all members of base class to the current class:
	for (auto &member : baseType->getMembers()) {
		typeClass->addToMembers(member);
	}

	//extract members' types and symbols from @classNode and append them to the TypeClass members, as a @PropertyWrapper
	for (int i = 0; i < classNode->classMemNodes.size(); i++) {
		PropertyWrapper* prop = 
			new PropertyWrapper(classNode->classMemNodes.at(i)->getNodeType(),
				classNode->classMemNodes.at(i)->getMemSymbol());
		typeClass->addToMembers(prop);
		typeClass->addToProps(prop);
	}	


	//extract methods' types and symbols from @classNode and append them to the TypeClass members as a @MethodWrapper
	for (auto &method : classNode->classMethodsNodes) {
		//check if the method is overriding a base method
		bool isOverriding = false;
		for (auto &baseMember : typeClass->members) {
			if (baseMember->getWrapperType() == MemberWrapper::METHOD_WRAPPER) {
				MethodWrapper* baseMethod = dynamic_cast<MethodWrapper*>(baseMember);
				if (TypeFunction::compareSignatures(method->methodSym->functionSignatures, baseMethod->getMethodType()->getSignatures())) {//overriding a base method
					//TODO: Overriding with the concept of Default values
					//TODO: type check assigning weaker privileges 
					baseMember = new MethodWrapper(method->getNodeType(), method->methodSym);
					isOverriding = true;
					break;//out of typeClass->members
				}
			}
		}
		if (!isOverriding) {
			MethodWrapper* methodWrapper = new MethodWrapper(method->getNodeType(), method->methodSym);
			typeClass->members.push_back(methodWrapper);
			typeClass->methods.push_back(methodWrapper);
		}
	}

	//and finally, add it to the TypeClasses we have:
	TypeClass::classInstances.push_back(typeClass);

	//all members added, tell typeClass to resize itself:
	typeClass->makeSize();	

	return typeClass;
}

bool TypeClass::isDefined(string name) {
	for (auto classType : TypeClass::classInstances) {
		if (classType->getName() == name)
			return true;
	}
	return false;
}

TypeClass* TypeClass::buildObjectClass() {
	TypeClass* object = new TypeClass("Object");
	object->super = nullptr;
	object->
	TypeClass::classInstances.push_back(object);

	return object;
}


//search @classInstances for a decalred class with name @name
TypeExpression* TypeClass::getInstance(string name) {
	for (auto classType : TypeClass::classInstances) {
		if (classType->getName() == name)
			return classType;
	}
	return new TypeError(name + " class is undefined");
}

TypeClass::TypeClass(string name) {
	this->name = name;
}


//TODO: I think opDot should have a string not a TypeId. 
//ex: obj.x  x is the name of the property we want to access, it doesn't have
//an id, and we don't know what type it is.
//additionally, it should have an access context.
TypeExpression* TypeClass::opDot(string memberStr, bool isMethod, string methodSign, MemberWrapper*& memWrapper) {

	if (!isMethod) { // calling a member
		//properties have '$' in their symbol's name,
		//but when called from a reference of an object, they're called without the '$'...
		//PHP MY ASS..
		PropertyWrapper* prop = this->lookupMembers("$" + memberStr);
		if (prop == nullptr) { // property not found
			memWrapper = nullptr;
			return new TypeError(this->getName() + " doesn't have property " + memberStr);
		}
		else {
			//TODO: check access context and what so ever....
			memWrapper = prop;
			return prop->getTypeExpr();;
		}
	}
	else { // calling a method
		MethodWrapper* method = this->lookupMembers(memberStr, methodSign);
		if (method == nullptr) { // method not found
			memWrapper = nullptr;
			return new TypeError(this->getName() + " doesn't have method " + memberStr);
		}
		else {
			//TODO: check access context and what so ever....
			memWrapper = method;
			TypeFunction* methodType = dynamic_cast<TypeFunction*>(method->getTypeExpr());
			return methodType->getReturnTypeExpression();
		}
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

void TypeClass::addToMembers(MemberWrapper* memberType) {
	this->members.push_back(memberType);
}

PropertyWrapper* TypeClass::lookupMembers(string memberStr) {
	for (auto member : this->members) {
		if (member->getName() == memberStr) {
			return dynamic_cast<PropertyWrapper*>(member);
		}
	}
	return nullptr;
}

MethodWrapper* TypeClass::lookupMembers(string memberStr, string methodSign) {
	for (auto member : this->members) {
		if (member->getName() == memberStr) {
			return dynamic_cast<MethodWrapper*>(member);
		}
	}
	return nullptr;
}


void TypeClass::makeSize() {
	int size = 0;

	for (auto mem : this->props) {
		size += mem->getSize();
	}

	for(auto methoed: this->methods){
		size += 4;
	}
	int parentSize = parentClass->getSize();
	this->size = size + parentSize;
}

bool TypeClass::equivelantTo(int secondTypeId){
	//TODO
	if(this->getTypeId() == secondTypeId)
		return true;
	return false;	
}

vector<MemberWrapper*> TypeClass::getMembers() {
	return this->members;
}

void TypeClass::addToProps(PropertyWrapper* prop) {
	this->props.push_back(prop);
}

//==============
//WRAPPERS:
//==============

int MemberWrapper::getAccessModifier() {
	return this->accessModifier;
}

void MemberWrapper::setTypeExpr(TypeExpression* te) {
	this->type = te;
}

TypeExpression* MemberWrapper::getTypeExpr() {
	return this->type;
}

int MemberWrapper::getSize() {
	return this->type->getSize();
}

PropertyWrapper::PropertyWrapper(TypeExpression* te, DataMember* dm){
	this->setTypeExpr(te);
	this->memberSymbol = dm;
}

string PropertyWrapper::getUniqueName() {
	return this->memberSymbol->getNameWithout();
}

string PropertyWrapper::getName() {
	return this->memberSymbol->getName();
}

int PropertyWrapper::getWrapperType() {
	return MemberWrapper::PROPERTY_WRAPPER;
}


MethodWrapper::MethodWrapper(TypeExpression* type, Method* method) {
	this->setTypeExpr(type);
	this->methodSymbol = method;
}

string MethodWrapper::getUniqueName() {
	return this->methodSymbol->getUniqueName();
}

string MethodWrapper::getName() {
	return this->methodSymbol->getName();
}

string MethodWrapper::getLabel() {
	return this->methodSymbol->getLabel();
}

int MethodWrapper::getWrapperType() {
	return MemberWrapper::METHOD_WRAPPER;
}

TypeFunction* MethodWrapper::getMethodType() {
	return dynamic_cast<TypeFunction*>(this->getTypeExpr());
}