#pragma once 
#include <map>
#include "../AST/DeclarationNode.hpp"
#include "../AST/VariableNode.hpp"
#include "../AST/ParameterNode.hpp"
#include "../SymbolTable/Symbol.h"
#include "../AST/FunctionDefineNode.hpp"

struct VariableComparer
{
    bool operator()(Variable *first ,Variable* second) const
    {
		return first->getId() < second->getId();
    }
};

class FunctionFrame
{
public:
	FunctionFrame();

	FunctionFrame(FunctionFrame *,FunctionDefineNode *);

	virtual void addVariable(DeclarationNode *);

	virtual void addParameter(ParameterNode *);

	virtual string getAddress(VariableNode *);

	FunctionFrame *parentFunctionFrame;

	/* Maps an argument symbol to its offset (positive) from the frame pointer. */
	map<Variable*, int,VariableComparer> arguments;

	/* Maps a local symbol to its offset (negative and stored so) from the frame pointer.  */
	map<Variable*, int,VariableComparer> locals;

	int paramtersOffset;
	int initialFrameSize;
	int stackSize;   
};


class GlobalFrame :public FunctionFrame
{
public:

	GlobalFrame();

	virtual void addVariable(DeclarationNode *);

	virtual string getAddress(VariableNode *);

	virtual void addParameter(ParameterNode *);
};

