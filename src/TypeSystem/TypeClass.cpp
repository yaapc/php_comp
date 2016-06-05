#pragma once
#include "TypeInteger.hpp"
#include "TypeBoolean.hpp"
#include "TypeError.hpp"
#include "TypeClass.hpp"
#include "../AST/ClassCallNode.hpp"

//static defination
vector<TypeClass*> TypeClass::classInstances;
vector<ClassDefineNode*> TypeClass::errorTypeClasses;
int TypeClass::idsCounter = TYPES_COUNTER;

TypeExpression* TypeClass::buildClass(ClassDefineNode* classNode, Class* classSymbol) {
	
	bool isDefined = TypeClass::isDefined(classSymbol->getName());
			
	if (isDefined) // a TypeClass already defined with the same name.
		return new TypeError(string(classSymbol->getName()) + " class is already defined.");

	
	//***  build the type:
	TypeClass* typeClass = new TypeClass(classSymbol->getName());
	typeClass->parentClass = nullptr;

	if (strcmp(classSymbol->getName(), "Object") != 0 ) { // if we are not building Object Class, then we must have a base class to inherit from 
		//get base class TypeClass
		TypeExpression* instance = TypeClass::getInstance(classSymbol->getInhertedFrom());
		//check if base class exists
		if (instance == nullptr){ // base class not defined... might yet.
			TypeClass::errorTypeClasses.push_back(classNode);
			return new TypeError(classSymbol->getInhertedFrom() + " class is undefined"); //cancel creating of type 
		}

		TypeClass* baseType = dynamic_cast<TypeClass*>(instance);
		typeClass->parentClass = baseType;
		//append all members of base class to the current class:
		for (auto &member : baseType->getMembers()) {
			typeClass->addToMembers(member);
		}
	}

	//extract properties' types and symbols from @classNode and append them to the TypeClass properties, as a @PropertyWrapper
	for (int i = 0; i < classNode->classMemNodes.size(); i++) {
		
		classNode->classMemNodes.at(i)->type_checking(); // let classNode recheck type in cases of second passes
		PropertyWrapper* prop = 
			new PropertyWrapper(classNode->classMemNodes.at(i)->getNodeType(),
				classNode->classMemNodes.at(i)->getMemSymbol());
		
		//check if there is an error in property definition
		if (dynamic_cast<TypeError*>(prop->getTypeExpr()) != nullptr) { // there is an error
			//DON't cancel creating of TypeClass but append the ClassDefineNode to errorClasses so we can 
			//redefine in future passes
			TypeClass::errorTypeClasses.push_back(classNode);
		}
		
		//if it's a static property, then just add it to @staticProps
		if (prop->isStatic()) {
			typeClass->staticMembers.push_back(prop);
		}
		else {
			typeClass->addToMembers(prop);
			typeClass->addToProps(prop);
		}
	}	


	//extract methods' types and symbols from @classNode and append them to the TypeClass members as a @MethodWrapper
	bool foundConstruct = false; // a constructor flag, to prevent inserting a default one
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
					typeClass->methods.push_back(dynamic_cast<MethodWrapper*>(baseMember));
					isOverriding = true;
					break;//out of typeClass->members
				}
			}
		}
		if (!isOverriding) {
			//check if a constructor
			if (method->methodSym->isConstructor) {
				foundConstruct = true;
				//set return type of constructor to TypeClass we are building
				dynamic_cast<TypeFunction*>(method->getNodeType())->setReturnTypeExpression(typeClass);
			}

			MethodWrapper* methodWrapper = new MethodWrapper(method->getNodeType(), method->methodSym);

			if (methodWrapper->isStatic()){
				typeClass->staticMembers.push_back(methodWrapper);
			}else{
				typeClass->members.push_back(methodWrapper);
				typeClass->methods.push_back(methodWrapper);
			}
		}
	}

	//if no constructor defined, define a default one
	if (!foundConstruct) {
		Method* conSym = dynamic_cast<Method*>(classSymbol->getBodyScope()->getSymbolTable()->lookup(classSymbol->getName()));
		conSym->generateLabel(classSymbol->getName());
		conSym->generateFunctionSignature();//tell constructor to generate signatures
		TypeFunction* conType = dynamic_cast<TypeFunction*>(TypeFunction::buildConstructor(conSym));
		conType->setReturnTypeExpression(typeClass);
		MethodWrapper* methodWrapper = new MethodWrapper(conType, conSym);
		
		
		typeClass->members.push_back(methodWrapper);
		typeClass->methods.push_back(methodWrapper);
	}


	//and finally, add the typeClass we are building to the TypeClasses we have:
	TypeClass::classInstances.push_back(typeClass);

	//all members added, tell typeClass to resize itself:
	typeClass->makeSize();

	//set id
	typeClass->typeId = TypeClass::idsCounter + 1;
	TypeClass::idsCounter++;

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


TypeExpression* TypeClass::opEqual(int secondTypeId) {
	if (secondTypeId == CLASS_TYPE_ID)
		return TypeBoolean::getInstance();
	return new TypeError(TypeSystemHelper::getTypeName(secondTypeId) + " Type doesn't support == operation");
}

//search @classInstances for a decalred class with name @name
TypeExpression* TypeClass::getInstance(string name) {
	for (auto classType : TypeClass::classInstances) {
		if (classType->getName() == name)
			return classType;
	}
	// let the caller handle the unfound event
	return nullptr;
}

TypeClass::TypeClass(string name) {
	this->name = name;
}


//TODO: I think opDot should have a string not a TypeId. 
//ex: obj.x  x is the name of the property we want to access, it doesn't have
//an id, and we don't know what type it is.
//additionally, it should have an access context.
TypeExpression* TypeClass::opDot(string memberStr, bool isMethod, string methodSign, MemberWrapper*& memWrapper, ClassCallNode* classCallNode) {

	if (!isMethod) { // calling a member
		//properties have '$' in their symbol's name,
		//but when called from a reference of an object, they're called without the '$'...
		//PHP MY ASS..
		PropertyWrapper* prop = this->lookupMembers("$" + memberStr);
		if (prop == nullptr) { // property not found
			memWrapper = nullptr;
			return nullptr;//let caller set TypeError
		}
		else {
			//TODO: check access context and what so ever....
			memWrapper = prop;
			return prop->getTypeExpr();;
		}
	}
	else { // calling a method
		MethodWrapper* method = this->lookupMembers(memberStr, methodSign, classCallNode);
		if (method == nullptr) { // method not found
			memWrapper = nullptr;
			return nullptr;//let called set TypeError
		}
		else {
			//TODO: check access context and what so ever....
			memWrapper = method;
			TypeFunction* methodType = dynamic_cast<TypeFunction*>(method->getTypeExpr());
			return methodType->getReturnTypeExpression();
		}
	}
}


/* This method tries to redefine classes in @errorTypeClasses.
   It loops the vector, If maximum of two times It didn't successfully redefine any error class,
   It breaks;
 */
bool TypeClass::tryReDefine() {
	int first, later;
	first = later = TypeClass::errorTypeClasses.size();
	while (true) {
		later = first;
		for (int i = 0; i < TypeClass::errorTypeClasses.size(); i++) {
			ClassDefineNode* classDefineNode = TypeClass::errorTypeClasses.at(i);
			TypeClass::errorTypeClasses.erase(errorTypeClasses.begin() + i);
			classDefineNode->type_checking();
			if (dynamic_cast<TypeError*>(classDefineNode->getNodeType()) == nullptr) {
				first--;
			}
		}		
		if (first == later)
			break;
	}
	if (first == 0)
		return true;
	return false;
}


MemberWrapper* TypeClass::getStaticProperty(string className, string propName) {
	for (auto &_class : TypeClass::classInstances) {
		if (_class->getName() == className) {
			for (auto &prop : _class->staticMembers) {
				if (prop->getName() == propName)
					return prop;
			}
			return nullptr;
		}
	}
	return nullptr;
}


int TypeClass::getSize() {
	return this->size; 
}

int TypeClass::getTypeId() {
	return typeId;
}


string TypeClass::getName() {
	return this->name;
}

void TypeClass::addToMembers(MemberWrapper* memberType) {
	this->members.push_back(memberType);
}

PropertyWrapper* TypeClass::lookupMembers(string memberStr) {
	//first search in class props
	for (auto member : this->props) {
		if (member->getName() == memberStr) {
			return dynamic_cast<PropertyWrapper*>(member);
		}
	}
	//if not found search on base class scope
	for (auto member : this->members) {
		if (member->getName() == memberStr) {
			return dynamic_cast<PropertyWrapper*>(member);
		}
	}
	return nullptr;
}

MethodWrapper* TypeClass::lookupMembers(string memberStr, string methodSign, ClassCallNode* classCallNode) {
	//TODO: there are a lot of redundencies in this method...

	//first search current class methods:
	for (auto member : this->methods) {
		if (member->getName() == memberStr) {
			for(auto sign : dynamic_cast<TypeFunction*>(member->getTypeExpr())->getSignatures())
				if(this->getName()+ "_" + methodSign == sign)
					return dynamic_cast<MethodWrapper*>(member);
		}
	}

	//fallback to searching with Types
	if (classCallNode != nullptr) { // not a NewNode
		MethodWrapper* wrapper = this->lookupMembers_types(memberStr, dynamic_cast<ListNode*>(classCallNode->argumentsList)->nodes);
		if (wrapper != nullptr)
			return wrapper;
	}

	//if not found in current class methods,
	//then search in super class methods:
	TypeClass* parent = dynamic_cast<TypeClass*>(this->parentClass);
	for (auto member : this->members) {
		if (member->getName() == memberStr) {
			for (auto sign : dynamic_cast<TypeFunction*>(member->getTypeExpr())->getSignatures())
				if (parent->getName() + "_" + methodSign == sign)
					return dynamic_cast<MethodWrapper*>(member);
		}
	}
	return nullptr;
}

MethodWrapper* TypeClass::lookupMembers_types(string calledMethodName, vector<Node*> args){
	for (auto methodWrapper : this->members) {
		if (methodWrapper->getName() == calledMethodName && methodWrapper->getWrapperType() == MemberWrapper::METHOD_WRAPPER) {
			TypeFunction* methodType = dynamic_cast<TypeFunction*>(methodWrapper->getTypeExpr());
			int masterArgs = methodType->getParamsTEs().size();
			int slaveArgs = args.size();
			int i = 0;
			bool diff = false;
			while (i < slaveArgs) {
				if (i == masterArgs) {
					return nullptr;
				}
				if (args.at(i)->getNodeType()->equivelantTo(methodType->getParamsTEs().at(i)->getTypeId())) {
					i++;
				}
				else {
					diff = true;
					break;
				}
			}
			if (!diff && (i == masterArgs || methodType->paramsSymbols.at(i + 1)->isDefault)) {
				return dynamic_cast<MethodWrapper*>(methodWrapper);
			}
			else
				return nullptr;
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

	if (this->getName() == "Object") {
		// :(
		this->size = size;
		return; // Object class has no parent so skip next resizing
	}

	int parentSize = parentClass->getSize();
	this->size = size + parentSize;
}

int TypeClass::equivelantTo(int secondTypeId){
	
	if (this->getTypeId() == secondTypeId)
		return secondTypeId;
	else
		if (parentClass 
			&& parentClass->equivelantTo(secondTypeId))
		return secondTypeId;
	return ERROR_TYPE_ID;	
}

vector<MemberWrapper*> TypeClass::getMembers() {
	return this->members;
}

void TypeClass::addToProps(PropertyWrapper* prop) {
	this->props.push_back(prop);
}

int TypeClass::getAccessMod(string memString) {
	int size = this->members.size();
	for (int i = 0; i < size; i++) {
		MemberWrapper* mem = this->members.at(i);
		if (mem->getName() == memString)
			return mem->getAccessModifier();
	}	
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
	this->accessModifier = this->memberSymbol->getAccessModifier();
}

string PropertyWrapper::getUniqueName() {
	return this->memberSymbol->getUniqeName();
}

string PropertyWrapper::getNameWithout(){
	return this->memberSymbol->getNameWithout();
}

string PropertyWrapper::getName() {
	return this->memberSymbol->getName();
}

int PropertyWrapper::getWrapperType() {
	return MemberWrapper::PROPERTY_WRAPPER;
}

bool PropertyWrapper::isStatic() {
	return this->memberSymbol->isStatic;
}

bool PropertyWrapper::isConst() {
	return this->memberSymbol->isConst;
}

MethodWrapper::MethodWrapper(TypeExpression* type, Method* method) {
	this->setTypeExpr(type);
	this->methodSymbol = method;
	this->returnType = dynamic_cast<TypeFunction*>(type)->getReturnTypeExpression();
	this->accessModifier = this->methodSymbol->getAccessModifier();
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

bool MethodWrapper::isStatic() {
	return this->methodSymbol->getStorageModifier() == STATIC_STORAGE;
}


bool MethodWrapper::isDefaultContructor() {
	return this->methodSymbol->isDefaultConstr;
}