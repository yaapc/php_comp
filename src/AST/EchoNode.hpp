#pragma once

#include "Node.hpp"
#include <iostream>

using namespace std;

class EchoNode : public Node {
public:
Node* expression;
EchoNode(Node* node) : expression(node) {
}
virtual bool type_checking() {
        return true;
}

virtual void print(ostream& os) {
        int self = int(this);
        os << self << "[label = \"Echo\"]" <<endl;
        expression->print(os);
        os << self << "->" << int(expression) << endl;
}

virtual void generate_code(){
        astLog.log("generate_code EchoNode");
        AsmGenerator::comment("<Echo Node");
        if (expression) {
			string t0 = "t0";
			if (ListNode* listNode = dynamic_cast<ListNode*>(expression)) {
					AsmGenerator::comment("<List Node");
					for (auto &node : listNode->nodes) {
							if (node == nullptr) continue;
							node->generate_code();
							int type = 1;
							if (type == 1) { //Integer
									AsmGenerator::pop(t0);
									AsmGenerator::print_reg(t0);
							}
							if (type == 2) { //String
									AsmGenerator::pop(t0);
									AsmGenerator::print_string(t0);
							}
					}
					AsmGenerator::comment("List Node/>");
			}else{
					AsmGenerator::comment("<Experesion Node");
					expression->generate_code();
					AsmGenerator::pop(t0);
					AsmGenerator::print_string(t0);
					AsmGenerator::comment("Experesion Node/>");
			}

        }
        AsmGenerator::comment("Echo Node/>");

}
};
