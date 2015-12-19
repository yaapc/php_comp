#include"Scope.h"

Scope::Scope(Scope* parentScope){
	this->parentScope = parentScope;
	this->innerScope = nullptr; // no childs for now 
	this->nextScope = nullptr; // no brothers for now 
	this->symTable = new SymbolTable();
	this->symTable->setOwnerScope(this); // point to the scope of the symbol table
	this->ownerSymbol = nullptr;
}

//TODO: implement destructor
Scope::~Scope(){

}

void Scope::setParentScope(Scope* scope){
	this->parentScope = scope;
}

Scope* Scope::getParentScope(){
	return this->parentScope;
}

void Scope::addToInnerScopes(Scope* scope){
	if (this->innerScope == nullptr){
		innerScope = scope;
		return;
	}
	Scope* lastInnerScope = this->innerScope;
	while (lastInnerScope->nextScope != nullptr){
		lastInnerScope = lastInnerScope->nextScope;
	}
	lastInnerScope->nextScope = scope;
	return;
}

Scope* Scope::getInnerScope(){
	return this->innerScope;
}

void Scope::setNextScope(Scope* scope){
	this->nextScope = scope;
}

Scope* Scope::getNextScope(){
	return this->nextScope;
}

SymbolTable* Scope::getSymbolTable(){
	return this->symTable;
}


void Scope::setOwnerSymbol(Symbol* owner){
	this->ownerSymbol = owner;
}

Symbol* Scope::getOwnerSymbol(){
	return this->ownerSymbol;
}
