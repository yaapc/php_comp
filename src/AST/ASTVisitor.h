#pragma once

#include <iostream>
class AssignmentNode;
class BinaryOperationNode;
class DeclarationNode;
class EchoNode;
class ElseNode;
class ForNode;
class FunctionNode;
class IfNode;
class ListNode;
class ReturnNode;
class ScalarNode;
class VariableNode;
class WhileNode;
class FunctionDefineNode;
class FunctionCallNode;
class ParameterNode;
class ASTVisitor {
public:
 
	virtual void visit(AssignmentNode *assignmentNode)				= 0;
	virtual void visit(BinaryOperationNode *binaryOperationNode)	= 0; 
	virtual void visit(DeclarationNode *declarationNode)			= 0; 
	virtual void visit(EchoNode *echoNode)							= 0; 
	virtual void visit(ElseNode *elseNode)							= 0; 
	virtual void visit(ForNode *forNode)							= 0; 
	virtual void visit(IfNode *ifNode)								= 0; 
	virtual void visit(ListNode *listNode)							= 0; 
	virtual void visit(ReturnNode *returnNode)						= 0; 
	virtual void visit(ScalarNode *scalarNode)						= 0;
	virtual void visit(VariableNode *variableNode)					= 0;
	virtual void visit(WhileNode *whileNode)						= 0;
		
	virtual void visit(FunctionDefineNode	*functionDefineNode)					= 0;
	virtual void visit(FunctionCallNode		*functionCallNode)						= 0;
	virtual void visit(ParameterNode		*parameterNode)							= 0;

};
