%{
  #include <unistd.h>
  #include <iostream>
  #include <string>
  #include "grammar.hpp"
  #include "definitions.hpp"
  using namespace std;
  int line_no = 0;
  int col_no = 0;
  string inline_html;
  void doForReturn(string yytext){
  //cout << yytext << endl;
  /*
  yylval.r.col_no = col_no;
  yylval.r.line_no = line_no;
  col_no += strlen(yytext);
  */
}

%}

DIGIT [0-9]
LITERAL [a-zA-Z]
%option noyywrap

%x multiLineComment
%x string
%x php eof_return

%%

"<?php" |
"<?" |
"<%" {
  BEGIN php;
  yylval.r.token_type = NORMAL;
  doForReturn(yytext);
  return T_OPEN_TAG;
}

"<?=" |
"<%=" {
  BEGIN php;
  yylval.r.token_type = WITH_ECHO;
  doForReturn(yytext);
  return T_OPEN_TAG;
}

<php>("?>"|"%>") {
  BEGIN INITIAL;
  return T_CLOSE_TAG;
}

(.|\n)/"<?" {
  inline_html.push_back(yytext[0]);
  yylval.r.str = new char[inline_html.size() + 1];
  for (int i = 0; i < inline_html.size(); ++i)
    yylval.r.str[i] = inline_html[i];
  yylval.r.str[inline_html.size() + 1] = '\0';
  inline_html.clear();
  return T_INLINE_HTML;
}

. |
\n {
  inline_html.push_back(yytext[0]);
}

<<EOF>> {
inline_html.push_back(yytext[0]);
  yylval.r.str = new char[inline_html.size() + 1];
  for (int i = 0; i < inline_html.size(); ++i)
    yylval.r.str[i] = inline_html[i];
  yylval.r.str[inline_html.size() + 1] = '\0';
  inline_html.clear();
  // put a dummy to input, and then switch to eof_return which is gonna return EOF
  unput('e');
  BEGIN eof_return;
  return T_INLINE_HTML;
}

<eof_return>e return 0;

<php>"abstract"  {
  doForReturn(yytext);
  return T_ABSTRACT;
}

<php>"&="  {
  doForReturn(yytext);
  return T_AND_EQUAL;
}

<php>"array()"  {
  doForReturn(yytext);
  return T_ARRAY;
}

<php>"(array)"  {
  doForReturn(yytext);
  return T_ARRAY_CAST;
}

<php>"as"  {
  doForReturn(yytext);
  return T_AS;
}

<php>"&&"  {
  doForReturn(yytext);
  return T_BOOLEAN_AND;
}

<php>"||"  {
  doForReturn(yytext);
  return T_BOOLEAN_OR;
}

<php>"(bool)"|"(boolean)"  {
  doForReturn(yytext);
}

<php>"boolean"   {
  doForReturn(yytext);
  yylval.r.token_type = T_TYPE_BOOLEAN;
  return T_TYPE;
}

<php>"break"  {
  doForReturn(yytext);
  return T_BREAK;
}

<php>"callable"  {
  doForReturn(yytext);
  return T_CALLBACK;
}

<php>"case"  {
  doForReturn(yytext);
  return T_CASE;
}

<php>"catch"  {
  doForReturn(yytext);
  return T_CATCH;
}

<php>"class"  {
  doForReturn(yytext);
  return T_CLASS;
}

<php>"__CLASS__"  {
  doForReturn(yytext);
  return T_CLASS_C;
}

<php>"clone"  {
  doForReturn(yytext);
  return T_CLONE;
}

<php>".="  {
  doForReturn(yytext);
  return T_CONCAT_EQUAL;
}

<php>"const"  {
  doForReturn(yytext);
  return T_CONST;
}

<php>"continue"  {
  doForReturn(yytext);
  return T_CONTINUE;
}

<php>"{$"  {
  doForReturn(yytext);
  return T_CURLY_OPEN;
}

<php>"--"  {
  doForReturn(yytext);
  return T_DEC;
}

<php>"declare"  {
  doForReturn(yytext);
  return T_DECLARE;
}

<php>"default"  {
  doForReturn(yytext);
  return T_DEFAULT;
}

<php>"__DIR__"  {
  doForReturn(yytext);
  return T_DIR;
}

<php>"/="  {
  doForReturn(yytext);
  return T_DIV_EQUAL;
}

<php>"do"  {
  doForReturn(yytext);
  return T_DO;
}

<php>"=>"  {
  doForReturn(yytext);
  return T_DOUBLE_ARROW;
}

<php>"double"  {
  doForReturn(yytext);
  yylval.r.token_type = T_TYPE_DOUBLE;
  return T_TYPE;
}

<php>"float"   {
  doForReturn(yytext);
  yylval.r.token_type = T_TYPE_FLOAT;
  return T_TYPE;
}

<php>("(real)"|"(double)"|"(float)")  {
  doForReturn(yytext);
  return T_DOUBLE_CAST;
}

<php>"::"  {
  doForReturn(yytext);
  return T_PAAMAYIM_NEKUDOTAYIM;
}

<php>"echo"  {
  doForReturn(yytext);
  return T_ECHO;
}

<php>"..."  {
  doForReturn(yytext);
  return T_ELLIPSIS;
}

<php>"else"  {
  doForReturn(yytext);
  return T_ELSE;
}

<php>"elseif"  {
  doForReturn(yytext);
  return T_ELSEIF;
}

<php>"empty"  {
  doForReturn(yytext);
  return T_EMPTY;
}

<php>"enddeclare"  {
  doForReturn(yytext);
  return T_ENDDECLARE;
}

<php>"endfor"  {
  doForReturn(yytext);
  return T_ENDFOR;
}

<php>"endforeach"  {
  doForReturn(yytext);
  return T_ENDFOREACH;
}

<php>"endif"  {
  doForReturn(yytext);
  return T_ENDIF;
}

<php>"endswitch"  {
  doForReturn(yytext);
  return T_ENDSWITCH;
}

<php>"endwhile"  {
  doForReturn(yytext);
  return T_ENDWHILE;
}

<php>"eval()"  {
  doForReturn(yytext);
  return T_EVAL;
}

<php>("exit"|"die") {
  doForReturn(yytext);
  return T_EXIT;
}

<php>"extends"  {
  doForReturn(yytext);
  return T_EXTENDS;
}

<php>"__FILE__"  {
  doForReturn(yytext);
  return T_FILE;
}

<php>"final"  {
  doForReturn(yytext);
  return T_FINAL;
}

<php>"finally"  {
  doForReturn(yytext);
  return T_FINALLY;
}

<php>"for"  {
  doForReturn(yytext);
  return T_FOR;
}

<php>"foreach"  {
  doForReturn(yytext);
  return T_FOREACH;
}

<php>"function"  {
  doForReturn(yytext);
  return T_FUNCTION;
}

<php>"global"  {
  doForReturn(yytext);
  return T_GLOBAL;
}

<php>"goto"  {
  doForReturn(yytext);
  return T_GOTO;
}

<php>"if"  {
  doForReturn(yytext);
  return T_IF;
}

<php>"implements"  {
  doForReturn(yytext);
  return T_IMPLEMENTS;
}

<php>"++"  {
  doForReturn(yytext);
  return T_INC;
}

<php>"include"  {
  doForReturn(yytext);
  return T_INCLUDE;
}

<php>"include_once"  {
  doForReturn(yytext);
  return T_INCLUDE_ONCE;
}

<php>"instanceof"  {
  doForReturn(yytext);
  return T_INSTANCEOF;
}

<php>"insteadof"  {
  doForReturn(yytext);
  return T_INSTEADOF;
}

<php>"int"   {
  doForReturn(yytext);
  yylval.r.token_type = T_TYPE_INTEGER;
  return T_TYPE;
}

<php>"(int)"|"(integer)"  {
  doForReturn(yytext);
  return T_INT_CAST;
}

<php>"interface"  {
  doForReturn(yytext);
  return T_INTERFACE;
}

<php>"isset"  {
  doForReturn(yytext);
  return T_ISSET;
}

<php>"=="  {
  doForReturn(yytext);
  return T_IS_EQUAL;
}

<php>">="  {
  doForReturn(yytext);
  return T_IS_GREATER_OR_EQUAL;
}

<php>"==="  {
  doForReturn(yytext);
  return T_IS_IDENTICAL;
}

<php>("!="|"<>")  {
  doForReturn(yytext);
  return T_IS_NOT_EQUAL;
}

<php>"!=="  {
  doForReturn(yytext);
  return T_IS_NOT_IDENTICAL;
}

<php>"<="  {
  doForReturn(yytext);
  return T_IS_SMALLER_OR_EQUAL;
}

<php>"__LINE__"  {
  doForReturn(yytext);
  return T_LINE;
}

<php>"list()"  {
  doForReturn(yytext);
  return T_LIST;
}

<php>"and"  {
  doForReturn(yytext);
  return T_LOGICAL_AND;
}

<php>"or"  {
  doForReturn(yytext);
  return T_LOGICAL_OR;
}

<php>"xor"  {
  doForReturn(yytext);
  return T_LOGICAL_XOR;
}

<php>"-="  {
  doForReturn(yytext);
  return T_MINUS_EQUAL;
}

<php>"%="  {
  doForReturn(yytext);
  return T_MOD_EQUAL;
}

<php>"*="  {
  doForReturn(yytext);
  return T_MUL_EQUAL;
}

<php>"namespace"  {
  doForReturn(yytext);
  return T_NAMESPACE;
}

<php>"new"  {
  doForReturn(yytext);
  return T_NEW;
}

<php>"(object)"  {
  doForReturn(yytext);
  return T_OBJECT_CAST;
}

<php>"->"  {
  doForReturn(yytext);
  return T_OBJECT_OPERATOR;
}

<php>"|="  {
  doForReturn(yytext);
  return T_OR_EQUAL;
}

<php>"+="  {
  doForReturn(yytext);
  return T_PLUS_EQUAL;
}

<php>"**"  {
  doForReturn(yytext);
  return T_POW;
}

<php>"**="  {
  doForReturn(yytext);
  return T_POW_EQUAL;
}

<php>"print()"  {
  doForReturn(yytext);
  return T_PRINT;
}

<php>"private"  {
  doForReturn(yytext);
  return T_PRIVATE;
}

<php>"public"  {
  doForReturn(yytext);
  return T_PUBLIC;
}

<php>"protected"  {
  doForReturn(yytext);
  return T_PROTECTED;
}

<php>"require"  {
  doForReturn(yytext);
  return T_REQUIRE;
}

<php>"require_once"  {
  doForReturn(yytext);
  return T_REQUIRE_ONCE;
}

<php>"return"  {
  doForReturn(yytext);
  return T_RETURN;
}

<php>"<<"  {
  doForReturn(yytext);
  return T_SL;
}

<php>"<<="  {
  doForReturn(yytext);
  return T_SL_EQUAL;
}

<php>">>"  {
  doForReturn(yytext);
  return T_SR;
}

<php>">>="  {
  doForReturn(yytext);
  return T_SR_EQUAL;
}

<php>"static"  {
  doForReturn(yytext);
  return T_STATIC;
}

<php>"string"  {
  doForReturn(yytext);
  yylval.r.token_type = T_TYPE_STRING;
  return T_TYPE;
}

<php>"(string)"  {
  doForReturn(yytext);
  return T_STRING_CAST;
}

<php>"switch"  {
  doForReturn(yytext);
  return T_SWITCH;
}

<php>"throw"  {
  doForReturn(yytext);
  return T_THROW;
}

<php>"trait"  {
  doForReturn(yytext);
  return T_TRAIT;
}

<php>"try"  {
  doForReturn(yytext);
  return T_TRY;
}

<php>"unset()"  {
  doForReturn(yytext);
  return T_UNSET;
}

<php>"(unset)"  {
  doForReturn(yytext);
  return T_UNSET_CAST;
}

<php>"use"  {
  doForReturn(yytext);
  return T_USE;
}

<php>"var"  {
  doForReturn(yytext);
  return T_VAR;
}

<php>"while"  {
  doForReturn(yytext);
  return T_WHILE;
}

<php>"^="  {
  doForReturn(yytext);
  return T_XOR_EQUAL;
}

<php>"yield"  {
  doForReturn(yytext);
  return T_YIELD;
}

<php>"char"  {
  doForReturn(yytext);
  yylval.r.token_type = T_TYPE_CHAR;
  return T_TYPE;
}

<php>"boolean[]"  {
  doForReturn(yytext);
}

<php>"string[]"  {
  doForReturn(yytext);
}

<php>"char[]"  {
  doForReturn(yytext);
}

<php>"int[]"  {
  doForReturn(yytext);
}

<php>"float[]"  {
  doForReturn(yytext);
}

<php>"double[]"  {
  doForReturn(yytext);
}

<php>$[a-zA-Z_]({DIGIT}|{LITERAL})*  {
  doForReturn(yytext);
  return T_VARIABLE;
}

<php>${DIGIT}+.*   {
  doForReturn(yytext);
}

<php>[+-]?({DIGIT}*\.?{DIGIT}+|{DIGIT}+\.) {
  doForReturn(yytext);
  return T_DNUMBER;
}

<php>\n   {
  //yylval.r.col_no = col_no;
  //yylval.r.line_no = line_no;
  col_no =0;
  line_no++;
}

<php>\/\*  {
  BEGIN multiLineComment;
}

<multiLineComment>\*\/  {
  BEGIN INITIAL;
  cout << "That was a MultiLineComment :P\n";
}

<multiLineComment>[^\*\/]* {;}
<php>\"  {
  BEGIN string;
}

<string>[^\"]  {;}

<string>\"  {
  BEGIN INITIAL;
  cout << "That was a STRING\n";
}

<php>.  {
  doForReturn(yytext);
  return T_BAD_INPUT;
}

%%
