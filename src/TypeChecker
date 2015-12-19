#pragma once
#include"ErrorRecovery\ErrorRecovery.h"
#include"SymbolTable\SymbolsParser.h"

class TypeChecker {
public:
	TypeChecker(ErrorRecovery* errRecovery, SymbolsParser* symbolsParser){
		this->errRecovery = errRecovery;
		this->symbolsParser = symbolsParser;
	}
	~TypeChecker();

	void checkVariable(char* name, int lineNo, int colNo){
		Symbol* sym = this->symbolsParser->lookUpSymbol(name, lineNo, colNo);
	}

private:
	ErrorRecovery* errRecovery;
	SymbolsParser* symbolsParser;
};
