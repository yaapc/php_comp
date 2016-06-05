#include "../ASTVisitor.h"

class TypeExpression;

class CheckerVisitor {
public:
	virtual void visit(AssignmentNode *assignmentNode, TypeExpression* context);
	virtual void visit(BinaryOperationNode *binaryOperationNode, TypeExpression* context);
	virtual void visit(DeclarationNode *declarationNode, TypeExpression* context);
	virtual void visit(EchoNode *echoNode, TypeExpression* context);
	virtual void visit(ElseNode *assignmentNode, TypeExpression* context);
	virtual void visit(ForNode *forNode, TypeExpression* context);
	virtual void visit(IfNode *ifNode, TypeExpression* context);
	virtual void visit(ListNode *listNode, TypeExpression* context);
	virtual void visit(ReturnNode *returnNode, TypeExpression* context);
	virtual void visit(ScalarNode *scalarNode, TypeExpression* context);
	virtual void visit(VariableNode *variableNode, TypeExpression* context);
	virtual void visit(WhileNode *whileNode, TypeExpression* context);
	virtual void visit(FunctionCallNode *functionCallNode, TypeExpression* context);
	virtual void visit(FunctionDefineNode *functionDefineNode, TypeExpression* context);
	virtual void visit(ParameterNode *parameterNode, TypeExpression* context);

	virtual void visit(ClassDefineNode	*classDefineNode, TypeExpression* context);
	virtual void visit(ClassMemNode		*classMemNode, TypeExpression* context);
	virtual void visit(ClassMethodNode	*classMethodNode, TypeExpression* context);
	virtual void visit(ClassCallNode	*classCallNode, TypeExpression* context);
	virtual void visit(NewNode			*newNode, TypeExpression* context);
	virtual void visit(BreakNode 		*breakNode, TypeExpression* context);
	virtual void visit(ContinueNode 	*continueNode, TypeExpression* context);
	virtual void visit(StaticCallNode 	*staticCallNode, TypeExpression* context);


private:
	bool doesReturn = false;

};

