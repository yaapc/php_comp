#pragma once
#include <iostream>
#include "../AST/ASTVisitor.h"
#include "FunctionFrame.hpp"
using namespace std;

class CodeGneratorVistor : public ASTVisitor
{
public:
	int symbolIDS; //used to set id for varibles (global and local) and parameters and function and methods and class member
	string funcRetLabel;
	GlobalFrame *currentFrame;
	virtual void generate(ListNode *ast);
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
};
