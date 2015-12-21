# php_comp
a php compiler contains for now the lexer (lex.l) and grammer parser (yacc.y) and SymbolTable


### MAIN TODOs: 
  * Minor todo's scattered and labeled in the code.
  * :heavy_check_mark: Link the paramters symbols to thier function symbol.
  * :heavy_check_mark: Link the data members and methods to thier class symbol.
  
### Type Checking TODOs:
  * Grammers doesn't support declarations of "const" variables. 
  * Forward Declerations is permitted for classes and functions in Inheretance and Invoking.
  * :heavy_check_mark: Implement default constructor for classes that don't have a constructor.
  * Inheretance from an undefined class in NOT permitted.
  * Circular Dependency is NOT permitted.
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

