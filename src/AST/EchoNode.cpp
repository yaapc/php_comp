#pragma once
#include "EchoNode.hpp"
#include "../Code Generator/AsmGenerator.h"
#include "ListNode.hpp"

EchoNode::EchoNode(Node* node) : expression(node) {
}

bool EchoNode::type_checking() {
        return true;
}

void EchoNode::print(ostream& os) {
        int self = int(this);
        os << self << "[label = \"Echo\"]" <<endl;
        expression->print(os);
        os << self << "->" << int(expression) << endl;
}

void EchoNode::generate_code(){
        AsmGenerator::comment("<Echo Node");
        if (expression) {
			string t0 = "t0";
			if (ListNode* listNode = dynamic_cast<ListNode*>(expression)) {
					AsmGenerator::comment("<List Node");
					for (auto &node : listNode->nodes) {
							if (node == nullptr) continue;
							
							node->generate_code();
							//int type = 3;
							if (node->getNodeType()->getTypeId() == INTEGER_TYPE_ID) { //Integer
								AsmGenerator::pop(t0);
								AsmGenerator::print_reg(t0);
							}
							if (node->getNodeType()->getTypeId() == STRING_TYPE_ID) { //String
								AsmGenerator::pop(t0);
								AsmGenerator::print_string(t0);
							}

							if (node->getNodeType()->getTypeId() == FLOAT_TYPE_ID){ // float
								AsmGenerator::f_pop("f0");
								AsmGenerator::f_print_reg("f0");
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