#pragma once 
#include <map>
#include "../AST/DeclarationNode.hpp"
#include "../AST/VariableNode.hpp"
#include "../AST/ParameterNode.hpp"
#include "../AST/ClassDefineNode.hpp"
#include "../SymbolTable/Symbol.h"
#include "../AST/FunctionDefineNode.hpp"

class GlobalFrame 
{
public:

	GlobalFrame();

	virtual void addLocal(Node *);

	virtual string getAddress(string);

	map<string, int> locals;

	GlobalFrame *parentFrame;

	int globalSize;
};

class FunctionFrame :public GlobalFrame
{
public:
	FunctionFrame();

	FunctionFrame(GlobalFrame *,FunctionDefineNode *);

	virtual void addParameter(ParameterNode *);

	virtual void addLocal(Node *);

	virtual string getAddress(string);

	map<string, int> arguments;

	int paramtersOffset;
	int initialFrameSize;
	int stackSize;   
};

class ObjectFrame : public GlobalFrame
{
public:
	ObjectFrame();

	ObjectFrame(GlobalFrame *,ClassDefineNode *);

	virtual void addLocal(Node *);

	virtual void addFunction(Node *);

	virtual string getAddress(string);

	map<string, int> functions;

	int membersOffset;
};


