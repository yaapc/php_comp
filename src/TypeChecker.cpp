#include"TypeChecker.h"


//walks the symbol table searching for classes and creating thier dependency graph
void TypeChecker::checkDependency(){
	Scope* scope = this->symbolsParser->getRootScope();
	searchScope(scope);
	
	dg->checkDependency(); // creates the graph and checks it for circular dependency 
	dg->reportCircles();
	dg->printGraph();
	std::ofstream dot_file("dependency_graph.dot");
	dg->generate_dot_file(dot_file);
	dot_file.close();
}

//searches the scope recursivly looking for classes declarations
void TypeChecker::searchScope(Scope* scope){
	if (scope == nullptr)
		return;
	parseScopeForNodes(scope);
	Scope* innersWalker = scope->getInnerScope();
	if (innersWalker == nullptr)
		return;
	while (innersWalker != nullptr){
		searchScope(innersWalker);
		innersWalker = innersWalker->getNextScope();
	}
}

//parses a given scope looking for class declaration and inserting them into the dependency graph @dg
void TypeChecker::parseScopeForNodes(Scope* scope){
	if (scope == nullptr)
		return;
	vector<Symbol*> symbols = scope->getSymbolTable()->symbols();
	vector<Symbol*>::iterator i;
	for (i = symbols.begin(); i != symbols.end(); ++i){
		Symbol* s = *i;
		int sType = s->getSymbolType();
		if ( sType == CLASS){
			Class* c = dynamic_cast<Class*>(s);
			this->dg->insertIntoNodes(c->getName(), c, c->getInhertedFrom()); // insert the caught class declaration into dependancy graph
		}
	}
}


void TypeChecker::checkForwardDeclarations(){
	this->searchScopesAndLink(this->symbolsParser->getRootScope());
	this->searchScopeForFunctions(this->symbolsParser->getRootScope());
}

void TypeChecker::searchScopesAndLink(Scope * scope){
	if (scope == nullptr)
		return;
	parseScopeForClassDecl(scope);
	Scope* innersWalker = scope->getInnerScope();
	if (innersWalker == nullptr)
		return;
	while (innersWalker != nullptr){
		searchScopesAndLink(innersWalker);
		innersWalker = innersWalker->getNextScope();
	}
}

void TypeChecker::parseScopeForClassDecl(Scope * scope){
	if (scope == nullptr)
		return;
	vector<Symbol*> symbols = scope->getSymbolTable()->symbols();
	vector<Symbol*>::iterator i;
	for (i = symbols.begin(); i != symbols.end(); ++i){
		Symbol* s = *i;
		int sType = s->getSymbolType();
		if (sType == CLASS){
			Class* c = dynamic_cast<Class*>(s);
			if (c->getInhertedFrom() != "Object"){ // we have inheritance, let's get the base class sym
				bool found = false; Scope *walker = scope;				
				while (!found && walker != nullptr){
					vector<Symbol*> currSymbols = walker->getSymbolTable()->symbols();
					vector<Symbol*>::iterator j = currSymbols.begin();
					bool foundBase = false;
					while (j != currSymbols.end() && !foundBase){
						Symbol* currSym = *j;
						if (currSym->getSymbolType() == CLASS && string(currSym->getName()) == c->getInhertedFrom()){
							if (dynamic_cast<Class*>(currSym)->isFinal){ // check if base class is final
								string errString = string(currSym->getName()) + " is Final and can't be inherited";
								this->errRecovery->errQ->enqueue(c->getLineNo(), c->getColNo(), errString.c_str(), "");
								c->setInhertedFrom("Object");
								found = true;
								foundBase = true;
							}
							else {
								c->setBaseClassSymbol(dynamic_cast<Class*>(currSym));
								foundBase = true;
								found = true;
								checkAbstraction(c);
								checkOverridingMethods(c);
							}
						}
						++j;
					}
					walker = walker->getParentScope();
				}
				if (!found) {
					if (c->getOuterClass() == nullptr){ 
						//if it's not an inner class report the error
						//otherwise, it'll be reported while checking inner's inheritance.
						string errString = string(c->getInhertedFrom()) + " is Not defined";
						this->errRecovery->errQ->enqueue(c->getLineNo(), c->getColNo(), errString.c_str(), "");
						//c->setInhertedFrom("Object");
					}
				}
			}
		}
	}
}

void TypeChecker::checkAbstraction(Class* subClass){
	if (!subClass->getBaseClassSymbol()->isAbstract){
		return; // no abstraction check required
	}

	if (subClass->isAbstract) // base class abstract and sub class also declared abstract
		return;

	Class* baseClass = subClass->getBaseClassSymbol();
	Method* walker = baseClass->getMethods();
	if (walker == nullptr){
		//the base class has no methods 
		//and declared as abstract
		//sub class should be declared too
		string errString = string(subClass->getName()) + " should be declared abstract";
		this->errRecovery->errQ->enqueue(subClass->getLineNo(), subClass->getColNo(), errString.c_str(), "");
		return; // done
	}
	//the base class has methods
	while (walker != nullptr){
		if (walker->isAbstract){ // abstract base method
			//search the sub class for overrided method
			Method* subsWalker = subClass->getMethods();
			if (subsWalker == nullptr){ // the sub class has no methods
				string errString = string(subClass->getName()) + " should be declared abstract, or override all abstract methods";
				this->errRecovery->errQ->enqueue(subClass->getLineNo(), subClass->getColNo(), errString.c_str(), "");
			}

			//the sub class has methods
			bool foundAbsMethod = false;
			while (subsWalker != nullptr){
				if (string(subsWalker->getName()) == string(walker->getName()))
					foundAbsMethod = true;	// we found the method in sub class	
				subsWalker = dynamic_cast<Method*>(subsWalker->node);
			}
			if (!foundAbsMethod){
				string errString = string(subClass->getName()) + " doesn't implement " + walker->getName();
				this->errRecovery->errQ->enqueue(subClass->getLineNo(), subClass->getColNo(), errString.c_str() , "");
			}
				
		}
		walker = dynamic_cast<Method*>(walker->node);
	}
}

void TypeChecker::checkOverridingMethods(Class* subClass){
	Class* baseClass = subClass->getBaseClassSymbol();
	Method* walker = baseClass->getMethods();

	if (walker == nullptr) // no methods
		return;

	while (walker != nullptr){
		
			Method* subsWalker = subClass->getMethods();
			while (subsWalker != nullptr){
				if (string(subsWalker->getName()) == string(walker->getName())){ // we found a method overriding the final base method
					if (walker->isFinal()){ // final base method
						string errString = string(subClass->getName()) + " can't implement final method " + walker->getName();
						this->errRecovery->errQ->enqueue(subClass->getLineNo(), subClass->getColNo(), errString.c_str(), "");
					}
					else { // the base method is not final, check access level
						switch(walker->getAccessModifier()){
							case PROTECTED_ACCESS:
								if (subsWalker->getAccessModifier() == PRIVATE_ACCESS){
									string errString = string(subClass->getName()) + " can't assign weaker access privileges for " + walker->getName();
									this->errRecovery->errQ->enqueue(subClass->getLineNo(), subClass->getColNo(), errString.c_str(), "");
								}
							break;
							case PUBLIC_ACCESS :
								string errString = string(subClass->getName()) + " can't assign weaker access privileges for " + walker->getName();
								this->errRecovery->errQ->enqueue(subClass->getLineNo(), subClass->getColNo(), errString.c_str(), "");
							break;
						}
					}
				}
				subsWalker = dynamic_cast<Method*>(subsWalker->node);
			}
		
		walker = dynamic_cast<Method*>(walker->node);
	}
}


//searches the scope recursivly looking for classes declarations
void TypeChecker::searchScopeForFunctions(Scope* scope){
	if (scope == nullptr)
		return;
	parseScopeForFunctions(scope);
	Scope* innersWalker = scope->getInnerScope();
	if (innersWalker == nullptr)
		return;
	while (innersWalker != nullptr){
		//parseScope(innersWalker);
		searchScopeForFunctions(innersWalker);
		innersWalker = innersWalker->getNextScope();
	}
}

void TypeChecker::parseScopeForFunctions(Scope* scope){
	if (scope == nullptr)
		return;
	vector<Symbol*> symbols = scope->getSymbolTable()->symbols();
	vector<Symbol*>::iterator i;
	for (i = symbols.begin(); i != symbols.end(); ++i){
		Symbol* s = *i;
		int sType = s->getSymbolType();
		if (sType == ERROR_SYMBOL){
			Symbol* lookedUpSym = this->symbolsParser->lookUpSymbol(scope->getParentScope(), s->getName());
			if (lookedUpSym != nullptr && (lookedUpSym->getSymbolType() == FUNCTION || lookedUpSym->getSymbolType() == METHODS)){
				scope->getSymbolTable()->remove(s->getName());
				this->errRecovery->removeFromQueue(dynamic_cast<ErrorSymbol*>(s)->errorId);
			}
		}
	}
}

void TypeChecker::searchScopeForInners(Scope* scope){
	if (scope == nullptr)
		return;
	vector<Symbol*> symbols = scope->getSymbolTable()->symbols();
	vector<Symbol*>::iterator i;
	for (i = symbols.begin(); i != symbols.end(); ++i){
				
		int sType = (*i)->getSymbolType();
		if (sType == CLASS){//inner class
			Class* s = dynamic_cast<Class*>(*i);
			
			searchScopeForInners(s->getBodyScope());
			checkNamingOfInners(s);
			checkInheritanceOfInners(s);

		}
	}
}

void TypeChecker::checkNamingOfInners(Class* inner){
	Class* outersWalker = inner->getOuterClass();
	while (outersWalker != nullptr){
		if (string(outersWalker->getName()) == string(inner->getName())){
			string errString = string(inner->getName()) + " can't has the same name of outer " + outersWalker->getName();
			this->errRecovery->errQ->enqueue(inner->getLineNo(), inner->getColNo(), errString.c_str(), "");
		}
		outersWalker = outersWalker->getOuterClass();
	}
}

void TypeChecker::checkInheritanceOfInners(Class* inner){
	if (inner->getBaseClassSymbol() != nullptr || inner->getInhertedFrom() == "Object") // base class found in previous check or not inheriting at all
		return;

	if (inner->getOuterClass() == nullptr)// not an inner class
		return;

	Class* bOuter = inner->getOuterClass()->getBaseClassSymbol(); // baseClass of outer
	if (bOuter == nullptr){ // outer not inhereting
		string errString = string(inner->getInhertedFrom()) + " is not defined ";
		this->errRecovery->errQ->enqueue(inner->getLineNo(), inner->getColNo(), errString.c_str(), "");
		return;
	}

	vector<Symbol*> symbols = bOuter->getBodyScope()->getSymbolTable()->symbols();
	vector<Symbol*>::iterator i;
	bool found = false;
	for (i = symbols.begin(); i != symbols.end(); ++i){
		int sType = (*i)->getSymbolType();
		if (sType == CLASS){//inner class
			Class* s = dynamic_cast<Class*>(*i);
			if (string(s->getName()) == inner->getInhertedFrom())
				found = true;

		}
	}
	if (!found){
		string errString = string(inner->getInhertedFrom()) + " is not defined ";
		this->errRecovery->errQ->enqueue(inner->getLineNo(), inner->getColNo(), errString.c_str(), "");
	}


}

void TypeChecker::checkInnerClasses(){
	this->searchScopeForInners(this->symbolsParser->getRootScope());
}
