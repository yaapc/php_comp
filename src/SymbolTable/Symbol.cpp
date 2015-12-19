#include"Symbol.h"
#include"../definitions.h"

Symbol::Symbol(char* name, int type, int colNo, int lineNo){
	this->name = name;
	this->symbolType = type;
	this->next = nullptr;
	this->colNo = colNo;
	this->lineNo = lineNo;
}

Symbol::~Symbol(){

}

void Symbol::setSymbolType(int type){
	this->symbolType = type;
}

void Symbol::setName(string name){
	this->name = _strdup(name.c_str()); // converting string to char*
}

char* Symbol::getName(){
	return this->name;
}

Symbol* Symbol::getNext(){
	return this->next;
}

Symbol* Symbol::setNext(Symbol* symbol){
	this->next = symbol;
	return symbol;
}

int Symbol::getColNo(){
	return this->colNo;
}

int Symbol::getLineNo(){
	return this->lineNo;
}

Variable::Variable(char * name, int symbolType, bool isInit, int colNo, int lineNo): Symbol(name, symbolType, colNo, lineNo) {	
	this->isInited = isInit;
}

int Variable::getSymbolType(){
	return VARIABLE;
}

char* Variable::getVariableType(){
	return this->variableType;
}

void Variable::setVariableType(char* type){
	this->variableType = type;
}

bool Variable::isInit(){
	return this->isInited;
}

string Variable::toString(){
	string name, variableType, isInit;
	this->getName()         ? name = this->getName()                 : name = "NULL";
	this->getVariableType() ? variableType = this->getVariableType() : variableType = "NULL";
	this->isInit()          ? isInit = "Initialized"                 : isInit = "NOT Initialized";
	return 
		" VARIABLE | " + name + " | " + variableType + " | " + isInit;
}


/*
=========================================
FUNCTION:
=========================================
*/

Function::Function(char* name, char* returnType, int colNo, int lineNo, Scope* bodyScope) : Symbol(name, FUNCTION, colNo, lineNo) {
	this->returnType = returnType;
	this->bodyScope = bodyScope;
}

string Function::toString(){
	string name, returnType;
	this->returnType ? returnType = this->returnType : returnType = "NULL";
	this->getName() ? name = this->getName() : name = "NULL";
	return " FUNCTION | " + name + " | " + returnType;
}

int Function::getSymbolType(){
	return FUNCTION;
}

void Function::setReturnType(char* returnType){
	this->returnType = returnType;
}

char* Function::getReturnType(){
	return this->returnType;
}

Scope* Function::getBodyScope(){
	return this->bodyScope;
}

void Function::setBodyScope(Scope* bodyScope){
	this->bodyScope = bodyScope;
}
/*
========================================
CLASS:
========================================
*/

Class::Class(char* name, int colNo, int lineNo, Scope* bodyScope): Symbol(name, CLASS, colNo, lineNo){
	this->isAbstract = false;
	this->isFinal = false;
	this->inhertedFrom = "Object"; 
	this->bodyScope = bodyScope;

}

Class::Class(int colNo, int lineNo, bool isFinal, bool isAbstract) : Symbol("", CLASS, colNo, lineNo){
	this->isAbstract = isAbstract;
	this->isFinal = isFinal;
	this->inhertedFrom = "Object";
	this->bodyScope = nullptr;

}

void Class::setAsAbstract(){
	this->isAbstract = true;
}

void Class::setAsFinal(){
	this->isFinal = true;
}

void Class::setInhertedFrom(string inhertedFrom){
	this->inhertedFrom = inhertedFrom;
}

int Class::getSymbolType(){
	return CLASS;
}

string Class::toString(){
	string name, isAbstract, isFinal;
	this->getName() ? name = this->getName() : name = "NULL";
	this->isAbstract ? isAbstract = "Abstract" : isAbstract = "Not Abstract";
	this->isFinal ? isFinal = "Final" : isFinal = "Not Final";
	return " CLASS    | " + name + " | " + inhertedFrom  + " | " + isAbstract + " | " + isFinal;
}

Scope* Class::getBodyScope(){
	return this->bodyScope;
}

void Class::setBodyScope(Scope* scope){
	this->bodyScope = scope;
}
/*
============================================
DATA MEMBER:
============================================
*/

DataMember::DataMember(char * name, bool isInit, int colNo, int lineNo) : Variable(name, DATA_MEMBER,isInit, colNo, lineNo){
	this->accessModifier = DEFAULT_ACCESS;
	this->storageModifier = DEFAULT_STORAGE;
};

int DataMember::getAccessModifier(){
	return this->accessModifier;
}

void DataMember::setAccessModifier(int accessModifier){
	this->accessModifier = accessModifier;
}

int DataMember::getStorageModifier(){
	return this->storageModifier;
}

void DataMember::setStorageModifier(int storageModifier){
	this->storageModifier = storageModifier;
}

int DataMember::getSymbolType(){
	return DATA_MEMBER;
}

string DataMember::toString(){
	string name, dataMemberType, isInit, accessModifier, storageModifier;
	this->getName() ? name = this->getName() : name = "NULL";
	this->getVariableType() ? dataMemberType = this->getVariableType() : dataMemberType = "NULL";
	this->isInit() ? isInit = "Initialized" : isInit = "NOT Initialized";
	switch (this->accessModifier){
		case DEFAULT_ACCESS:    accessModifier = "DEFAULT ACCESS"; break;
		case PUBLIC_ACCESS:     accessModifier = "PUBLIC ACCESS"; break;
		case PRIVATE_ACCESS:    accessModifier = "PRIVATE ACCESS"; break;
		case PROTECTED_ACCESS:  accessModifier = "PROTECTED ACCESS"; break;
	}
	switch (this->storageModifier){
		case DEFAULT_STORAGE: storageModifier = "DEFAULT STORAGE"; break;	
		case STATIC_STORAGE: storageModifier = "STATIC STORAGE"; break;
		case FINAL_STORAGE: storageModifier = "FINAL STORAGE"; break;
		case FINAL_STATIC_STORAGE: storageModifier = "FINAL STATIC STORAGE"; break;
	}
	return
		" DATA MEMBER | " + name + " | " + dataMemberType + " | " + isInit + " | " + accessModifier + " | " + storageModifier;
}

/*
============================================
Method:
============================================
*/

Method::Method(char* name, char* returnType, int colNo, int lineNo, Scope* bodyScope, int accessModifier, int storageModifier) 
				: Function(name, returnType, colNo, lineNo, bodyScope){
	this->accessModifier = accessModifier;
	this->storageModifier = storageModifier;
	this->isConstructor = false;
	this->isAbstract = false;
}

int Method::getAccessModifier(){
	return this->accessModifier;
}

void Method::setAccessModifier(int accessModifier){
	this->accessModifier = accessModifier;
}

int Method::getStorageModifier(){
	return this->storageModifier;
}

void Method::setStorageModifier(int storageModifier){
	this->storageModifier = storageModifier;
}

int Method::getSymbolType(){
	return METHODS;
}

string Method::toString(){
	string name, returnType, isConstructor, isAbstract, accessModifier, storageModifier;
	this->getName() ? name = this->getName() : name = "NULL";
	this->getReturnType() ? returnType = this->getReturnType() : returnType = "NULL";
	this->isConstructor ? isConstructor = "is Constructor" : isConstructor = "NOT Constructor";
	this->isAbstract ? isAbstract = "is Abstract" : isAbstract = " NOT abstarct";
	switch (this->accessModifier){
	case DEFAULT_ACCESS:    accessModifier = "DEFAULT ACCESS"; break;
	case PUBLIC_ACCESS:     accessModifier = "PUBLIC ACCESS"; break;
	case PRIVATE_ACCESS:    accessModifier = "PRIVATE ACCESS"; break;
	case PROTECTED_ACCESS:  accessModifier = "PROTECTED ACCESS"; break;
	}
	switch (this->storageModifier){
	case DEFAULT_STORAGE: storageModifier = "DEFAULT STORAGE"; break;
	case STATIC_STORAGE: storageModifier = "STATIC STORAGE"; break;
	case FINAL_STORAGE: storageModifier = "FINAL STORAGE"; break;
	case FINAL_STATIC_STORAGE: storageModifier = "FINAL STATIC STORAGE"; break;
	}
	return
		" CLASS METHOD | " + name + " | " + returnType + " | " + isConstructor + " | " + accessModifier + " | " + storageModifier + " | " + isAbstract;
}

/*
============================================
PARAMETER:
============================================
*/
Parameter::Parameter(char * name, int colNo, int lineNo, bool isDefault) : Variable(name, PARAMETER, true, colNo, lineNo){ // a parameter is always inited
	this->isDefault = isDefault;
}
