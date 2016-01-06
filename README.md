# php_comp
a php compiler contains for now the lexer (lex.l) and grammer parser (yacc.y) and a SymbolTable and a simple TypeChecker.


### MAIN TODOs: 
  * AST.
  * Minor todo's scattered and labeled in the code.
  * SymbolTable doesn't support declarations of "const" and "static" variables. 
  * Forward Declerations is permitted for classes and functions in Inheretance and Invoking.
  * Inheretance from an undefined class in NOT permitted.
  * "final" methods are not to be overrided.
  * "final" classes are not to be inhereted from.
  * access modifiers of overriding methods should be the same or of a higher level than the overrided.
  * any class that inherit abstract class should override all abstract methods.
  * Inner classes should not have the same name as for outer classes.
  * various inner classes checks...
  * declaring arrays like ` int[] $array ` is not supported only when declaring like ` int [] $array `.
  * report missing `$` before variable identifier when declaring.
  * ? `iAmClass $iAmclass = new static;` ?
  * report extra `;` at the end of function declaration.
  * abstract method with no `abstract` keyword should be warned.
  * add `$this` pointer to class members.
  * check access of `$this` pointer like `$this->member`.
  * enhance error report of redeclarations of functions, it says "identifier not found".
  * access modifiers should not be assigned together `private public $param`.
  * an access modifier should only be assigned once `public public $param`.
  * variable names should not start with punctuations.
  * :heavy_check_mark: "final abstract" are not allowed. 
  * :heavy_check_mark: implement "abstract declaration of method" in the grammer for example: " abstract function iAmAFunction(): void; ".
  * :heavy_check_mark: Abstract method should have no body.
  * :heavy_check_mark: Abstract method should only have two types of access modifiers which are PUPLIC and PROTECTED.
  * :heavy_check_mark: Any class that has an Abstract method IS abstract.
  * :heavy_check_mark: Circular Dependency is NOT permitted.
  * :heavy_check_mark: Implement default constructor for classes that don't have a constructor.
  * :heavy_check_mark: Link the paramters symbols to thier function symbol.
  * :heavy_check_mark: Link the data members and methods to thier class symbol.

  
### Error Recovery TODOs:
  * report an error for methods with no body and not declared as abstract.

### SymbolTable And TypeChecker Class Diagram : 
 ![Class Diagram](http://s4.postimg.org/94t0nclfh/Class_Diagram.png)

### TypeChecker :
what the type checker can do for now:
 * checkVariable() : checks if a variable is declared looking up the symbol table from the scope that the variable is declared in up to root scope.
 * checkDependency() : creates a dependency (inhertence) graph, looks for circular dependencies ,report circles and logs the graph.

### Dependency Graph: 
 quoting from Wikipedia: a dependency graph is a directed graph representing dependencies of several objects towards each other.
 we use the dependency graph to represesnt inheretence dependency between classes.
 we create the directed graph then use Tarjan's strongly connected components algorithm to catch the circles and identify them.
 References : 
 * http://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/ 
 * https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
 * P334 of Compiler Design - Compiler's Principles, Techniques and Tools
 
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

