#include"Symbol.h"
#include"../definitions.h"
#include <sstream>


Symbol::Symbol(char* name, int type, int colNo, int lineNo){
	this->name = name;
	this->symbolType = type;
	this->next = nullptr;
	this->node = nullptr;
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

/*
=========================================
VARIABLE:
=========================================
*/

Variable::Variable(char * name, int symbolType, bool isInit, int colNo, int lineNo) : Symbol(name, symbolType, colNo, lineNo) {
	this->isInited = isInit;
	this->isConst = false;
	this->isStatic = false;
	this->variableType = "Object";
	this->id = -1;
}

Variable::Variable(char * name, int symbolType, bool isInit, int colNo, int lineNo, bool isConst, bool isStatic) : Symbol(name, symbolType, colNo, lineNo) {
	this->isInited = isInit;
	this->isStatic = isStatic;
	this->isConst = isConst;
	this->variableType = "Object";
	this->id = -1;
}

Variable::Variable(char * name, int symbolType, bool isInit, int colNo, int lineNo, bool isStatic) : Symbol(name, symbolType, colNo, lineNo) {
	this->isInited = isInit;
	this->isStatic = isStatic;
	this->isConst = false;
	this->variableType = "Object";
	this->id = -1;
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
	string name, variableType, isInit, isStatic, isConst;
	this->getName() ? name = this->getName() : name = "NULL";
	this->getVariableType() ? variableType = this->getVariableType() : variableType = "NULL";
	this->isInit() ? isInit = "Initialized" : isInit = "NOT Initialized";
	this->isStatic ? isStatic = "Static" : isStatic = "NOT Static";
	this->isConst ? isConst = "Constant" : isConst = "NOT Constant";
	return
		" VARIABLE | " + name + " | " + variableType + " | " + isInit + " | " + isStatic + " | " + isConst + " | " + to_string(id);
}

void Variable::setId(int id) {
	this->id = id;
}

int Variable::getId() {
	return this->id;
}

string Variable::getNameWithout() {
	string uniqueName = this->getName();
	return uniqueName.erase(0,1);
}
/*
=========================================
FUNCTION:
=========================================
*/

Function::Function(char* name, char* returnType, int colNo, int lineNo, Scope* bodyScope) : Symbol(name, FUNCTION, colNo, lineNo) {
	this->returnType = returnType;
	this->bodyScope = bodyScope;
	this->params = nullptr;
	this->id = -1;
}

string Function::toString(){
	string name, returnType;
	this->returnType ? returnType = this->returnType : returnType = "NULL";
	this->getName() ? name = this->getName() : name = "NULL";
	return " FUNCTION | " + name + " | " + returnType + " | " + this->generateFunctionSignature();
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

void Function::setParams(Symbol* params){
	this->params = params;
}

vector<Parameter*> Function::parameters() {
	vector<Parameter*> res;
	auto par = params;
	while (par) {
		res.push_back(dynamic_cast<Parameter*>(par));
		par = par->node;
	}
	return res;
}

void Function::setId(int id) {
	this->id = id;
}

int Function::getId() {
	return this->id;
}

string Function::generateFunctionSignature() {

	std::ostringstream os;
	os << "(";
	auto par = this->params;
	bool firstParamFlag = true;
	while (par) {
		if (!firstParamFlag)
			os << ",";
		
		Parameter* parameter = dynamic_cast<Parameter*>(par);
		os << parameter->getVariableType();

		par = par->node;

		firstParamFlag = false; //we passed the first parameter
	}
	os << ")" << ">" << this->getReturnType();
	return os.str();
}

string Function::getUniqueName() {
	string uniqueName = this->getName();
	return uniqueName+to_string(id);
}

/*
========================================
CLASS:
========================================
*/

Class::Class(char* name, int colNo, int lineNo, Scope* bodyScope) : Symbol(name, CLASS, colNo, lineNo){
	this->isAbstract = false;
	this->isFinal = false;
	this->inhertedFrom = "Object";
	this->bodyScope = bodyScope;
	this->baseClassSymbol = nullptr;
}

Class::Class(char* name, int colNo, int lineNo, bool isFinal, bool isAbstract) : Symbol(name, CLASS, colNo, lineNo) {
	this->isAbstract = isAbstract;
	this->isFinal = isFinal;
	this->inhertedFrom = "Object";
	this->bodyScope = nullptr;
	this->dataMembers = nullptr;
	this->methodMembers = nullptr;
	this->baseClassSymbol = nullptr;
}

Class::Class(int colNo, int lineNo, bool isFinal, bool isAbstract) : Symbol("", CLASS, colNo, lineNo){
	this->isAbstract = isAbstract;
	this->isFinal = isFinal;
	this->inhertedFrom = "Object";
	this->bodyScope = nullptr;
	this->dataMembers = nullptr;
	this->methodMembers = nullptr;
	this->baseClassSymbol = nullptr;
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

string Class::getInhertedFrom(){
	return this->inhertedFrom;
}

int Class::getSymbolType(){
	return CLASS;
}

string Class::toString(){
	string name, isAbstract, isFinal;
	this->getName() ? name = this->getName() : name = "NULL";
	this->isAbstract ? isAbstract = "Abstract" : isAbstract = "Not Abstract";
	this->isFinal ? isFinal = "Final" : isFinal = "Not Final";
	return " CLASS    | " + name + " | " + inhertedFrom + " | " + isAbstract + " | " + isFinal;
}

Scope* Class::getBodyScope(){
	return this->bodyScope;
}

void Class::setBodyScope(Scope* scope){
	this->bodyScope = scope;
}

Symbol* Class::addToDataMembers(DataMember* dataMem){
	//double check @dataMem @node
	dataMem->node = nullptr;
	if (this->dataMembers == nullptr){
		this->dataMembers = dataMem;
		return dataMem;
	}

	DataMember* walker = this->dataMembers;
	while (walker->node != nullptr){
		walker = dynamic_cast<DataMember*>(walker->node);
	}
	walker->node = dataMem;
	return dataMem;
}

Symbol* Class::addToMethodMembers(Method* method){
	//double check @method @node
	method->node = nullptr;
	if (this->methodMembers == nullptr){
		this->methodMembers = method;
		return method;
	}

	Method* walker = this->methodMembers;
	while (walker->node != nullptr){
		walker = dynamic_cast<Method*>(walker->node);
	}
	walker->node = method;
}

void Class::setBaseClassSymbol(Class* baseClass){
	this->baseClassSymbol = baseClass;
}

Class* Class::getBaseClassSymbol(){
	return this->baseClassSymbol;
}

Method* Class::getMethods(){
	return this->methodMembers;
}

DataMember* Class::getDataMember(){
	return this->dataMembers;
}

Class* Class::getOuterClass(){
	return this->outerClass;
}

void Class::setOuterClass(Class* outerClass){
	this->outerClass = outerClass;
}


/*
============================================
DATA MEMBER:
============================================
*/

DataMember::DataMember(char * name, bool isInit, int colNo, int lineNo) : Variable(name, DATA_MEMBER, isInit, colNo, lineNo){
	this->accessModifier = PRIVATE_ACCESS;
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
	this->isDefaultConstr = false;
}

Method::Method(char* name, char* returnType, int colNo, int lineNo, Scope* bodyScope)
: Function(name, returnType, colNo, lineNo, bodyScope){
	this->accessModifier = PRIVATE_ACCESS;
	this->storageModifier = DEFAULT;
	this->isConstructor = false;
	this->isAbstract = false;
	this->isDefaultConstr = false;
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
	if (this->isConstructor){
		if (this->isDefaultConstr)
			isConstructor = "Is Default Constructor";
		else
			isConstructor = "Is Constructor";
	}
	else { isConstructor = "Not Constructor"; }

	this->isAbstract ? isAbstract = "is Abstract" : isAbstract = " NOT abstarct";
	switch (this->accessModifier){
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

bool Method::isFinal(){
	return (this->storageModifier == FINAL_STORAGE || this->storageModifier == FINAL_STATIC_STORAGE) ?  true :  false;
}

/*
============================================
PARAMETER:
============================================
*/
Parameter::Parameter(char * name, int colNo, int lineNo, bool isDefault) : Variable(name, PARAMETER, true, colNo, lineNo){ // a parameter is always inited
	this->isDefault = isDefault;
}


/*
=========================
ERROR SYMBOL:
=========================
*/

ErrorSymbol::ErrorSymbol(char* name, int colNo, int lineNo) :Symbol(name, ERROR_SYMBOL, colNo, lineNo){
	this->errorId = 0;
}

ErrorSymbol::ErrorSymbol(char* name, int colNo, int lineNo, int errId) : Symbol(name, ERROR_SYMBOL, colNo, lineNo){
	this->errorId = errId;
}

int	ErrorSymbol::getSymbolType(){
	return ERROR_SYMBOL;
}

string ErrorSymbol::toString(){
	string name;
	this->getName() ? name = this->getName() : name = "NULL";

	return
		" ERROR SYMBOL | " + name;
}
