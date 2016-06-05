#pragma once
#include "CheckerVisitor.hpp"
#include "../../TypeSystem/TypeExpression.hpp"
#include "../all.hpp"
#include "../../definitions.h"
#include "../../TypeSystem/TypeError.hpp"
#include "../../TypeSystem/TypeFunction.hpp"

void CheckerVisitor::visit(AssignmentNode* assNode, TypeExpression* context) {
	assNode->lhs->accept(this, context);
	assNode->rhs->accept(this, context);
}

void CheckerVisitor::visit(BinaryOperationNode* binaryNode, TypeExpression* context) {
	binaryNode->left->accept(this, context);
	binaryNode->right->accept(this, context);
}

void CheckerVisitor::visit(DeclarationNode* node, TypeExpression* context) {
	
}

void CheckerVisitor::visit(EchoNode* node, TypeExpression* context) {
	node->expression->accept(this, context);
}

void CheckerVisitor::visit(ElseNode* node, TypeExpression* context) {
	node->body->accept(this, context);
}

void CheckerVisitor::visit(ForNode* node, TypeExpression* context) {
	node->condition->accept(this, context);
	node->initializer->accept(this, context);
	node->body->accept(this, context);
}

void CheckerVisitor::visit(IfNode* node, TypeExpression* context) {
	node->condition->accept(this, context);
}

void CheckerVisitor::visit(ListNode* listNode, TypeExpression* context) {
	for (auto& node : listNode->nodes)
		node->accept(this, context);
}

void CheckerVisitor::visit(ReturnNode* node, TypeExpression* context) {
	if (!node->returned_node->getNodeType()->equivelantTo(dynamic_cast<TypeFunction*>(context)->getReturnTypeExpression()->getTypeId()))
		node->setNodeType(new TypeError("return value type doesn't match function type. line:" + to_string(node->line) + ". col:" + to_string(node->col)));
	
}

void CheckerVisitor::visit(ScalarNode* node, TypeExpression* context) {
	//no thing
}

void CheckerVisitor::visit(VariableNode* node, TypeExpression* context) {

}

void CheckerVisitor::visit(WhileNode* node, TypeExpression* context) {

}

void CheckerVisitor::visit(FunctionCallNode* node, TypeExpression* context) {

}

void CheckerVisitor::visit(FunctionDefineNode* node, TypeExpression* context) {
	//return type checking:
	node->bodySts->accept(this, node->getNodeType());
}

void CheckerVisitor::visit(ParameterNode* node, TypeExpression* context) {

}

void CheckerVisitor::visit(ClassDefineNode* node, TypeExpression* context) {
	//pre visit
	TypeExpression* newContext = node->getNodeType();
	//visit
	node->body->accept(this, newContext);
	//post visit
}

void CheckerVisitor::visit(ClassMemNode* node, TypeExpression* context) {
	//no thing
}

void CheckerVisitor::visit(ClassMethodNode* node, TypeExpression* context) {
	node->bodySts->accept(this, node->getNodeType());
}

void CheckerVisitor::visit(ClassCallNode* node, TypeExpression* context) {
	if (dynamic_cast<TypeError*>(node->getNodeType()) != nullptr)// if it already has a semantic error
		return;

	if (context != nullptr && node->object->getNodeType()->getTypeId() == context->getTypeId()) {// are we in exactly the same context?
		return; // no access problems
	}

	//here we are in a different context than definition
	if (node->member->accessModifier == PRIVATE_ACCESS){
		node->setNodeType(new TypeError(node->propertyString + " is inaccessible. line:" + to_string(node->line) + ",col:" + to_string(node->col)));
	}
	//else we are good.
}

void CheckerVisitor::visit(NewNode* node, TypeExpression* context) {
	//
}

void CheckerVisitor::visit(BreakNode* node, TypeExpression* context) {
	//no thing
}

void CheckerVisitor::visit(ContinueNode* node, TypeExpression* context) {
	//no thing
}

void CheckerVisitor::visit(StaticCallNode* node, TypeExpression* context) {
	//no thing
}