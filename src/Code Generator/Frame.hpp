#pragma once 
#include <map>
#include "../AST/DeclarationNode.hpp"
#include "../AST/VariableNode.hpp"
#include "../AST/ParameterNode.hpp"
#include "../AST/ClassDefineNode.hpp"
#include "../SymbolTable/Symbol.h"
#include "../AST/FunctionDefineNode.hpp"
#include "../TypeSystem/TypeClass.hpp"

class Frame {
public:
	virtual void addLocal(Node *)		= 0;

	virtual string getAddress(string)	= 0;

	virtual int addStatic(Node *)		= 0;

	map<string, int> locals;

	map<string, int> statics;

	vector<string> objectsLocals;

	Frame *parentFrame;

	int frameSize;

	int initialFrameSize;

	string reg;
};

class GlobalFrame : public Frame
{
public:
	GlobalFrame();

	virtual void addLocal(Node *);

	virtual string getAddress(string);

	virtual int addStatic(Node *);
};

class ScopeFrame : public Frame{
public:
	ScopeFrame(Frame *,bool isFunction);

	virtual void addLocal(Node *);

	virtual string getAddress(string);

	virtual int addStatic(Node *);

	bool isFunction;
};

class FunctionFrame :public Frame
{
public:
	FunctionFrame();

	FunctionFrame(Frame *,ListNode *);

	virtual void addParameter(ParameterNode *);

	virtual void addLocal(Node *);

	virtual string getAddress(string);

	map<string, int> arguments;

	virtual int addStatic(Node *);

	int paramtersOffset;

};

class ObjectFrame : public Frame
{
public:
	ObjectFrame();

	ObjectFrame(Frame *,TypeClass *);

	virtual void addLocal(Node *);

	virtual string getAddress(string);

	virtual int addStatic(Node *);

	virtual void fillFrame(TypeClass*);

	int objectsCount;

	string classTagAddress;

	TypeClass *classType;

	string thisReg;
};


