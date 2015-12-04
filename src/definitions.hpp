#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

int yylex(void);
int yyparse();
void yyerror(char *);

enum type {
  T_TYPE_INTEGER = 1,T_TYPE_FLOAT,T_TYPE_DOUBLE,T_TYPE_BOOLEAN,T_TYPE_STRING,T_TYPE_CHAR
};

enum tag_type {
  NORMAL, WITH_ECHO
};

#endif