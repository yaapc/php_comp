%{
  #include <unistd.h>
  #include <iostream>
  #include <string>
  #include <string.h>
  #include <queue>
  #include <stack>
  #include "grammar.hpp"
  #include "definitions.hpp"
  #include "Logger.hpp"

  using namespace std;
  int line_no = 1;
  int col_no = 1;
  string inline_html;
  string str;
  queue<int> tokens_queue;
  Logger l("lexer_log.txt");
  extern ErrorRecovery errorRec;
  struct bracket {
    char br;
    int line_no;
    int col_no;
  };
  stack<bracket> brackets;

  void doForReturn(char *yytext){
    yylval.r.col_no = col_no;
    yylval.r.line_no = line_no;
    col_no += strlen(yytext);
}

%}

DIGIT ([0-9])
LITERAL ([a-zA-Z])
EXP ([eE][+-]?[0-9]+)
NAME ([a-zA-Z_][a-zA-Z0-9_]*)
VARIABLE (${NAME})
BAD_NAME ([0-9]+[a-zA-Z_][a-zA-Z0-9_]*)
BAD_VARIABLE (${BAD_NAME})
%option noyywrap

%x multi_line_comment
%x string
%x s_string d_string
%x variable_substitution variable_substitution_array variable_substitution_property
%x php

%%

%{
  /* this piece of code gonna be placed in the beginnig of yylex() */
  /* it handles returnig multiple tokens */
  while (!tokens_queue.empty()) {
    int token = tokens_queue.front();
    tokens_queue.pop();
    l.log("token", 0);
    l.log(token);
    return token;
  }
%}

"<?php" |
"<?" |
"<%" {
  BEGIN php;
  l.log("T_OPEN_TAG");
  yylval.r.token_type = NORMAL;
  doForReturn(yytext);
  return T_OPEN_TAG;
}

"<?=" |
"<%=" {
  BEGIN php;
  l.log("T_OPEN_TAG");
  yylval.r.token_type = WITH_ECHO;
  doForReturn(yytext);
  return T_OPEN_TAG;
}

<php>("?>"|"%>") {
  BEGIN INITIAL;
  if (!brackets.empty()) {
    char msg[128];
    bracket b = brackets.top();
    sprintf(msg, "Unexpected end of code: %c at line %d column %d should be matched", b.br, b.line_no, b.col_no);
    errorRec.errQ->enqueue(line_no, col_no, msg, "");
    errorRec.printErrQueue();
    yyterminate();
  }
  doForReturn(yytext);
  l.log("T_CLOSE_TAG");
  return T_CLOSE_TAG;
}

(.|\n)/"<?" {
  if (yytext[0] == '\n') {
    ++line_no;
    col_no = 1;
  } else {
    ++col_no;
  }
  inline_html.push_back(yytext[0]);
  l.log("T_INLINE_HTML:");
  l.log(inline_html);
  yylval.r.str = new char[inline_html.size() + 1];
  strcpy(yylval.r.str, inline_html.c_str());
  inline_html.clear();
  return T_INLINE_HTML;
}

<php><<EOF>> {errorRec.errQ->enqueue(line_no, col_no, "unexpected end of file: missing ?>",""); errorRec.printErrQueue(); yyterminate();}

. |
\n {
  if (yytext[0] == '\n') {
    ++line_no;
    col_no = 1;
  } else {
    ++col_no;
  }
  inline_html.push_back(yytext[0]);
}

<INITIAL><<EOF>> {
  inline_html.push_back(yytext[0]);
  l.log("T_INLINE_HTML");
  l.log(inline_html);
  yylval.r.str = new char[inline_html.size() + 1];
  strcpy(yylval.r.str, inline_html.c_str());
  inline_html.clear();
  /* token 0 indicates the end of file */
  tokens_queue.push(0);
  return T_INLINE_HTML;
}

<php>{
  ("//"|"#").* {
    l.log("SINGLE_LINE_COMMENT");
    doForReturn(yytext);
  }

  "/*" {
    BEGIN multi_line_comment;
    col_no += 2;
    l.log("MULTI_LINE_COMMENT");
  }

  \n   {
    col_no = 1;
    line_no++;
  }

  [ \t\r] {
    ++col_no;
  }
}

<multi_line_comment>{
  "*/" {
    BEGIN php;
    col_no += 2;
    l.log("END_MULTI_LINE_COMMENT");
  }

  <<EOF>> {errorRec.errQ->enqueue(line_no, col_no, "unexpected end of file: unclosed multi-line comment",""); errorRec.printErrQueue(); yyterminate();}

  . {
    ++col_no;
  }

  \n {
    ++line_no;
    col_no = 1;
  }
}

<php>{
  [0-9]+ {
    doForReturn(yytext);
    yylval.r.i = atoi(yytext);
    l.log("T_INTEGER", 0); l.log(yylval.r.i);
    return T_LNUMBER;
  }

  [0-9]*\.[0-9]+{EXP}? |
  [0-9]+\.{EXP}? |
  [0-9]+{EXP} {
    doForReturn(yytext);
    yylval.r.f = atof(yytext);
    l.log("T_FLOAT", 0); l.log(yylval.r.f);
    return T_DNUMBER;
  }

  true {
    doForReturn(yytext);
    l.log("T_TRUE");
    return T_TRUE;
  }

  false {
    doForReturn(yytext);
    l.log("T_FALSE");
    return T_FALSE;
  }
}
<php>"abstract"  {
  doForReturn(yytext);
  l.log("T_ABSTRACT");
  return T_ABSTRACT;
}

  /* Strings */
<php>{
  "'" {
    BEGIN s_string;
    ++col_no;
  }

  "\"" {
    BEGIN d_string;
    // double quoted strig might be splitted into multiple strings due to variables substitution
    // they should be grouped using ( )
    l.log("(");
    ++col_no;
    return '(';
  }
}

  /* single quoted string */
  /* it accepts new lines (multiline string) */
  /* it accepts only \\ and \' as escape characters, \n, \t, \r ... will be outputed literally */
<s_string>{
  \\\\ {str.push_back('\\'); ++col_no;}

  "\\'" {str.push_back('\''); ++col_no;}

  \n {str.push_back('\n'); ++line_no; col_no = 1;}

  "'" {
    yylval.r.str = new char[str.size() + 1];
    strcpy(yylval.r.str, str.c_str());
    l.log("single quoted string:"); l.log(str);
    str.clear();
    BEGIN php;
    return T_CONSTANT_ENCAPSED_STRING;
  }

  . {str.push_back(yytext[0]); ++col_no;}

  <<EOF>> {errorRec.errQ->enqueue(line_no, col_no, "unexpected end of file: unclosed string",""); errorRec.printErrQueue(); yyterminate();}
}

  /* double quoted string */
  /* it has more more escape sequences for special characters than single quoted string */
  /* it can handle variable substitution by spliting the string and variables into multiple concatenated strings and variables
  i.e: "i hate $smo because he's done $smth" --> ("i hate " . $smo . " because he's done " . $smth . "")
  note the parentheses, they are handled */
<d_string>{
  \\n {str.push_back('\n'); ++col_no;}
  \\r {str.push_back('\r'); ++col_no;}
  \\t {str.push_back('\t'); ++col_no;}
  \\e {str.push_back('\e'); ++col_no;}
  \\v {str.push_back('\v'); ++col_no;}
  \\f {str.push_back('\f'); ++col_no;}
  \\\\ {str.push_back('\\'); ++col_no;}
  \\\$ {str.push_back('$'); ++col_no;}
  "\\\"" {str.push_back('"'); ++col_no;}

  \n {str.push_back('\n'); ++line_no; col_no = 1;}

  "\"" {
    ++col_no;
    yylval.r.str = new char[str.size() + 1];
    strcpy(yylval.r.str, str.c_str());
    l.log("double quoted string:"); l.log(str);
    str.clear();
    tokens_queue.push(')');
    BEGIN php;
    return T_CONSTANT_ENCAPSED_STRING;
  }

  {VARIABLE}\[{NAME}\] |
  {VARIABLE}\[[0-9]+\] |
  {VARIABLE}\[{VARIABLE}\] {
    BEGIN variable_substitution_array;
    yylval.r.str = new char[str.size() + 1];
    strcpy(yylval.r.str, str.c_str());
    l.log("double quoted string: "); l.log(str);
    str.clear();
    tokens_queue.push('.');
    yyless(0);
    return T_CONSTANT_ENCAPSED_STRING;
  }

  {VARIABLE}->{NAME} {
    BEGIN variable_substitution_property;
    yylval.r.str = new char[str.size() + 1];
    strcpy(yylval.r.str, str.c_str());
    l.log("double quoted string: "); l.log(str);
    str.clear();
    tokens_queue.push('.');
    yyless(0);
    return T_CONSTANT_ENCAPSED_STRING;
  }

  {VARIABLE} {
    BEGIN variable_substitution;
    yylval.r.str = new char[str.size() + 1];
    strcpy(yylval.r.str, str.c_str());
    l.log("double quoted string: "); l.log(str);
    str.clear();
    tokens_queue.push('.');
    yyless(0);
    return T_CONSTANT_ENCAPSED_STRING;
  }

  . {str.push_back(yytext[0]); ++col_no;}

  <<EOF>> {errorRec.errQ->enqueue(line_no, col_no, "unexpected end of file: unclosed string",""); errorRec.printErrQueue(); yyterminate();}
}

<variable_substitution>{
  {VARIABLE} {
    yylval.r.str = new char[yyleng + 1];
    strcpy(yylval.r.str, yytext);
    doForReturn(yytext);
    l.log("T_VARIABLE", 0); l.log(yytext);
    tokens_queue.push('.');
    BEGIN d_string;
    return T_VARIABLE;
  }
}

<variable_substitution_array>{
  {VARIABLE} {
    yylval.r.str = new char[yyleng + 1];
    strcpy(yylval.r.str, yytext);
    doForReturn(yytext);
    l.log("T_VARIABLE", 0); l.log(yytext);
    return T_VARIABLE;
  }

  \[ {
   doForReturn(yytext);
   l.log("[");
   return '[';
  }

  {NAME} {
    yylval.r.str = new char[yyleng + 1];
    strcpy(yylval.r.str, yytext);
    doForReturn(yytext);
    l.log("T_STRING", 0); l.log(yytext);
    return T_STRING;
  }

  [0-9]+ {
    yylval.r.i = atoi(yytext);
    doForReturn(yytext);
    l.log("T_INTEGER", 0); l.log(yylval.r.i);
    return T_LNUMBER;
  }

  \] {
    doForReturn(yytext);
    l.log("]");
    BEGIN d_string;
    tokens_queue.push('.');
    return ']';
  }
}

<variable_substitution_property>{
  {VARIABLE} {
    yylval.r.str = new char[yyleng + 1];
    strcpy(yylval.r.str, yytext);
    doForReturn(yytext);
    l.log("T_VARIABLE", 0); l.log(yytext);
    return T_VARIABLE;
  }

  -> {
   doForReturn(yytext);
   l.log("->");
   return T_OBJECT_OPERATOR;
  }

  {NAME} {
    yylval.r.str = new char[yyleng + 1];
    strcpy(yylval.r.str, yytext);
    doForReturn(yytext);
    l.log("T_STRING", 0); l.log(yylval.r.str);
    tokens_queue.push('.');
    BEGIN d_string;
    return T_STRING;
  }
}

<php>{
  "\\" |
  "," |
  "=" |
  "?" |
  ":" |
  ";" |
  "|" |
  "^" |
  "&" |
  "<" |
  ">" |
  "+" |
  "-" |
  "." |
  "*" |
  "/" |
  "%" |
  "!" |
  "~" |
  "@" |
  "$" {
    doForReturn(yytext);
    l.log(yytext);
    return yytext[0];
  }

  "[" {
    bracket b = {'[', line_no, col_no};
    brackets.push(b);
    doForReturn(yytext);
    l.log(yytext);
    return yytext[0];
  }

  "]" {
    doForReturn(yytext);
    if (!brackets.empty()) {
      bracket b = brackets.top();
      brackets.pop();
      if (b.br != '[') {
        char msg[128];
        sprintf(msg, "Unexpected ]: bracket %c at line %d column %d should be matched", b.br, b.line_no, b.col_no);
        errorRec.errQ->enqueue(line_no, col_no, msg, "");
        errorRec.printErrQueue();
        yyterminate();
      }
    } else {
      errorRec.errQ->enqueue(line_no, col_no, "stray ]", "");
      errorRec.printErrQueue();
      yyterminate();
    }
    l.log(yytext);
    return yytext[0];
  }

  "{" {
    bracket b = {'{', line_no, col_no};
    brackets.push(b);
    doForReturn(yytext);
    l.log(yytext);
    return yytext[0];
  }

  "}" {
    doForReturn(yytext);
    if (!brackets.empty()) {
      bracket b = brackets.top();
      brackets.pop();
      if (b.br != '{') {
        char msg[128];
        sprintf(msg, "Unexpected }: bracket %c at line %d column %d should be matched", b.br, b.line_no, b.col_no);
        errorRec.errQ->enqueue(line_no, col_no, msg, "");
        errorRec.printErrQueue();
        yyterminate();
      }
    } else {
      errorRec.errQ->enqueue(line_no, col_no, "stray }", "");
      errorRec.printErrQueue();
      yyterminate();
    }
    return yytext[0];
  }

  "(" {
    bracket b = {'(', line_no, col_no};
    brackets.push(b);
    doForReturn(yytext);
    l.log(yytext);
    return yytext[0];
  }

  ")" {
    doForReturn(yytext);
    if (!brackets.empty()) {
      bracket b = brackets.top();
      brackets.pop();
      if (b.br != '(') {
        char msg[128];
        sprintf(msg, "Unexpected ): bracket %c at line %d column %d should be matched", b.br, b.line_no, b.col_no);
        errorRec.errQ->enqueue(line_no, col_no, msg, "");
        errorRec.printErrQueue();
        yyterminate();
      }
    } else {
      errorRec.errQ->enqueue(line_no, col_no, "stray )", "");
      errorRec.printErrQueue();
      yyterminate();
    }
    l.log(yytext);
    return yytext[0];
  }
}

<php>"&="  {
  doForReturn(yytext);
  l.log("T_AND_EQUAL", 0); l.log(yytext);
  return T_AND_EQUAL;
}

<php>"array"  {
  doForReturn(yytext);
  l.log("T_ARRAY");
  return T_ARRAY;
}

<php>\[[ \t]*\] {
  doForReturn(yytext);
  l.log("T_SQUARE_BRACKETS");
  return T_SQUARE_BRACKETS;
}

<php>"as"  {
  doForReturn(yytext);
  l.log("T_AS");
  return T_AS;
}

<php>"&&"  {
  doForReturn(yytext);
  l.log("T_BOOLEAN_AND");
  return T_BOOLEAN_AND;
}

<php>"||"  {
  doForReturn(yytext);
  l.log("T_BOOLEAN_OR");
  return T_BOOLEAN_OR;
}

<php>"break"  {
  doForReturn(yytext);
  l.log("T_BREAK");
  return T_BREAK;
}

<php>"callable"  {
  doForReturn(yytext);
  l.log("T_CALLBACK");
  return T_CALLBACK;
}

<php>"case"  {
  doForReturn(yytext);
  l.log("T_CASE");
  return T_CASE;
}

<php>"catch"  {
  doForReturn(yytext);
  l.log("T_CATCH");
  return T_CATCH;
}

<php>"class"  {
  doForReturn(yytext);
  l.log("T_CLASS");
  return T_CLASS;
}

<php>"__CLASS__"  {
  doForReturn(yytext);
  l.log("T_CLASS_C");
  return T_CLASS_C;
}

<php>"clone"  {
  doForReturn(yytext);
  l.log("T_CLONE");
  return T_CLONE;
}

<php>".="  {
  doForReturn(yytext);
  l.log("T_CONCAT_EQUAL");
  return T_CONCAT_EQUAL;
}

<php>"const"  {
  doForReturn(yytext);
  l.log("T_CONST");
  return T_CONST;
}

<php>"continue"  {
  doForReturn(yytext);
  l.log("T_CONTINUE");
  return T_CONTINUE;
}

<php>"{$"  {
  doForReturn(yytext);
  l.log("T_CURLY_OPEN");
  return T_CURLY_OPEN;
}

<php>"--"  {
  doForReturn(yytext);
  l.log("T_DEC");
  return T_DEC;
}

<php>"declare"  {
  doForReturn(yytext);
  l.log("T_DECLARE");
  return T_DECLARE;
}

<php>"default"  {
  doForReturn(yytext);
  l.log("T_DEFAULT");
  return T_DEFAULT;
}

<php>"__DIR__"  {
  doForReturn(yytext);
  l.log("T_DIR");
  return T_DIR;
}

<php>"/="  {
  doForReturn(yytext);
  l.log("T_DIV_EQUAL");
  return T_DIV_EQUAL;
}

<php>"do"  {
  doForReturn(yytext);
  l.log("T_DO");
  return T_DO;
}

<php>"=>"  {
  doForReturn(yytext);
  l.log("T_DOUBLE_ARROW");
  return T_DOUBLE_ARROW;
}

<php>"::"  {
  doForReturn(yytext);
  l.log("::");
  return T_PAAMAYIM_NEKUDOTAYIM;
}

<php>"echo"  {
  doForReturn(yytext);
  l.log("T_ECHO");
  return T_ECHO;
}

<php>"..."  {
  doForReturn(yytext);
  l.log("...");
  return T_ELLIPSIS;
}

<php>"else"  {
  doForReturn(yytext);
  l.log("T_ELSE");
  return T_ELSE;
}

<php>"elseif"  {
  doForReturn(yytext);
  l.log("T_ELSEIF");
  return T_ELSEIF;
}

<php>"empty"  {
  doForReturn(yytext);
  l.log("T_EMPTY");
  return T_EMPTY;
}

<php>"enddeclare"  {
  doForReturn(yytext);
  l.log("T_ENDDECLARE");
  return T_ENDDECLARE;
}

<php>"endfor"  {
  doForReturn(yytext);
  l.log("T_ENDFOR");
  return T_ENDFOR;
}

<php>"endforeach"  {
  doForReturn(yytext);
  return T_ENDFOREACH;
  l.log("T_ENDFOREACH");
}

<php>"endif"  {
  doForReturn(yytext);
  l.log("T_ENDIF");
  return T_ENDIF;
}

<php>"endswitch"  {
  doForReturn(yytext);
  l.log("T_ENDSWITCH");
  return T_ENDSWITCH;
}

<php>"endwhile"  {
  doForReturn(yytext);
  l.log("T_ENDWHILE");
  return T_ENDWHILE;
}

<php>"eval"  {
  doForReturn(yytext);
  l.log("T_EVAL");
  return T_EVAL;
}

<php>("exit"|"die") {
  doForReturn(yytext);
  l.log("T_EXIT");
  return T_EXIT;
}

<php>"extends"  {
  doForReturn(yytext);
  l.log("T_EXTENDS");
  return T_EXTENDS;
}

<php>"__FILE__"  {
  doForReturn(yytext);
  l.log("T_FILE");
  return T_FILE;
}

<php>"final"  {
  doForReturn(yytext);
  l.log("T_FINAL");
  return T_FINAL;
}

<php>"finally"  {
  doForReturn(yytext);
  l.log("T_FINALLY");
  return T_FINALLY;
}

<php>"for"  {
  doForReturn(yytext);
  l.log("T_FOR");
  return T_FOR;
}

<php>"foreach"  {
  doForReturn(yytext);
  l.log("T_FOREACH");
  return T_FOREACH;
}

<php>"function"  {
  doForReturn(yytext);
  l.log("T_FUNCTION");
  return T_FUNCTION;
}

<php>"global"  {
  doForReturn(yytext);
  l.log("T_GLOBAL");
  return T_GLOBAL;
}

<php>"goto"  {
  doForReturn(yytext);
  l.log("T_GOTO");
  return T_GOTO;
}

<php>"if"  {
  doForReturn(yytext);
  l.log("T_IF");
  return T_IF;
}

<php>"implements"  {
  doForReturn(yytext);
  l.log("T_IMPLEMENTS");
  return T_IMPLEMENTS;
}

<php>"++"  {
  doForReturn(yytext);
  l.log("T_INC");
  return T_INC;
}

<php>"include"  {
  doForReturn(yytext);
  l.log("T_INCLUDE");
  return T_INCLUDE;
}

<php>"include_once"  {
  doForReturn(yytext);
  l.log("T_INCLUDE_ONCE");
  return T_INCLUDE_ONCE;
}

<php>"instanceof"  {
  doForReturn(yytext);
  l.log("T_INSTANCEOF");
  return T_INSTANCEOF;
}

<php>"insteadof"  {
  doForReturn(yytext);
  l.log("T_INSTEADOF");
  return T_INSTEADOF;
}

<php>"interface"  {
  doForReturn(yytext);
  l.log("T_INTERFACE");
  return T_INTERFACE;
}

<php>"isset"  {
  doForReturn(yytext);
  l.log("T_ISSET");
  return T_ISSET;
}

<php>"=="  {
  doForReturn(yytext);
  l.log("T_IS_EQUAL");
  return T_IS_EQUAL;
}

<php>">="  {
  doForReturn(yytext);
  l.log("T_IS_GREATER_OR_EQUAL");
  return T_IS_GREATER_OR_EQUAL;
}

<php>"==="  {
  doForReturn(yytext);
  l.log("T_IS_IDENTICAL");
  return T_IS_IDENTICAL;
}

<php>("!="|"<>")  {
  doForReturn(yytext);
  l.log("T_IS_NOT_EQUAL");
  return T_IS_NOT_EQUAL;
}

<php>"!=="  {
  doForReturn(yytext);
  l.log("T_IS_NOT_IDENTICAL");
  return T_IS_NOT_IDENTICAL;
}

<php>"<="  {
  doForReturn(yytext);
  l.log("T_IS_SMALLER_OR_EQUAL");
  return T_IS_SMALLER_OR_EQUAL;
}

<php>"__LINE__"  {
  doForReturn(yytext);
  l.log("T_LINE");
  return T_LINE;
}

<php>"list"  {
  doForReturn(yytext);
  l.log("T_LIST");
  return T_LIST;
}

<php>"and"  {
  doForReturn(yytext);
  l.log("T_LOGICAL_AND");
  return T_LOGICAL_AND;
}

<php>"or"  {
  doForReturn(yytext);
  l.log("T_LOGICAL_OR");
  return T_LOGICAL_OR;
}

<php>"xor"  {
  doForReturn(yytext);
  l.log("T_LOGICAL_XOR");
  return T_LOGICAL_XOR;
}

<php>"-="  {
  doForReturn(yytext);
  l.log("T_MINUS_EQUAL");
  return T_MINUS_EQUAL;
}

<php>"%="  {
  doForReturn(yytext);
  l.log("T_MOD_EQUAL");
  return T_MOD_EQUAL;
}

<php>"*="  {
  doForReturn(yytext);
  l.log("T_MUL_EQUAL");
  return T_MUL_EQUAL;
}

<php>"namespace"  {
  doForReturn(yytext);
  l.log("T_NAMESPACE");
  return T_NAMESPACE;
}

<php>"new"  {
  doForReturn(yytext);
  l.log("T_NEW");
  return T_NEW;
}

<php>"->"  {
  doForReturn(yytext);
  l.log(yytext);
  return T_OBJECT_OPERATOR;
}

<php>"|="  {
  doForReturn(yytext);
  l.log("T_OR_EQUAL");
  return T_OR_EQUAL;
}

<php>"+="  {
  doForReturn(yytext);
  l.log("T_PLUS_EQUAL");
  return T_PLUS_EQUAL;
}

<php>"**"  {
  doForReturn(yytext);
  l.log("T_POW");
  return T_POW;
}

<php>"**="  {
  doForReturn(yytext);
  l.log("T_POW_EQUAL");
  return T_POW_EQUAL;
}

<php>"print"  {
  doForReturn(yytext);
  l.log("T_PRINT");
  return T_PRINT;
}

<php>"private"  {
  doForReturn(yytext);
  l.log("T_PRIVATE");
  return T_PRIVATE;
}

<php>"public"  {
  doForReturn(yytext);
  l.log("T_PUBLIC");
  return T_PUBLIC;
}

<php>"protected"  {
  doForReturn(yytext);
  l.log("T_PROTECTED");
  return T_PROTECTED;
}

<php>"require"  {
  doForReturn(yytext);
  l.log("T_REQUIRE");
  return T_REQUIRE;
}

<php>"require_once"  {
  doForReturn(yytext);
  l.log("T_REQUIRE_ONCE");
  return T_REQUIRE_ONCE;
}

<php>"return"  {
  doForReturn(yytext);
  l.log("T_RETURN");
  return T_RETURN;
}

<php>"<<"  {
  doForReturn(yytext);
  l.log("T_SL", 0); l.log(yytext);
  return T_SL;
}

<php>"<<="  {
  doForReturn(yytext);
  l.log("T_SL_EQUAL", 0); l.log(yytext);
  return T_SL_EQUAL;
}

<php>">>"  {
  doForReturn(yytext);
  l.log("T_SR", 0); l.log(yytext);
  return T_SR;
}

<php>">>="  {
  doForReturn(yytext);
  l.log("T_SR_EQUAL", 0); l.log(yytext);
  return T_SR_EQUAL;
}

<php>"static"  {
  doForReturn(yytext);
  l.log("T_STATIC");
  return T_STATIC;
}

<php>"switch"  {
  doForReturn(yytext);
  l.log("T_SWITCH");
  return T_SWITCH;
}

<php>"throw"  {
  doForReturn(yytext);
  l.log("T_THROW");
  return T_THROW;
}

<php>"trait"  {
  doForReturn(yytext);
  l.log("T_TRAIT");
  return T_TRAIT;
}

<php>"try"  {
  doForReturn(yytext);
  l.log("T_TRY");
  return T_TRY;
}

<php>"unset"  {
  doForReturn(yytext);
  l.log("T_UNSET");
  return T_UNSET;
}

<php>"(unset)"  {
  doForReturn(yytext);
  return T_UNSET_CAST;
}

<php>"use"  {
  doForReturn(yytext);
  l.log("T_USE");
  return T_USE;
}

<php>"var"  {
  doForReturn(yytext);
  l.log("T_VAR");
  return T_VAR;
}

<php>"while"  {
  doForReturn(yytext);
  l.log("T_WHILE");
  return T_WHILE;
}

<php>"^="  {
  doForReturn(yytext);
  l.log("T_XOR_EQUAL");
  return T_XOR_EQUAL;
}

<php>"yield"  {
  doForReturn(yytext);
  l.log("T_YIELD");
  return T_YIELD;
}

  /* Types */
<php>"int"   {
  doForReturn(yytext);
  l.log("T_PRIMITIVE: T_INT");
  yylval.r.token_type = T_PRIMITIVE_INTEGER;
  return T_PRIMITIVE;
}

<php>"double"  {
  doForReturn(yytext);
  l.log("T_PRIMITIVE: T_DOUBLE");
  yylval.r.token_type = T_PRIMITIVE_DOUBLE;
  return T_PRIMITIVE;
}

<php>"float"   {
  doForReturn(yytext);
  l.log("T_PRIMITIVE: T_FLOAT");
  yylval.r.token_type = T_PRIMITIVE_FLOAT;
  return T_PRIMITIVE;
}

<php>"char"  {
  doForReturn(yytext);
  l.log("T_PRIMITIVE: T_CHAR");
  yylval.r.token_type = T_PRIMITIVE_CHAR;
  return T_PRIMITIVE;
}

<php>("boolean"|"bool")   {
  doForReturn(yytext);
  l.log("T_PRIMITIVE: BOOLEAN");
  yylval.r.token_type = T_PRIMITIVE_BOOLEAN;
  return T_PRIMITIVE;
}

<php>"string"   {
  doForReturn(yytext);
  l.log("T_PRIMITIVE: STRING");
  yylval.r.token_type = T_PRIMITIVE_STRING;
  return T_PRIMITIVE;
}

<php>"void" {
  doForReturn(yytext);
  l.log("T_PRIMITIVE: VOID");
  yylval.r.token_type = T_PRIMITIVE_VOID;
  return T_PRIMITIVE;
}

<php>{NAME} {
  doForReturn(yytext);
  yylval.r.str = new char[yyleng + 1];
  strcpy(yylval.r.str, yytext);
  l.log("T_STRING", 0); l.log(yytext);
  return T_STRING;
}

<php>{VARIABLE}  {
  doForReturn(yytext);
  yylval.r.str = new char[yyleng + 1];
  strcpy(yylval.r.str, yytext);
  l.log("T_VARIABLE", 0); l.log(yytext);
  return T_VARIABLE;
}

<php>{BAD_NAME} {
  doForReturn(yytext);
  yylval.r.str = new char[yyleng + 1];
  strcpy(yylval.r.str, yytext);
  char msg[128];
  sprintf(msg, "Bad name '%s': names can't start with a digit", yytext);
  errorRec.errQ->enqueue(line_no, col_no, msg, "");
  l.log("T_STRING: BAD", 0); l.log(yytext);
  return T_STRING;
}

<php>{BAD_VARIABLE}  {
  doForReturn(yytext);
  yylval.r.str = new char[yyleng + 1];
  strcpy(yylval.r.str, yytext);
  char msg[128];
  sprintf(msg, "Bad variable name '%s': variables names can't start with a digit", yytext);
  errorRec.errQ->enqueue(line_no, col_no, msg, "");
  l.log("T_VARIABLE: BAD", 0); l.log(yytext);
  return T_VARIABLE;
}
<php>.  {
  doForReturn(yytext);
  char msg[128];
  sprintf(msg, "Unexpected character '%c'", yytext);
  errorRec.errQ->enqueue(line_no, col_no, msg, "");
  l.log("T_BAD_INPUT", 0); l.log(yytext);
  return T_BAD_INPUT;
}

%%
