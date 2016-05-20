#include "Node.hpp"

class BreakNode : public Node {
public:

	BreakNode();

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);
};
