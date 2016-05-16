#include "Node.hpp"
#include "ListNode.hpp"
#include <iostream>

using namespace std;

class NewNode : public Node {
public:
	string className;
	ListNode* argumentsList;       // a list of parameter nodes

	NewNode(Node* args,string className);

	virtual bool type_checking();

	virtual void print(ostream& os);

	virtual TypeExpression* getNodeType();

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

};