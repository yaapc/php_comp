#include "Node.hpp"

class BreakNode : public Node {
public:

	BreakNode(int line, int col);

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

	virtual void accept(TypeErrorVisitor *);
	virtual void accept(CheckerVisitor*, TypeExpression*);

};
