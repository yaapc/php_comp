#pragma once
#include "TypeErrorVisitor.hpp"
#include "../all.hpp"

void TypeErrorVisitor::visit(AssignmentNode *assNode) {
	TypeExpression* nType = assNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	assNode->rhs->accept(this);
	assNode->lhs->accept(this);
}

void TypeErrorVisitor::visit(BinaryOperationNode* binNode) {
	TypeExpression* nType = binNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	binNode->right->accept(this);
	binNode->left->accept(this);
}

void TypeErrorVisitor::visit(DeclarationNode* decNode) {
	TypeExpression* nType = decNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}

void TypeErrorVisitor::visit(EchoNode* echoNode) {
	TypeExpression* nType = echoNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	echoNode->expression->accept(this);
}

void TypeErrorVisitor::visit(ElseNode* elseNode) {
	TypeExpression* nType = elseNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	elseNode->body->accept(this);
}


void TypeErrorVisitor::visit(ForNode *forNode) {
	TypeExpression* nType = forNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	forNode->condition->accept(this);
	forNode->initializer->accept(this);
	forNode->body->accept(this);
}
void TypeErrorVisitor::visit(IfNode *ifNode) {
	TypeExpression* nType = ifNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	ifNode->body->accept(this);
	ifNode->condition->accept(this);
}
void TypeErrorVisitor::visit(ListNode *listNode) {
	TypeExpression* nType = listNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	for (auto node : listNode->nodes)
		node->accept(this);
}

void TypeErrorVisitor::visit(ReturnNode *returnNode) {
	TypeExpression* nType = returnNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}

void TypeErrorVisitor::visit(ScalarNode *scalarNode) {
	TypeExpression* nType = scalarNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}
void TypeErrorVisitor::visit(VariableNode *variableNode) {
	TypeExpression* nType = variableNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}
void TypeErrorVisitor::visit(WhileNode *whileNode) {
	TypeExpression* nType = whileNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	whileNode->condition->accept(this);
	whileNode->body->accept(this);
}

void TypeErrorVisitor::visit(FunctionCallNode *functionCallNode) {
	TypeExpression* nType = functionCallNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}

void TypeErrorVisitor::visit(FunctionDefineNode *functionDefineNode) {
	TypeExpression* nType = functionDefineNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	functionDefineNode->bodySts->accept(this);
}

void TypeErrorVisitor::visit(ParameterNode *parameterNode) {
	TypeExpression* nType = parameterNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}

void TypeErrorVisitor::visit(ClassDefineNode	*classDefineNode) {
	TypeExpression* nType = classDefineNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	classDefineNode->body->accept(this);
}

void TypeErrorVisitor::visit(ClassMemNode		*classMemNode) {
	TypeExpression* nType = classMemNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}

void TypeErrorVisitor::visit(ClassMethodNode	*classMethodNode) {
	TypeExpression* nType = classMethodNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
	classMethodNode->bodySts->accept(this);
}

void TypeErrorVisitor::visit(ClassCallNode	*classCallNode) {
	TypeExpression* nType = classCallNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}

void TypeErrorVisitor::visit(NewNode			*newNode) {
	TypeExpression* nType = newNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}

void TypeErrorVisitor::visit(BreakNode 		*breakNode) {
	TypeExpression* nType = breakNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}

void TypeErrorVisitor::visit(ContinueNode 	*continueNode) {
	TypeExpression* nType = continueNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}

void TypeErrorVisitor::visit(StaticCallNode 	*staticCallNode) {
	TypeExpression* nType = staticCallNode->getNodeType();
	if (nType != nullptr && dynamic_cast<TypeError*>(nType) != nullptr) { // we have a TypeError
		this->errQ.push_back(nType);
	}
}






