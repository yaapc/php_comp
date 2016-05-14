#pragma once 
#include <map>
#include "../AST/DeclarationNode.hpp"
#include "../AST/VariableNode.hpp"
#include "../AST/ParameterNode.hpp"
#include "../AST/ClassDefineNode.hpp"
#include "../SymbolTable/Symbol.h"
#include "../AST/FunctionDefineNode.hpp"

struct VariableComparer
{
    bool operator()(Variable *first ,Variable* second) const
    {
		return first->getId() < second->getId();
    }
};

class GlobalFrame 
{
public:

	GlobalFrame();

	virtual void addLocal(Node *);

	virtual string getAddress(Node *);

	/* Maps a local symbol to its offset (negative and stored so) from the frame pointer.  */
	map<Variable*, int,VariableComparer> locals;

	GlobalFrame *parentFrame;


};

class FunctionFrame :public GlobalFrame
{
public:
	FunctionFrame();

	FunctionFrame(GlobalFrame *,FunctionDefineNode *);

	virtual void addParameter(ParameterNode *);

	virtual void addLocal(Node *);

	virtual string getAddress(Node *);


	/* Maps an argument symbol to its offset (positive) from the frame pointer. */
	map<Variable*, int,VariableComparer> arguments;

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

	virtual string getAddress(Node *);

	int objectSize;
};


