#pragma once
#include <iostream>
#include "../TypeSystem/TypeExpression.hpp"
class CodeGneratorVistor;
class OptimizationVistor;
using namespace std;

class Node {
public:
	Node();

	virtual void print(ostream& os);

	virtual bool type_checking();

    virtual TypeExpression* getNodeType();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	bool hasScopeFrame;

protected:
	TypeExpression* nodeType;
};
