#pragma once
#include <iostream>
#include "../TypeSystem/TypeExpression.hpp"

using namespace std;

class Node {
public:

	virtual void generate_code();

	virtual void print(ostream& os);

	virtual bool type_checking();


    virtual TypeExpression* getNodeType();
protected:
	TypeExpression* nodeType;
};
