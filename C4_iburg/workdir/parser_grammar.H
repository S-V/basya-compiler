%{
	#include <stdlib.h>
	#include <malloc.h>
	
	#include "../iburg.h"

	void	yyerror( char * format, ... );
%}

%union {
	int		number;
	char *	string;
	Tree *	tree;
}

%term	START		// "%start"
%term	TERMINAL	// "%term"
%term	PPERCENT	// "%%"
//%term	NIL			// nil
//%term   CLOBBER	// clobber
%term	CONSTRUCTOR	// "%ctor"
%term	DESTRUCTOR	// "%dtor"

%token	<string>	IDENTIFIER
					TEMPLATE	// assembler template
					CODE		// a piece of C-code in curly braces ( for writing semantic actions )
					COST		// '<' /* cost */... '>'
					
%token	<number>	INTEGER

%type	<tree>		tree

%type	<string>	Id
					nonterminal
					term_list
					template

%type	<string>	cost
					result
					operand_constraints
					side_effects
					instruction_attributes

%type	<string>	code
					ctor
					dtor
%%

specification
	: declarations	PPERCENT	rules	'!'		{ printf( "\n\tSuccessfully parsed the whole specification with rules!\n\n" ); return 0; }
	;

declarations
	: /* nothing */
	| declarations		declaration
	;

declaration
	: START		nonterminal		'\n'		{   if ( MakeNonterminal( $2 )->number > 1 ) {
													yyerror( "redeclaration of the start symbol!\n" ); 
												} 
											}
	| '\n'
	| TERMINAL	term_list		'\n'		{ }
	| error	'\n'
	;

Id:	IDENTIFIER		{ $$ = (char* )malloc( strlen( yytext ) + 1 ); 
					  strcpy( $$, yytext );		
					}
	;
	
term_list:	/* nothing */		{ $$ = NULL; }
	| term_list		Id			{ MakeTerminal( $2 ); }
	;

rules:	// nothing
	| rules  nonterminal ':'  tree  
		template  
		cost 
		ctor
		code
		dtor
	{
		MakeRule( $2, $4 /* tree*/, 
			$5 /* template */, 
			$6 /* cost */, 
			$7 /* ctor */,
			$8 /* code */,
			$9 /* dtor */ );
	}
	
	| rules
		'[' result ':' tree '\n'
		operand_constraints
		side_effects
		instruction_attributes
		template	'\n'			 
		cost		'\n'					 
		ctor
		code
		dtor
		opt_newline ']'
	{
		MakeInstruction( 
			$3 /* result */, $5 /* tree */, 
			$7 /* operand constraints */,
			$8 /* side effects */,
			$9 /* attributes */, 
			$10 /* template */, 
			$12 /* cost */, 

			$14 /* ctor */, 
			$15 /* code */, 
			$16 /* dtor */ 
		);
	}
	
	| rules  '\n'
//	| rules  error  '\n'
	;

nonterminal:  Id	{ $$ = $1; MakeNonterminal( $$ ); }
	;

tree:	Id							{ $$ = MakeTree( $1, NULL, NULL ); }
	|	Id '(' tree ')'				{ $$ = MakeTree( $1, $3, NULL ); }
	|	Id '(' tree ',' tree ')'	{ $$ = MakeTree( $1, $3, $5 ); }
	|	'#'							{ $$ = NULL; }
	;

template:	/* nothing */	{ $$ = NULL; }
	| TEMPLATE				{	$$ = (char* )malloc( strlen( yytext ) + 1 );
								strcpy( $$, yytext );
							}
	;

cost: /* nothing */			{ $$ = NULL; }
	| COST					{	$$ = (char* )malloc( strlen( yytext ) + 1 ); 
								strcpy( $$, yytext );
							}
	;

code: /* nothing */			{ $$ = NULL; }
	| CODE					{	$$ = (char* )malloc( strlen( yytext ) + 1 ); 
								strcpy( $$, yytext );
							}
	;

ctor: /* nothing */			{ $$ = NULL; }
	| CONSTRUCTOR code '\n'	{ $$ = $2; }
	;

dtor: /* nothing */			{ $$ = NULL; }
	| DESTRUCTOR code '\n'	{ $$ = $2; }
	;

result:  /* nothing */		{ $$ = NULL; }
	| '%' INTEGER			{ char  buf[ 4 ];
						      char * p = buf;
							  
							  *p++ = '%';
							  *p++ = *yytext;
							  *p = '\0';
								
								$$ = (char* )malloc( strlen( buf ) + 1 );
								strcpy( $$, buf ); 
							}
	| nonterminal
	;

operand_constraints: /* nothing */	{ $$ = NULL; }
	| '?' code '\n'				{ $$ = $2; }
	;
	
side_effects: /* nothing */	{ $$ = NULL; }
	| '!' code '\n'			{ $$ = $2; }
	;
	
instruction_attributes: /* nothing */ { $$ = NULL; }
	| '[' code ']' '\n' { $$ = $2; }
	;

opt_newline: // nothing
	| opt_newline '\n'
	;

%%

void	yyerror( char * format, ... ) {
	char		buf[ 128 ];
	va_list		ap;

	va_start( ap, format );
	sprintf( buf, format, ap );
	va_end( ap );

	printf( "Error(%d): %s.\n", g_lineNo, buf );
	printf( "Curr. text: '%s'.\n", yytext );
	getchar();
	exit( -1 );
}
