#pragma once

#include "Node.hpp"
#include <iostream>

using namespace std;

class EchoNode : public Node {
public:
	Node* expression;
	EchoNode(Node* node);
	virtual bool type_checking();
	virtual void print(ostream& os);
	virtual void generate_code();
};
