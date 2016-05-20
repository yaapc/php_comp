#pragma once 
#include "OptimizationVistor.hpp"
#include "../AST/all.hpp"


void OptimizationVistor::optmize(ListNode *ast)
{
	ast->optmize(this);
}

Node* OptimizationVistor::visit(AssignmentNode *assignmentNode)
{
	assignmentNode->lhs = assignmentNode->lhs->optmize(this);
	assignmentNode->rhs	= assignmentNode->rhs->optmize(this);
	return assignmentNode;
}

Node* OptimizationVistor::visit(BinaryOperationNode *binaryOperationNode)
{
	binaryOperationNode->left	= binaryOperationNode->left->optmize(this);
	binaryOperationNode->right	= binaryOperationNode->right->optmize(this);

	ScalarNode* lhs = dynamic_cast<ScalarNode*>(binaryOperationNode->left);
	ScalarNode* rhs = dynamic_cast<ScalarNode*>(binaryOperationNode->right);

	if (lhs && rhs){

		if (binaryOperationNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID){
			if (*(binaryOperationNode->op_type) == '+'){
				return new ScalarNode(lhs->value.int_val + rhs->value.int_val);
			}

			if (*(binaryOperationNode->op_type) == '-'){
				return new ScalarNode(lhs->value.int_val - rhs->value.int_val);
			}

			if (*(binaryOperationNode->op_type) == '*'){
				return new ScalarNode(lhs->value.int_val * rhs->value.int_val);
			}

			if (*(binaryOperationNode->op_type) == '/'){
				return new ScalarNode(lhs->value.int_val / rhs->value.int_val);
			}

			if (*(binaryOperationNode->op_type) == '%'){
				return new ScalarNode(lhs->value.int_val % rhs->value.int_val);
			}
		}


		if (binaryOperationNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
			if (*(binaryOperationNode->op_type) == '+'){
				return new ScalarNode(lhs->value.float_val + rhs->value.float_val);
			}

			if (*(binaryOperationNode->op_type) == '-'){
				return new ScalarNode(lhs->value.float_val - rhs->value.float_val);
			}

			if (*(binaryOperationNode->op_type) == '*'){
				return new ScalarNode(lhs->value.float_val * rhs->value.float_val);
			}

			if (*(binaryOperationNode->op_type) == '/'){
				return new ScalarNode(lhs->value.float_val / rhs->value.float_val);
			}
		}

		
		if (binaryOperationNode->getNodeType()->getTypeId() == STRING_TYPE_ID){

			if (*(binaryOperationNode->op_type) == '+'){
				return new ScalarNode(lhs->value.string_val + rhs->value.string_val);
			}
		}

		
	}
}

Node* OptimizationVistor::visit(DeclarationNode *declarationNode)
{
	return declarationNode;
}

Node* OptimizationVistor::visit(VariableNode *variableNode)
{
	return variableNode;
}

Node* OptimizationVistor::visit(EchoNode *echoNode)
{
	echoNode->expression = echoNode->expression->optmize(this);
	return echoNode;
}

Node* OptimizationVistor::visit(ElseNode *elseNode)
{
	elseNode->body = elseNode->body->optmize(this);
	return elseNode;
}

Node* OptimizationVistor::visit(ForNode *forNode)
{
	return forNode;
}

Node* OptimizationVistor::visit(IfNode *ifNode)
{
	ifNode->body = ifNode->body->optmize(this);
	return ifNode;
}

Node* OptimizationVistor::visit(ListNode *listNode)
{
	for (auto &node : listNode->nodes) {
		if (node == nullptr) continue;
		node = node->optmize(this);
	}
	return listNode;
}

Node* OptimizationVistor::visit(ScalarNode *scalarNode)
{
	return scalarNode;
}

Node* OptimizationVistor::visit(WhileNode *whileNode)
{
	whileNode->body = whileNode->body->optmize(this);
	return whileNode;
}

Node* OptimizationVistor::visit(FunctionCallNode *functionCallNode)
{
	return functionCallNode;
}

Node* OptimizationVistor::visit(FunctionDefineNode *functionDefineNode)
{
	functionDefineNode->bodySts = functionDefineNode->bodySts->optmize(this);
	return functionDefineNode;
}

Node* OptimizationVistor::visit(ParameterNode *parameterNode)
{
	return parameterNode;
}

Node* OptimizationVistor::visit(ReturnNode *returnNode)
{
	return returnNode;
}

Node* OptimizationVistor::visit(ClassDefineNode	*classDefineNode)
{
	return classDefineNode;
}

Node* OptimizationVistor::visit(ClassMemNode	*classMemNode)
{
	return classMemNode;
}

Node* OptimizationVistor::visit(ClassMethodNode *classMethodNode)
{
	return classMethodNode;
}

Node* OptimizationVistor::visit(ClassCallNode *classCallNode)
{
	return classCallNode;
}


Node* OptimizationVistor::visit(NewNode *newNode)
{

	return newNode;
	
}
