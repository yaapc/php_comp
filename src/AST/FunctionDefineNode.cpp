#pragma once
#include "FunctionDefineNode.hpp"
  

FunctionDefineNode::FunctionDefineNode(Symbol* func, Node* bod) {
    functionSym = dynamic_cast<Function*>(func);
    bodySts = dynamic_cast<ListNode*>(bod);
  }

void FunctionDefineNode::print(ostream &os) {
    int self = int(this);
    os << self << "[label = \""
       << functionSym->getName()
       << "\"]"
       <<endl;
    os << self << "->" << int(bodySts) << endl;
    if (bodySts) bodySts->print(os);
  }

