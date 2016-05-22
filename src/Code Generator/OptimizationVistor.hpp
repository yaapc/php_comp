#pragma once
#include <iostream>
#include "../AST/ASTVisitor.h"
#include "FunctionFrame.hpp"
#include <map>
using namespace std;

class OptimizationVistor
{
public:
	
	Node * parent;

	virtual void optmize(ListNode *ast);

	virtual Node* visit(AssignmentNode		*assignmentNode);
	virtual Node* visit(BinaryOperationNode *binaryOperationNode);
	virtual Node* visit(DeclarationNode *declarationNode);
	virtual Node* visit(EchoNode		*echoNode);
	virtual Node* visit(ElseNode		*assignmentNode);
	virtual Node* visit(ForNode			*forNode);
	virtual Node* visit(IfNode			*ifNode);
	virtual Node* visit(ListNode		*listNode);
	virtual Node* visit(ReturnNode		*returnNode);
	virtual Node* visit(ScalarNode		*scalarNode);
	virtual Node* visit(VariableNode	*variableNode);
	virtual Node* visit(WhileNode			*whileNode);
	virtual Node* visit(FunctionCallNode	*functionCallNode);
	virtual Node* visit(FunctionDefineNode	*functionDefineNode);
	virtual Node* visit(ParameterNode	*parameterNode);
	virtual Node* visit(ClassDefineNode	*classDefineNode);
	virtual Node* visit(ClassMemNode	*classMemNode);
	virtual Node* visit(ClassMethodNode	*classMethodNode);
	virtual Node* visit(ClassCallNode	*classCallNode);
	virtual Node* visit(NewNode			*newNode);
	virtual Node* visit(BreakNode		*breakNode);
	virtual Node* visit(ContinueNode	*contineNode);
};
