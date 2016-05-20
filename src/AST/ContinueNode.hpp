#include "Node.hpp"

class ContinueNode : public Node {
public:

	ContinueNode();

	virtual void print(ostream &os);

	virtual void generate_code(CodeGneratorVistor *codeGneratorVistor);

	virtual Node* optmize(OptimizationVistor *);

};
