#include"SymbolTable.h"
#include"string.h"
#include"Scope.h"

SymbolTable::SymbolTable(){
	//init hash map
	this->map = new Symbol* [MAX_LENGTH];
	for (int i = 0; i < MAX_LENGTH; i++)
		this->map[i] = nullptr;
}

//TODO: implement destructor
SymbolTable::~SymbolTable(){

}

Symbol* SymbolTable::insert(Symbol* symbol){
	//look it up first 
	if (this->lookup(symbol->getName())) // if found
		return nullptr;	//declared, error should be logged at caller

	int hashIndex = this->hash(symbol->getName());
	Symbol* next = this->get(hashIndex);
	if (next == nullptr){ //empty bucket
		return this->put(hashIndex, symbol);
	}
	//non-empty bucket 
	while (next->getNext()){
		next = next->getNext();
	}
	next->setNext(symbol); // append to bucket
	return symbol;

}

Symbol* SymbolTable::lookup(char* name){
	int hashIndex = this -> hash(name);
	if (this->get(hashIndex) != nullptr && strcmp(this->get(hashIndex)->getName(), name) == 0)
		return this->get(hashIndex);
	if (this->get(hashIndex) == nullptr)
		return nullptr;
	Symbol* next = this->get(hashIndex)->getNext();
	while (next != nullptr){
		if (next->getName() == name)
			return next;
		next = next->getNext();
	}
	return nullptr;
}

int SymbolTable::hash(char* name){
	unsigned int i;
	int retVal = 0;
	for (i = 0; i < strlen(name); i++)
	{
		retVal <<= 2;
		retVal ^= (int)(*name);
		name++;
	}
	return (retVal % MAX_LENGTH);
}

Symbol* SymbolTable::get(int index){
	return this->map[index];
}
Symbol* SymbolTable::put(int index, Symbol* symbol){
	this->map[index] = symbol;
	return symbol;
}

string SymbolTable::toString(){
	//TODO : link symbol table to its scope
	string owner;
	this->getOwnerScope()->getOwnerSymbol() != nullptr ? owner = this->getOwnerScope()->getOwnerSymbol()->getName() : owner = "NULL";
	string str = "Scope of " + owner + "\r\n";
	for (int i = 0; i < MAX_LENGTH; i++){
		Symbol* symWalker = this->map[i]; // symbols(bucket) walker
		while (symWalker != nullptr){
			str.append(symWalker->toString() + "\r\n");
			symWalker = symWalker->getNext();
		}
	}
	str.append("-----------------------\r\n");
	return str;
}

void SymbolTable::setOwnerScope(Scope* owner){
	this->ownerScope = owner;
}


Scope* SymbolTable::getOwnerScope(){
	return this->ownerScope;
}
