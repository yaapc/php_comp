# php_comp
a php compiler contains for now the lexer (lex.l) and grammer parser (yacc.y)

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

