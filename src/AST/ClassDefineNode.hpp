#pragma once

#include "Node.hpp"
#include "ListNode.hpp"
#include "../SymbolTable/Symbol.h"
#include "ClassMemNode.hpp"
#include "ClassMethodNode.hpp"

class ClassDefineNode : public Node {
public:
	ListNode* body;
	vector<TypeExpression*>  classMemTE;		 // class' members Type Expressions
	vector<ClassMemNode*>    classMemNodes;     // class' members Nodes
	vector<ClassMethodNode*> classMethodsNodes; // class' methods Nodes
	vector<TypeExpression*>* classMethodsTE;     // class' methods Type Expressions
	Class* classSymbol;
	ClassDefineNode* parent;
	//TODO: think about inner classes!

	ClassDefineNode(Symbol* classSym, Node* body, int line, int col);


	virtual void print(ostream &os);

	virtual bool type_checking();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);
	virtual void accept(CheckerVisitor*, TypeExpression*);

private:
	void extractInfo(); // extracts class node information from class symbol and body nodes.

};