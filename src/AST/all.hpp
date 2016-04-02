#pragma once

#include "Node.hpp"
#include "ListNode.hpp"
#include "EchoNode.hpp"
#include "FunctionNode.hpp"
#include "ReturnNode.hpp"
#include "VariableNode.hpp"
#include "BinaryOperationNode.hpp"

#include <iostream>
void print_ast(Node *root, std::ostream &os) {
  os << "digraph AST {\n";
  root->print(os);
  os << "}\n";
}
