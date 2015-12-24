#include "definitions.h"
#include "grammar.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;

extern void initSymbolsParser();
extern SymbolsParser * symbolsParser;
extern void initTypeChecker();

int main(int argc, char** argv) {
  initSymbolsParser();
  initTypeChecker();
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
