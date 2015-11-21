%output ="yacc.cpp"
%{
	#include <iostream>
	using namespace std;
	#include <FlexLexer.h>
	int yylex(void);
	int yyparse();
	void yyerror(char *);
	
	FlexLexer* lexer = new yyFlexLexer();
	
	class Parser
	{
		public:
		int	parse()
		{
			return yyparse();
		}
	};
	
%}

%nonassoc _def_val_ test test2
%left T_INCLUDE T_INCLUDE_ONCE T_EVAL T_REQUIRE T_REQUIRE_ONCE
%left ','
%left T_LOGICAL_OR
%left T_LOGICAL_XOR
%left T_LOGICAL_AND
%right T_PRINT
%right T_YIELD
%right T_DOUBLE_ARROW
%right T_YIELD_FROM
%left '=' T_PLUS_EQUAL T_MINUS_EQUAL T_MUL_EQUAL T_DIV_EQUAL T_CONCAT_EQUAL T_MOD_EQUAL T_AND_EQUAL T_OR_EQUAL T_XOR_EQUAL T_SL_EQUAL T_SR_EQUAL T_POW_EQUAL
%left '?' ':'
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
%right '['
%nonassoc T_NEW T_CLONE
%token T_EXIT
%token T_IF
%left T_ELSEIF
%left T_ELSE
%left T_ENDIF
%token T_LNUMBER
%token T_DNUMBER
%token T_STRING
%token T_STRING_VARNAME
%token T_VARIABLE
%token T_NUM_STRING
%token T_INLINE_HTML
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
%token T_OPEN_TAG_WITH_ECHO
%token T_CLOSE_TAG
%token T_WHITESPACE
%token T_START_HEREDOC
%token T_END_HEREDOC
%token T_DOLLAR_OPEN_CURLY_BRACES
%token T_CURLY_OPEN
%token T_PAAMAYIM_NEKUDOTAYIM
%token T_NAMESPACE
%token T_NS_C
%token T_DIR
%token T_NS_SEPARATOR
%token T_ELLIPSIS


%nonassoc right_arc left_arc


%union{
	struct R{
		int i;
		float f;
		char c;
		char* str;
		int myLineNo;
		int myColno;
		}r;
	}





%%

start:
    top_statement_list                                      {  }
;

top_statement_list:
      top_statement_list top_statement                      { }
    | /* empty */                                           { }
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
      T_STRING                                              { }
    | semi_reserved                                         { }
;

namespace_name_parts:
      T_STRING                                              { }
    | namespace_name_parts T_NS_SEPARATOR T_STRING          { }
;

namespace_name:
      namespace_name_parts                                  { }
;

top_statement:
      statement                                             { }
    | function_declaration_statement                        { }
    | class_declaration_statement                           { }
    | T_HALT_COMPILER
          {}
    | T_NAMESPACE namespace_name ';'                        { }
    | T_NAMESPACE namespace_name '{' top_statement_list '}' { }
    | T_NAMESPACE '{' top_statement_list '}'                { }
    | T_USE use_declarations ';'                            { }
    | T_USE use_type use_declarations ';'                   
    | group_use_declaration ';'                             
    | T_CONST constant_declaration_list ';'                 
;

use_type:
      T_FUNCTION                                            
    | T_CONST                                               
;

/* Using namespace_name_parts here to avoid s/r conflict on T_NS_SEPARATOR */
group_use_declaration:
      T_USE use_type namespace_name_parts T_NS_SEPARATOR '{' unprefixed_use_declarations '}'
          
    | T_USE use_type T_NS_SEPARATOR namespace_name_parts T_NS_SEPARATOR '{' unprefixed_use_declarations '}'
          
    | T_USE namespace_name_parts T_NS_SEPARATOR '{' inline_use_declarations '}'
          
    | T_USE T_NS_SEPARATOR namespace_name_parts T_NS_SEPARATOR '{' inline_use_declarations '}'
          
;

unprefixed_use_declarations:
      unprefixed_use_declarations ',' unprefixed_use_declaration
          
    | unprefixed_use_declaration                            
;

use_declarations:
      use_declarations ',' use_declaration                  
    | use_declaration                                       
;

inline_use_declarations:
      inline_use_declarations ',' inline_use_declaration    
    | inline_use_declaration                                
;

unprefixed_use_declaration:
      namespace_name                                        
    | namespace_name T_AS T_STRING                          
;

use_declaration:
      unprefixed_use_declaration                            
    | T_NS_SEPARATOR unprefixed_use_declaration             
;

inline_use_declaration:
      unprefixed_use_declaration                            
    | use_type unprefixed_use_declaration                   
;

constant_declaration_list:
      constant_declaration_list ',' constant_declaration    
    | constant_declaration                                  
;

constant_declaration:
    T_STRING '=' static_scalar                              
;

class_const_list:
      class_const_list ',' class_const                      
    | class_const                                           
;

class_const:
    identifier '=' static_scalar                            
;

inner_statement_list:
      inner_statement_list inner_statement                  
    | /* empty */                                           
;

inner_statement:
      statement                                             
    | function_declaration_statement                        
    | class_declaration_statement                           
    | T_HALT_COMPILER
          
;

statement:
      '{' inner_statement_list '}'                          
    | T_IF parentheses_expr statement elseif_list else_single
          
    | T_IF parentheses_expr ':' inner_statement_list new_elseif_list new_else_single T_ENDIF ';'
          
    | T_WHILE parentheses_expr while_statement
    | T_DO statement T_WHILE parentheses_expr ';'
    | T_FOR left_arc for_expr ';'  for_expr ';' for_expr right_arc for_statement
          
    | T_SWITCH parentheses_expr switch_case_list 
    | T_BREAK ';'                                
    | T_BREAK expr ';'                            
    | T_CONTINUE ';'                              
    | T_CONTINUE expr ';'                         
    | T_RETURN ';'                                
    | T_RETURN expr ';'                           
    | yield_expr ';'                              
    | T_GLOBAL global_var_list ';'                
    | T_STATIC static_var_list ';'                
    | T_ECHO expr_list ';'                        
    | T_INLINE_HTML                               
    | expr ';'                                    
    | T_UNSET left_arc variables_list right_arc ';'          
    | T_FOREACH left_arc expr T_AS foreach_variable right_arc foreach_statement
          
    | T_FOREACH left_arc expr T_AS variable T_DOUBLE_ARROW foreach_variable right_arc foreach_statement
          
    | T_DECLARE left_arc declare_list right_arc declare_statement    
    | ';'                                                 
    | T_TRY '{' inner_statement_list '}' catches optional_finally
          
    | T_THROW expr ';'  
    | T_GOTO T_STRING ';'                                  
    | T_STRING ':'                                         
    | error                                                
;

catches:
      /* empty */                                          
    | catches catch                                        
;

catch:
    T_CATCH left_arc name T_VARIABLE right_arc '{' inner_statement_list '}'
        
;

optional_finally:
      /* empty */                                          
    | T_FINALLY '{' inner_statement_list '}'               
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
    T_FUNCTION optional_ref T_STRING left_arc parameter_list right_arc optional_return_type '{' inner_statement_list '}'
        
;

class_declaration_statement:
      class_entry_type T_STRING extends_from implements_list '{' class_statement_list '}'
        
    | T_INTERFACE T_STRING interface_extends_list '{' class_statement_list '}'
        
    | T_TRAIT T_STRING '{' class_statement_list '}'
        
;

class_entry_type:
      T_CLASS  
    | T_ABSTRACT T_CLASS
    | T_FINAL T_CLASS   
;

extends_from:
      /* empty */       
    | T_EXTENDS name    
;

interface_extends_list:
      /* empty */       
    | T_EXTENDS name_list
;

implements_list:
      /* empty */        
    | T_IMPLEMENTS name_list 
;

name_list:
      name                   
    | name_list ',' name     
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
      T_STRING '=' static_scalar                 
;

switch_case_list:
      '{' case_list '}'                          
    | '{' ';' case_list '}'                      
    | ':' case_list T_ENDSWITCH ';'              
    | ':' ';' case_list T_ENDSWITCH ';'          
;

case_list:
      /* empty */                                
    | case_list case                             
;

case:
      T_CASE expr case_separator inner_statement_list  
    | T_DEFAULT case_separator inner_statement_list    
;

case_separator:
      ':'
    | ';'
;

while_statement:
      statement 
    | ':' inner_statement_list T_ENDWHILE ';'              
;

elseif_list:
      /* empty */                                          
    | elseif_list elseif                                   
;

elseif:
      T_ELSEIF parentheses_expr statement                  
;

new_elseif_list:
      /* empty */                                          
    | new_elseif_list new_elseif                           
;

new_elseif:
     T_ELSEIF parentheses_expr ':' inner_statement_list    
;

else_single:
      /* empty */                                          
    | T_ELSE statement                                     
;

new_else_single:
      /* empty */                                          
    | T_ELSE ':' inner_statement_list                      
;

foreach_variable:
      variable                                             
    | '&' variable                                         
    | list_expr                                            
;

parameter_list:
      non_empty_parameter_list                            
    | /* empty */                                         
;

non_empty_parameter_list:
      parameter                                           
    | non_empty_parameter_list ',' parameter
;

parameter:
      optional_param_type optional_ref optional_ellipsis T_VARIABLE
          
    | optional_param_type optional_ref optional_ellipsis T_VARIABLE '=' static_scalar
          
;

type:
      name
    | T_ARRAY 
    | T_CALLABLE 
;

optional_param_type:
      /* empty */ 
    | type                                                 
;

optional_return_type:
      /* empty */                                          
    | ':' type                                             
;

argument_list:
      left_arc right_arc                                              
    | left_arc non_empty_argument_list right_arc                      
    | left_arc yield_expr right_arc                                   
;

non_empty_argument_list:
      argument                                             
    | non_empty_argument_list ',' argument                 
;

argument:
      expr                                                 
    | '&' variable                                         
    | T_ELLIPSIS expr                                      
;

global_var_list:
      global_var_list ',' global_var                       
    | global_var                                           
;

global_var:
      T_VARIABLE                                           
    | '$' variable                                         
    | '$' '{' expr '}'                                     
;

static_var_list:
      static_var_list ',' static_var                       
    | static_var                                           
;

static_var:
      T_VARIABLE         
    | T_VARIABLE '=' static_scalar   
;

class_statement_list:
      class_statement_list class_statement    
    | /* empty */                             
;

class_statement:
      variable_modifiers property_declaration_list ';'   
    | T_CONST class_const_list ';'                       
    | method_modifiers T_FUNCTION optional_ref identifier left_arc parameter_list right_arc optional_return_type method_body
          
    | T_USE name_list trait_adaptations   
;

trait_adaptations:
      ';'                                               
    | '{' trait_adaptation_list '}'                     
;

trait_adaptation_list:
      /* empty */                                       
    | trait_adaptation_list trait_adaptation            
;

trait_adaptation:
      trait_method_reference_fully_qualified T_INSTEADOF name_list ';'
          
    | trait_method_reference T_AS member_modifier identifier ';'
          
    | trait_method_reference T_AS member_modifier ';'
          
    | trait_method_reference T_AS T_STRING ';'
          
    | trait_method_reference T_AS reserved_non_modifiers ';'
          
;

trait_method_reference_fully_qualified:
      name T_PAAMAYIM_NEKUDOTAYIM identifier 
;
trait_method_reference:
      trait_method_reference_fully_qualified 
    | identifier                             
;

method_body:
      ';' /* abstract method */              
    | '{' inner_statement_list '}'           
;

variable_modifiers:
      non_empty_member_modifiers             
    | T_VAR                                  
;

method_modifiers:
      /* empty */                                     
    | non_empty_member_modifiers                      
;

non_empty_member_modifiers:
      member_modifier                                 
    | non_empty_member_modifiers member_modifier      
;

member_modifier:
      T_PUBLIC                                        
    | T_PROTECTED                                     
    | T_PRIVATE                                       
    | T_STATIC                                      
    | T_ABSTRACT                                    
    | T_FINAL                                       
;

property_declaration_list:
      property_declaration                          
    | property_declaration_list ',' property_declaration  
;

property_declaration:
      T_VARIABLE       
    | T_VARIABLE '=' static_scalar     
;

expr_list:
      expr_list ',' expr               
    | expr                             
;

for_expr:
      /* empty */                      
    | expr_list      
;

expr:
      variable       
    | list_expr '=' expr        
    | variable '=' expr         
    | variable '=' '&' variable 
    | variable '=' '&' new_expr 
    | new_expr      %prec _def_val_                                  
    | T_CLONE expr                                    
    | variable T_PLUS_EQUAL expr                      
    | variable T_MINUS_EQUAL expr                     
    | variable T_MUL_EQUAL expr                       
    | variable T_DIV_EQUAL expr                       
    | variable T_CONCAT_EQUAL expr                    
    | variable T_MOD_EQUAL expr                       
    | variable T_AND_EQUAL expr                       
    | variable T_OR_EQUAL expr                        
    | variable T_XOR_EQUAL expr                       
    | variable T_SR_EQUAL expr                              
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
    | expr '.' expr                                         
    | expr '+' expr                                         
    | expr '-' expr                                         
    | expr '*' expr                                         
    | expr '/' expr                                         
    | expr '%' expr                                         
    | expr T_SL expr                                        
    | expr T_SR expr                                        
    | expr T_POW expr                                       
    | '+' expr %prec T_INC                                  
    | '-' expr %prec T_INC                                  
    | '!' expr                                              
    | '~' expr                                              
    | expr T_IS_IDENTICAL expr                              
    | expr T_IS_NOT_IDENTICAL expr                          
    | expr T_IS_EQUAL expr                                  
    | expr T_IS_NOT_EQUAL expr                              
    | expr T_SPACESHIP expr                                 
    | expr '<' expr                                         
    | expr T_IS_SMALLER_OR_EQUAL expr                       
    | expr '>' expr                                         
    | expr T_IS_GREATER_OR_EQUAL expr                       
    | expr T_INSTANCEOF class_name_reference                
    | parentheses_expr                                      
    /* we need a separate left_arc new_expr right_arc rule to avoid problems caused by a s/r conflict */
    | left_arc new_expr right_arc                                      
    | expr '?' expr ':' expr                                
    | expr '?' ':' expr                                     
    | expr T_COALESCE expr                                  
    | T_ISSET left_arc variables_list right_arc                        
    | T_EMPTY left_arc expr right_arc                                  
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
    | T_FUNCTION optional_ref left_arc parameter_list right_arc lexical_vars optional_return_type      '{' inner_statement_list '}'
          
    | T_STATIC T_FUNCTION optional_ref left_arc parameter_list right_arc lexical_vars optional_return_type      '{' inner_statement_list '}'
          
;

parentheses_expr:
      left_arc expr right_arc                        
    | left_arc yield_expr right_arc                  
;

yield_expr:
      T_YIELD expr                        
    | T_YIELD expr T_DOUBLE_ARROW expr    
;

array_expr:
      T_ARRAY left_arc array_pair_list right_arc     
    | '[' array_pair_list ']'             
;

scalar_dereference:
      array_expr '[' dim_offset ']'       
    | T_CONSTANT_ENCAPSED_STRING '[' dim_offset ']'
          
    | constant '[' dim_offset ']'         
    | scalar_dereference '[' dim_offset ']' 
    /* alternative array syntax missing intentionally */
;

anonymous_class:
      T_CLASS ctor_arguments extends_from implements_list '{' class_statement_list '}'
          

new_expr:
      T_NEW class_name_reference ctor_arguments         
    | T_NEW anonymous_class
          
;

lexical_vars:
      /* empty */        
    | T_USE left_arc lexical_var_list right_arc      
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
    | class_name_or_var T_PAAMAYIM_NEKUDOTAYIM identifier argument_list
      
    | class_name_or_var T_PAAMAYIM_NEKUDOTAYIM '{' expr '}' argument_list
      
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
      namespace_name_parts                             
    | T_NS_SEPARATOR namespace_name_parts              
    | T_NAMESPACE T_NS_SEPARATOR namespace_name_parts  
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
    | left_arc right_arc                                              
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
      T_LNUMBER                    
    | T_DNUMBER                    
    | T_CONSTANT_ENCAPSED_STRING   
    | T_LINE                       
    | T_FILE                       
    | T_DIR                        
    | T_CLASS_C                    
    | T_TRAIT_C                    
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
    | T_ARRAY left_arc static_array_pair_list right_arc         
    | '[' static_array_pair_list ']'                 
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
    | left_arc static_scalar right_arc                             
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
      left_arc new_expr right_arc '[' dim_offset ']'                
    | new_expr_array_deref '[' dim_offset ']'            
      /* alternative array syntax missing intentionally */
;

object_access:
      variable_or_new_expr T_OBJECT_OPERATOR object_property %prec test2
          
    | variable_or_new_expr T_OBJECT_OPERATOR object_property argument_list
          
    | object_access argument_list         
    | object_access '[' dim_offset ']'     
    | object_access '{' expr '}'           
;

variable_or_new_expr:
      variable                            
    | left_arc new_expr right_arc                    
;

variable_without_objects:
      reference_variable      %prec test            
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
          
    | class_name_or_var T_PAAMAYIM_NEKUDOTAYIM '$' '{' expr '}'
          
    | static_property_with_arrays '[' dim_offset ']'   
    | static_property_with_arrays '{' expr '}'         
;

reference_variable:
      reference_variable '[' dim_offset ']'     
    | reference_variable '{' expr '}'           
    | T_VARIABLE                                
    | '$' '{' expr '}'                          
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
      T_LIST left_arc list_expr_elements right_arc         
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
    | T_DOLLAR_OPEN_CURLY_BRACES expr '}'       
    | T_DOLLAR_OPEN_CURLY_BRACES T_STRING_VARNAME '}'    
    | T_DOLLAR_OPEN_CURLY_BRACES T_STRING_VARNAME '[' expr ']' '}'
          
    | T_CURLY_OPEN variable '}'     
;

encaps_var_offset:
      T_STRING                      
    | T_NUM_STRING                  
    | T_VARIABLE                    
;



%%



void yyerror(char *s) 
{
	;
}

int yylex()
{
	return lexer->yylex();
}
void main(void)
{
	Parser* p = new Parser();
	p->parse();
	
}