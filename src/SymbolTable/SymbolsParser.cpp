#include "SymbolsParser.h"

SymbolsParser::SymbolsParser(){
	//init root scope 
	this->rootScope = new Scope(nullptr);
	//set the current scope to root scope
	this->currScope = this->rootScope;
	//init our error recovery object:
	this->errRecovery = &errorRec;

	this->currClassSym = nullptr;
	this->classesStack = new stack<Class*>();
}

//TODO : implement destructor.
SymbolsParser::~SymbolsParser(){

}

void SymbolsParser::setCurrentScope(Scope* currScope){
	this->currScope = currScope;
}

Scope* SymbolsParser::getCurrentScope(){
	return this->currScope;
}

Scope* SymbolsParser::getRootScope(){
	return this->rootScope;
}

Class* SymbolsParser::getCurrentClassSym(){
	return this->classesStack->top();
}

void SymbolsParser::setCurrentClassSym(Class* sym){
	this->currClassSym = sym;
}


Symbol* SymbolsParser::insertSymbol(Symbol* symbol){
	//invoke @currScope's symbolTable insert method
	Symbol* insertedSym = this->currScope->getSymbolTable()->insert(symbol);
	while (!insertedSym){ //error when trying to insert the new symbol		
		this->errRecovery->errQ->enqueue(symbol->getLineNo(), symbol->getColNo(), "Identifier Already Declared.", symbol->getName());
		//TODO:change name of symbol, insert it and continue:
		symbol->setName(string(symbol->getName()) + "_ERR");
		insertedSym = this->currScope->getSymbolTable()->insert(symbol); // reinsert it.
	}
	
	return insertedSym;
}

Symbol* SymbolsParser::insertSymbol(Symbol* symbol, Scope* scope){
	//invoke @currScope's symbolTable insert method
	Symbol* insertedSym = scope->getSymbolTable()->insert(symbol);
	while (!insertedSym){ //error when trying to insert the new symbol		
		this->errRecovery->errQ->enqueue(symbol->getLineNo(), symbol->getColNo(), "Identifier Already Declared.", symbol->getName());
		//TODO:change name of symbol, insert it and continue:
		symbol->setName(string(symbol->getName()) + "_ERR");
		insertedSym = scope->getSymbolTable()->insert(symbol); // reinsert it.
	}

	return insertedSym;
}
/*
* using 'most closely nested' scoping rule 
*/
Symbol* SymbolsParser::lookUpSymbol(char* name,int lineNo,int colNo){
	
	Scope* scanningScope = this->currScope; // starts searching from the current scope
	while (scanningScope != nullptr){ // while not out of root scope
		//invoke @scanningScope's symbolTable lookup Method
		Symbol* symbol = scanningScope->getSymbolTable()->lookup(name);
		if (symbol)
			return symbol;
		else
			scanningScope = scanningScope->getParentScope(); // let's search outer scope 
	}
	int errId = this->errRecovery->errQ->enqueue(lineNo, colNo, "Undefined", name);
	//if a symbol not found, insert an error symbol for forward declaration
	this->insertSymbol(new ErrorSymbol(name, colNo, lineNo, errId));
	
	return nullptr;
	
}


/*
 * looks up a symbol from a given scope 
 */
Symbol* SymbolsParser::lookUpSymbol(Scope* scope, char* name, int lineNo, int colNo){
	Scope* scanningScope = scope; // starts searching from the current scope
	while (scanningScope != nullptr){ // while not out of root scope
		//invoke @scanningScope's symbolTable lookup Method
		Symbol* symbol = scanningScope->getSymbolTable()->lookup(name);
		if (symbol)
			return symbol;
		else
			scanningScope = scanningScope->getParentScope(); // let's search outer scope 
	}
	int errId = this->errRecovery->errQ->enqueue(lineNo, colNo, "Undefined", name);
	//if a symbol not found, insert an error symbol for forward declaration
	this->insertSymbol(new ErrorSymbol(name, colNo, lineNo, errId));

	return nullptr;
}


/*
 * looks up a symbole from a given scope
 * invoked when searching for symbols in Scopes.
 */
Symbol* SymbolsParser::lookUpSymbol(Scope* scope, char* name) {
	Scope* scanningScope = scope; // starts searching from the current scope
	while (scanningScope != nullptr) { // while not out of root scope
									   //invoke @scanningScope's symbolTable lookup Method
		Symbol* symbol = scanningScope->getSymbolTable()->lookup(name);
		if (symbol)
			return symbol;
		else
			scanningScope = scanningScope->getParentScope(); // let's search outer scope 
	}
	return nullptr;
}

void SymbolsParser::goUp(){
	this->currScope = this->currScope->getParentScope();
}

void SymbolsParser::printSymbolTables(){
	std::ofstream os;
	os.open("scope.txt", std::ofstream::out);
	os << this->buildTableString(rootScope) << std::endl;
	os.close();
}


string SymbolsParser::buildTableString(Scope* scope){
	string str;
	str = scope->getSymbolTable()->toString();
	Scope* innersWalker = scope->getInnerScope();
	while (innersWalker){
		str.append(this->buildTableString(innersWalker));
		innersWalker = innersWalker->getNextScope();
	}
	return str;
}

Symbol* SymbolsParser::insertFunctionSymbol(char* name, char* returnType,int colNo, int lineNo, Scope* scope, Symbol* params){
	//we insert the function symbol
	Function* funcSym = new Function(name, returnType, colNo, lineNo, scope);
	this->insertSymbol(funcSym, scope->getParentScope());
	//set the scope's owner pointer to the function symbol
	scope->setOwnerSymbol(funcSym);
	funcSym->setBodyScope(scope);
	this->insertParams(params, scope);

	//linking params symbols to the function symbol
	funcSym->setParams(params);
	funcSym->generateFunctionSignature();

	return funcSym;
}

/*
  this function is called when decalring functions or class methods, it's used to insert the params symbols in the body scope.
  when @scope is null, it means we're inserting params in a null scope (i.e. abstract method) because abstract methods has no body scopes
  to resolve this we create the body scope and insert it as an inner scope of the current scope.
*/
Scope* SymbolsParser::insertParams(Symbol* paramSymbol,Scope* scope){

	if (scope == nullptr && paramSymbol != nullptr){
		scope = new Scope(this->currScope);
		this->currScope->addToInnerScopes(scope);
	}

	Symbol* walker = paramSymbol;
	while (walker != nullptr){
		this->insertSymbol(walker, scope);
		walker = walker->node;
	}

	return scope;
}

Symbol* SymbolsParser::finishClassInsertion(char* inhertedFrom, Class* classSymbol, Scope* scope){

	//if there is extends_from add it to @Class.inhertedFrom
	if (inhertedFrom != nullptr)
		classSymbol->setInhertedFrom(inhertedFrom);

	//** DEFAULT CONSTRUCTOR CHECKING:
	if (scope->getSymbolTable()->lookup(classSymbol->getName()) == nullptr) { // look up  in the class' scope for a constructor i.e look for any symbol with the same name of the class
		Method* constructor = new Method(classSymbol->getName(), nullptr, classSymbol->getColNo(), classSymbol->getLineNo(), nullptr, PUBLIC_ACCESS , DEFAULT_STORAGE);
		constructor->isConstructor = true;//TODO : encapsualte in a constructor
		constructor->isDefaultConstr = true;
		insertSymbol(constructor, scope);
		classSymbol->addToMethodMembers(constructor);
	}
	classSymbol->setBodyScope(scope);
	scope->setOwnerSymbol(classSymbol);

	return classSymbol;
}

Symbol* SymbolsParser::finishDataMembersDeclaration(DataMember* dataMem, int *modifiers,int modCount, char* type){
	DataMember* walker = dataMem;
	while (walker != nullptr){ // TODO: document this 
		walker->setVariableType(type);
		checkModifiersAndSet(walker, modifiers, modCount);

		//for every data member 
		//add it to current class in declaration process
		this->getCurrentClassSym()->addToDataMembers(walker);


		DataMember* prevNode = walker; // used to clear @node
		walker = dynamic_cast<DataMember*>(walker->node);
		prevNode->node = nullptr; // remove the pointer to chain, no need for it anymore.
	}
	return dataMem;
}

Symbol* SymbolsParser::joinSymbolsLists(Symbol* firstList, Symbol* secondList){
	
	if (firstList == nullptr)
		return secondList;
	
	Symbol* walker = firstList;
	Symbol* prev = walker;
	while (walker != nullptr){
		prev = walker;
		walker = walker->node;
	}
	prev->node = secondList;
	return firstList;
}

//TODO: remove this method
Symbol* SymbolsParser::finishMethodDeclaration(Method* methodSymbol, char* returnType, Scope* bodyScope, Symbol* paramSymbol){
		return methodSymbol;
}

Symbol* SymbolsParser::insertMethodSymbol(char* name, int colNo, int lineNo, int* modifiers, int modsCount,char* returnType, Scope* bodyScope, Symbol* paramSymbol){
	
	// no body scope and no return type
	Method* methodSymbol = new Method(name, nullptr, colNo, lineNo, nullptr);
	checkModifiersAndSet(methodSymbol, modifiers, modsCount);
	

	//if we have a bodyScope (i.e not an abstract method) 
	//insert the method scope in the parent of the bodyScope
	//else insert in the current scope, which is the appropriate place.
	if (bodyScope != nullptr)
		this->insertSymbol(methodSymbol, bodyScope->getParentScope());
	else
		this->insertSymbol(methodSymbol);


	if (returnType == nullptr) // should be a constructor
		methodSymbol->isConstructor = true;
	else
		methodSymbol->setReturnType(returnType);

	if (bodyScope == nullptr){ // no body scope i.e abstract
		methodSymbol->isAbstract = true;
		if (methodSymbol->getAccessModifier() == PRIVATE_ACCESS)
			this->errRecovery->errQ->enqueue(lineNo, colNo, "private access modifier for an abstract method is not allowed", "");
	}
	methodSymbol->setBodyScope(bodyScope);
	if (bodyScope != nullptr) // if we have a body scope , i.e. we don't have an abstract method
		bodyScope->setOwnerSymbol(methodSymbol);

	//insert params in scope
	if (bodyScope != nullptr) // if we're not at an abstract method add the params to the @bodyScope
		Scope* scope = this->insertParams(paramSymbol, bodyScope);
	
	//link params to method symbol
	methodSymbol->setParams(paramSymbol);

	if (bodyScope != nullptr)
		bodyScope->setOwnerSymbol(methodSymbol);

	
	//add to current class in declaration process:
	this->getCurrentClassSym()->addToMethodMembers(methodSymbol);
	methodSymbol->generateLabel(this->getCurrentClassSym()->getName());//generate label and signs based on class name

	return methodSymbol;
}

Scope* SymbolsParser::createNewScope(bool &flag){
	if (flag){
		flag = false;
		return nullptr;
	}
	
	return this->createNewScope();
}

Scope* SymbolsParser::createNewScope(){
	Scope* scope = new Scope(this->getCurrentScope());
	this->getCurrentScope()->addToInnerScopes(scope);
	this->setCurrentScope(scope);
	return scope;
}

void SymbolsParser::checkModifiersAndSet(DataMember *mem, int* modifiers, int modCount){
	int mods[] = { 0, 0 }; // the modifiers that will be assigned, the first will be the access modifier, the second is the storage.
	
	if (modCount == 0){ // no modifiers
		mem->setAccessModifier(PRIVATE_ACCESS); // the default access
		mem->setStorageModifier(DEFAULT); // the default storage
		return;
	}

	for (int i = 0; i < modCount; i++){
		switch (modifiers[i]){
		case PRIVATE_ACCESS :
			if (mods[0] != 0 && mods[0] == PRIVATE_ACCESS)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " repeated modifier - private", "");
			else if (mods[0] != 0)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " illegal combination of modifiers", "");
			else
				mods[0] = PRIVATE_ACCESS;
			break;
		case PUBLIC_ACCESS :
			if (mods[0] != 0 && mods[0] == PUBLIC_ACCESS)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " repeated modifier - public", "");
			else if (mods[0] != 0)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " illegal combination of modifiers", "");
			else
				mods[0] = PUBLIC_ACCESS;
			break;
		case PROTECTED_ACCESS :
			if (mods[0] != 0 && mods[0] == PROTECTED_ACCESS)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " repeated modifiers - protected", "");
			else if (mods[0] != 0)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " illegal combination of modifiers", "");
			else
				mods[0] = PROTECTED_ACCESS;
			break;

		case FINAL_STORAGE :
			if (mods[1] != 0 && (mods[1] == FINAL_STORAGE || mods[1] == FINAL_STATIC_STORAGE))
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " repeated modifier - final", "");
			else if (mods[1] != 0 && mods[1] == STATIC_STORAGE)
				mods[1] = FINAL_STATIC_STORAGE;
			else
				mods[1] = FINAL_STORAGE;
			break;
		case STATIC_STORAGE :
			if (mods[1] != 0 && (mods[1] == STATIC_STORAGE || mods[1] == FINAL_STATIC_STORAGE))
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " repeated modifier - static", "");
			else if (mods[1] != 0 && mods[1] == FINAL_STATIC_STORAGE)
				mods[1] = FINAL_STATIC_STORAGE;
			else
				mods[1] = STATIC_STORAGE;
			break;
		case DEFAULT :
			mods[0] = PRIVATE_ACCESS;
			mods[1] = DEFAULT_STORAGE;
			break;
		}
	}
	mem->setAccessModifier(mods[0]);
	if (mods[1] == 0)
		mem->setStorageModifier(DEFAULT_STORAGE);
	else
		mem->setStorageModifier(mods[1]);
}

void SymbolsParser::checkModifiersAndSet(Method *mem, int* modifiers, int modCount){
	int mods[] = { 0, 0 }; // the modifiers that will be assigned, first one will be the access modifier, the second is the storage.

	if (modCount == 0){ // no modifiers
		mem->setAccessModifier(PRIVATE_ACCESS); // the default access
		mem->setStorageModifier(DEFAULT); // the default storage
		return;
	}

	for (int i = 0; i < modCount; i++){
		switch (modifiers[i]){
		case PRIVATE_ACCESS:
			if (mods[0] != 0 && mods[0] == PRIVATE_ACCESS)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " repeated modifier", "");
			else if (mods[0] != 0)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " illegal combination of modifiers", "");
			else
				mods[0] = PRIVATE_ACCESS;
			break;
		case PUBLIC_ACCESS:
			if (mods[0] != 0 && mods[0] == PUBLIC_ACCESS)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " repeated modifier", "");
			else if (mods[0] != 0)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " illegal combination of modifiers", "");
			else
				mods[0] = PUBLIC_ACCESS;
			break;
		case PROTECTED_ACCESS:
			if (mods[0] != 0 && mods[0] == PROTECTED_ACCESS)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " repeated modifier", "");
			else if (mods[0] != 0)
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " illegal combination of modifiers", "");
			else
				mods[0] = PROTECTED_ACCESS;
			break;

		case FINAL_STORAGE:
			if (mods[1] != 0 && (mods[1] == FINAL_STORAGE || mods[1] == FINAL_STATIC_STORAGE))
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " repeated modifier", "");
			else if (mods[1] != 0 && mods[1] == STATIC_STORAGE)
				mods[1] = FINAL_STATIC_STORAGE;
			else
				mods[1] = FINAL_STORAGE;
			break;
		case STATIC_STORAGE:
			if (mods[1] != 0 && (mods[1] == STATIC_STORAGE || mods[1] == FINAL_STATIC_STORAGE))
				this->errRecovery->errQ->enqueue(mem->getLineNo(), mem->getColNo(), " repeated modifier", "");
			else if (mods[1] != 0 && mods[1] == FINAL_STATIC_STORAGE)
				mods[1] = FINAL_STATIC_STORAGE;
			else
				mods[1] = STATIC_STORAGE;
			break;
		case DEFAULT:
			mods[0] = PRIVATE_ACCESS;
			mods[1] = DEFAULT_STORAGE;
			break;
		}
	}
	mem->setAccessModifier(mods[0]);
	if (mods[1] == 0)
		mem->setStorageModifier(DEFAULT_STORAGE);
	else
		mem->setStorageModifier(mods[1]);
}

void SymbolsParser::pushToClassesStack(Class* classSym){
	this->classesStack->push(classSym);
}

void SymbolsParser::popFromClassesStack(){
	return this->classesStack->pop();
}

int SymbolsParser::getStaticsCounter() {
	return this->getRootScope()->getStaticsCounter();
}

int SymbolsParser::addToStaticsCounter() {
	return this->getRootScope()->addToStaticsCounter();
}