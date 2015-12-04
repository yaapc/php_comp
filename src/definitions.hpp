#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

int yylex(void);
int yyparse();
void yyerror(char *);

enum primitive {
  T_PRIMITIVE_INTEGER = 1,T_PRIMITIVE_FLOAT,T_PRIMITIVE_DOUBLE,T_PRIMITIVE_BOOLEAN,T_PRIMITIVE_STRING,T_PRIMITIVE_CHAR
};

enum open_tag_type {
  NORMAL, WITH_ECHO
};

#endif