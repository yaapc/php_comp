#pragma once 
#include "Node.hpp"
#include "../SymbolTable/Symbol.h"

class ClassMemNode : public Node {
public:
	ClassMemNode(Symbol* memSym, int line , int col);

	virtual void print(ostream &os);

	virtual bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);
	virtual void accept(CheckerVisitor*, TypeExpression*);

	//Symbol interfacing methods:
	bool isStatic();
	bool isInited();
	bool isConst();
	string getName();
	DataMember* getMemSymbol();

private:
	DataMember* memberSym;
};
