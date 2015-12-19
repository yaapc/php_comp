#include "definitions.hpp"
#include "grammar.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
int main(int argc, char** argv) {
  initSymbolsParser(); 
  cout<<"php-compiler"<<endl<<endl;

  extern FILE *yyin;
  if (!(yyin = fopen("index.php", "r"))) {
    cerr<<"index.php not found"<<endl;
    return 1;
  }

  yyparse();
  symbolsParser->printSymbolTables();
  cout<<"compilation done"<<endl;
  return 0;
}
