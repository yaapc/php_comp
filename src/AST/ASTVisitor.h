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
class ClassMethodNode;
class ClassMemNode;
class ClassDefineNode;
class ClassCallNode;
class NewNode;
class ASTVisitor {
public:
	virtual void visit(AssignmentNode *)		= 0;
	virtual void visit(BinaryOperationNode *)	= 0; 
	virtual void visit(DeclarationNode *)		= 0; 
	virtual void visit(EchoNode *)				= 0; 
	virtual void visit(ElseNode *)				= 0; 
	virtual void visit(ForNode *)				= 0; 
	virtual void visit(IfNode *)				= 0; 
	virtual void visit(ListNode *)				= 0; 
	virtual void visit(ReturnNode *)			= 0; 
	virtual void visit(ScalarNode *)			= 0;
	virtual void visit(VariableNode*)			= 0;
	virtual void visit(WhileNode*)				= 0;
	virtual void visit(FunctionDefineNode*)		= 0;
	virtual void visit(FunctionCallNode*)		= 0;
	virtual void visit(ParameterNode*)			= 0;
	virtual void visit(ClassMethodNode*)		= 0;
	virtual void visit(ClassMemNode*)			= 0;
	virtual void visit(ClassDefineNode*)		= 0;
	virtual void visit(ClassCallNode*)			= 0;
	virtual void visit(NewNode*)				= 0;
};
