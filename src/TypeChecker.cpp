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
void TypeChecker::parseScope(Scope* scope){
	vector<Symbol*> symbols = scope->getSymbolTable()->symbols();
	vector<Symbol*>::iterator i;
	for (i = symbols.begin(); i != symbols.end(); ++i){
		Symbol* s = *i;
		int sType = s->getSymbolType();
		if ( sType == CLASS){
			Class* c = dynamic_cast<Class*>(s);
			this->dg->insertIntoNodes(c->getName(), c, c->getInhertedFrom()); // insert the catched class declaration into dependancy graph
		}
	}
}
