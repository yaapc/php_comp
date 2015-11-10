%{
        #include <stdlib.h>
		#include <unistd.h>
		#include <iostream>
		#include <string>
		using namespace std;
		int lineNo = 0;
		int colNo = 0;
		
		void doForReturn(string yytext){
			//cout << yytext << endl;
			/*
			yylval.r.myColno = colNo;
			yylval.r.myLineNo = lineNo;
			colNo += strlen(yytext);
							*/
		}
		
		enum yytokenType{
			T_ABSTRACT = 258,
			T_AND_EQUAL = 259,
			T_ARRAY = 300,
			T_ARRAY_CAST = 301, 
			T_AS = 302,
			T_BOOLEAN_AND = 303, 
			T_BOOLEAN_CAST = 304,
			T_BOOLEAN_OR = 305,
			T_BREAK = 306,
			T_CALLBACK = 307,
			T_CASE = 308,
			T_CATCH = 309,
			T_CLASS = 310,
			T_CLASS_C = 311,
			T_CLONE = 312,
			T_CLOSE_TAG = 313, 
			T_CONCAT_EQUAL = 314,
			T_CONST = 315,
			T_CONTINUE = 316,
			T_CURLY_OPEN = 317,
			T_DEC = 318,
			T_DECLARE =319, 
			T_DEFAULT = 320,
			T_DIR = 321,
			T_DIV_EQUAL = 322,
			T_DNUMBER = 323,
			T_DO = 324,
			T_DOUBLE_ARROW = 325, 
			T_DOUBLE_CAST = 327,
			T_DOUBLE_COLON = 328,
			T_ECHO = 329,
			T_ELLIPSIS =330, 
			T_ELSE = 331,
			T_ELSEIF = 332,
			T_EMPTY = 333,
			T_ENDDECLARE = 334,
			T_ENDFOR = 335,
			T_ENDFOREACH = 336,
			T_ENDIF = 337,
			T_ENDSWITCH =338, 
			T_ENDWHILE = 339,
			T_EVAL = 340,
			T_EXIT = 341,
			T_EXTENDS = 342,
			T_FILE = 343,
			T_FINAL = 344,
			T_FINALLY = 345,
			T_FOR = 346,
			T_FOREACH =347, 
			T_FUNCTION = 348,
			T_GLOBAL =349,
			T_GOTO = 350,
			T_IF = 351,
			T_IMPLEMENTS = 352,
			T_INC = 353,
			T_INCLUDE = 354, 
			T_INCLUDE_ONCE = 355, 
			T_INSTANCEOF = 356,
			T_INSTEADOF = 357,
			T_INTERFACE = 358,
			T_INT_CAST = 359,
			T_ISSET = 360,
			T_IS_EQUAL = 361,
			T_IS_GREATER_OR_EQUAL = 362, 
			T_IS_IDENTICAL = 364,
			T_IS_NOT_EQUAL = 365,
			T_IS_NOT_IDENTICAL =366, 
			T_IS_SMALLER_OR_EQUAL =367, 
			T_LINE = 368,
			T_LIST = 369,
			T_LNUMBER = 370,
			T_LOGICAL_AND =371, 
			T_LOGICAL_OR = 372,
			T_LOGICAL_XOR = 373,
			T_MINUS_EQUAL = 374,
			T_MOD_EQUAL = 375,
			T_MUL_EQUAL = 376,
			T_NAMESPACE = 377,
			T_NEW = 378,
			T_OBJECT_CAST = 379, 
			T_OBJECT_OPERATOR =380, 
			T_OPEN_TAG = 381,
			T_OR_EQUAL = 382,
			T_PLUS_EQUAL = 383,
			T_POW = 384,
			T_POW_EQUAL = 385, 
			T_PRINT = 386,
			T_PRIVATE = 387,
			T_PUBLIC = 388,
			T_PROTECTED = 389,
			T_REQUIRE = 390,
			T_REQUIRE_ONCE = 391, 
			T_RETURN = 392,
			T_SL = 393,
			T_SL_EQUAL = 394, 
			T_SR = 999,
			T_SR_EQUAL = 395,
			T_STATIC = 396,
			T_STRING_CAST = 397, 
			T_SWITCH = 398,
			T_THROW = 399,
			T_TRAIT = 400,
			T_TRY = 401,
			T_UNSET = 402,
			T_UNSET_CAST = 403, 
			T_USE = 404,
			T_VAR = 405,
			T_VARIABLE= 406, 
			T_WHILE = 407,
			T_XOR_EQUAL =408, 
			T_YIELD = 409,
			T_NAME = 410,
			T_WRONG_VARIABLE = 411
		};
		
		struct LVAL {
			struct r {
				int myColno = 0;
				int myLineNo = 0;
			} r;
		} myyylval;
		int yylval;
%}
DIGIT [0-9]
LITERAL [a-zA-Z]
%option c++
%option noyywrap
%x multiLineComment
%x string
%%
"abstract"				{
							doForReturn(yytext);
							return T_ABSTRACT;	
						}
"&="					{
							doForReturn(yytext);
							return T_AND_EQUAL;	
						}

"array()"				{
							doForReturn(yytext);
							return T_ARRAY;	
						}
"(array)"				{
							doForReturn(yytext);
							return T_ARRAY_CAST;	
						}
"as"					{
							doForReturn(yytext);
							return T_AS;	
						}
"&&"					{
							doForReturn(yytext);
							return T_BOOLEAN_AND;	
						}
"||"					{
							doForReturn(yytext);
							return T_BOOLEAN_OR;	
						}
"(bool)"|"(boolean)"	{
							doForReturn(yytext);
							return T_BOOLEAN_CAST;	
						}
"break"					{
							doForReturn(yytext);
							return T_BREAK;	
						}
"callable"				{
							doForReturn(yytext);
							return T_CALLBACK;	
						}
"case"					{
							doForReturn(yytext);
							return T_CASE;	
						}
"catch"					{
							doForReturn(yytext);
							return T_CATCH;	
						}
"class"					{
							doForReturn(yytext);
							return T_CLASS;	
						}
"__CLASS__"				{
							doForReturn(yytext);
							return T_CLASS_C;	
						}
"clone"					{
							doForReturn(yytext);
							return T_CLONE;	
						}
"?>"|"%>"				{
							doForReturn(yytext);
							return T_CLOSE_TAG;	
						}
".="					{
							doForReturn(yytext);
							return T_CONCAT_EQUAL;	
						}
"const"					{
							doForReturn(yytext);
							return T_CONST;	
						}
"continue"				{
							doForReturn(yytext);
							return T_CONTINUE;	
						}
"{$"					{
							doForReturn(yytext);
							return T_CURLY_OPEN;	
						}
"--"					{
							doForReturn(yytext);
							return T_DEC;	
						}
"declare"				{
							doForReturn(yytext);
							return T_DECLARE;	
						}
"default"				{
							doForReturn(yytext);
							return T_DEFAULT;	
						}
"__DIR__"				{
							doForReturn(yytext);
							return T_DIR;	
						}
"/="					{
							doForReturn(yytext);
							return T_DIV_EQUAL;	
						}
"do"					{
							doForReturn(yytext);
							return T_DO;	
						}
"=>"					{
							doForReturn(yytext);
							return T_DOUBLE_ARROW;	
						}
"(real)"|"(double)"|"(float)"				{
							doForReturn(yytext);
							return T_DOUBLE_CAST;	
						}
"::"					{
							doForReturn(yytext);
							return T_DOUBLE_COLON;	
						}
"echo"					{
							doForReturn(yytext);
							return T_ECHO;	
						}
"..."					{
							doForReturn(yytext);
							return T_ELLIPSIS;	
						}
"else"					{
							doForReturn(yytext);
							return T_ELSE;	
						}
"elseif"				{
							doForReturn(yytext);
							return T_ELSEIF;	
						}
"empty"					{
							doForReturn(yytext);
							return T_EMPTY;	
						}
"enddeclare"			{
							doForReturn(yytext);
							return T_ENDDECLARE;	
						}
"endfor"				{
							doForReturn(yytext);
							return T_ENDFOR;	
						}
"endforeach"			{
							doForReturn(yytext);
							return T_ENDFOREACH;	
						}
"endif"					{
							doForReturn(yytext);
							return T_ENDIF;	
						}
"endswitch"				{
							doForReturn(yytext);
							return T_ENDSWITCH;	
						}
"endwhile"				{
							doForReturn(yytext);
							return T_ENDWHILE;	
						}
"eval()"				{
							doForReturn(yytext);
							return T_EVAL;	
						}
"exit"|"die"			{
							doForReturn(yytext);
							return T_EXIT;	
						}
"extends"				{
							doForReturn(yytext);
							return T_EXTENDS;	
						}
"__FILE__"				{
							doForReturn(yytext);
							return T_FILE;	
						}
"final"					{
							doForReturn(yytext);
							return T_FINAL;	
						}
"finally"				{
							doForReturn(yytext);
							return T_FINALLY;	
						}
"for"					{
							doForReturn(yytext);
							return T_FOR;	
						}
"foreach"				{
							doForReturn(yytext);
							return T_FOREACH;	
						}
"function"				{
							doForReturn(yytext);
							return T_FUNCTION;	
						}
"global"				{
							doForReturn(yytext);
							return T_GLOBAL;	
						}
"goto"					{
							doForReturn(yytext);
							return T_GOTO;	
						}
"if"					{
							doForReturn(yytext);
							return T_IF;	
						}
"implements"				{
							doForReturn(yytext);
							return T_IMPLEMENTS;	
						}
"++"					{
							doForReturn(yytext);
							return T_INC;	
						}
"include"				{
							doForReturn(yytext);
							return T_INCLUDE;	
						}
"include_once"			{
							doForReturn(yytext);
							return T_INCLUDE_ONCE;	
						}
"instanceof"			{
							doForReturn(yytext);
							return T_INSTANCEOF;	
						}
"insteadof"				{
							doForReturn(yytext);
							return T_INSTEADOF;	
						}
"(int)"|"(integer)"	{
							doForReturn(yytext);
							return T_INT_CAST;	
						}
"interface"				{
							doForReturn(yytext);
							return T_INTERFACE;	
						}
"isset"					{
							doForReturn(yytext);
							return T_ISSET;	
						}
"=="					{
							doForReturn(yytext);
							return T_IS_EQUAL;	
						}
">="					{
							doForReturn(yytext);
							return T_IS_GREATER_OR_EQUAL;	
						}
"==="					{
							doForReturn(yytext);
							return T_IS_IDENTICAL;	
						}
"!="|"<>"				{
							doForReturn(yytext);
							return T_IS_NOT_EQUAL;	
						}
"!=="					{
							doForReturn(yytext);
							return T_IS_NOT_IDENTICAL;	
						}
"<="					{
							doForReturn(yytext);
							return T_IS_SMALLER_OR_EQUAL;	
						}
"__LINE__"				{
							doForReturn(yytext);
							return T_LINE;	
						}
"list()"				{
							doForReturn(yytext);
							return T_LIST;	
						}
"and"					{
							doForReturn(yytext);
							return T_LOGICAL_AND;	
						}
"or"					{
							doForReturn(yytext);
							return T_LOGICAL_OR;	
						}
"xor"					{
							doForReturn(yytext);
							return T_LOGICAL_XOR;	
						}
"-="					{
							doForReturn(yytext);
							return T_MINUS_EQUAL;	
						}
"%="					{
							doForReturn(yytext);
							return T_MOD_EQUAL;	
						}
"*="					{
							doForReturn(yytext);
							return T_MUL_EQUAL;	
						}
"namespace"				{
							doForReturn(yytext);
							return T_NAMESPACE;	
						}
"new"					{
							doForReturn(yytext);
							return T_NEW;	
						}
"(object)"				{
							doForReturn(yytext);
							return T_OBJECT_CAST;	
						}
"->"					{
							doForReturn(yytext);
							return T_OBJECT_OPERATOR;	
						}
"<?php"|"<?"|"<%"	{
							doForReturn(yytext);
							return T_OPEN_TAG;	
						}
"|="					{
							doForReturn(yytext);
							return T_OR_EQUAL;	
						}
"+="					{
							doForReturn(yytext);
							return T_PLUS_EQUAL;	
						}
"**"					{
							doForReturn(yytext);
							return T_POW;	
						}
"**="					{
							doForReturn(yytext);
							return T_POW_EQUAL;	
						}
"print()"				{
							doForReturn(yytext);
							return T_PRINT;	
						}
"private"				{
							doForReturn(yytext);
							return T_PRIVATE;	
						}
"public"				{
							doForReturn(yytext);
							return T_PUBLIC;	
						}
"protected"				{
							doForReturn(yytext);
							return T_PROTECTED;	
						}
"require"				{
							doForReturn(yytext);
							return T_REQUIRE;	
						}
"require_once"			{
							doForReturn(yytext);
							return T_REQUIRE_ONCE;	
						}
"return"				{
							doForReturn(yytext);
							return T_RETURN;	
						}
"<<"					{
							doForReturn(yytext);
							return T_SL;	
						}
"<<="					{
							doForReturn(yytext);
							return T_SL_EQUAL;	
						}
">>"					{
							doForReturn(yytext);
							return T_SR;	
						}
">>="					{
							doForReturn(yytext);
							return T_SR_EQUAL;	
						}
"static"				{
							doForReturn(yytext);
							return T_STATIC;	
						}
"(string)"				{
							doForReturn(yytext);
							return T_STRING_CAST;	
						}
"switch"				{
							doForReturn(yytext);
							return T_SWITCH;	
						}
"throw"					{
							doForReturn(yytext);
							return T_THROW;	
						}
"trait"					{
							doForReturn(yytext);
							return T_TRAIT;	
						}
"try"					{
							doForReturn(yytext);
							return T_TRY;	
						}
"unset()"				{
							doForReturn(yytext);
							return T_UNSET;	
						}
"(unset)"				{
							doForReturn(yytext);
							return T_UNSET_CAST;	
						}
"use"					{
							doForReturn(yytext);
							return T_USE;	
						}
"var"					{
							doForReturn(yytext);
							return T_VAR;	
						}
"while"					{
							doForReturn(yytext);
							return T_WHILE;	
						}
"^="					{
							doForReturn(yytext);
							return T_XOR_EQUAL;	
						}
"yield"					{
							doForReturn(yytext);
							return T_YIELD;	
						}
$[a-zA-Z_]({DIGIT}|{LITERAL})*	{
							doForReturn(yytext);
							return T_VARIABLE;
						}
${DIGIT}+.*             {
							doForReturn(yytext);
							return T_WRONG_VARIABLE;
						}
[+-]?({DIGIT}*\.?{DIGIT}+|{DIGIT}+\.) {
							doForReturn(yytext);
							return T_DNUMBER;
						}						
\n 						{
							//yylval.r.myColno = colNo;
							//yylval.r.myLineNo = lineNo;
							colNo =0;
							lineNo++;
						}
\/\*                    {
							BEGIN multiLineComment;
						}
<multiLineComment>\*\/  {   
							BEGIN INITIAL;
							cout << "That was a MultiLineComment :P\n";
						}
<multiLineComment>[^\*\/]* {;}
\"                      {
							BEGIN string;
						}
<string>[^\"]            {;}
<string>\"              {   
							BEGIN INITIAL;
							cout << "That was a STRING\n";
						}


						
.*	{
		doForReturn(yytext);
		return T_NAME;
	}				
%%
int main()
{	
	FlexLexer* lexer = new yyFlexLexer;
	int tokenType;
	while(tokenType = lexer->yylex()){
		cout << tokenType << endl;
	}
	cout << "Columns:" << colNo << "|Lines:" << lineNo;
    return 0;
}
/**
	
[+-]?[0-9]+                  {
							doForReturn(yytext);
							//return T_LNUMBER;	
						}
*/
