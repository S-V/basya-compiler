#ifndef __IBURG_H__
#define __IBURG_H__

#include <stdio.h>
#include <string.h>
#include <assert.h>
//#include <memory.h>
//#include <malloc.h>
#include <stdarg.h>

#define C4_DEBUG

typedef char	byte;
typedef int		bool;

enum { false = 0, true = 1 };

typedef int		int32;

#define inline

#ifndef BIT
#define BIT(n)  (1 << (n))
#endif

//----------------------------------------------------------------------------//---------------------------------------
#define DEBUG( code )	(code)

#define  Unreachable()	\
	printf( "Unreachable code in %s, at '%s', line %d", __FUNCTION__, __FILE__, __LINE__ ), getchar(), exit( -1 ); 
//----------------------------------------------------------------------------//---------------------------------------

/*
=============================
	terminals and nonterminals
=============================
*/

typedef enum {
	E_UNDEFINED,
	E_TERMINAL,
	E_NONTERMINAL,
} Kind;

typedef struct terminal_s  Terminal;
typedef struct rule_s      Rule;

struct terminal_s {
	char *	name;	// terminal name
	Kind	kind;	// == E_TERMINAL
	
	int		arity;	// operator arity
	int		number; // number in natural order

	Terminal * next; // for linking them together in a list

	Rule *	rules;	// rules whose pattern starts with this terminal
};

typedef struct nonterminal_s  Nonterminal;

struct nonterminal_s {
	char *	name;		// nonterminal name
	Kind	kind;		// == E_NONTERMINAL
	int		number;		// identifying number

	Rule *	rules;		// rules with this nonterm on lhs
	Rule *	chain;		// chain rules with this nonterm on rhs

	Nonterminal * next;
};

extern Terminal *		MakeTerminal( char * name );	// create a new terminal with the given name
extern Nonterminal *	MakeNonterminal( char * name );	// create a new nonterminal with the given name

/*
=====================
	tree patterns
=====================
*/
typedef struct tree_s  Tree;
struct tree_s {
	Kind		kind;	// E_TERMINAL or E_NONTERMINAL
	union {
		Terminal *		term;
		Nonterminal *	nonterm;
	};

	char *		original_name;

	Tree *		left;		// left child
	Tree *		right;		// right child
	int			num_terms;	// number of terminal nodes in this tree

	Rule *		rule;	// the rule associated with this tree
};

extern Tree *	MakeTree( char *op, Tree *left, Tree *right );

//-------------------------------------------------------------------------------------

#include <common/x86_codegen.h>

/*
==============
	Rules

  instructions are also represented as rules.
==============
*/

// pre-defined identifiers
//
/*typedef enum 
{
	reservedId_Reg_8,
	reservedId_Reg_16,
	reservedId_Reg_32,		// any general-purpose 32-bit register
	reservedId_Register,	// only the specified register

} ReservedType;
*/
typedef enum EOperandType	ReservedType;

typedef struct {
	Kind		kind;	// E_TERMINAL or E_NONTERMINAL
	union {
		char *	term_name;
		char *	nonterm_name;
	};
	char *			name;	// EAX ( eax ), ECX ( ecx )

	ReservedType	type;
	int32			data;

	//char *		general_name;	// r8, r32, r16, etc.
	//byte		size;	// in bits ( 8, 18, 32, 64, etc. )
} ReservedIdentifier;

typedef struct rule_data_s {
	char *		name;
	int32		flags;	// INSTRUCTION_FLAGS
	
	operand_info_t	result;
	operand_info_t	operands[ MAX_NUM_OPERANDS ];

	// any of these fields can be NULL !
	//
	char *		operand_constraints;
	char *		side_effects;

	char *		attributes;

	char *		asm_template;	// assembler template
	char *		cost;

	char *		constructor;
	char *		semantic_action;	// a piece of C code to be executed
	char *		destructor;

	int		numOperandsInTemplate; // for debugging
} RuleData;

struct rule_s {
	Nonterminal *	lhs_nonterm;	// lefthand side nonterminal
	Tree *			pattern;		// rule pattern
	int				number;			// external rule number

	Rule *		link;		// next rule in number order

	Rule *		next;		// next rule with the same pattern root
	Rule *		chain;		// next chain rule with the same rhs
	Rule *		decode;		// next rule with the same lhs
	
	RuleData *	ruleData;

	unsigned	numUses;	// == 0 then this rule isn't used and we should issue a warning
};

extern Rule * MakeRule( char *lhs_nonterm, Tree *pattern,
					   char *asm_template, char* cost, 
					   char *ctor, char *code, char *dtor );

extern Rule * MakeInstruction( char *result, Tree *pattern,
						char *operand_constraints, char *side_effects, char *attributes,
						char *asm_template, char *cost,
						char *ctor, char *code, char *dtor );

//------------------------------------------------------------
// lexer and parser stuff

extern unsigned g_lineNo;	// for reporting errors

//-------------------
//extern void  Error( const char * format, ... );
#define Error( string )   yyerror( string )

int		yylex( void );
int		yyparse( void );
void	yywarn( char * format, ... );
void	yyerror( char * format, ... );

//extern char * yytext;
extern char yytext[];
extern FILE * yyin;

#endif // !__IBURG_H__