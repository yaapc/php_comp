#pragma once
#include <iostream>
#include "../TypeSystem/TypeExpression.hpp"

class CodeGneratorVistor;
class OptimizationVistor;
class TypeErrorVisitor;
class CheckerVisitor;
using namespace std;

class Node {
public:
	Node();

	virtual void print(ostream& os);

	virtual bool type_checking();

    virtual TypeExpression* getNodeType();

	virtual void setNodeType(TypeExpression* type);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);
	virtual void accept(CheckerVisitor*, TypeExpression*);

	bool hasScopeFrame;

	int line, col;

protected:
	TypeExpression* nodeType;
};
