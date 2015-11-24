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
"boolean"               {
							doForReturn(yytext);
							yylval.r.type = T_TYPE_BOOLEAN;
							return T_TYPE;
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
"double"			    {
							doForReturn(yytext);
							yylval.r.type = T_TYPE_DOUBLE;
							return T_TYPE;
						}
"float"                 {
							doForReturn(yytext);
							yylval.r.type = T_TYPE_FLOAT;
							return T_TYPE;
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
"int"                   {
							doForReturn(yytext);
							yylval.r.type = T_TYPE_INTEGER;
							return T_TYPE;
						}
"(int)"|"(integer)"	    {
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
"string"				{
							doForReturn(yytext);
							yylval.r.type = T_TYPE_STRING;
							return T_TYPE;
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
"char"					{
							doForReturn(yytext);
							yylval.r.type = T_TYPE_CHAR;
							return T_TYPE;
						}
"boolean[]"				{
							doForReturn(yytext);
							return T_BOOLEAN_ARRAY;
						}
"string[]"				{
							doForReturn(yytext);
							return T_STRING_ARRAY;
						}
"char[]"				{
							doForReturn(yytext);
							return T_CHAR_ARRAY;
						}
"int[]"					{
							doForReturn(yytext);
							return T_INTEGER_ARRAY;
						}
"float[]"				{
							doForReturn(yytext);
							return T_FLOAT_ARRAY;
						}
"double[]"				{
							doForReturn(yytext);
							return T_DOUBLE_ARRAY;
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
/**
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

	
[+-]?[0-9]+                  {
							doForReturn(yytext);
							//return T_LNUMBER;	
						}
*/
