#pragma once
#include "CheckerVisitor.hpp"
#include "../../TypeSystem/TypeExpression.hpp"
#include "../all.hpp"
#include "../../definitions.h"
#include "../../TypeSystem/TypeError.hpp"
#include "../../TypeSystem/TypeFunction.hpp"

CheckerVisitor::CheckerVisitor() : doesReturn(false), classTracker(nullptr) {}

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
	node->body->accept(this, context);
}

void CheckerVisitor::visit(ListNode* listNode, TypeExpression* context) {
	for (auto& node : listNode->nodes)
		node->accept(this, context);
}

void CheckerVisitor::visit(ReturnNode* node, TypeExpression* context) {
	doesReturn = true;
	if (dynamic_cast<TypeFunction*>(context)->isConstructorFT) {
		node->setNodeType(new TypeError("Constructors should not return a value. line: " + to_string(node->line) + ". col:" + to_string(node->col)));
		return;
	}


	if (node->returned_node == nullptr) // returning void 
		if (dynamic_cast<TypeFunction*>(context)->getReturnTypeExpression()->getTypeId() == VOID_TYPE_ID)
			return;
		else {
			node->setNodeType(new TypeError("return value type doesn't match function type. line:" + to_string(node->line) + ". col:" + to_string(node->col)));
			return;
		}
	if (!node->returned_node->getNodeType()->equivelantTo(dynamic_cast<TypeFunction*>(context)->getReturnTypeExpression()->getTypeId()))
		node->setNodeType(new TypeError("return value type doesn't match function type. line:" + to_string(node->line) + ". col:" + to_string(node->col)));
	
}

void CheckerVisitor::visit(ScalarNode* node, TypeExpression* context) {
	//no thing
}

void CheckerVisitor::visit(VariableNode* node, TypeExpression* context) {

	//Static and Dynamic members check:
	if (dynamic_cast<TypeFunction*>(context) != nullptr && this->classTracker != nullptr) {
		// we are in class method definition
		TypeFunction* typeFunction = dynamic_cast<TypeFunction*>(context);
		for(auto param : typeFunction->paramsSymbols){
			if(strcmp(param->getName(), node->variableName.c_str()) == 0)
				return;                          // we have the variable in the params of the function
		}

		if (dynamic_cast<TypeFunction*>(context)->isStaticMethod && !node->variable->isStatic) {
			node->setNodeType(new TypeError("Can't access non-statics in static contexts. line:" + to_string(node->line) + ". col:" + to_string(node->col)));
			return;
		}
	}

}

void CheckerVisitor::visit(WhileNode* node, TypeExpression* context) {

}

void CheckerVisitor::visit(FunctionCallNode* node, TypeExpression* context) {

}

void CheckerVisitor::visit(FunctionDefineNode* node, TypeExpression* context) {
	node->bodySts->accept(this, node->getNodeType());

	if (dynamic_cast<TypeFunction*>(node->getNodeType())->getReturnTypeExpression()->getTypeId() == VOID_TYPE_ID)//no need to check return paths
		return;	


	//return type checking:
	doesReturn = false;
	node->bodySts->accept(this, node->getNodeType());

	//test  1:
	for (auto bodyNode : dynamic_cast<ListNode*>(node->bodySts)->nodes) {
		if (dynamic_cast<ReturnNode*>(bodyNode) != nullptr)
			doesReturn = true;
	}
	//test 2:
	//look for any if/else node if they return a value of not
	for (auto bodyNode : dynamic_cast<ListNode*>(node->bodySts)->nodes) {
		if (dynamic_cast<IfNode*>(bodyNode) != nullptr) {
			IfNode* ifNode = dynamic_cast<IfNode*>(bodyNode);
			if (ifNode->else_node != nullptr) { // it has an else
				ifNode->accept(this, node->getNodeType());
				bool ifDoesReturn = doesReturn;
				doesReturn = false;
				ifNode->else_node->accept(this, node->getNodeType());
				if (ifDoesReturn && doesReturn)
					doesReturn = true; // it really does return
				else
					doesReturn = false;
			}
		}
	}

	if (!doesReturn) 
		node->setNodeType(new TypeError("function " + node->functionSym->getLabel() + ": Not all paths return a value. line:" + to_string(node->line) + ".col: " + to_string(node->col)));
	
	doesReturn = false;
}

void CheckerVisitor::visit(ParameterNode* node, TypeExpression* context) {

}

void CheckerVisitor::visit(ClassDefineNode* node, TypeExpression* context) {
	//pre visit
	TypeExpression* newContext = node->getNodeType();
	this->classTracker = newContext;
	//visit
	node->body->accept(this, newContext);
	//post visit
	this->classTracker = nullptr;
}

void CheckerVisitor::visit(ClassMemNode* node, TypeExpression* context) {
	//no thing
}

void CheckerVisitor::visit(ClassMethodNode* node, TypeExpression* context) {
	node->bodySts->accept(this, node->getNodeType());

	if (node->methodSym->isConstructor || dynamic_cast<TypeFunction*>(node->getNodeType())->getReturnTypeExpression()->getTypeId() == VOID_TYPE_ID ) //no need to check return paths
		return;

	//return type checking:
	doesReturn = false;
	node->bodySts->accept(this, node->getNodeType());

	//test  1:
	for (auto bodyNode : dynamic_cast<ListNode*>(node->bodySts)->nodes) {
		if (dynamic_cast<ReturnNode*>(bodyNode) != nullptr)
			doesReturn = true;
	}
	//test 2:
	//look for any if/else node if they return a value of not
	for (auto bodyNode : dynamic_cast<ListNode*>(node->bodySts)->nodes) {
		if (dynamic_cast<IfNode*>(bodyNode) != nullptr) {
			IfNode* ifNode = dynamic_cast<IfNode*>(bodyNode);
			if (ifNode->else_node != nullptr) { // it has an else
				ifNode->accept(this, node->getNodeType());
				bool ifDoesReturn = doesReturn;
				doesReturn = false;
				ifNode->else_node->accept(this, node->getNodeType());
				if (ifDoesReturn && doesReturn)
					doesReturn = true; // it really does return
				else
					doesReturn = false;
			}
		}
	}

	if (!doesReturn)
		node->setNodeType(new TypeError("function " + node->methodSym->getLabel() + ": Not all paths return a value. line:" + to_string(node->line) + ".col: " + to_string(node->col)));

	doesReturn = false;
}

void CheckerVisitor::visit(ClassCallNode* node, TypeExpression* context) {
	if (dynamic_cast<TypeError*>(node->getNodeType()) != nullptr)// if it already has a semantic error
		return;

	if (context != nullptr && node->object->getNodeType()->getTypeId() == context->getTypeId()) {// are we in exactly the same context?
		return; // no access problems
	}

	if(context != nullptr && node->object->variableName.compare("$this") == 0)
		return;

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