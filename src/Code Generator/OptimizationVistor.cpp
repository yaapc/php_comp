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

			if (binaryOperationNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID)
			{
				if (*(binaryOperationNode->op_type) == '+'){
					return new ScalarNode(lhs->value.int_val + rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);
				}

				if (*(binaryOperationNode->op_type) == '-'){
					return new ScalarNode(lhs->value.int_val - rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);
				}

				if (*(binaryOperationNode->op_type) == '*'){
					return new ScalarNode(lhs->value.int_val * rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);
				}

				if (*(binaryOperationNode->op_type) == '/'){
					return new ScalarNode(lhs->value.int_val / rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);
				}

				if (*(binaryOperationNode->op_type) == '%'){
					return new ScalarNode(lhs->value.int_val % rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);
				}
			}


			if (binaryOperationNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){

				float lFloat;
				float rFloat;
				if (lhs->getNodeType()->getTypeId() == INTEGER_TYPE_ID){
					lFloat	= (float)lhs->value.int_val;
					rFloat	= rhs->value.float_val;
				}else
				if (rhs->getNodeType()->getTypeId() == INTEGER_TYPE_ID){
					lFloat	= lhs->value.float_val;
					rFloat	= (float)rhs->value.int_val;
				}
				else{
					lFloat	= lhs->value.float_val;
					rFloat	= rhs->value.float_val;
				}
			
				if (*(binaryOperationNode->op_type) == '+')
					return new ScalarNode(lFloat + rFloat, binaryOperationNode->line, binaryOperationNode->col);

				if (*(binaryOperationNode->op_type) == '-')
					return new ScalarNode(lFloat - rFloat, binaryOperationNode->line, binaryOperationNode->col);

				if (*(binaryOperationNode->op_type) == '*')
					return new ScalarNode(lFloat * rFloat, binaryOperationNode->line, binaryOperationNode->col);

				if (*(binaryOperationNode->op_type) == '/')
					return new ScalarNode(lFloat / rFloat, binaryOperationNode->line, binaryOperationNode->col);
			}	
	
		
			if (binaryOperationNode->getNodeType()->getTypeId() == STRING_TYPE_ID)
			{

				if (*(binaryOperationNode->op_type) == '+')
				{

					string lString;
					string rString;
					if (lhs->getNodeType()->getTypeId() == INTEGER_TYPE_ID){
						lString	= to_string(lhs->value.int_val);
						rString	= rhs->value.string_val;
					}else
					if (rhs->getNodeType()->getTypeId() == INTEGER_TYPE_ID){
						lString	= lhs->value.string_val;
						rString	= to_string(rhs->value.int_val);
					}else
					if (lhs->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){
						lString	= to_string(lhs->value.bool_val);
						rString	= rhs->value.string_val;
					}else
					if (rhs->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){
						lString	= lhs->value.string_val;
						rString	= to_string(rhs->value.bool_val);
					}
					else{
						lString	= lhs->value.string_val;
						rString	= rhs->value.string_val;
					}	
					return new ScalarNode(lString + rString, binaryOperationNode->line, binaryOperationNode->col);
				}
			}

			if (binaryOperationNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){

				if (lhs->getNodeType()->getTypeId() == INTEGER_TYPE_ID && 
					rhs->getNodeType()->getTypeId() == INTEGER_TYPE_ID){
			
					if (strcmp(binaryOperationNode->op_type, ">") == 0)
						return new ScalarNode(lhs->value.int_val > rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, "<") == 0)
						return new ScalarNode(lhs->value.int_val < rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, ">=") == 0)
						return new ScalarNode(lhs->value.int_val >= rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, "<=") == 0)
						return new ScalarNode(lhs->value.int_val <= rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, "==") == 0)
						return new ScalarNode(lhs->value.int_val == rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, "!=") == 0)
						return new ScalarNode(lhs->value.int_val != rhs->value.int_val, binaryOperationNode->line, binaryOperationNode->col);

				}

				if (lhs->getNodeType()->getTypeId() == FLOAT_TYPE_ID || 
					rhs->getNodeType()->getTypeId() == FLOAT_TYPE_ID){

					float lFloat;
					float rFloat;
					if (lhs->getNodeType()->getTypeId() == INTEGER_TYPE_ID){
						lFloat	= (float)lhs->value.int_val;
						rFloat	= rhs->value.float_val;
					}else
					if (rhs->getNodeType()->getTypeId() == INTEGER_TYPE_ID){
						lFloat	= lhs->value.float_val;
						rFloat	= (float)rhs->value.int_val;
					}
					else{
						lFloat	= lhs->value.float_val;
						rFloat	= rhs->value.float_val;
					}			

					if (strcmp(binaryOperationNode->op_type, ">") == 0)
						return new ScalarNode(lFloat > rFloat, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, "<") == 0)
						return new ScalarNode(lFloat < rFloat, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, ">=") == 0)
						return new ScalarNode(lFloat >= rFloat, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, "<=") == 0)
						return new ScalarNode(lFloat <= rFloat, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, "==") == 0)
						return new ScalarNode(lFloat == rFloat, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, "!=") == 0)
						return new ScalarNode(lFloat != rFloat, binaryOperationNode->line, binaryOperationNode->col);

				}

				if (lhs->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID && 
					rhs->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){

					if (strcmp(binaryOperationNode->op_type, "&&") == 0)
						return new ScalarNode(lhs->value.bool_val && rhs->value.bool_val, binaryOperationNode->line, binaryOperationNode->col);

					if (strcmp(binaryOperationNode->op_type, "||") == 0)
						return new ScalarNode(lhs->value.bool_val || rhs->value.bool_val, binaryOperationNode->line, binaryOperationNode->col);
				}
			}
		}		
	return binaryOperationNode;
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
	if (echoNode->expression)
		return echoNode;
	else
		return nullptr;
}

Node* OptimizationVistor::visit(ElseNode *elseNode)
{
	elseNode->body = elseNode->body->optmize(this);
	if (elseNode->body){
		return elseNode;
	}else{
		return nullptr;
	}
}

Node* OptimizationVistor::visit(ForNode *forNode)
{	
	forNode->body = forNode->body->optmize(this);
	if (forNode->body)
		return forNode;
	else
		return nullptr;
}

Node* OptimizationVistor::visit(IfNode *ifNode)
{
	if (ifNode->condition)
		ifNode->condition	= ifNode->condition->optmize(this);

	if (ifNode->body)
		ifNode->body		= ifNode->body->optmize(this);

	if (ifNode->else_node)
		ifNode->else_node	= ifNode->else_node->optmize(this);

	ScalarNode* conditionNode = dynamic_cast<ScalarNode*>(ifNode->condition);
	if (conditionNode){
		bool conditionResult;
		if (conditionNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID){
			int conditionResultInt  = conditionNode->value.int_val;
			conditionResult = conditionResultInt == 0 ? false : true;
		}

		if (conditionNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){
			conditionResult			= conditionNode->value.bool_val;
		}
	
		if (conditionResult){

			if (ifNode->body)
				return ifNode->body;
			else
				return nullptr;

		}else{

			if (ifNode->else_node)
				return ifNode->else_node;
			else
				return nullptr;

		}
	}
	return ifNode;
}

Node* OptimizationVistor::visit(ListNode *listNode)
{
	if (listNode->nodes.size() == 0)
		return nullptr;
	vector<Node*> nodess;

	for (auto &node : listNode->nodes) {
		if (node == nullptr) continue;
		node = node->optmize(this);
		nodess.push_back(node);

		//TODO return may come in for/While body ?!
		if (dynamic_cast<ReturnNode*>(node) || dynamic_cast<BreakNode*>(node) || dynamic_cast<ContinueNode*>(node)){
			listNode->nodes = nodess;
			break;
		}	
	}
	return listNode;
}

Node* OptimizationVistor::visit(ScalarNode *scalarNode)
{
	return scalarNode;
}

Node* OptimizationVistor::visit(WhileNode *whileNode)
{
	if (whileNode->body)
		whileNode->body			= whileNode->body->optmize(this);

	if (!whileNode->body)
		return nullptr;

	if (whileNode->condition)
		whileNode->condition	= whileNode->condition->optmize(this);

	ScalarNode* conditionNode = dynamic_cast<ScalarNode*>(whileNode->condition);
	if (conditionNode){
		bool conditionResult = conditionNode->value.bool_val;
		if (!conditionResult){
			whileNode->body = nullptr;
			return nullptr;
		}
	}

	return whileNode;
}

Node* OptimizationVistor::visit(FunctionCallNode *functionCallNode)
{
	if (functionCallNode->argumentsList)
		functionCallNode->argumentsList = functionCallNode->argumentsList->optmize(this);
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

Node* OptimizationVistor::visit(ClassMemNode *classMemNode)
{
	return classMemNode;
}

Node* OptimizationVistor::visit(ClassMethodNode *classMethodNode)
{
	//TODO optmize method
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

Node* OptimizationVistor::visit(BreakNode *breakNode)
{
	return breakNode;
}

Node* OptimizationVistor::visit(ContinueNode *continueNode)
{
	return continueNode;
}


Node* OptimizationVistor::visit(StaticCallNode 	*staticCallNode)
{
	return staticCallNode;
}

