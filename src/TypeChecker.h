#pragma once
#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include"DependencyGraph\DependencyGraph.h"
#include"SymbolTable\SymbolsParser.h"
#include "definitions.h"

class TypeChecker {
public:
	TypeChecker(ErrorRecovery* errRecovery, SymbolsParser* symbolsParser){
		this->errRecovery = errRecovery;
		this->symbolsParser = symbolsParser;
		this->dg = new DependencyGraph(errRecovery);
	}
	~TypeChecker();

	void checkVariable(char* name, int lineNo, int colNo){
		Symbol* sym = this->symbolsParser->lookUpSymbol(name, lineNo, colNo);
	}

	void checkDependency();

private:
	ErrorRecovery* errRecovery;
	SymbolsParser* symbolsParser;

	void searchScope(Scope* scope);
	void parseScope(Scope* scope);
	DependencyGraph* dg;
};


#endif
