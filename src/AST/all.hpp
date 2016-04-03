#pragma once

#include "Node.hpp"
#include "ListNode.hpp"
#include "EchoNode.hpp"
#include "FunctionNode.hpp"
#include "ReturnNode.hpp"
#include "VariableNode.hpp"
#include "BinaryOperationNode.hpp"
#include "DeclarationNode.hpp"
#include "AssignmentNode.hpp"
#include "ScalarNode.hpp"
#include "IfNode.hpp"
#include "ElseNode.hpp"
#include "WhileNode.hpp"
#include "ForNode.hpp"

#include <iostream>
void print_ast(Node *root, std::ostream &os) {
  cout<< "Printing Ast\n";
  os << "digraph AST {\n";
  root->print(os);
  os << "}\n";
  cout<< "Printing Done\n";
}
