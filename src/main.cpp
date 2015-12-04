#include "definitions.hpp"
#include "grammar.hpp"
#include <iostream>

using namespace std;
int main(int argc, char** argv) {
  cout<<"php-compiler"<<endl<<endl;
  yyparse();
  return 0;
}