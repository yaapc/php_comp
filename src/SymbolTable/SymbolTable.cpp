#include"SymbolTable.h"
#include"string.h"
#include"Scope.h"

SymbolTable::SymbolTable(){
	//init hash map
	this->map = new Symbol*[MAX_LENGTH];
	for (int i = 0; i < MAX_LENGTH; i++)
		this->map[i] = nullptr;
}

//TODO: implement destructor
SymbolTable::~SymbolTable(){

}

Symbol* SymbolTable::insert(Symbol* symbol){
	//look it up first
	Symbol* sym = this->lookup(symbol->getName());
	if (sym){ // if found
		return nullptr;	//declared, error should be logged at caller
	}
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
	int hashIndex = this->hash(name);
	if (this->get(hashIndex) != nullptr && strcmp(this->get(hashIndex)->getName(), name) == 0)
		return this->get(hashIndex);
	if (this->get(hashIndex) == nullptr)
		return nullptr;
	Symbol* next = this->get(hashIndex)->getNext();
	while (next != nullptr){
		if (strcmp(next->getName(), name) == 0) 
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

vector<Symbol*> SymbolTable::symbols() {
	vector<Symbol*> symbols_vector;
	for (int i = 0; i < MAX_LENGTH; ++i) {
		auto symbol = map[i];
		while (symbol) {
			symbols_vector.push_back(symbol);
			symbol = symbol->getNext();
		}
	}
	return symbols_vector;
}

bool SymbolTable::remove(char* name){
	int hashIndex = this->hash(name);
	if (this->get(hashIndex) != nullptr && strcmp(this->get(hashIndex)->getName(), name) == 0){
		this->map[hashIndex] = this->map[hashIndex]->getNext();
		return true;
	}

	if (this->get(hashIndex) == nullptr)
		return true;
	Symbol* next = this->get(hashIndex)->getNext();
	Symbol* prev = next;
	while (next != nullptr){
		if (strcmp(next->getName(), name) == 0){
			prev->setNext(next->getNext());
			return true;
		}
		prev = next;
		next = next->getNext();
	}
	return nullptr;
}

void reverse_list(Symbol **head) {
	Symbol *prev = nullptr;
	Symbol *current = *head;
	Symbol *next;
	while (current != nullptr) {
		next = current->node;
		current->node = prev;
		prev = current;
		current = next;
	}
	*head = prev;
}
