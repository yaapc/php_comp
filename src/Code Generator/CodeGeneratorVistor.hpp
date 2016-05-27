#pragma once
#include <iostream>
#include "../AST/ASTVisitor.h"
#include "Frame.hpp"
#include <map>
using namespace std;

class CodeGneratorVistor
{
public:
	int symbolIDS;

	string returnLabel;

	string continueLabel;

	GlobalFrame *currentFrame;

	virtual void generate(ListNode *ast);

	map<string,ObjectFrame*> objectsFrames;

public:
	virtual void visit(AssignmentNode *assignmentNode);
	virtual void visit(BinaryOperationNode *binaryOperationNode);
	virtual void visit(DeclarationNode *declarationNode);
	virtual void visit(EchoNode *echoNode);
	virtual void visit(ElseNode *assignmentNode);
	virtual void visit(ForNode *forNode);
	virtual void visit(IfNode *ifNode);
	virtual void visit(ListNode *listNode);
	virtual void visit(ReturnNode *returnNode);
	virtual void visit(ScalarNode *scalarNode);
	virtual void visit(VariableNode *variableNode);
	virtual void visit(WhileNode *whileNode);
	virtual void visit(FunctionCallNode *functionCallNode);
	virtual void visit(FunctionDefineNode *functionDefineNode);
	virtual void visit(ParameterNode *parameterNode);

	virtual void visit(ClassDefineNode	*classDefineNode);
	virtual void visit(ClassMemNode		*classMemNode);
	virtual void visit(ClassMethodNode	*classMethodNode);
	virtual void visit(ClassCallNode	*classCallNode);
	virtual void visit(NewNode			*newNode);
	virtual void visit(BreakNode 		*breakNode);
	virtual void visit(ContinueNode 	*continueNode);

	//Helper Methods
	string getClassMemberAddress(ClassCallNode*,string);
};
