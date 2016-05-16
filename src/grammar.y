%output = "grammar.cpp"
%{
	#include <iostream>
	#include <fstream>
	using namespace std;
	#include "definitions.h"
	#include "Logger.hpp"
	#include "ErrorRecovery/ErrorRecovery.h"
	#include "TypeChecker.h"

	#include "AST/all.hpp"

	Logger pl("parser_log.txt");
	ErrorRecovery errorRec;

	/**
	*	 SymbolsParser definations:
	*	 --------------------------
	*/
	SymbolsParser * symbolsParser;
	bool forLoopFlag = false;
	bool modifierListingFlag = false;

	Class* outerClass; // a Class symbol to keep track of outerclasses when declaring inner classes

	// an array to group modifiers (access, storage)
	int *modifiersTags = new int[25]; // max of 25 modifier
	int arrCounter = 0;

	/** this routine is used to initialize the symbols parser.
	 *	TODO: add Object class to root symbol table.
	 *		add primitive types to root symbol table.
	 */
	void initSymbolsParser(){
		symbolsParser = new SymbolsParser();
		symbolsParser->insertSymbol(new Class("Object", 1, 1, nullptr));
	}

	/**
	*	 Type Checker definations:
	*	 --------------------------
	*/
	TypeChecker* typeChecker;
	void initTypeChecker(){
		typeChecker = new TypeChecker(&errorRec, symbolsParser);
	}

	ListNode *tree = new ListNode();
%}

%nonassoc _def_val_ low_prec
%left T_INCLUDE T_INCLUDE_ONCE T_EVAL T_REQUIRE T_REQUIRE_ONCE
%left ',' ';'
%left T_LOGICAL_OR
%left T_LOGICAL_XOR
%left T_LOGICAL_AND
%right T_PRINT
%right T_YIELD
%right T_DOUBLE_ARROW
%right T_YIELD_FROM
%left '=' T_PLUS_EQUAL T_MINUS_EQUAL T_MUL_EQUAL T_DIV_EQUAL T_CONCAT_EQUAL T_MOD_EQUAL T_AND_EQUAL T_OR_EQUAL T_XOR_EQUAL T_SL_EQUAL T_SR_EQUAL T_POW_EQUAL
%left '?' ':' T_DOUBLE_COLON
%right T_COALESCE
%left T_BOOLEAN_OR
%left T_BOOLEAN_AND
%left '|'
%left '^'
%left '&'
%nonassoc T_IS_EQUAL T_IS_NOT_EQUAL T_IS_IDENTICAL T_IS_NOT_IDENTICAL T_SPACESHIP
%nonassoc '<' T_IS_SMALLER_OR_EQUAL '>' T_IS_GREATER_OR_EQUAL
%left T_SL T_SR
%left '+' '-' '.'
%left '*' '/' '%'
%right '!'
%nonassoc T_INSTANCEOF
%right '~' T_INC T_DEC T_INT_CAST T_DOUBLE_CAST T_STRING_CAST T_ARRAY_CAST T_OBJECT_CAST T_BOOL_CAST T_UNSET_CAST '@'
%right T_POW
%left '[' '{' ']' '}'
%nonassoc T_NEW T_CLONE
%token T_EXIT
%token T_IF
%left T_ELSEIF
%left T_ELSE
%left T_ENDIF
%token T_TRUE T_FALSE
%token T_LNUMBER
%token T_DNUMBER
%token T_STRING
%token T_STRING_VARNAME
%token T_PRIMITIVE
%token T_VARIABLE
%token T_NUM_STRING
%left T_INLINE_HTML
%token T_CHARACTER
%token T_BAD_CHARACTER
%token T_ENCAPSED_AND_WHITESPACE
%token T_CONSTANT_ENCAPSED_STRING
%token T_ECHO
%token T_DO
%token T_WHILE
%token T_ENDWHILE
%token T_FOR
%token T_ENDFOR
%token T_FOREACH
%token T_ENDFOREACH
%token T_DECLARE
%token T_ENDDECLARE
%token T_AS
%token T_SWITCH
%token T_ENDSWITCH
%token T_CASE
%token T_DEFAULT
%token T_BREAK
%token T_CONTINUE
%token T_GOTO
%token T_FUNCTION
%token T_CONST
%token T_RETURN
%token T_TRY
%token T_CATCH
%token T_FINALLY
%token T_THROW
%token T_USE
%token T_INSTEADOF
%token T_GLOBAL
%right T_STATIC T_ABSTRACT T_FINAL T_PRIVATE T_PROTECTED T_PUBLIC
%token T_VAR
%token T_UNSET
%token T_ISSET
%token T_EMPTY
%token T_HALT_COMPILER
%token T_CLASS
%token T_TRAIT
%token T_INTERFACE
%token T_EXTENDS
%token T_IMPLEMENTS
%token T_OBJECT_OPERATOR
%token T_DOUBLE_ARROW
%token T_LIST
%token T_ARRAY
%token T_CALLABLE
%token T_CLASS_C
%token T_TRAIT_C
%token T_METHOD_C
%token T_FUNC_C
%token T_LINE
%token T_FILE
%token T_COMMENT
%token T_DOC_COMMENT
%token T_OPEN_TAG
%token T_CLOSE_TAG
%token T_SQUARE_BRACKETS
%token T_WHITESPACE
%token T_START_HEREDOC
%token T_END_HEREDOC
%token T_DOLLAR_OPEN_CURLY_BRACES
%token T_CURLY_OPEN
%token T_PAAMAYIM_NEKUDOTAYIM
%token T_NAMESPACE
%token T_NS_C
%token T_DIR
%token '\\'
%token T_ELLIPSIS
%token T_CALLBACK
%token T_BAD_INPUT
%left '(' ')'


%union {
	struct R{
		int i;
		float f;
		char c;
		char* str;
		int line_no;
		int col_no;
		int token_type;
		int m; // modifier
		//symbol
		class Symbol * symbol;
		class Scope * scope;
		class Node* node;
	} r;

}


%%
program: start {pl.log("program");}

start:
		start_part { pl.log("start"); tree->add_nodes(dynamic_cast<ListNode*>($<r.node>1)->nodes); }
	| start start_part { tree->add_nodes(dynamic_cast<ListNode*>($<r.node>2)->nodes); }
;

start_part:
	optional_inline_html T_OPEN_TAG top_statement_list T_CLOSE_TAG optional_inline_html	{
		pl.log("start part");
		auto list = new ListNode();
		if ($<r.node>1) list->add_node($<r.node>1);
		list->add_nodes(dynamic_cast<ListNode*>($<r.node>3)->nodes);
		if ($<r.node>5) list->add_node($<r.node>5);
	 	$<r.node>$ = list; }
;

optional_inline_html:
		T_INLINE_HTML {
			pl.log("inline_html");
			$<r.node>$ = new EchoNode(new ScalarNode(string($<r.str>1))); }
	| /* empty */ %prec low_prec { $<r.node>$ = nullptr; }
;

top_statement_list:
		top_statement_list top_statement	{
			pl.log("top_statement");
			//$<r.node>2 = new Node();
			dynamic_cast<ListNode*>($<r.node>1)->add_node($<r.node>2);
			$<r.node>$ = $<r.node>1; }
	| /* empty */ { $<r.node>$ = new ListNode(); }
;

reserved_non_modifiers:
		T_INCLUDE | T_INCLUDE_ONCE | T_EVAL | T_REQUIRE | T_REQUIRE_ONCE | T_LOGICAL_OR | T_LOGICAL_XOR | T_LOGICAL_AND
	| T_INSTANCEOF | T_NEW | T_CLONE | T_EXIT | T_IF | T_ELSEIF | T_ELSE | T_ENDIF | T_ECHO | T_DO | T_WHILE
	| T_ENDWHILE | T_FOR | T_ENDFOR | T_FOREACH | T_ENDFOREACH | T_DECLARE | T_ENDDECLARE | T_AS | T_TRY | T_CATCH
	| T_FINALLY | T_THROW | T_USE | T_INSTEADOF | T_GLOBAL | T_VAR | T_UNSET | T_ISSET | T_EMPTY | T_CONTINUE | T_GOTO
	| T_FUNCTION | T_CONST | T_RETURN | T_PRINT | T_YIELD | T_LIST | T_SWITCH | T_ENDSWITCH | T_CASE | T_DEFAULT
	| T_BREAK | T_ARRAY | T_CALLABLE | T_EXTENDS | T_IMPLEMENTS | T_NAMESPACE | T_TRAIT | T_INTERFACE | T_CLASS
;

semi_reserved:
		reserved_non_modifiers
	| T_STATIC | T_ABSTRACT | T_FINAL | T_PRIVATE | T_PROTECTED | T_PUBLIC
;

identifier:
		T_STRING
	| semi_reserved
;

namespace_name_parts:
		T_STRING	{ $<r.str>$ = $<r.str>1; }
	| namespace_name_parts '\\' T_STRING
;
namespace_name:
		namespace_name_parts
;

top_statement:
		statement { pl.log("statment"); }
	| function_declaration_statement	{ pl.log("function declaration statement");}
	| class_declaration_statement { pl.log("class declaration statement");}
	| T_HALT_COMPILER { $<r.node>$ = nullptr; }
	| T_CONST type constant_declaration_list ';' {
			pl.log("constant declaration list");
			//TODO:encapsulate
			Variable* walker = dynamic_cast<Variable*>( $<r.symbol>3 );
			while(walker != nullptr){ // TODO: document this
				walker->setVariableType($<r.str>2);
				walker->isConst = true;
				Variable* prevNode = walker;//used to clear @node
				walker = dynamic_cast<Variable*>(walker->node);
				prevNode->node = nullptr; // remove the pointer to chain, no need for it anymore.
			}
		}
	| T_CONST constant_declaration_list ';'
		{
			/* ERROR RULE: constant without type */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"expecting type,you can\'t declare constant without type","");

			pl.log("wrong constant declaration list");
			//TODO:encapsulate
			Variable* walker = dynamic_cast<Variable*>( $<r.symbol>2 );
			while(walker != nullptr){ // TODO: document this
				walker->setVariableType("Object");
				walker->isConst = true;
				Variable* prevNode = walker;//used to clear @node
				walker = dynamic_cast<Variable*>(walker->node);
				prevNode->node = nullptr; // remove the pointer to chain, no need for it anymore.
			}
		}
;

constant_declaration_list:
		constant_declaration_list ',' constant_declaration {
			//**chain symbols in the list and pass it:
			$<r.symbol>3->node = $<r.symbol>1;
			$<r.symbol>$ = $<r.symbol>3;
		}
	| constant_declaration {
			$<r.symbol>$ = $<r.symbol>1;
		}
;

constant_declaration:
	  T_STRING '=' static_scalar {
			pl.log("constant declaration", 0); pl.log($<r.str>1);
			$<r.symbol>$ = symbolsParser->insertSymbol(new Variable($<r.str>1,VARIABLE, true, $<r.line_no>1, $<r.col_no>1));
		}
	| T_STRING '=' {
			/* ERROR RULE: constant = without value */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpecting token, expecting value","");
			$<r.symbol>$ = symbolsParser->insertSymbol(new Variable($<r.str>1,VARIABLE, false, $<r.line_no>1, $<r.col_no>1));
		}
	| T_STRING 	{
			/* ERROR RULE: constant = without value */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpecting token, expecting value","");
			$<r.symbol>$ = symbolsParser->insertSymbol(new Variable($<r.str>1,VARIABLE, true, $<r.line_no>1, $<r.col_no>1));
		}
;

class_const_list:
		class_const_list ',' class_const {
			//**chain symbols in the list and pass it:
			$<r.symbol>3->node = $<r.symbol>1;
			$<r.symbol>$ = $<r.symbol>3;
		}
	| class_const {$<r.symbol>$ = $<r.symbol>1;}
;

class_const:
	  identifier '=' static_scalar {
			pl.log("class constant", 0); pl.log($<r.str>1);
			$<r.symbol>$ = symbolsParser->insertSymbol(new DataMember($<r.str>1, true, $<r.col_no>1, $<r.line_no>1));
		}
	| identifier '=' {
			/* ERROR RULE: constant = without value */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"unexpected \';\', expecting value","");
			$<r.symbol>$ = symbolsParser->insertSymbol(new DataMember($<r.str>1, false, $<r.col_no>1, $<r.line_no>1));
		}
	| identifier 	{
			/* ERROR RULE: constant without value */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"unexpected \';\', expecting \'=\'","");
			$<r.symbol>$ = symbolsParser->insertSymbol(new DataMember($<r.str>1, false, $<r.col_no>1, $<r.line_no>1));
		}
;

inner_statement_list:
		inner_statement_list inner_statement {
			auto list = dynamic_cast<ListNode*>($<r.node>1);
			list->add_node($<r.node>2);
			$<r.node>$ = list;
		}
	| /* empty */ { $<r.node>$ = new ListNode(); }
;

inner_statement:
		statement	{pl.log("statement - inner");}
	| function_declaration_statement {pl.log("function declaration statement - inner");}
	| class_declaration_statement	{pl.log("class declaration statement - inner");}
	| T_HALT_COMPILER
;

variable_declaration_list:
		variable_declaration_list ',' variable_declaration
		{
			//**chain symbols in the list and pass it:
			$<r.symbol>3->node = $<r.symbol>1;
			$<r.symbol>$ = $<r.symbol>3;
			dynamic_cast<ListNode*>($<r.node>1)->add_node($<r.node>3);
			$<r.node>$ = $<r.node>1;
		}
	| variable_declaration {
			$<r.symbol>$ = $<r.symbol>1;
			$<r.node>$ = new ListNode();
			dynamic_cast<ListNode*>($<r.node>$)->add_node($<r.node>1); }
;

variable_declaration:
	  T_VARIABLE '=' expr
		{
			pl.log("initialized variable declaration.", 0); pl.log($<r.str>1);
			$<r.symbol>$ = symbolsParser->insertSymbol(new Variable($<r.str>1,VARIABLE, true, $<r.col_no>1, $<r.line_no>1));
			$<r.node>$ = (new ListNode())->add_node(new DeclarationNode($<r.symbol>$))
																	 ->add_node(new AssignmentNode(new VariableNode($<r.symbol>$), $<r.node>3));
		}
	| T_VARIABLE
		{
			pl.log("uninitialized variable declaration.");
			$<r.symbol>$ = symbolsParser->insertSymbol(new Variable($<r.str>1,VARIABLE, false, $<r.col_no>1, $<r.line_no>1));
			$<r.node>$ = (new ListNode())->add_node(new DeclarationNode($<r.symbol>$));
		}
	| T_VARIABLE '='
		{
			/* ERROR RULE: variable without value */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"unexpected token, expecting value","");
			//continue and assume variable is not initialized:
			$<r.symbol>$ = symbolsParser->insertSymbol(new Variable($<r.str>1,VARIABLE, false, $<r.col_no>1, $<r.line_no>1));
		}
;

statement:
		open_par inner_statement_list close_par {pl.log("code block"); $<r.node>$ = $<r.node>2; }
	|   type variable_declaration_list ';'
		{
			pl.log("variable declaration list.");
			//TODO:encapsulate
			reverse_list(&$<r.symbol>2);
			Variable* walker = dynamic_cast<Variable*>( $<r.symbol>2 );
			while(walker != nullptr){ // TODO: document this
				walker->setVariableType($<r.str>1);
				Variable* prevNode = walker;//used to clear @node
				walker = dynamic_cast<Variable*>(walker->node);
				prevNode->node = nullptr; // remove the pointer to chain, no need for it anymore.
			}
			auto nodes_list = new ListNode();
			for (auto &node :dynamic_cast<ListNode*>($<r.node>2)->nodes) {
				nodes_list->add_nodes(dynamic_cast<ListNode*>(node)->nodes);
			}
			$<r.node>$ = nodes_list;
		}
	| T_IF parentheses_expr statement elseif_list else_single {
		pl.log("if statement");
		Node *else_node = nullptr;
		if ($<r.node>4) {
			IfNode *deepest_if = IfNode::find_deepest_if($<r.node>4);
			deepest_if->else_node = dynamic_cast<ElseNode*>($<r.node>5);
			else_node = new ElseNode($<r.node>4);
		} else {
			else_node = $<r.node>5;
		}
		$<r.node>$ = new IfNode($<r.node>2, $<r.node>3, else_node); }
	| T_IF parentheses_expr ':' inner_statement_list new_elseif_list new_else_single T_ENDIF ';' {pl.log("if stmt new");}
	| T_WHILE parentheses_expr while_statement {
		pl.log("while stmt");
		$<r.node>$ = new WhileNode($<r.node>2, $<r.node>3); }
	| do_while_loop {pl.log("do while stmt");}
	| for_loop {pl.log("for loop stmt");}
	| switch_start parentheses_expr switch_case_list {pl.log("switch stmt");}
	| T_BREAK ';' {pl.log("break stmt");}
	| T_BREAK expr ';' {pl.log("break exp stmt");}
	| T_CONTINUE ';'{pl.log("contintue stmt");}
	| T_CONTINUE expr ';' {pl.log("contintue expr stmt");}
	| T_RETURN ';' {pl.log("return stmt"); $<r.node>$ = new ReturnNode(nullptr);}
	| T_RETURN expr ';' { pl.log("return expr stmt"); $<r.node>$ = new ReturnNode($<r.node>2); }
	| yield_expr ';' {pl.log("yield stmt");}
	| global_variable_statement {pl.log("global variable stmt");}
	| static_variable_statement {pl.log("static variable stmt");}
	| T_ECHO expr_list ';' { pl.log("echo stmt"); $<r.node>$ = new EchoNode($<r.node>2); }
	| T_INLINE_HTML
	| expr ';' {pl.log("expr stmt");}
	| T_UNSET '(' variables_list ')' ';'
	| T_FOREACH '(' expr T_AS foreach_variable ')' foreach_statement {pl.log("foreach stmt");}
	| T_FOREACH '(' expr T_AS foreach_key T_DOUBLE_ARROW foreach_variable ')' foreach_statement {pl.log("foreach stmt associative");}
	| T_DECLARE '(' declare_list ')' declare_statement {pl.log("declare stmt");}
	| ';' {pl.log("; stmt");}
	| T_TRY open_par inner_statement_list close_par catches optional_finally {pl.log("try stmt");}
	| T_THROW expr ';' {pl.log("throw stmt");}
	| T_GOTO T_STRING ';' {pl.log("goto stmt");}
	| T_STRING ':' {pl.log("label stmt");}
	| error {pl.log("ERROR");}
;

catches:
		/* empty */
	| catches catch
;

catch:
	T_CATCH '(' name T_VARIABLE ')' open_par inner_statement_list close_par
;

optional_finally:
		/* empty */
	| T_FINALLY open_par inner_statement_list close_par
;

variables_list:
		variable
	| variables_list ',' variable
;

optional_ref:
		/* empty */
	| '&'
;

optional_ellipsis:
		/* empty */
	| T_ELLIPSIS
;

function_declaration_statement:
		function_header inner_statement_list close_par {
			pl.log("function:", 0); pl.log($<r.str>3);
			$<r.node>$ = new FunctionDefineNode($<r.symbol>1, $<r.node>2, $<r.node>1); }
;

function_header:
		T_FUNCTION optional_ref T_STRING '(' parameter_list ')' ':' type open_par
		{
			pl.log("function header:", 0); pl.log($<r.str>3);
			$<r.symbol>$ = symbolsParser->insertFunctionSymbol($<r.str>3, $<r.str>8, $<r.col_no>1, $<r.line_no>1, $<r.scope>9, $<r.symbol>5);
			
			$<r.node>$ = $<r.node>5; // pass parameter nodes
		}
	| T_FUNCTION optional_ref T_STRING '(' parameter_list ')' ':'	open_par
		{
			pl.log("function header:", 0); pl.log($<r.str>3);
			/* ERROR RULE: function without returned type */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"expecting return type, functions must have return type","");
			symbolsParser->insertFunctionSymbol($<r.str>3, "void", $<r.col_no>1, $<r.line_no>1, $<r.scope>8, $<r.symbol>5);// assume return type is void
			
			$<r.node>$ = $<r.node>5; // pass parameter nodes
		}
	| T_STATIC T_FUNCTION optional_ref T_STRING '(' parameter_list ')' ':' type open_par
		{
			pl.log("function header:", 0); pl.log($<r.str>3);
			/* ERROR RULE: Global function with modifiers(public,private,protected,static,final,abstract) */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Global funtion dosn't accept modifier","");
			symbolsParser->insertFunctionSymbol($<r.str>4, $<r.str>9, $<r.col_no>1, $<r.line_no>1, $<r.scope>10, $<r.symbol>6);

			$<r.node>$ = $<r.node>6; // pass parameter nodes
	}
	|  member_modifier_without_static T_FUNCTION optional_ref T_STRING '(' parameter_list ')' ':' type open_par
		{
			pl.log("function header:", 0); pl.log($<r.str>3);
			/* ERROR RULE: Global function with modifiers(public,private,protected,static,final,abstract) */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Global funtion dosn't accept modifier","");
			symbolsParser->insertFunctionSymbol($<r.str>4, $<r.str>9, $<r.col_no>1, $<r.line_no>1, $<r.scope>10, $<r.symbol>6);
		
			$<r.node>$ = $<r.node>6; // pass parameter nodes
		}
;

class_declaration_statement:
	class_entry extends_from implements_list open_par class_statement_list close_par
		{
			pl.log("class_declaration_statement");
			symbolsParser->finishClassInsertion($<r.str>2, dynamic_cast<Class*>($<r.symbol>1), $<r.scope>4);
			$<r.symbol>$ = symbolsParser->getCurrentClassSym();
			$<r.node>$ = new ClassDefineNode($<r.symbol>$, $<r.node>5);
			symbolsParser->popFromClassesStack();
		}
	| T_INTERFACE T_STRING interface_extends_list open_par class_statement_list close_par {pl.log("interface:", 0); pl.log($<r.str>2);}
	| class_entry_without_name extends_from implements_list open_par class_statement_list close_par
		{
			/* ERROR RULE: class without name */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpected \'{\', expecting identifier (T_STRING)","");
			//symbolsParser->finishClassInsertion("ERR_C_NO_NAME", $<r.str>2, dynamic_cast<Class*>($<r.symbol>1), $<r.scope>4);
			$<r.symbol>$ = symbolsParser->getCurrentClassSym();
			$<r.node>$ = new ClassDefineNode($<r.symbol>$, $<r.node>5);
			symbolsParser->popFromClassesStack();
		}
;

class_entry:
	  T_CLASS T_STRING
		{
			//starting class declaration
			pl.log("class_entry") ;
			// TODO: encapsulate
			Class* classSym = new Class($<r.str>2, $<r.col_no>1, $<r.line_no>1, false,false);
			$<r.symbol>$ = symbolsParser->insertSymbol(classSym);
			//symbolsParser->setCurrentClassSym(classSym);
			symbolsParser->pushToClassesStack(classSym);
		}
	| T_ABSTRACT T_CLASS T_STRING
		{
			pl.log("class_entry") ;
			Class* classSym = new Class($<r.str>3, $<r.col_no>1, $<r.line_no>1, false,true );
			$<r.symbol>$ = symbolsParser->insertSymbol(classSym);
//			symbolsParser->setCurrentClassSym(classSym);
			symbolsParser->pushToClassesStack(classSym);
		}
	| T_FINAL T_CLASS T_STRING
		{
			pl.log("class_entry") ;
			Class* classSym = new Class($<r.str>3, $<r.col_no>1, $<r.line_no>1, true,false);
			$<r.symbol>$ = symbolsParser->insertSymbol(classSym);
			//symbolsParser->setCurrentClassSym(classSym);
			symbolsParser->pushToClassesStack(classSym);
		}
	| T_ABSTRACT T_FINAL T_CLASS T_STRING
		{
			pl.log("class_entry") ;
			/* ERROR RULE: abstract final class*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Abstract final class not allowed","");
			Class* classSym = new Class($<r.str>4, $<r.col_no>1, $<r.line_no>1, false,false);
			$<r.symbol>$ = symbolsParser->insertSymbol(classSym); // assuming not final and not abstract
			symbolsParser->pushToClassesStack(classSym);
		}
	| T_FINAL T_ABSTRACT T_CLASS T_STRING
		{
			pl.log("class_entry") ;
			/* ERROR RULE: final abctract class*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Final abstract class not allowed","");
			Class* classSym = new Class($<r.str>4, $<r.col_no>1, $<r.line_no>1, false,false);
			$<r.symbol>$ = symbolsParser->insertSymbol(classSym); // assuming not final and not abstract
			symbolsParser->pushToClassesStack(classSym);
		}
;


class_entry_without_name:
	 T_CLASS
		{
			//starting class declaration
			pl.log("class_entry_without_name") ;
			// TODO: encapsulate
			Class* classSym = new Class("ERR_CLASS",$<r.col_no>1, $<r.line_no>1, false,false);
			$<r.symbol>$ = symbolsParser->insertSymbol(classSym);
			//symbolsParser->setCurrentClassSym(classSym);
			symbolsParser->pushToClassesStack(classSym);
		}
	| T_ABSTRACT T_CLASS
		{
			pl.log("class_entry_without_name") ;
			Class* classSym = new Class("ERR_CLASS", $<r.col_no>1, $<r.line_no>1, false,true );
			$<r.symbol>$ = symbolsParser->insertSymbol(classSym);
			//symbolsParser->setCurrentClassSym(classSym);
			symbolsParser->pushToClassesStack(classSym);
		}
	| T_FINAL T_CLASS
		{
			pl.log("class_entry_without_name") ;
			Class* classSym = new Class("ERR_CLASS", $<r.col_no>1, $<r.line_no>1, true,false);
			$<r.symbol>$ = symbolsParser->insertSymbol(classSym);
			//symbolsParser->setCurrentClassSym(classSym);
			symbolsParser->pushToClassesStack(classSym);
		}
	| T_ABSTRACT T_FINAL T_CLASS
		{
			pl.log("class_entry_without_name") ;
			/* ERROR RULE: abstract final class*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Abstract final class not allowed","");
			Class* classSym = new Class("ERR_CLASS", $<r.col_no>1, $<r.line_no>1, false,false);
			$<r.symbol>$ = symbolsParser->insertSymbol(classSym); // assuming not final and not abstract
			//symbolsParser->setCurrentClassSym(classSym);
			symbolsParser->pushToClassesStack(classSym);
		}
	| T_FINAL T_ABSTRACT	T_CLASS
		{
			pl.log("class_entry_without_name") ;
			/* ERROR RULE: final abctract class*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Final abstract class not allowed","");
			Class* classSym = new Class("ERR_CLASS", $<r.col_no>1, $<r.line_no>1, false,false);
			$<r.symbol>$ = symbolsParser->insertSymbol(classSym); // assuming not final and not abstract
			//symbolsParser->setCurrentClassSym(classSym);
			symbolsParser->pushToClassesStack(classSym);
		}
;

extends_from:
		/* empty */ { $<r.str>$ = nullptr;}
	| T_EXTENDS name { $<r.str>$ = $<r.str>2;}
	| T_EXTENDS
		{
			/* ERROR RULE: Extent empty*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpecting token,expecting class name","");
			$<r.str>$ = nullptr;
		}
;

class_statement_list:
	class_statement_list class_statement 
	{	
		pl.log("class stmt");
		dynamic_cast<ListNode*>($<r.node>1) -> add_node($<r.node>2);	
	}
	| error
	| /* empty */ 
	{
		$<r.node>$ = new ListNode();
	}
;

class_statement:
	  member_modifiers type property_declaration_list ';' 
	    {
			pl.log("property declaration list");
			$<r.symbol>$ = symbolsParser->finishDataMembersDeclaration(dynamic_cast<DataMember*>($<r.symbol>3), modifiersTags, arrCounter, $<r.str>2);
			arrCounter = 0;
			$<r.node>$ = new ClassMemNode($<r.symbol>$);
		}
	| member_modifiers T_CONST type class_const_list ';' 
	    {
			pl.log("constant list");
			$<r.symbol>$ = symbolsParser->finishDataMembersDeclaration(dynamic_cast<DataMember*>($<r.symbol>4), modifiersTags, arrCounter, $<r.str>3);
			arrCounter = 0;
			$<r.node>$ = new ClassMemNode($<r.symbol>$);
		}
	| method_header	method_body	close_par {	/* optional return type in case of constructor */
			pl.log("method");
			$<r.symbol>$ = $<r.symbol>1;
			$<r.node>$ = new ClassMethodNode($<r.symbol>$);
		}
	| method_header_abstract ';' 
	    {
			$<r.symbol>$ = $<r.symbol>1;
			$<r.node>$ = new ClassMethodNode($<r.symbol>$);	
		}
	| member_modifiers property_declaration_list ';' 
	    {
			/* ERROR RULE: variable without type */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpected type, class member must have type","");
			$<r.symbol>$ = symbolsParser->finishDataMembersDeclaration(dynamic_cast<DataMember*>($<r.symbol>2), modifiersTags, arrCounter, "Object"); // assume type OBJECT
			arrCounter = 0;
			$<r.node>$ = new ClassMethodNode($<r.symbol>$);
		}
	|	method_header_without_name method_body close_par 
	    {
			/* ERROR RULE: method without name */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpected \'(\', expecting identifier (T_STRING)","");
			pl.log("error method-no id");
			$<r.symbol>$ = $<r.symbol>1;
			$<r.node>$ = new ClassMethodNode($<r.symbol>$);
		}
	| method_header_without_name_abstract ';' 
	    {
			/* ERROR RULE: method without name */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpected \'(\', expecting identifier (T_STRING)","");
			$<r.symbol>$ = $<r.symbol>1;
			pl.log("error method-no id");
			$<r.node>$ = new ClassMethodNode($<r.symbol>$);	
		}
	| class_declaration_statement 
	    {
			pl.log("inner class decalaration");
			dynamic_cast<Class*>($<r.symbol>1)->setOuterClass(symbolsParser->getCurrentClassSym());
			$<r.node>$ = $<r.node>1; // TODO: needs thinking, this is just a work around for now
		}
;


method_header:
	member_modifiers T_FUNCTION optional_ref identifier '(' parameter_list ')' optional_return_type open_par
	{
		//we insert the method symbol
		$<r.symbol>$ = symbolsParser->insertMethodSymbol($<r.str>4,$<r.col_no>1,$<r.line_no>1,modifiersTags, arrCounter, $<r.str>8, $<r.scope>9, $<r.symbol>6);
		arrCounter = 0;
	}
;

method_header_abstract:
	T_ABSTRACT member_modifiers T_FUNCTION optional_ref identifier '(' parameter_list ')' optional_return_type
	{
		$<r.symbol>$ = symbolsParser->insertMethodSymbol($<r.str>5, $<r.col_no>1,$<r.line_no>1, modifiersTags, arrCounter, $<r.str>9, nullptr, $<r.symbol>7);	//abstract
		symbolsParser->getCurrentClassSym()->isAbstract = true;
		arrCounter = 0;
	}
;

method_header_without_name :
	member_modifiers T_FUNCTION optional_ref '(' parameter_list ')' optional_return_type open_par
	{
		//we insert the method symbol with a PREDEFIND NAME , TODO : use a mechanism for unique naming of method.
		$<r.symbol>$ = symbolsParser->insertMethodSymbol("ERR_METHOD", $<r.col_no>1,$<r.line_no>1, modifiersTags, arrCounter, $<r.str>7, $<r.scope>8, $<r.symbol>5);
		arrCounter = 0;
	}
;

method_header_without_name_abstract :
	T_ABSTRACT member_modifiers T_FUNCTION optional_ref '(' parameter_list ')' optional_return_type
	{
		//we insert the method symbol with a PREDEFIND NAME , TODO : use a mechanism for unique naming of method.
		$<r.symbol>$ = symbolsParser->insertMethodSymbol("ERR_METHOD", $<r.col_no>1,$<r.line_no>1, modifiersTags, arrCounter, $<r.str>8, nullptr, $<r.symbol>6);	//abstract
		symbolsParser->getCurrentClassSym()->isAbstract = true;
		arrCounter = 0;
	}
;
method_body:
		inner_statement_list {pl.log("method body");}
;

member_modifiers : /* empty */
	|  member_modifier_list
;

member_modifier_list : member_modifier
	| member_modifier_list member_modifier
;

member_modifier:
	  T_PUBLIC {
			$<r.m>$ = PUBLIC_ACCESS;
			if(arrCounter < 25)
				modifiersTags[arrCounter++] = PUBLIC_ACCESS;
		}
	| T_PROTECTED {
			$<r.m>$ = PROTECTED_ACCESS;
			if(arrCounter < 25)
				modifiersTags[arrCounter++] = PROTECTED_ACCESS;
		}
	| T_PRIVATE {
			$<r.m>$ = PRIVATE_ACCESS;
			if(arrCounter < 25)
				modifiersTags[arrCounter++] = PRIVATE_ACCESS;
		}
	| T_STATIC {
			$<r.m>$ = STATIC_STORAGE;
			if(arrCounter < 25)
				modifiersTags[arrCounter++] = STATIC_STORAGE;
		}
	| T_FINAL {
			$<r.m>$ = FINAL_STORAGE;
			if(arrCounter < 25)
				modifiersTags[arrCounter++] = FINAL_STORAGE;
		}
;

member_modifier_without_static: T_PUBLIC
	| T_PROTECTED
	| T_PRIVATE
	| T_ABSTRACT
	| T_FINAL
;



interface_extends_list:
		/* empty */
	| T_EXTENDS name_list
	| T_EXTENDS
		{
			/* ERROR RULE: Extent empty*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpecting token,expecting interface name","");
		}
;

implements_list:
		/* empty */
	| T_IMPLEMENTS name_list
;

name_list:
		name
	| name_list ',' name
;

for_loop:
		for_loop_start '(' for_expr ';'	for_expr ';' for_expr ')' for_statement
		{
			if(forLoopFlag){ // we are in a single for statement and the scope hasn't been closed by an '}'
				forLoopFlag = false;
				symbolsParser->goUp();
			}
			$<r.node>$ = new ForNode($<r.node>3, $<r.node>5, $<r.node>7, $<r.node>9);
		}
	| for_loop_start '(' for_expr	')' for_statement
		{
			/* ERROR RULE: for loop with colon instead of semi colon */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"For loop syntax error, expected a ;","");
			if(forLoopFlag){ // we are in a single for statement and the scope hasn't been closed by an '}'
				forLoopFlag = false;
				symbolsParser->goUp();
			}
		}
	| for_loop_start '(' for_expr ';' for_expr	')' for_statement
		{
			/* ERROR RULE: for loop with colon instead of semi colon	*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"For loop syntax error, expected a ;","");
			if(forLoopFlag){ // we are in a single for statement and the scope hasn't been closed by an '}'
				forLoopFlag = false;
				symbolsParser->goUp();
			}
		}
;

for_loop_start:
	T_FOR { forLoopFlag = true; symbolsParser->createNewScope();}
;

for_statement:
		statement
	| ':' inner_statement_list T_ENDFOR ';'
;

foreach_statement:
		statement
	| ':' inner_statement_list T_ENDFOREACH ';'
;

declare_statement:
		statement
	| ':' inner_statement_list T_ENDDECLARE ';'
;

declare_list:
		declare_list_element
	| declare_list ',' declare_list_element
;

declare_list_element:
		type T_STRING '=' static_scalar
	| type T_STRING '='
		{
			/* ERROR RULE: constant without value */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Constant can't be without value","");
		}
	| T_STRING '=' static_scalar
		{
			/* ERROR RULE: constant without type */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Constant can't be without type","");
		}
;

switch_start :
	T_SWITCH {}
;

switch_case_list:
		open_par case_list close_par
	| open_par ';' case_list close_par
	| ':' case_list T_ENDSWITCH ';'
	| ':' ';' case_list T_ENDSWITCH ';'
;

case_list:
		/* empty */
	| case_list case
;

case:
		T_CASE expr case_separator inner_statement_list {}
	| T_DEFAULT case_separator inner_statement_list
;

case_separator:
		':'
	| ';'
;

do_while_loop:
	to_do_loop_start statement T_WHILE parentheses_expr ';' {
	}
	| to_do_loop_start statement parentheses_expr ';'		{
		/* ERROR RULE: do while loop without while keyword */
		errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"unexpected \'(\', expecting while (T_WHILE)","");
	}
	| to_do_loop_start statement T_WHILE parentheses_expr %prec low_prec
	{
		/* ERROR RULE: do while loop without ;	*/
		errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"unexpected token after while","");
	}
;

to_do_loop_start :
	T_DO {	}
;

while_statement:
		statement { $<r.node>$ = $<r.node>1; }
	| ':' inner_statement_list T_ENDWHILE ';' { $<r.node>$ = $<r.node>2; }
;

elseif_list:
		/* empty */ { $<r.node>$ = nullptr; }
	| elseif_list elseif {
		if ($<r.node>1 == nullptr) {
			// $<r.node>1 = $<r.node>2;
			$<r.node>$ = $<r.node>2;
		} else {
			IfNode *deepest_if = IfNode::find_deepest_if($<r.node>1);
			deepest_if->else_node = new ElseNode($<r.node>2);
			$<r.node>$ = $<r.node>1;
		}
	}
;

elseif:
		 T_ELSEIF parentheses_expr statement { $<r.node>$ = new IfNode($<r.node>2, $<r.node>3, nullptr); }
;

new_elseif_list:
		/* empty */
	| new_elseif_list new_elseif
;

new_elseif:
	 T_ELSEIF parentheses_expr ':' inner_statement_list
;

else_single:
		/* empty */ %prec low_prec { $<r.node>$ = nullptr; }
	| T_ELSE statement { $<r.node>$ = new ElseNode($<r.node>2);	}
;

new_else_single:
		/* empty */
	| T_ELSE ':' inner_statement_list
;

foreach_key:
		type T_VARIABLE
;

foreach_variable:
		type T_VARIABLE {pl.log("foreach var:", 0); pl.log($<r.str>2);}
	| type '&' T_VARIABLE {pl.log("foreach var:", 0); pl.log($<r.str>3);}
	| type list_expr
	| T_VARIABLE
		{
			/* ERROR RULE:	foreach variable without type */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"expecting type,you can\'t declare foreach variable without type","");
		}
	| '&'	T_VARIABLE
		{
			/* ERROR RULE:	foreach variable without type */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"expecting type,you can\'t declare foreach variable without type","");
		}
	| list_expr
		{
			/* ERROR RULE:	foreach variable without type */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"expecting type,you can\'t declare foreach variable without type","");
		}
;

parameter_list:
		non_empty_parameter_list 
		{
			pl.log("non empty parameters list");				
		}
	| non_empty_parameter_list ',' non_empty_default_parameter_list
		{
			pl.log("non empty mixed parameters list");
			//joining the two lists of params
			$<r.symbol>$ = SymbolsParser::joinSymbolsLists($<r.symbol>1,$<r.symbol>3);

			$<r.node>$ = ListNode::joinNodeLists($<r.node>1, $<r.node>3);
		}
	| non_empty_default_parameter_list 
	    {
			pl.log("non empty default parameters list");
		}
	| /* empty */
		{
			pl.log("empty parameters list");
			$<r.symbol>$ = nullptr;
			$<r.node>$ = new ListNode(); // an empty list of parameter nodes
		}
;

non_empty_parameter_list:
	  parameter {
			ListNode* list = new ListNode();
			list->add_node($<r.node>1);
			$<r.node>$ = list;
	  }
	| non_empty_parameter_list ',' parameter
		{
			//**chain symbols in the list and pass it:
			pl.log("non_empty_parameter_list with parameter");
			$<r.symbol>3->node = $<r.symbol>1;
			$<r.symbol>$ = $<r.symbol>3;

			ListNode* list = dynamic_cast<ListNode*>($<r.node>1);
			list->add_node($<r.node>3);
			$<r.node>$ = list;
		}
;

non_empty_default_parameter_list:
		default_parameter 
		{
			ListNode* list = new ListNode();
			list->add_node($<r.node>1);
			$<r.node>$ = list;
		}
	| non_empty_default_parameter_list ',' default_parameter
		{
			//**chain symbols in the list and pass it:
			$<r.symbol>3->node = $<r.symbol>1;
			$<r.symbol>$ = $<r.symbol>3;

			ListNode* list = dynamic_cast<ListNode*>($<r.node>1);
			list->add_node($<r.node>3);
			$<r.node>$ = list;
		}
	| non_empty_default_parameter_list ',' parameter
		{
			/* ERROR RULE */
			errorRec.errQ->enqueue($<r.line_no>3,$<r.col_no>3,"default parameters must appear only at the end","");
			//**chain symbols in the list and pass it:
			$<r.symbol>3->node = $<r.symbol>1;
			$<r.symbol>$ = $<r.symbol>3;

			ListNode* list = dynamic_cast<ListNode*>($<r.node>1);
			list->add_node($<r.node>3);
			$<r.node>$ = list;
		}
;

parameter:
		type optional_ref optional_ellipsis T_VARIABLE
		{
			pl.log("parameter:", 0); pl.log($<r.str>4);
			Parameter* paramSymbol = new Parameter($<r.str>4, $<r.col_no>1, $<r.line_no>1, false); // we assume variable is initialized
			paramSymbol->setVariableType($<r.str>1);// TODO: encapsulate variabelType within the constructor
			$<r.symbol>$ = paramSymbol;

			ParameterNode* parNode = new ParameterNode(paramSymbol,nullptr,false);
			$<r.node>$ = parNode;
		}
	| optional_ref optional_ellipsis T_VARIABLE
		{
			/* ERROR RULE */
			errorRec.errQ->enqueue($<r.line_no>3, $<r.col_no>3, "missing type in parameter", "");
			Parameter* paramSymbol = new Parameter($<r.str>3, $<r.col_no>1, $<r.line_no>1, false); // we assume variable is initialized
			paramSymbol->setVariableType("Object");// assume type is Object and continue // TODO: encapsulate variabelType within the constructor
			$<r.symbol>$ = paramSymbol;

			ParameterNode* parNode = new ParameterNode(paramSymbol,nullptr,false);
			$<r.node>$ = parNode;
		}
;

default_parameter:
		type optional_ref optional_ellipsis T_VARIABLE '=' static_scalar
		{
			pl.log("default parameter", 0); pl.log($<r.str>4);
			Parameter* paramSymbol = new Parameter($<r.str>4, $<r.col_no>1, $<r.line_no>1, true); // we assume variable is initialized
			paramSymbol->setVariableType($<r.str>1);// TODO: encapsulate variabelType within the constructor
			$<r.symbol>$ = paramSymbol;

			ParameterNode* parNode = new ParameterNode(paramSymbol,$<r.node>6,true);
			$<r.node>$ = parNode;
		}
	| optional_ref optional_ellipsis T_VARIABLE '=' static_scalar
		{
			/* ERROR RULE */
			errorRec.errQ->enqueue($<r.line_no>3, $<r.col_no>3, "missing type in parameter", "");
			Parameter* paramSymbol = new Parameter($<r.str>3, $<r.col_no>1, $<r.line_no>1, true); // we assume variable is initialized
			paramSymbol->setVariableType("Object"); // assume type is Object and continue// TODO: encapsulate variabelType within the constructor
			$<r.symbol>$ = paramSymbol;

			ParameterNode* parNode = new ParameterNode(paramSymbol,$<r.node>5,true);
			$<r.node>$ = parNode;
		}
	| type optional_ref optional_ellipsis T_VARIABLE '='
		{
			/* ERROR RULE */
			errorRec.errQ->enqueue($<r.line_no>5, $<r.col_no>5, "missing value in parameter", "");
			Parameter* paramSymbol = new Parameter($<r.str>4, $<r.col_no>1, $<r.line_no>1, true); // we assume variable is initialized
			paramSymbol->setVariableType($<r.str>1); // assume type is Object and continue// TODO: encapsulate variabelType within the constructor
			$<r.symbol>$ = paramSymbol;

			ParameterNode* parNode = new ParameterNode(paramSymbol,nullptr,true);
			$<r.node>$ = parNode;
		}
;

array_type:
		type T_SQUARE_BRACKETS {pl.log("array type");}
	| T_SQUARE_BRACKETS
	{
			/* ERROR RULE: array without type */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"expecting type, array must have type","");
	}
;

type:
		T_PRIMITIVE
		{
			pl.log("primitive type");
			int type = $<r.token_type>1;
			switch(type){
				case T_PRIMITIVE_INTEGER : $<r.str>$ = "int"; break;
				case T_PRIMITIVE_FLOAT : $<r.str>$ = "float"; break;
				case T_PRIMITIVE_DOUBLE: $<r.str>$ = "double"; break;
				case T_PRIMITIVE_CHAR	: $<r.str>$ = "char"; break;
				case T_PRIMITIVE_BOOLEAN : $<r.str>$ = "bool"; break;
				case T_PRIMITIVE_STRING : $<r.str>$ = "string"; break;
				case T_PRIMITIVE_VOID	 : $<r.str>$ = "void"; break;
			}
		}
	| name
		{
			pl.log("user defined type");
			$<r.str>$ = $<r.str>1; // pass it
		}
	| array_type
	| T_ARRAY
	| T_CALLABLE
;

optional_return_type:
		/* empty */ {$<r.str>$ = nullptr;}
	| ':' type {$<r.str>$ = $<r.str>2;}
;

argument_list:
		'(' ')' 
		{
			$<r.node>$ = new ListNode(); // an empty list of argument nodes
		}
	| '(' non_empty_argument_list ')'
		{
			$<r.node>$ = $<r.node>2;
		}
	| '(' yield_expr ')'
;

non_empty_argument_list:
		argument 
		{
			ListNode* argsList = new ListNode();
			argsList->add_node($<r.node>1);
			$<r.node>$ = argsList;
		}
	| non_empty_argument_list ',' argument 
		{
			dynamic_cast<ListNode*>($<r.node>1)
						           ->add_node($<r.node>3);
			$<r.node>$ = $<r.node>1;
		}
;

argument:
		expr {pl.log("expr argument");}
	| '&' variable {pl.log("reference variable argument");}
	| T_ELLIPSIS expr
;

global_variable_statement:
	T_GLOBAL type global_var_list ';'
	| T_GLOBAL global_var_list ';'
		{
			/* ERROR RULE: constant without type */
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"expecting type,you can\'t declare globel variable without type","");
		}
;

global_var_list:
		global_var_list ',' global_var
	| global_var
;

global_var:
		T_VARIABLE {pl.log("global variable", 0); pl.log($<r.str>1);}
	| '$' variable
	| '$' open_par expr close_par
;

static_variable_statement:
	T_STATIC type static_var_list ';'{
			pl.log("static variable declaration list.");
			//TODO:encapsulate
			Variable* walker = dynamic_cast<Variable*>( $<r.symbol>3 );
			while(walker != nullptr){ // TODO: document this
				walker->setVariableType($<r.str>2);
				walker->isStatic = true;
				symbolsParser->insertSymbol(walker);
				Variable* prevNode = walker;//used to clear @node
				walker = dynamic_cast<Variable*>(walker->node);
				prevNode->node = nullptr; // remove the pointer to chain, no need for it anymore.
			}
	}
	| T_STATIC static_var_list ';'
	{
			/* ERROR RULE: static variable	without type*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpected token expecting type","");
			pl.log("static variable declaration list.");
			//TODO:encapsulate
			Variable* walker = dynamic_cast<Variable*>( $<r.symbol>2 );
			while(walker != nullptr){ // TODO: document this
				walker->setVariableType("Object");
				walker->isStatic = true;
				symbolsParser->insertSymbol(walker);
				Variable* prevNode = walker;//used to clear @node
				walker = dynamic_cast<Variable*>(walker->node);
				prevNode->node = nullptr; // remove the pointer to chain, no need for it anymore.
			}
	}
;

static_var_list:
		static_var_list ',' static_var {
			//chaining symbols:
			$<r.symbol>3->node = $<r.symbol>1;
			$<r.symbol>$ = $<r.symbol>3;
		}
	| static_var {$<r.symbol>$ = $<r.symbol>1;}
;

static_var:
		T_VARIABLE {
				pl.log("static variable", 0); pl.log($<r.str>1);
				$<r.symbol>$ = new Variable($<r.str>1,VARIABLE, false, $<r.line_no>1, $<r.col_no>1);
			}
	| T_VARIABLE '=' static_scalar {
				pl.log("static variable - assigned", 0); pl.log($<r.str>1);
				$<r.symbol>$ = new Variable($<r.str>1,VARIABLE, true, $<r.line_no>1, $<r.col_no>1);
			}
	| T_VARIABLE '='
		{
			/* ERROR RULE: static variable = without value*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpected token expecting value","");
			$<r.symbol>$ = symbolsParser->insertSymbol(new Variable($<r.str>1,VARIABLE, false, $<r.line_no>1, $<r.col_no>1));
		}
;


property_declaration_list:
		property_declaration {$<r.symbol>$ = $<r.symbol>1;}
	| property_declaration_list ',' property_declaration
		{
			//**chain symbols in the list and pass it:
			$<r.symbol>3->node = $<r.symbol>1;
			$<r.symbol>$ = $<r.symbol>3;
		}
;

property_declaration:
		T_VARIABLE
		{
			pl.log("property", 0); pl.log($<r.str>1);
			$<r.symbol>$ = symbolsParser->insertSymbol(new DataMember($<r.str>1, false, $<r.col_no>1, $<r.line_no>1));
		}
	| T_VARIABLE '=' static_scalar
		{
			pl.log("property assigned", 0); pl.log($<r.str>1);
			$<r.symbol>$ = symbolsParser->insertSymbol(new DataMember($<r.str>1, true, $<r.col_no>1, $<r.line_no>1));
		}
	| T_VARIABLE '='
		{
			/* ERROR RULE: variable = without value*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpected token expecting value","");
			$<r.symbol>$ = symbolsParser->insertSymbol(new DataMember($<r.str>1, false, $<r.col_no>1, $<r.line_no>1));//assume not initialized and continue
		}
;

expr_list:
		expr_list ',' expr { dynamic_cast<ListNode*>($<r.node>1)->add_node($<r.node>3); }
	| expr { $<r.node>$ = (new ListNode())->add_node($<r.node>1); }
;

for_expr_list:
		for_expr_list ',' expr_or_declaration { dynamic_cast<ListNode*>($<r.node>1)->add_node($<r.node>3); }
	| expr_or_declaration { $<r.node>$ = (new ListNode())->add_node($<r.node>1); }
;

expr_or_declaration:
		expr
	| type T_VARIABLE '=' expr
		{
			//symbolsStack->push(new Variable($<r.str>2,VARIABLE, true, $<r.line_no>1, $<r.col_no>1));
			Variable* var = new Variable($<r.str>2,VARIABLE, true, $<r.line_no>1, $<r.col_no>1);
			var->setVariableType($<r.str>1);
			symbolsParser->insertSymbol(var);
			
			$<r.symbol>$ = var;
			$<r.node>$ = (new ListNode)->add_node(new DeclarationNode(var))
																 ->add_node(new AssignmentNode(new VariableNode(var), $<r.node>4));
		}
	| type T_VARIABLE '='
		{
			/* ERROR RULE: variable = without value*/
			errorRec.errQ->enqueue($<r.line_no>1,$<r.col_no>1,"Unexpected token expecting value","");
			//symbolsStack->push(new Variable($<r.str>2,VARIABLE, false, $<r.line_no>1, $<r.col_no>1));
			Variable* var = new Variable($<r.str>2,VARIABLE, false, $<r.line_no>1, $<r.col_no>1);
			var->setVariableType($<r.str>1);
			symbolsParser->insertSymbol(var);
			
			$<r.symbol>$ = var;
		}
;

for_expr:
		/* empty */ {$<r.symbol>$ = nullptr;}
	| for_expr_list
;

expr:
		variable
		{
			pl.log($<r.str>1,0);
			typeChecker->checkVariable($<r.str>1,$<r.line_no>1,$<r.col_no>1);
			$<r.node>$ = $<r.node>1;
		}
	| '(' T_PRIMITIVE ')' expr
	| list_expr '=' expr
	| variable '=' expr { $<r.node>$ = new AssignmentNode($<r.node>1, $<r.node>3); }
	| variable '=' '&' variable
	| variable '=' '&' new_expr
	| new_expr		%prec _def_val_
	| T_CLONE expr
	| variable T_PLUS_EQUAL expr { $<r.node>$ = new AssignmentNode($<r.node>1, new BinaryOperationNode("+", $<r.node>1, $<r.node>3)); }
	| variable T_MINUS_EQUAL expr { $<r.node>$ = new AssignmentNode($<r.node>1, new BinaryOperationNode("-", $<r.node>1, $<r.node>3)); }
	| variable T_MUL_EQUAL expr { $<r.node>$ = new AssignmentNode($<r.node>1, new BinaryOperationNode("*", $<r.node>1, $<r.node>3)); }
	| variable T_DIV_EQUAL expr { $<r.node>$ = new AssignmentNode($<r.node>1, new BinaryOperationNode("/", $<r.node>1, $<r.node>3)); }
	| variable T_CONCAT_EQUAL expr { $<r.node>$ = new AssignmentNode($<r.node>1, new BinaryOperationNode(".", $<r.node>1, $<r.node>3)); }
	| variable T_MOD_EQUAL expr
	| variable T_AND_EQUAL expr
	| variable T_OR_EQUAL expr
	| variable T_XOR_EQUAL expr
	| variable T_SR_EQUAL expr
	| variable T_SL_EQUAL expr
	| variable T_POW_EQUAL expr
	| variable T_INC
	| T_INC variable
	| variable T_DEC
	| T_DEC variable
	| expr T_BOOLEAN_OR expr
	| expr T_BOOLEAN_AND expr
	| expr T_LOGICAL_OR expr
	| expr T_LOGICAL_AND expr
	| expr T_LOGICAL_XOR expr
	| expr '|' expr
	| expr '&' expr
	| expr '^' expr
	| expr '+' expr { $<r.node>$ = new BinaryOperationNode("+", $<r.node>1, $<r.node>3); }
	| expr '.' expr { $<r.node>$ = new BinaryOperationNode("+", $<r.node>1, $<r.node>3); }
	| expr '-' expr	{ $<r.node>$ = new BinaryOperationNode("-", $<r.node>1, $<r.node>3); }
	| expr '*' expr { $<r.node>$ = new BinaryOperationNode("*", $<r.node>1, $<r.node>3); }
	| expr '/' expr { $<r.node>$ = new BinaryOperationNode("/", $<r.node>1, $<r.node>3); }
	| expr '%' expr { $<r.node>$ = new BinaryOperationNode("%", $<r.node>1, $<r.node>3); }
	| expr T_SL expr
	| expr T_SR expr
	| expr T_POW expr
	| '+' expr %prec T_INC
	| '-' expr %prec T_INC
	| '!' expr
	| '~' expr
	| expr T_IS_IDENTICAL expr
	| expr T_IS_NOT_IDENTICAL expr
	| expr T_IS_EQUAL expr { $<r.node>$ = new BinaryOperationNode("==", $<r.node>1, $<r.node>3); }
	| expr T_IS_NOT_EQUAL expr
	| expr T_SPACESHIP expr
	| expr '<' expr { $<r.node>$ = new BinaryOperationNode("<", $<r.node>1, $<r.node>3); }
	| expr T_IS_SMALLER_OR_EQUAL expr { $<r.node>$ = new BinaryOperationNode("<=", $<r.node>1, $<r.node>3); }
	| expr '>' expr { $<r.node>$ = new BinaryOperationNode(">", $<r.node>1, $<r.node>3); }
	| expr T_IS_GREATER_OR_EQUAL expr { $<r.node>$ = new BinaryOperationNode(">=", $<r.node>1, $<r.node>3); }
	| expr T_INSTANCEOF class_name_reference
	| parentheses_expr
	/* we need a separate '(' new_expr ')' rule to avoid problems caused by a s/r conflict */
	| '(' new_expr ')'
	| expr '?' expr ':' expr
	| expr '?' ':' expr
	| expr T_COALESCE expr
	| T_ISSET '(' variables_list ')'
	| T_EMPTY '(' expr ')'
	| T_INCLUDE expr
	| T_INCLUDE_ONCE expr
	| T_EVAL parentheses_expr
	| T_REQUIRE expr
	| T_REQUIRE_ONCE expr
	| T_INT_CAST expr
	| T_DOUBLE_CAST expr
	| T_STRING_CAST expr
	| T_ARRAY_CAST expr
	| T_OBJECT_CAST expr
	| T_BOOL_CAST expr
	| T_UNSET_CAST expr
	| T_EXIT exit_expr
	| '@' expr
	| scalar
	| array_expr
	| scalar_dereference
	| '`' backticks_expr '`'
	| T_PRINT expr
	| T_YIELD
	| T_YIELD_FROM expr
	| T_FUNCTION optional_ref '(' parameter_list ')' lexical_vars ':' type		open_par inner_statement_list close_par
	| T_STATIC T_FUNCTION optional_ref '(' parameter_list ')' lexical_vars ':' type		open_par inner_statement_list close_par
;

parentheses_expr:
		'(' expr ')' { $<r.node>$ = $<r.node>2; }
	| '(' yield_expr ')'
;

yield_expr:
		T_YIELD expr
	| T_YIELD expr T_DOUBLE_ARROW expr
;

array_expr: /* typing? */
		T_ARRAY '(' array_pair_list ')'
	| '[' array_pair_list ']'
	| T_SQUARE_BRACKETS	/* because of T_SQUARE_BRACKETS */
;

scalar_dereference:
		array_expr '[' dim_offset ']'
	| T_CONSTANT_ENCAPSED_STRING '[' dim_offset ']'
	| constant '[' dim_offset ']'
	| scalar_dereference '[' dim_offset ']'
	/* alternative array syntax missing intentionally */
;

anonymous_class:
		T_CLASS ctor_arguments extends_from implements_list open_par class_statement_list close_par
;

new_expr:
		T_NEW class_name_reference ctor_arguments
	| T_NEW anonymous_class
;

lexical_vars:
		/* empty */
	| T_USE '(' lexical_var_list ')'
;

lexical_var_list:
		lexical_var
	| lexical_var_list ',' lexical_var
;

lexical_var:
		optional_ref T_VARIABLE
;

function_call:
		name argument_list 
		{
			pl.log("function call");
			$<r.node>$ = new FunctionCallNode($<r.str>1, $<r.node>2);	
		}
	| class_name_or_var T_PAAMAYIM_NEKUDOTAYIM identifier argument_list {pl.log(":: function call");}
	| class_name_or_var T_PAAMAYIM_NEKUDOTAYIM open_par expr close_par argument_list
	| static_property argument_list
	| variable_without_objects argument_list
	| function_call '[' dim_offset ']'
		/* alternative array syntax missing intentionally */
;

class_name:
		T_STATIC
	| name
;

name:
		namespace_name_parts { $<r.str>$ = $<r.str>1; }
	| '\\' namespace_name_parts
	| T_NAMESPACE '\\' namespace_name_parts
;

class_name_reference:
		class_name
	| dynamic_class_name_reference
;

dynamic_class_name_reference:
		object_access_for_dcnr
	| base_variable
;

class_name_or_var:
		class_name
	| reference_variable
;

object_access_for_dcnr:
		base_variable T_OBJECT_OPERATOR object_property
	| object_access_for_dcnr T_OBJECT_OPERATOR object_property
	| object_access_for_dcnr '[' dim_offset ']'
	| object_access_for_dcnr '{' expr '}'
;

exit_expr:
		/* empty */
	| '(' ')'
	| parentheses_expr
;

backticks_expr:
		/* empty */
	| T_ENCAPSED_AND_WHITESPACE
	| encaps_list
;

ctor_arguments:
		/* empty */
	| argument_list
;

common_scalar:
		T_LNUMBER { $<r.node>$ = new ScalarNode($<r.i>1); }
	| T_DNUMBER   { $<r.node>$ = new ScalarNode($<r.f>1);}
	| T_TRUE	  { $<r.node>$ = new ScalarNode(bool(true));}
	| T_FALSE	  { $<r.node>$ = new ScalarNode(bool(false));}
	| T_CONSTANT_ENCAPSED_STRING  { $<r.node>$ = new ScalarNode(std::string($<r.str>1));}
	| T_LINE
	| T_FILE
	| T_DIR
	| T_CLASS_C
	| T_METHOD_C
	| T_FUNC_C
	| T_NS_C
	| T_START_HEREDOC T_ENCAPSED_AND_WHITESPACE T_END_HEREDOC
	| T_START_HEREDOC T_END_HEREDOC
;

static_scalar:
		common_scalar
	| class_name T_PAAMAYIM_NEKUDOTAYIM identifier
	| name
	| T_ARRAY '(' static_array_pair_list ')'
	| '[' static_array_pair_list ']'
	| T_SQUARE_BRACKETS /* empty array */
	| static_operation
;

static_operation:
		static_scalar T_BOOLEAN_OR static_scalar
	| static_scalar T_BOOLEAN_AND static_scalar
	| static_scalar T_LOGICAL_OR static_scalar
	| static_scalar T_LOGICAL_AND static_scalar
	| static_scalar T_LOGICAL_XOR static_scalar
	| static_scalar '|' static_scalar
	| static_scalar '&' static_scalar
	| static_scalar '^' static_scalar
	| static_scalar '.' static_scalar
	| static_scalar '+' static_scalar
	| static_scalar '-' static_scalar
	| static_scalar '*' static_scalar
	| static_scalar '/' static_scalar
	| static_scalar '%' static_scalar
	| static_scalar T_SL static_scalar
	| static_scalar T_SR static_scalar
	| static_scalar T_POW static_scalar
	| '+' static_scalar %prec T_INC
	| '-' static_scalar %prec T_INC
	| '!' static_scalar
	| '~' static_scalar
	| static_scalar T_IS_IDENTICAL static_scalar
	| static_scalar T_IS_NOT_IDENTICAL static_scalar
	| static_scalar T_IS_EQUAL static_scalar
	| static_scalar T_IS_NOT_EQUAL static_scalar
	| static_scalar '<' static_scalar
	| static_scalar T_IS_SMALLER_OR_EQUAL static_scalar
	| static_scalar '>' static_scalar
	| static_scalar T_IS_GREATER_OR_EQUAL static_scalar
	| static_scalar '?' static_scalar ':' static_scalar
	| static_scalar '?' ':' static_scalar
	| static_scalar '[' static_scalar ']'
	| '(' static_scalar ')' {
		$<r.node>$ = $<r.node>2;
	}
;

constant:
		name
	| class_name_or_var T_PAAMAYIM_NEKUDOTAYIM identifier
;

scalar:
		common_scalar
	| constant
	| '"' encaps_list '"'
	| T_START_HEREDOC encaps_list T_END_HEREDOC
;

static_array_pair_list:
		/* empty */
	| non_empty_static_array_pair_list optional_comma
;

optional_comma:
		/* empty */
	| ','
;

non_empty_static_array_pair_list:
		non_empty_static_array_pair_list ',' static_array_pair
	| static_array_pair
;

static_array_pair:
		static_scalar T_DOUBLE_ARROW static_scalar
	| static_scalar
;

variable:
	  object_access
	| base_variable
	| function_call 
	| new_expr_array_deref
;

new_expr_array_deref:
		'(' new_expr ')' '[' dim_offset ']'
	| new_expr_array_deref '[' dim_offset ']'
		/* alternative array syntax missing intentionally */
;

object_access:
	  variable_or_new_expr T_OBJECT_OPERATOR object_property %prec low_prec 
	    {
			$<r.node>$ = new ClassCallNode($<r.node>1, $<r.str>3);
		}
	| variable_or_new_expr T_OBJECT_OPERATOR object_property argument_list
	| object_access argument_list
	| object_access '[' dim_offset ']'
	| object_access '{' expr '}'
;

variable_or_new_expr:
		variable
	| '(' new_expr ')'
;

variable_without_objects:
		reference_variable		%prec low_prec
	| '$' variable_without_objects
;

base_variable:
		variable_without_objects
	| static_property
;

static_property:
		class_name_or_var T_PAAMAYIM_NEKUDOTAYIM '$' reference_variable
	| static_property_with_arrays
;

static_property_with_arrays:
		class_name_or_var T_PAAMAYIM_NEKUDOTAYIM T_VARIABLE
	| class_name_or_var T_PAAMAYIM_NEKUDOTAYIM '$' open_par expr close_par
	| static_property_with_arrays '[' dim_offset ']'
	| static_property_with_arrays open_par expr close_par
;

reference_variable:
		reference_variable '[' dim_offset ']'
	| reference_variable open_par expr close_par
	| T_VARIABLE {
		$<r.symbol>$ = symbolsParser->lookUpSymbol(symbolsParser->getCurrentScope(), $<r.str>1, $<r.line_no>1, $<r.col_no>1);
		$<r.node>$ = new VariableNode($<r.symbol>$); }
	| '$' open_par expr close_par
;

dim_offset:
		/* empty */
	| expr
;

object_property:
		T_STRING
	| '{' expr '}'
	| variable_without_objects
;

list_expr:
		T_LIST '(' list_expr_elements ')'
;

list_expr_elements:
		list_expr_elements ',' list_expr_element
	| list_expr_element
;

list_expr_element:
		variable
	| list_expr
	| /* empty */
;

array_pair_list:
		/* empty */
	| non_empty_array_pair_list optional_comma
;

non_empty_array_pair_list:
		non_empty_array_pair_list ',' array_pair
	| array_pair
;

array_pair:
		expr T_DOUBLE_ARROW expr
	| expr
	| expr T_DOUBLE_ARROW '&' variable
	| '&' variable
;

encaps_list:
		encaps_list encaps_var
	| encaps_list T_ENCAPSED_AND_WHITESPACE
	| encaps_var
	| T_ENCAPSED_AND_WHITESPACE encaps_var
;

encaps_var:
		T_VARIABLE
	| T_VARIABLE '[' encaps_var_offset ']'
	| T_VARIABLE T_OBJECT_OPERATOR T_STRING
	| T_DOLLAR_OPEN_CURLY_BRACES expr close_par
	| T_DOLLAR_OPEN_CURLY_BRACES T_STRING_VARNAME close_par
	| T_DOLLAR_OPEN_CURLY_BRACES T_STRING_VARNAME '[' expr ']' close_par
	| T_CURLY_OPEN variable close_par
;

encaps_var_offset:
		T_STRING
	| T_NUM_STRING
	| T_VARIABLE
;

open_par:
	'{' {
		pl.log("open_par",0);
		//we create a new scope
		$<r.scope>$ = symbolsParser->createNewScope(forLoopFlag);
	}
;

close_par:
	'}' {
		symbolsParser->goUp();
	}
;
%%

void yyerror(const char* s)
{

	extern int line_no, col_no;
	errorRec.errQ->enqueue(line_no, col_no, s, "");
}
