# php_comp
a php compiler contains for now the lexer (lex.l) and grammer parser (yacc.y) and a SymbolTable and a simple TypeChecker.


### MAIN TODOs: 
  * Minor todo's scattered and labeled in the code.
  * :heavy_check_mark: Link the paramters symbols to thier function symbol.
  * :heavy_check_mark: Link the data members and methods to thier class symbol.
  
### Type Checking TODOs:
  * Grammers doesn't support declarations of "const" variables. 
  * Forward Declerations is permitted for classes and functions in Inheretance and Invoking.
  * :heavy_check_mark: Implement default constructor for classes that don't have a constructor.
  * Inheretance from an undefined class in NOT permitted.
  * :heavy_check_mark: Circular Dependency is NOT permitted.
  * "final" methods are not to be overrided.
  * "final" classes are not to be inhereted from.
  * :heavy_check_mark: "final abstract" are not allowed. 
  * :heavy_check_mark: implement "abstract declaration of method" in the grammer for example: " abstract function iAmAFunction(): void; ".
  * :heavy_check_mark: Abstract method should have no body.
  * :heavy_check_mark: Abstract method should only have two types of access modifiers which are PUPLIC and PROTECTED.
  * :heavy_check_mark: Any class that has an Abstract method IS abstract.
  * Inner classes should not have the same name as for outer classes.
  * various inner classes checks...

### Error Recovery TODOs:
  * report an error for methods with no body and not declared as abstract.

### SymbolTable And TypeChecker Class Diagram : 
 ![Class Diagram](http://s11.postimg.org/y2apfnppv/Main.jpg)

### TypeChecker :
what the type checker can do for now:
 * checkVariable() : checks if a variable is declared looking up the symbol table from the scope that the variable is declared in up to root scope.
 * checkDependency() : creates a dependency (inhertence) graph, looks for circular dependencies ,report circles and logs the graph.

### Dependency Graph: 
 quoting from Wikipedia:
 a dependency graph is a directed graph representing dependencies of several objects towards each other.
 we use the dependency graph to represesnt inheretence dependency between classes.
 we create the directed graph then use Tarjan's strongly connected components algorithm to catch the circles and identify them.
 References : 
   http://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/
   https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
   P334 of Compiler Design - Compiler's Principles, Techniques and Tools
TODO: remove these and introduce the Replacer tool:
these includes at line 23 :
```
#include <stdlib.h>
class istream;
#include <unistd.h>
```

to be replaced with : 

```
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
using namespace std;
```

