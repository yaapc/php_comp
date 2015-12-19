# php_comp
a php compiler contains for now the lexer (lex.l) and grammer parser (yacc.y) and SymbolTable


### MAIN TODOs: 
  * Minor todo's scattered and labeled in the code.
  * :red_circle: Link the paramters symbols to thier function symbol.
  * :red_circle: Link the data members and methods to thier class symbol.
  
### Type Checking TODOs:
  * Grammers doesn't support declarations of "const" variables. 
  * Forward Declerations is permitted for classes and functions in Inheretance and Invoking.
  * Implement default constructor for classes that don't have a constructor.
  * Inheretance from an undefined class in NOT permitted.
  * Circular Dependency is NOT permitted.
  * "final" methods are not to be overrided.
  * "final" classes are not to be inhereted from.
  * :heavy_check_mark: "final abstract" are not allowed. 
  * implement "abstract declaration of method" in the grammer for example: " abstract function iAmAFunction(): void; ".
  * Abstract method should have no body.
  * Abstract method should only have two types of access modifiers which are PUPLIC and PROTECTED.
  * Any class that has an Abstract method IS abstract.
  * Inner classes should not have the same name as for outer classes.
  * various inner classes checks...



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

