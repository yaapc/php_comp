#include"TypeChecker.h"
#include <fstream>

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
	parseScope(scope);
	Scope* innersWalker = scope->getInnerScope();
	if (innersWalker == nullptr)
		return;
	while (innersWalker != nullptr){
		parseScope(innersWalker);
		searchScope(innersWalker);
		innersWalker = innersWalker->getNextScope();
	}
}

//parses a given scope looking for class declaration and inserting them into the dependency graph @dg
void TypeChecker::parseScopeForNodes(Scope* scope){
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
					vector<Symbol*> currSymbols = scope->getSymbolTable()->symbols();
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
								checkFinalMethods(c);
							}
						}
						++j;
					}
					walker = scope->getParentScope();
				}
				if (!found) {
					string errString = string(c->getInhertedFrom()) + " is Not Declared";
					this->errRecovery->errQ->enqueue(c->getLineNo(), c->getColNo(), errString.c_str(), "");
					c->setInhertedFrom("Object");
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
