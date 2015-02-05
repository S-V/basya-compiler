#include <malloc.h>
#include "iburg.h"

//char * g_input_filename = "x86_md.h";
//char * g_output_filename = "x86_cg.cpp";

static const char * g_class_name = "Machine_x86";

static const char * g_input_filename = "C:/Documents and Settings/admin/Рабочий стол/C4/source/backend/x86/x86.h"; //"C:/project/C4/source/backend/x86/x86.h";
static const char * g_output_filename = "C:/Documents and Settings/admin/Рабочий стол/C4/source/backend/x86/x86_cg.cpp";//"C:/project/C4/source/backend/x86/x86_cg.cpp";

static FILE *  g_outputFile;

// max. number of chars for storing temporary string data;
// if too low, may cause buffer overrun;
// if too high, the overhead of allocating arrays will also be high
enum { MAX_STR_LENGTH = 128 };

#define ZERO_COST	"0"

//---------------------------------------------------------------------------
static void  Init();
static void  EmitWholeProgram();

//---------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
	FILE *	inputFile = fopen( g_input_filename, "r" );
	if ( ! inputFile ) {
		printf( "failed to open file '%s'.\n", g_input_filename );
		return -1;
	}

	Init();

	yyin = inputFile;
	yyparse();
	fclose( inputFile );
	
	// create output file
	g_outputFile = fopen( g_output_filename, "w" );
	EmitWholeProgram();
	//-------------------
	fclose( g_outputFile );
	
	return 0;
}

/*
====================
	Terminals 
====================
*/
static Terminal *		g_terminals = NULL;		// list of terminals in number order ( 1, 2, 3, ... )
static unsigned			g_numTerms = 0;			// number of terminals

static void		InsertTerm( Terminal * newTerm );

/*
====================
	Nonterminals 
====================
*/
static Nonterminal *	g_startNonterm = NULL;	// start nonterminal in number order
static Nonterminal *	g_nonterms = NULL;		// list of nonterminals
static unsigned			g_numNonterms = 0;		// number of nonterminals
static char *			g_suffix = "_NT";

static void		InsertNonterm( Nonterminal* newNonterm );

/*
====================
	Rules
====================
*/
static Rule *			g_rules = NULL;		// list of rules in number order
static unsigned			g_numRules = 0;		// number of rules

static void		InsertRule( Rule* newRule );

int			g_indent = 1;	// how many '\t' to write
static void		Indent();

//==========================================================================================
static void  EmitHeader();
static void  EmitListOfNonterminals();
static void  EmitStructState();
static void  EmitAssemblerTemplates();
static void  EmitConstructors();
static void  EmitSemanticActions();
static void  EmitDestructors();
static void  EmitCaseInGetStateFunction( Terminal *p );
static void  EmitLabellingFunction();
static void  WriteRule( Rule * rule, bool bWriteSemanticAction );
static void  EmitRules();
static void  EmitNonterminals();
static void  EmitKids( Rule *rules );
static void  EmitInstructionData();
static void  EmitCodeGenerationFunctions();
static void  EmitEnding();

static void  Error( char *format, ... );
//---------------------------------------------------------------------------
static void  EmitWholeProgram() {
	EmitHeader();
	EmitListOfNonterminals();
	EmitStructState();
	EmitLabellingFunction();
	EmitAssemblerTemplates();
	EmitRules();
	EmitNonterminals();
	EmitKids( g_rules, g_numRules );
	
	EmitConstructors();
	EmitSemanticActions();
	EmitDestructors();

	EmitInstructionData();
	EmitCodeGenerationFunctions();
	// ...
	EmitEnding();
}

//---------------------------------------------------------------------------

//static void  Emit( const char* format, ... );
#define Emit( string )	fprintf( g_outputFile, string )

/*
===============================
	Symbol table management
===============================
*/
#define TABLE_SIZE	211

typedef struct {
	char *	name;
	Kind	kind;
	union {
		Terminal		term;
		Nonterminal		nonterm;
		//Register		reg;
	};
} Symbol_t;

// symbol table for storing terminals and nonterminals
//
struct Entry_s {
	Symbol_t	symbol;

	struct Entry_s *  next;

} *g_table[ TABLE_SIZE ];

// return hash number for str
//
static unsigned GetHash( char *str ) {
	unsigned h = 0;

	while ( *str ) {
		h = (h << 1) + *str;
		++str;
	}
	return h;
}

// install symbol name
//
static Symbol_t * Install( char *name ) {
	struct Entry_s * pEntry = (struct Entry_s* )malloc( sizeof *pEntry );

	unsigned index = GetHash( name ) % TABLE_SIZE;

	pEntry->symbol.name = name;
	pEntry->next = g_table[ index ];

	g_table[ index ] = pEntry;

	return &( pEntry->symbol );
}

// lookup symbol name
//
static Symbol_t * Lookup( char *name ) {
	unsigned hash = GetHash( name );

	struct Entry_s * pEntry = g_table[ hash % TABLE_SIZE ];
	
	for ( ; pEntry != NULL; pEntry = pEntry->next )
	{
		if ( strcmp( name, pEntry->symbol.name ) == 0 ) {
			return &( pEntry->symbol );
		}
	}
	return NULL;
}

//-----------------------------------------------------------
// Reserved identifiers
//-----------------------------------------------------------

struct ReservedId_Entry_s
{
	ReservedIdentifier			data;
	struct ReservedId_Entry_s *  next;

} *g_reservedId_table[ TABLE_SIZE ];

static ReservedIdentifier * InstallReservedId( char *name ) {
	struct ReservedId_Entry_s * pEntry = (struct ReservedId_Entry_s* )malloc( sizeof *pEntry );

	unsigned index = GetHash( name ) % TABLE_SIZE;
	
	pEntry->data.name = name;
	pEntry->next = g_reservedId_table[ index ];
	g_reservedId_table[ index ] = pEntry;

	return &( pEntry->data );
}

static ReservedIdentifier * LookupReservedId( char *name ) {
	unsigned hash = GetHash( name );

	struct ReservedId_Entry_s * pEntry = g_reservedId_table[ hash % TABLE_SIZE ];
	for ( ; pEntry != NULL; pEntry = pEntry->next )
	{
		if ( strcmp( name, pEntry->data.name ) == 0 ) {
			return &( pEntry->data );
		}
	}
	return NULL;
}

//-----------------------------------------------------------
// installs pre-defined names ( names of registers 
// and names of corresponding nonterminals ),
// initializes everything
//
static void  Init()
{
	char *	name;
	ReservedIdentifier * pId;

	// create the start symbol
	MakeNonterminal( "stmt" );

	// create registers
	pId = InstallReservedId( "r32" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r32";
	pId->type = eReg_32;
	pId->data = 0;

	pId = InstallReservedId( "reg" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r32";
	pId->type = eReg_32;
	pId->data = 0;

	//------------ 32-bit registers --------------

	pId = InstallReservedId( "EAX" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r32";
	pId->type = eRegister;
	pId->data = rEAX;

	pId = InstallReservedId( "ECX" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r32";
	pId->type = eRegister;
	pId->data = rECX;

	pId = InstallReservedId( "EDX" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r32";
	pId->type = eRegister;
	pId->data = rEDX;

	pId = InstallReservedId( "ESI" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r32";
	pId->type = eRegister;
	pId->data = rESI;

	pId = InstallReservedId( "EDI" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r32";
	pId->type = eRegister;
	pId->data = rEDI;

	pId = InstallReservedId( "EBX" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r32";
	pId->type = eRegister;
	pId->data = rEBX;

	//-------- 16-bit registers ------------

	pId = InstallReservedId( "AX" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r16";
	pId->type = eRegister;
	pId->data = rAX;

	//-------- 8-bit registers
	pId = InstallReservedId( "CL" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r8";
	pId->type = eRegister;
	pId->data = rCL;

	//--- register pairs -----------------

/*	// EDX : EAX
	//
	pId = InstallReservedId( "EDX:EAX" );
	pId->kind = E_NONTERMINAL;
	pId->nonterm_name = "r32";
	pId->type = REGISTERS;
	pId->data = BIT( rEDI ) | BIT( ...;
*/
}
//-----------------------------------------------------------
static void InsertTerm( Terminal * newTerm ) {
	if ( ! g_terminals ) {
		g_terminals = newTerm;
		g_terminals->next = NULL;
		return;
	}
	else {
		Terminal * p = g_terminals;
		while ( p->next ) {
			p = p->next;
		}
		assert( p->number < newTerm->number );
		
		p->next = newTerm;
		newTerm->next = NULL;
	}
}
//----------------------------------
// create a new terminal
//
Terminal * MakeTerminal( char * name )
{
	Symbol_t * symbol = Lookup( name );

	if ( symbol ) {
		printf( "redefinition of terminal '%s'.\n", name );
		Error( "fatal error" );
	}
	else {
		symbol = Install( name );
		symbol->kind = E_TERMINAL;

		++g_numTerms;
		{
			Terminal * newTerm = & symbol->term;
			
			newTerm->kind = E_TERMINAL;
			newTerm->name  = name;
			
			newTerm->number  = g_numTerms;
			
			// this field ( rules whose pattern starts with this terminal ) will be set in MakeRule()
			newTerm->rules = NULL;
			
			// the arity of this terminal will be inferred
			// from the rules in which it is used;
			// this field will be set in MakeTree()
			newTerm->arity = -1;

			InsertTerm( newTerm );
		}
	}

	return &( symbol->term );
}
//-----------------------------------------------------------
// insert a new nonterminal into the list
//
void InsertNonterm( Nonterminal* newNonterm )
{
	if ( ! g_nonterms ) {
		g_nonterms = newNonterm;
		g_nonterms->next = NULL;
		return;
	}
	else {
		Nonterminal * p = g_nonterms;
		while ( p->next ) {
			p = p->next;
		}
		assert( p->number < newNonterm->number );
		
		p->next = newNonterm;
		newNonterm->next = NULL;
	}
}
//-----------------------------------------------------------
// create a new nonterminal, if necessary
//
Nonterminal * MakeNonterminal( char *name )
{
	Nonterminal * newNonterm;
	Symbol_t * symbol = Lookup( name );

	//*** pre-defined identifiers

	// for example, if 'name' is 'EAX', 
	// then the corresponding nonterminal is 'r32'
	ReservedIdentifier * reservedId = LookupReservedId( name );
	if ( reservedId )
	{
		if ( reservedId->kind == E_NONTERMINAL ) {
			name = reservedId->nonterm_name;
			symbol = Lookup( name );
		}
		else if ( reservedId->kind == E_TERMINAL ) {
			Error( "'%s' is a terminal\n", name );
		}
	}

	if ( symbol )
	{
		if ( symbol->kind == E_NONTERMINAL ) {
			return & symbol->nonterm;
		}
		if ( symbol->kind == E_TERMINAL ) {
			Error( "'%s' is a terminal\n", name );
		}
	}

	symbol = Install( name );
	symbol->kind = E_NONTERMINAL;

	++g_numNonterms;

	newNonterm = & symbol->nonterm;

	newNonterm->name = name;
	newNonterm->kind = E_NONTERMINAL;

	newNonterm->number = g_numNonterms;
	if ( newNonterm->number == 1 ) {
		g_startNonterm = newNonterm;
	}

	newNonterm->rules = NULL;
	newNonterm->chain = NULL;

	InsertNonterm( newNonterm );

	return newNonterm;
}
//------------------------------------------------------------------------------------------
#define TREE_NAME( tree )	( (tree)->kind == E_TERMINAL ? (tree)->term->name : (tree)->nonterm->name )

#define TREE_COST( tree )	( (tree)->rule ? (tree)->rule->ruleData->cost ? (tree)->rule->ruleData->cost : ZERO_COST : ZERO_COST )
//------------------------------------------------------------------------------------------
// create and initialize a tree node with the given fields
// example: "stmt: ASSIGN( reg, mrc )", "stmt" is 'op', "reg" is 'left', "mrc" is 'right'
// example: "mrc:  reg", where "mrc" is 'op', 'left' and 'right' are NULL
//
Tree * MakeTree( char *op, Tree *left, Tree *right )
{
	Tree * newTree = ( Tree* ) malloc( sizeof *newTree );
	//memset( newTree, 0, sizeof *newTree );

	Symbol_t * symbol = Lookup( op );

	int  arity = 0;

	if ( left && right ) {
		arity = 2;
	}
	else if ( left ) {
		arity = 1;
	}

	newTree->original_name = op;

	// example: 'EAX' is a nonterminal 'r32' and vice versa
	//if ( symbol && symbol->kind == E_RESERVED ) {
	//	//DEBUG( printf( "reserved id: %s\n", op ) );
	//	newTree->kind = E_NONTERMINAL;
	//	newTree->nonterm = MakeNonterminal( symbol->reg.general_name );
	//	goto L_DONE;
	//}

	if ( !symbol && arity == 0 ) {
		// example: "reg: some_nonterm"
		// create a new nonterminal
		newTree->kind = E_NONTERMINAL;
		newTree->nonterm = MakeNonterminal( op );
		goto L_DONE;
	}
	else if ( !symbol && arity > 0 ) {
		// example: "reg: UNKNOWN_TERM( mr, mrc )"
		yyerror( "undefined terminal: '%s'\n", op );
	}
	else if ( symbol && symbol->kind == E_NONTERMINAL && arity > 0 ) {
		// example: "reg: some_nonterm( reg, mrc )"
		yyerror( "'%s' is a nonterminal\n", op );
	}
	
	if ( !symbol ) {
		yyerror( "undefined identifier '%s'\n", op );
	}

	// if this terminal has been declared, but hasn't been used yet
	if ( symbol->kind == E_TERMINAL && symbol->term.arity == -1 ) {
		// then set it's arity
		symbol->term.arity = arity;
		
		newTree->kind = E_TERMINAL;
		newTree->term = & symbol->term;
		goto L_DONE;
	}
	if ( symbol->kind == E_TERMINAL && symbol->term.arity == arity ) {
		// then everything's OK
		newTree->kind = E_TERMINAL;
		newTree->term = & symbol->term;
		goto L_DONE;
	}
	if ( symbol->kind == E_TERMINAL && symbol->term.arity != arity ) {
		yyerror( "inconsistent arity for terminal '%s'\n", op );
	}
	if ( symbol->kind == E_NONTERMINAL ) {
		newTree->kind = E_NONTERMINAL;
		newTree->nonterm = & symbol->nonterm;
	}

L_DONE:
	if ( newTree->kind == E_TERMINAL ) {
		newTree->num_terms = 1;
	} else if ( newTree->kind == E_NONTERMINAL ) {
		newTree->num_terms = 0;
	} else {
		yyerror( "invalid kind '%d' of tree '%s'\n", newTree->kind, op );
	}
	newTree->left = left;
	newTree->right = right;

	if ( left ) {
		newTree->num_terms += left->num_terms;
	}
	if ( right ) {
		newTree->num_terms += right->num_terms;
	}
	// will be set in MakeRule()
	newTree->rule = NULL;
	return newTree;
}
//------------------------------------------------------------------------------------------
// In assember templates "%3" stands for the third operand's name;
// Max. number of operands: 0 - 9 ( one digit ).
//
static int CalcNumOfOperandsInTemplate( char *asm_template ) {
	int  numOperands = 0;
	char * p;

	if ( ! asm_template ) {
		return 0;
	}
	
	p = asm_template;
	while ( *p ) {
		if( *p == '%' ) {
			++p;  // skip '%'
			if ( *p >= '0' && *p <= '9' ) {
				++p; // skip digit
				++numOperands;
				continue;
			}
			else if ( *p == 'c' ) {
				++p; // skip 'c'
				continue;
			}
			else if ( *p == 'L' ) {
				// a name of some label will be inserted here
				++p;
				continue;
			}
			else {
				yyerror( "CalcNumOfOperandsInTemplate() : invalid character '%c' after '%%'!", *p );
			}
		}
		++p;
	}
	return numOperands;
}
//------------------------------------------------------------------------------------------
// returns the name of the Nth child of the given pattern
// example:
//  Given a tree pattern "assign( addr, rc )",
// GetNameForNontermWithNumber( pattern, 1 ) will return "rc"
//
char * GetNameForNontermWithNumber( Tree *pattern, int num )
{
	if ( num == 0 ) {
		return TREE_NAME( pattern->left );
	}
	if ( num == 1 ) {
		return TREE_NAME( pattern->right );
	}
	Error( "GetNameForNontermWithNumber(): num == %d", num );
	return NULL;

/* UNDONE: all cases, where 'pattern' has more than two children
	// TODO: get rid of this static var!
	static int prevNum = 0;
	if ( pattern->kind == E_NONTERMINAL )
	{
		++prevNum;
		if ( prevNum == num ) {
			return TREE_NAME( pattern );
		}
	}
	return NULL;*/
}
//------------------------------------------------------------------------------------------
static bool HasResultNameInTemplate( char *asm_template ) {
	char * p;
	if ( ! asm_template ) {
		return false;
	}
	p = asm_template;
	while ( *p ) {
		if( *p == '%' ) {
			++p; // skip '%'
			if ( *p >= '0' && *p <= '9' ) {
				continue;
			}
			else if ( *p == 'c' ) {
				return true;
			}
			else if ( *p == 'L' ) {
				// a name of some label will be inserted here
				++p;
				continue;
			}
			else {
				yyerror( "HasResultNameInTemplate() : invalid character '%c' after '%%'!", *p );
			}
		}
		++p;
	}
	return false;
}
//------------------------------------------------------------------------------------------
static void  InitRuleOperands( RuleData *ruleData, Tree *pattern, int *opNumber )
{
	if ( pattern->kind == E_TERMINAL ) 
	{
		if ( pattern->left ) {
			InitRuleOperands( ruleData, pattern->left, opNumber );
		}
		if ( pattern->right ) {
			InitRuleOperands( ruleData, pattern->right, opNumber );
		}
	}
	else // if 'pattern' is a nonterminal
	{
		operand_info_t * pInfo = &( ruleData->operands[ *opNumber ] );

		ReservedIdentifier * reservedId = LookupReservedId( pattern->original_name );
		if ( reservedId )
		{
			switch ( reservedId->type ) {
				case eReg_8 :
					pInfo->type = eReg_8;
					break;

				case eReg_16 :
					pInfo->type = eReg_16;
					break;

				case eReg_32 :
					pInfo->type = eReg_32;
					break;

				case eRegister :
					pInfo->type = eRegister;
					pInfo->data = reservedId->data;
					break;

				default:
					pInfo->type = eDefault;
					pInfo->data = 0;
			}
		}
		else
		{
			pInfo->type = eDefault;
			pInfo->data = 0;
		}

		++*opNumber;
	}
}
//------------------------------------------------------------------------------------------
// appends 'src' to 'dest'
//
static void  Append( char *dest, char* src ) {
	char * p1, *p2;
	p1 = dest;
	while ( *p1 ) {
		++p1;
	}
	p2 = src;
	while ( *p2 ) {
		*p1++ = *p2++;
	}
	*p1 = '\0';
}
//------------------------------------------------------------------------------------------
Rule * MakeRule( char *lhs_nonterm, Tree *pattern,
		   char *asm_template, char* cost, 
		   char *ctor, char *code, char *dtor )
{
	int  num = 0; // temp var to store integer values

	Rule * newRule = (Rule* )malloc( sizeof *newRule );

	RuleData * ruleData = malloc( sizeof *ruleData );
	memset( ruleData, 0, sizeof *ruleData );

	// 'lhs_nonterm' can begin with a '%%' to specify the result's location
	if ( *lhs_nonterm == '%' )
	{
		char  c = lhs_nonterm[ 1 ];
		assert( '0' <= c && c <= '9' );
		
		num = c - '0';
		lhs_nonterm = GetNameForNontermWithNumber( pattern, num );

		ruleData->result.type = eResultInOperand;
		ruleData->result.data = num;
	}
	// 'lhs_nonterm' can be a pre-defined identifier,
	// for example, 'r32', 'EAX', 'imm', 'EDX:EAX', etc.,
	// so check this
	else
	{
		ReservedIdentifier * reservedId = LookupReservedId( lhs_nonterm );
		if ( reservedId )
		{
			switch ( reservedId->type ) {
				case eReg_8 :
					ruleData->result.type = eReg_8;
					break;

				case eReg_16 :
					ruleData->result.type = eReg_16;
					break;

				case eReg_32 :
					ruleData->result.type = eReg_32;
					break;

				case eRegister :
					ruleData->result.type = eRegister;
					ruleData->result.data = reservedId->data;
					break;

				default:
					Unreachable();
			}
		}
	}//end check if 'lhs_nonterm' is a reserved identifier

	newRule->lhs_nonterm = MakeNonterminal( lhs_nonterm );
	// init. the field 'newRule->lhs_nonterm->rules' - list of rules with this nonterm on lhs
	{
		Rule ** p = & newRule->lhs_nonterm->rules;
		while ( *p != NULL )
		{
			p = & ( *p )->decode;
		}
		*p = newRule;
	}
	newRule->pattern = pattern;
	newRule->pattern->rule = newRule;

	++g_numRules;

	newRule->number = g_numRules;

	newRule->chain = NULL;
	newRule->decode = NULL;

	if ( pattern->kind == E_TERMINAL )
	{
		// init. the field 'pattern->term->rules' - list of rules whose pattern with this nonterminal
		Terminal * term = pattern->term;
		
		Rule ** p = & term->rules;
		while ( *p != NULL ) 
		{
			p = & ( *p )->next;
		}
		*p = newRule;
	} 
	else if ( pattern->kind == E_NONTERMINAL 
		&& pattern->left == NULL 
		&& pattern->right == NULL )
	{
		Nonterminal * nonterm = pattern->nonterm;
		newRule->chain = nonterm->chain;
		nonterm->chain = newRule;
	}
	else {
		yyerror( "invalid pattern in rule: '%s : %s ... '\n", lhs_nonterm, TREE_NAME( pattern ) );
	}

	newRule->next = NULL;

	//--- init. rule data ------------------
	
	// init. the field 'operands'
	InitRuleOperands( ruleData, pattern, &num );

	// init. constructors, semantic actions and destructors
	if ( ctor ) {
		ruleData->flags |= HAS_CONSTRUCTOR;
	}
	if ( code ) {
		ruleData->flags |= HAS_SEMANTIC_ACTION;
	}
	if ( dtor ) {
		ruleData->flags |= HAS_DESTRUCTOR;
	}

	if ( HasResultNameInTemplate( asm_template ) ) {
		ruleData->flags |= HAS_RESULT_NAME_IN_TEMPLATE;
	}

	
	// generate a unique name for this rule
	{
		char	buf[ MAX_STR_LENGTH ];
		char	bufForNumber[ 6 ];
		
		buf[ 0 ] = '\0';
		bufForNumber[ 0 ] = '\0';

		Append( buf, TREE_NAME( pattern ) );

		if ( pattern->left ) {
			Append( buf, "_" );
			Append( buf, TREE_NAME( pattern->left ) );
		}
		if ( pattern->right ) {
			Append( buf, "_" );
			Append( buf, TREE_NAME( pattern->right ) );
		}
		
		sprintf( bufForNumber, "_%d", newRule->number );

		Append( buf, bufForNumber );
		
		ruleData->name = (char* )malloc(strlen( buf ) + 1);
		strcpy( ruleData->name, buf );
	}

	ruleData->asm_template	= asm_template;
	ruleData->cost = (cost != NULL) ? cost : "0";;

	ruleData->constructor	= ctor;
	ruleData->semantic_action = code;
	ruleData->destructor	= dtor;

	ruleData->numOperandsInTemplate = CalcNumOfOperandsInTemplate( asm_template );

	newRule->ruleData = ruleData;

	//------------------------------------
	InsertRule( newRule );
	return newRule;
}
//------------------------------------------------------------------------------------------
static void InsertRule( Rule * newRule ) {
	if ( ! g_rules ) {
		g_rules = newRule;
		g_rules->link = NULL;
		return;
	}
	else {
		Rule * p = g_rules;
		while ( p->link ) {
			p = p->link;
		}
		assert( p->number < newRule->number );
		
		p->link = newRule;
		newRule->link = NULL;
	}
}
//------------------------------------------------------------------------------------------
// instruction is represented as rule;
// "side_effects" are processed and appended to the rule's 'code'.
//
Rule * MakeInstruction( char *result, Tree *pattern,
			char *operand_constraints, char *side_effects, char *attributes,
			char *asm_template, char *cost,
			char *ctor, char *code, char *dtor )
{
	Rule * newRule; 

	if ( ! asm_template ) {
		Error( "assembler template cannot be NULL in an instruction!\n" );
	}

	newRule = MakeRule( result, pattern, asm_template, cost, ctor, code, dtor );

	// initialize the other fields in the rule data, which have not been yet initialized after calling MakeRule()
	
	newRule->ruleData->flags |= IS_INSTRUCTION;

	newRule->ruleData->operand_constraints	= operand_constraints;
	newRule->ruleData->side_effects			= side_effects;

	newRule->ruleData->attributes = attributes;

	return newRule;
}

//=======================================================
static void  EmitHeader() {
	Emit(
		"/*\n"
		"===============================================\n"
	);
	fprintf( g_outputFile, "This file is generated from %s.h\n", g_input_filename );
	Emit(
		"===============================================\n"
		"*/\n"
	//	"#include \"stdafx.h\"\n"
	//	"#pragma hdrstop\n"
		"#include <include/c4.h>\n"
		"\n"
		"#include \"../IR.h\"\n"
		"#include \"Machine_x86.h\"\n"
		"\n"
		"namespace c4 {\n"
	//	"namespace cg {
		"\n"
		"enum {	MAX_COST = 9999	};\n"
		"\n"
	);
}
static void  EmitEnding() {
	Emit(
	//	}//end namespace cg
		"\n"
		"}//end namespace c4\n"
	);
}
//------------------------------------------
int		yywrap() { return 0; }
int		isatty( int a ) { return 0; }
//------------------------------------------
static void  EmitListOfNonterminals() {
	Nonterminal * p = g_nonterms;
	Emit( "// nonterminals\n//\n" );
	while ( p ) {
		Emit( "const int\t" );
		fprintf( g_outputFile, "%s%s", p->name, g_suffix );
		Emit( " = " );
		fprintf( g_outputFile, "%d", p->number );
		Emit( ";\n" );

		p = p->next;
	}
	fprintf( g_outputFile, "\nconst int\tNUM_NONTERMS = %d; // number of nonterminals", g_numNonterms );
	fprintf( g_outputFile, "\nconst int\tNUM_RULES = %d; // number of rules", g_numRules );
}
//------------------------------------------------------------------------------------------
// emit the definition of the state structure
//
static void  EmitStructState() {
	Emit(
		"\n\n"
		"struct state_s \n"
		"{\n"
		"	state_s()\n"
		"		: op( 0 )\n"
		"		, left( NULL )\n"
		"		, right( NULL )\n"
		"	{\n"
		"		Init();\n"
		"	}\n"
		"\n"
		"	state_s( int opcode, state_s* leftChild, state_s* rightChild ) \n"
		"		: op( opcode )\n"
		"		, left( leftChild )\n"
		"		, right( rightChild )\n"
		"	{\n"
		"		Init();\n"
		"	}\n"
		"\n"
		"	// for tree pattern matching\n"
		"	int			op;\n"
		"	state_s *	left;\n"
		"	state_s *	right;\n"
		"\n"
 		"	int			cost[ NUM_NONTERMS + 1 ];\t// for recording costs of matches\n"
		"	int			rule[ NUM_NONTERMS + 1 ];\t// for recording matches\n"
		"\n"
		"public:\n"
		"	// record a match only if its cost is less than that of previous matches\n"
		"	__forceinline void Record( int nonterm, int _cost, int _rulenum ) {\n"
		"		if ( _cost < cost[ nonterm ] ) {\n"
		"			cost[ nonterm ] = _cost;\n"
		"			rule[ nonterm ] = _rulenum;\n"
		"		}\n"
		"	}\n"
		"private:\n"
		"	// called in the constructor to initialize the data for the first time\n"
		"	void  Init() {\n"
		"		memset( cost, MAX_COST, (NUM_NONTERMS + 1) * sizeof( cost[ 0 ] ) );\n"
		"		memset( rule, 0, (NUM_NONTERMS + 1) * sizeof( rule[ 0 ] ) );\n"
		"	}\n"
		"};\n\n"
	);
}
//------------------------------------------------------------------------------------------
static void  EmitAssemblerTemplates() {
	Emit(
		"\n// assembler templates ( they are indexed by rule numbers )\n"
		"//\n"
		"const char * g_templates[ NUM_RULES + 1 ] = {\n"
		"/* 0 */		NULL,\n"
	);
	{
		Rule * rule = g_rules;
		while ( rule )
		{
			fprintf( g_outputFile, "/* %d */ %s,\t", 
				rule->number, rule->ruleData->asm_template ? rule->ruleData->asm_template : "NULL" ); //"\"\"" );

			Emit( "/* " );
			WriteRule( rule, false );
			Emit( " */" );

			Emit( "\n" );

			rule = rule->link;
		}
	}
	Emit( "};\n\n" );
}
//------------------------------------------------------------------------------------------
// writes out a textual representation of the given rule
// example: "stmt: ASSING( reg, mem )"
//
static void  WriteRule( Rule * rule, bool bWriteSemanticAction ) {
	Tree * pattern = rule->pattern;
	Tree * left = pattern->left;
	Tree * right = pattern->right;

	fprintf( g_outputFile, "%s : ", rule->lhs_nonterm->name );

	if ( pattern->kind == E_TERMINAL )
	{
		Terminal * term = pattern->term;
		
		switch ( term->arity ) {
		case 0:
			fprintf( g_outputFile, "%s", term->name );
			break;

		case 1:
			fprintf( g_outputFile, "%s", term->name );
			Emit( "( " );
			fprintf( g_outputFile, "%s", TREE_NAME( left ) );
			Emit( " )" );
			break;

		case 2:
			fprintf( g_outputFile, "%s", term->name );
			Emit( "( " );
			fprintf( g_outputFile, "%s", TREE_NAME( left ) );
			Emit( ", " );
			fprintf( g_outputFile, "%s", TREE_NAME( right ) );
			Emit( " )" );
			break;

		default:
			yyerror( "invalid arity '%d' for terminal '%s'\n", term->arity, term->name );
		}
	}
	else {
		fprintf( g_outputFile, "%s", pattern->nonterm->name );
	}

	// write assembler template
	fprintf( g_outputFile, ", %s", rule->ruleData->asm_template ? rule->ruleData->asm_template : "\"\"" );

	// write cost
	if ( strcmp( rule->ruleData->cost,  "0" ) != 0 ) {
		fprintf( g_outputFile, ", <%s>", rule->ruleData->cost );
	}

	// write the code for semantic action
	if ( bWriteSemanticAction && rule->ruleData->semantic_action ) {
		fprintf( g_outputFile, ", %s", rule->ruleData->semantic_action );
	}
}
//------------------------------------------------------------------------------------------
// emit cost computation for the given tree
// example: " + 0 + 1 + 3"
//
static void  EmitCostForRecord( Tree *tree )
{
	Rule * rule = tree->rule;

	Tree * left = tree->left;
	Tree * right = tree->right;

	if ( tree->kind == E_TERMINAL )
	{
		switch ( tree->term->arity ) {
		case 0:
			fprintf( g_outputFile, " + %s", rule->ruleData->cost );
			break;

		case 1:
			EmitCostForRecord( left );
			break;

		case 2:
			fprintf( g_outputFile, " + left->cost[ %s%s ]", left->nonterm->name, g_suffix );
			fprintf( g_outputFile, " + right->cost[ %s%s ]", right->nonterm->name, g_suffix );
			fprintf( g_outputFile, " + %s", rule->ruleData->cost );
			return;

		default:
			yyerror( "operator '%s' has invalid arity %d", tree->term->name, tree->term->arity );
		}
	}
	else {
		fprintf( g_outputFile, " + %s", rule->ruleData->cost );
	}
}
//------------------------------------------------------------------------------------------
/* example:
	case rEAX:
		Record( p, reg_NT, c + 0, 1 );	// reg:	rEAX
		Record( p, rc_NT, c + 0 + 0, 9 );	// rc:	reg
		Record( p, mrc_NT, c + 0 + 0 + 0, 12 );	// mrc:	rc
*/
static void  EmitRecord( Rule *rule, char *cost ) 
{
	Indent();	

	// example:
	//	if ( l->rule[ reg_NT ] && r->rule[ mrc_NT ] ) {
	//		c = 0 + l->cost[ reg_NT ] + r->cost[ mrc_NT ] + 2;	//ADD( reg, mrc ) : reg
	//
	fprintf( g_outputFile, "p->Record( %s%s, c + %s, %d );\t", 
		rule->lhs_nonterm->name, g_suffix, cost, rule->number );

	Emit( "/* " ); WriteRule( rule, false ); Emit( " */\n" );
}
//------------------------------------------------------------------------------------------
// example:
//	Record( p, reg, c + 1, 12 );	/* reg : VAR, "mov\t%c, %0\n", <1>, { Print( "mov reg, VAR\n" ); } */
//	Record( p, stmt, c + 1 + 0, 16 );	/* stmt : reg, "" */
//	Record( p, rc, c + 1 + 0, 7 );		/* rc : reg, "%0" */
//	Record( p, mrc, c + 1 + 0 + 0, 10 );	/* mrc : rc, "%0" */
//		
static void  EmitChainRulesOfRule( Rule *rule, char* prevCost )
{
	Rule * pRule = rule->lhs_nonterm->chain;
	
	char	cost[ MAX_STR_LENGTH ];
	strcpy( cost, prevCost );
	
	for ( ; pRule != NULL; pRule = pRule->chain ) 
	{
		char	cost2[ MAX_STR_LENGTH ];
		strcpy( cost2, cost );
		Append( cost2, " + " );
		Append( cost2, pRule->ruleData->cost );

		EmitRecord( pRule, cost2 );

		EmitChainRulesOfRule( pRule, cost2 );	
	}
}
//------------------------------------------------------------------------------------------
// emit clause for testing a match
// example:
// reg: add( add(reg, reg), const )
// left->left->rule[reg] && left->right->rule[reg]
//	&& right->rule[const]
//
// 'bTreeRoot' must be true if the function is called for the first time
//
static void  EmitTestForPattern( Tree *tree, char* prevTest, bool bTreeRoot ) {
	char	tmp[ MAX_STR_LENGTH ];
	strcpy( tmp, prevTest );

	if ( tree->kind == E_TERMINAL ) {
		if ( ! bTreeRoot ) {
			Emit( prevTest );
			fprintf( g_outputFile, "op == %s", TREE_NAME( tree ) );
			if ( tree->left ) {
				Emit( " && " );
			}
		}
		if ( tree->left ) {
			char	tmp2[ MAX_STR_LENGTH ];
			strcpy( tmp2, tmp );

			Append( tmp2, "left->" );
			EmitTestForPattern( tree->left, tmp2, false );
		}
		if ( tree->right ) {
			char	tmp2[ MAX_STR_LENGTH ];
			strcpy( tmp2, tmp );

			Emit( " && " );
			
			Append( tmp2, "right->" );
			EmitTestForPattern( tree->right, tmp2, false );
			return;
		}
	} else {
		Emit( prevTest );
		fprintf( g_outputFile, "rule[%s%s]", TREE_NAME( tree ), g_suffix );
	}
}
//------------------------------------------------------------------------------------------
// example: "left->left->cost[ nonterm_NT ] + 2;"
// example:
// reg: add( add(reg, reg), const )
//  "left->left->cost[reg] + left->right->cost[reg] + right->cost[const]"
//
static void  EmitCostForPattern( Tree *tree, char* prevCost ) {
	char	tmp[ MAX_STR_LENGTH ];
	strcpy( tmp, prevCost );
	
	if ( tree->kind == E_TERMINAL ) {
		if ( tree->left ) {
			char	tmp2[ MAX_STR_LENGTH ];
			strcpy( tmp2, tmp );
			Append( tmp2, "left->" );
			EmitCostForPattern( tree->left, tmp2 );
		}
		if ( tree->right ) {
			char	tmp2[ MAX_STR_LENGTH ];
			strcpy( tmp2, tmp );
			Emit( " + " );
			Append( tmp2, "right->" );
			EmitCostForPattern( tree->right, tmp2 );
			return;
		}
		if ( tree->rule ) {
			Emit( prevCost );
			fprintf( g_outputFile, "+ %s", TREE_COST( tree ) );
		} else {
			Emit( ZERO_COST );
		}
	} else {
		Emit( prevCost );
		fprintf( g_outputFile, "cost[%s%s]", TREE_NAME( tree ), g_suffix );
	}
}
//------------------------------------------------------------------------------------------
// emit one case in the function "_GetState"
//
static void  EmitCaseInGetStateFunction( Terminal *term ) {
	Rule * rule;
	fprintf( g_outputFile, "\tcase %s:\n", term->name );

	g_indent = 2;	// "\t\t"

	for ( rule = term->rules; rule != NULL; rule = rule->next ) {
		switch ( term->arity ) {
		case 0:
			// example:
			//	case rEAX:
			//		Record( p, reg_NT, c + 0, 1 );	// reg:	rEAX
			//		Record( p, rc_NT, c + 0 + 0, 9 );	// rc:	reg
			//		Record( p, mrc_NT, c + 0 + 0 + 0, 12 );	// mrc:	rc
			//		Record( p, stmt_NT, c + 0, 17 );	// stmt: reg
			//		break;
			//
			EmitRecord( rule, rule->ruleData->cost );
			EmitChainRulesOfRule( rule, rule->ruleData->cost );
			break;
			
		case 1:
			Emit( "\t\tc = 0;\n" );
			Emit( "\t\tif ( " ); EmitTestForPattern( rule->pattern, "", true ); Emit( " ) {\n" );
			Emit( "\t\t\tc = " ); EmitCostForPattern( rule->pattern, "" ); Emit( ";\n" );
			g_indent = 3;
			EmitRecord( rule, rule->ruleData->cost );
			EmitChainRulesOfRule( rule, rule->ruleData->cost );
			g_indent = 2;
			Emit( "\t\t}\n" );
			break;

		case 2:
			Emit( "\t\tif ( " ); EmitTestForPattern( rule->pattern, "", true ); Emit( " ) {\n" );
			Emit( "\t\t\tc = " ); EmitCostForPattern( rule->pattern, "" ); Emit( ";\n" );
			g_indent = 3;
			EmitRecord( rule, rule->ruleData->cost );
			EmitChainRulesOfRule( rule, rule->ruleData->cost );
			g_indent = 2;
			Emit( "\t\t}\n" );
			break;

		default:
			yyerror( "invalid operator '%s' arity: %d\n", term->name, term->arity );
		}
	}
	Emit( "\t\tbreak;\n\n" );
}
//------------------------------------------------------------------------------------------
static void  EmitLabellingFunction() {
	Emit(
		"state_s *  Machine_x86::_GetState( IRCode * insn, state_s* left, state_s* right ) {\n"
		"	int  c = 0;  // cost\n\n"
		"	state_s * p = new state_s( insn->GetOpCode(), left, right );\n\n"
		"	switch ( insn->GetOpCode() ) {\n"
	);
	{
		Terminal * p;
		for ( p = g_terminals; p != NULL; p = p->next ) {
			EmitCaseInGetStateFunction( p );
		}
	}
	Emit(
		"	default:\n"
		"		c4::common->Error( \"Machine_x86::GetState() : unknown opcode: '%%s'.\\n\", IRCode::GetOpcodeName( insn->GetOpCode() ) );\n"
		"	}\n"
		"	return p;\n"
		"}\n\n"
	);
}
//------------------------------------------------------------------------------------------
static void  Indent() {
	int i;
	for ( i = g_indent; i > 0; i-- ) {
		Emit( "\t" );
	}
}
//------------------------------------------------------------------------------------------
static void  WriteRuleAsText( Rule * rule )
{
	Tree * pattern = rule->pattern;
	Tree * left = pattern->left;
	Tree * right = pattern->right;

	fprintf( g_outputFile, "%s : ", rule->lhs_nonterm->name );

	if ( pattern->kind == E_TERMINAL )
	{
		Terminal * term = pattern->term;
		switch ( term->arity ) {
		case 0:
			fprintf( g_outputFile, "%s", term->name );
			break;

		case 1:
			fprintf( g_outputFile, "%s", term->name );
			Emit( "( " );
			fprintf( g_outputFile, "%s", TREE_NAME( left ) );
			Emit( " )" );
			break;

		case 2:
			fprintf( g_outputFile, "%s", term->name );
			Emit( "( " );
			fprintf( g_outputFile, "%s", TREE_NAME( left ) );
			Emit( ", " );
			fprintf( g_outputFile, "%s", TREE_NAME( right ) );
			Emit( " )" );
			break;

		default:
			yyerror( "invalid arity '%d' for terminal '%s'\n", term->arity, term->name );
		}
	}
	else {
		fprintf( g_outputFile, "%s", pattern->nonterm->name );
	}
}
//------------------------------------------------------------------------------------------
static void  EmitRules() {
	Rule * rule = g_rules;
	Emit(
		"\n// rules ( for debugging )\n"
		"//\n"
		"const char * g_rules[ NUM_RULES + 1 ] = {\n"
		"/* 0 */		NULL,\n"
	);
	while ( rule != NULL )
	{
		fprintf( g_outputFile, "/* %d */ \"", rule->number );
		WriteRuleAsText( rule );			
		Emit( "\",\n" );
		rule = rule->link;
	}
	Emit( "};\n\n" );
}
//------------------------------------------------------------------------------------------
// example: tree is "ADD( mem, rc )"
// nonterms are: "mem, rc".
//
static void EmitNontermsOfTree( Tree * tree ) {
	if ( tree )
	{
		if ( tree->kind == E_NONTERMINAL ) 
		{
			fprintf( g_outputFile, "%s%s", tree->nonterm->name, g_suffix );
		} 
		else {
			if ( tree->left == NULL && tree->right == NULL )
			{
				Emit( "0" );
				return;
			}
			EmitNontermsOfTree( tree->left );
			if ( tree->right ) {
				Emit( ", " );
				EmitNontermsOfTree( tree->right );
			}
		}
	}
}
//------------------------------------------------------------------------------------------
static void  EmitNonterminals()
{
	Emit( "// nonterminals on rhs of rules\n//\n" );
	{
		Rule * rule = g_rules;
		while ( rule )
		{
			fprintf( g_outputFile, "const int _nonterms_%d[] = { ", rule->number );
			EmitNontermsOfTree( rule->pattern );
			Emit( ", 0 };\n" );

			rule = rule->link;
		}
	}
	Emit( "\n" );

	Emit( "// indices into the jagged array above\n//\n" );
	Emit( "const int * g_nonterms[ NUM_RULES + 1 ] = {\n" );
	Emit( "/* 0 */\t\t{ 0 },\n" );
	{
		Rule * rule = g_rules;
		while ( rule )
		{
			fprintf( g_outputFile, "/* %d */ ", rule->number );
			fprintf( g_outputFile, "_nonterms_%d,", rule->number );

			Emit( "\t\t/* " );
			WriteRule( rule, false );
			Emit( " */\n" );

			rule = rule->link;
		}
	}
	Emit( "};\n\n" );
}
//------------------------------------------------------------------------------------------
// example:
//[ add : r32
//	( add( mul( r32, iconst_2_4_8 ), r32 ) )
//	( imm )
//	"lea %c, [%2 + %0 * %1 + %3]"
//	<2>
//	// ...
//	kids[0] = irTree->op1->op1->op1;	// r32
//	kids[1] = irTree->op1->op1->op2;	// iconst_2_4_8
//	kids[2] = irTree->op1->op2;			// r32
//	kids[3] = irTree->op2;				// imm
//
static void  EmitOneCaseInGetKidsFunction( Tree *tree, char *prevText, int *kid_number ) {
	char	tmp[ MAX_STR_LENGTH ];
	strcpy( tmp, prevText );

	if ( tree->kind == E_TERMINAL ) {
		if ( tree->left ) {
			char	tmp2[ MAX_STR_LENGTH ];
			strcpy( tmp2, tmp );
			
			//Append( tmp2, "->op1" );
			Append( tmp2, "->GetOp1()" );
			
			EmitOneCaseInGetKidsFunction( tree->left, tmp2, kid_number );
		}
		if ( tree->right ) {
			char	tmp2[ MAX_STR_LENGTH ];
			strcpy( tmp2, tmp );
			
			//Append( tmp2, "->op2" );
			Append( tmp2, "->GetOp2()" );

			EmitOneCaseInGetKidsFunction( tree->right, tmp2, kid_number );
		}
	} else {
		Indent();
		fprintf( g_outputFile, "kids[%d] = irTree%s;\n", *kid_number, tmp );
		++*kid_number;
	}
}
//------------------------------------------------------------------------------------------
static void  EmitKids( Rule *rules ) {
	Emit(
		"// stores the kids of the given tree into the array kids[]\n"
		"void Machine_x86::_GetKids( IRCode * irTree, int rulenum, IRCode * kids[], int *numKids )\n"
		"{\n"
		"	if ( ! irTree ) {\n"
		"		c4::common->Error( \"Machine_x86::_GetKids() : null tree!\\n\" );\n"
		"	}\n"
		"	if ( ! kids ) {\n"
		"		c4::common->Error( \"Machine_x86::_GetKids() : null kids!\\n\" );\n"
		"	}\n"
		"	switch ( rulenum ) {\n"
		"	case 0: return;\n"
	);
	{
		Rule * rule = g_rules;
		g_indent = 2;
		while ( rule )
		{
			//fprintf( g_outputFile, "/* %d */ ", rule->number );
			int kid_number = 0;
			fprintf( g_outputFile, "\tcase %d:\t/* ", rule->number );
			WriteRuleAsText( rule );	Emit( " */\n" );
			
			EmitOneCaseInGetKidsFunction( rule->pattern, "", &kid_number );
			fprintf( g_outputFile, "\t\tkids[%d] = NULL;\n", kid_number );
			fprintf( g_outputFile, "\t\t*numKids = %d;\n", kid_number );

			Emit( "\t\tbreak;\n\n" );
			rule = rule->link;
		}
	}
	//Emit( "\tdefault: Unreachable();\n\t}\n}\n\n" );
	Emit( "\tdefault: printf( \"Invalid rule number: %%d\\n\", rulenum );\n\t}\n}\n\n" );
	g_indent = 0;
}
//------------------------------------------------------------------------------------------
static void  EmitCodeGenerationFunctions()
{
	Emit( "\n#include \"x86_cg_inc.h\"\n" );
}
//------------------------------------------------------------------------------------------
static void  Error( char *format, ... )
{
	va_list	 argptr;
	char	buffer[ MAX_STR_LENGTH ];
	
	va_start( argptr, format );
	vsprintf( buffer, format, argptr );
	va_end( argptr );

	printf( "Error: %s\n", buffer );
	exit( -1 );
}
//------------------------------------------------------------------------------------------
static void  EmitConstructors() {
	Rule * rule = g_rules;
	Emit(
		"void Machine_x86::_ExecuteConstructor( IRCode * insn, int rulenum, Operand *& node, Operand ** pOperands ) {\n"
		"	switch ( rulenum ) {\n"
		"	case 0:  // no action\n"
		"		break;\n\n"
	);
	while ( rule ) 
	{
		if ( rule->ruleData->constructor != NULL )
		{
			char * p;
			fprintf( g_outputFile, "\tcase %d:\t", rule->number );
			Emit( "/* " ); WriteRule( rule, false ); Emit( " */\n\t\t" );
			
			p = rule->ruleData->constructor;
			while ( *p )
			{
				if ( *p == '$' ) {
					const char n = *(p + 1);
					if ( n >= '0' && n <= '9' ) {
						fprintf( g_outputFile, "pOperands[%c]", n );
						p += 2;
						continue;
					}
					else if ( n == '$' )
					{
						Emit( "node" );
						p += 2;
						continue;
					}
					else {
						yyerror( "invalid number '%d' after '$' in instruction definition\n", *(p + 1) );
					}
				} else {
					fprintf( g_outputFile, "%c", *p );
				}
				++p;
			}//while( *p )
			Emit( "\n\t\tbreak;\n" );
		}
		
		rule = rule->link;

	}//while ( rule )
	Emit( 
		"\t}\n"
		"}\n"
	);
}
//------------------------------------------------------------------------------------------
// emit the method Machine_x86::_ExecuteAction
//
static void  EmitSemanticActions() {
	Emit(
		"void Machine_x86::_ExecuteAction( IRCode * insn, int rulenum, Operand *& node, Operand ** operands )\n"
		"{\n"
		"\tswitch ( rulenum ) {\n"
		"\tcase 0:  // no action\n"
		"\t\tbreak;\n\n"
	);
	// emit cases:
	{
		Rule * rule = g_rules;
		while ( rule ) 
		{
			if ( rule->ruleData->semantic_action != NULL )
			{
				fprintf( g_outputFile, "\tcase %d:\t", rule->number );
				Emit( "/* " ); WriteRule( rule, false ); Emit( " */\n\t\t" );
				{		
					char * p = rule->ruleData->semantic_action;
					while ( *p )
					{
						if ( *p == '$' ) {
							const char n = *(p + 1);
							if ( n >= '0' && n <= '9' ) {
								//fprintf( g_outputFile, "node->GetOperand(%c)", n );								
								fprintf( g_outputFile, "operands[%c]", n );
								p += 2;
								continue;
							}
							else if ( n == '$' )
							{
								Emit( "node" );
								p += 2;
								continue;
							}
							else {
								yyerror( "invalid number '%d' after '$' in instruction definition\n", *(p + 1) );
							}
						} else {
							fprintf( g_outputFile, "%c", *p );
						}
						++p;
					}//while( *p )
				}
				Emit( "\n\t\tbreak;\n\n" );
			}
			rule = rule->link;
		}//while ( rule )
	}//end emit cases
	Emit( 
		"\t}\n"
		"}\n"
	);
}
//------------------------------------------------------------------------------------------
static void  EmitDestructors()
{
	Rule * rule = g_rules;
	Emit(
		"void Machine_x86::_ExecuteDestructor( IRCode * insn, int rulenum, Operand *& node ) {\n"
		"	switch ( rulenum ) {\n"
		"	case 0:  // no action\n"
		"		break;\n\n"
	);
	while ( rule ) 
	{
		if ( rule->ruleData->destructor != NULL )
		{
			char * p;
			fprintf( g_outputFile, "\tcase %d:\t", rule->number );
			Emit( "/* " ); WriteRule( rule, false ); Emit( " */\n\t\t" );
			
			p = rule->ruleData->destructor;
			while ( *p )
			{
				if ( *p == '$' ) {
					const char n = *(p + 1);
					if ( n >= '0' && n <= '9' ) {
						fprintf( g_outputFile, "node->GetOperand(%c)", n );
						p += 2;
						continue;
					}
					else if ( n == '$' )
					{
						Emit( "node" );
						p += 2;
						continue;
					}
					else {
						yyerror( "invalid number '%d' after '$' in instruction definition\n", *(p + 1) );
					}
				} else {
					fprintf( g_outputFile, "%c", *p );
				}
				++p;
			}//while( *p )
			Emit( "\n\t\tbreak;\n" );
		}

		rule = rule->link;

	}//while ( rule )
	Emit( 
		"\t}\n"
		"}\n"
	);
}
//------------------------------------------------------------------------------------------
static void  EmitOpType( EOperandType  type ) {
	switch ( type ) {
	case eDefault:
		Emit( "eDefault" );
		break;

	case eReg_8:
		Emit( "eReg_8" );
		break;

	case eReg_16:
		Emit( "eReg_16" );
		break;

	case eReg_32:
		Emit( "eReg_32" );
		break;

	case eRegister:
		Emit( "eRegister" );
		break;

	case eResultInOperand:
		Emit( "eResultInOperand" );
		break;

	default:
		printf( "EmitOpType() : type is '%d'\n", type );
		Unreachable();
	}
}
//------------------------------------------------------------------------------------------
int ReadTokenIntoBuffer( char* buffer, char** pSrc ) {
	char c = '\0';
	char * p = buffer;
	*p = '\0';

	if ( *pSrc == NULL ) {
		return false;
	}
	if ( **pSrc == '\0' ) {
		return false;
	}
	while ( *pSrc && **pSrc != '\0' )
	{
		switch ( **pSrc ) {
			case '{' :
			case '}' :

			case ' ' :			
			case '\t' :
			case '\n' :
				++*pSrc;
				continue;

			case ':' :
				++*pSrc;
				c = ':';
				goto L_DONE;

			case '\0' :
				*p = '\0';
				return 0;

			case ',' :
				//buffer[ 0 ] = '\0';
				++*pSrc;
				*p = '\0';
				c = buffer[ 0 ];
				return c;
				continue;
		}
		
		*p++ = *(*pSrc)++;
	}

	c = buffer[ 0 ];

L_DONE:
	*p = '\0';	
	return c;
}
//------------------------------------------------------------------------------------------
static bool StrEqual( const char *str1, const char *str2 ) {
	return strcmp( str1, str2 ) == 0;
}
//------------------------------------------------------------------------------------------
static void ParseSideEffects( char *side_effects, side_effects_t *outSideEffects ) 
{
	char *   pStr; // temp. pointer to 'side_effects'
	char	 buffer[ MAX_STR_LENGTH ];  // temp. buffer for holding strings
	
	char	c = '\0';

	outSideEffects->clobberedRegs = 0;
	outSideEffects->modifiedFlags = 0;

	if ( ! side_effects ) {
		return;
	}
	
	pStr = side_effects;
	
	while ( c = ReadTokenIntoBuffer( buffer, &pStr ) )
	{
		if ( StrEqual( buffer, "clobber" ) )
		{
			assert( c == ':' );
			while ( ReadTokenIntoBuffer( buffer, &pStr ) ) 
			{
				if ( StrEqual( buffer, "EAX" ) ) {
					outSideEffects->clobberedRegs |= BIT( rEAX );
				}
				else if ( StrEqual( buffer, "EDX" ) ) {
					outSideEffects->clobberedRegs |= BIT( rEDX );
				}
				else if ( StrEqual( buffer, "ECX" ) ) {
					outSideEffects->clobberedRegs |= BIT( rECX );
				}
				// TODO: other cases
				else {
					assert( false );
				}
			}
			
			if ( ReadTokenIntoBuffer( buffer, &pStr )
				&& StrEqual( buffer, ";" ) )
			{
				continue;
			}
		}

		if ( StrEqual( buffer, "set" ) )
		{
			assert( c == ':' );
			
			//ReadTokenIntoBuffer( buffer, &pStr );
			while ( ReadTokenIntoBuffer( buffer, &pStr ) ) 
			{
				if ( StrEqual( buffer, "CF" ) ) {
					outSideEffects->modifiedFlags |= CF;
				}
				else if ( StrEqual( buffer, "OF" ) ) {
					outSideEffects->modifiedFlags |= OF;
				}
				else if ( StrEqual( buffer, "SF" ) ) {
					outSideEffects->modifiedFlags |= SF;
				}
				else if ( StrEqual( buffer, "ZF" ) ) {
					outSideEffects->modifiedFlags |= ZF;
				}
				else if ( StrEqual( buffer, "AF" ) ) {
					outSideEffects->modifiedFlags |= AF;
				}
				else if ( StrEqual( buffer, "PF" ) ) {
					outSideEffects->modifiedFlags |= PF;
				}
				else if ( StrEqual( buffer, "DF" ) ) {
					outSideEffects->modifiedFlags |= DF;
				}

				else if ( StrEqual( buffer, "FLAGS" ) ) {
					outSideEffects->modifiedFlags |= (CF | PF | AF | ZF | SF | TF | IF | DF | OF);
				}
				// TODO: other cases
				else {
					assert( false );
				}
			}
		}
		//printf( "unknown side effect: %s\n", buffer );
	}
}
//------------------------------------------------------------------------------------------
static void ParseAttributes( char *attributes, attributes_t *outAttributes ) {
	char *   pAttributes; // temp. pointer to 'attributes'
	char	 buffer[ MAX_STR_LENGTH ];  // temp. buffer for holding strings
	
	int   iResult = 0;
	int   iData = 0;

	if ( ! attributes ) {
		outAttributes->flags = 0;
		outAttributes->data = 0;
		return;
	}
	
	pAttributes = attributes;
	
	while ( ReadTokenIntoBuffer( buffer, &pAttributes ) )
	{
		// TODO: make some sort of hash table
		if ( strcmp( buffer, "\"emit_the_most_complex_nodes_first\"" ) == 0 )
		{
			iResult |= EMIT_COMPLEX_CHILDREN_FIRST;
			continue;
		}
		if ( strcmp( buffer, "\"do_not_emit_children\"" ) == 0 )
		{
			iResult |= DO_NOT_EMIT_CHILDREN;
			continue;
		}
		if ( strcmp( buffer, "\"result_can_be_%0\"" ) == 0 )
		{
			//iResult |= RESULT_LOCATIONS_SPECIFIED;
			//iData |= ...
			continue;
		}
		if ( strcmp( buffer, "\"ensure_operands_have_equal_size\"" ) == 0 )
		{
			iResult |= OPERANDS_MUST_HAVE_EQUAL_SIZE;
			continue;
		}

		printf( "unknown instruction attribute: %s\n", buffer );
	}

	outAttributes->flags = iResult;
	outAttributes->data = iData;
}
//------------------------------------------------------------------------------------------
static void EmitOperandInfo( operand_info_t * pInfo )
{
	Emit( "{ " );
#if 0
	EmitOpType( pInfo->type );
#else
	fprintf( g_outputFile, "%d", pInfo->type );
#endif
	fprintf( g_outputFile, ", %d", pInfo->data );
	Emit( " }" );
}
//------------------------------------------------------------------------------------------
static void  EmitInstructionData() {
	Rule * rule = g_rules;
	Emit(
	"const InstructionData  g_instruction_data[ NUM_RULES + 1 ] = {\n"
	"/* 0 */  { 0 },\n"
	);
	// emit array of structures
	while ( rule )
	{
		RuleData * ruleData = rule->ruleData;
		fprintf( g_outputFile, "/* %d */  { ", rule->number );

		//----------------------------------
		// 0) emit name
		fprintf( g_outputFile, "\"%s\", ", ruleData->name );

		// 1) emit flags
		fprintf( g_outputFile, "%d, ", ruleData->flags );

		// 2) emit result_info_s:
		EmitOperandInfo( & ruleData->result );

		Emit( ", " );
		
		// 3) emit operand_info_s
		Emit( "{ " );
		{
			int i;
			for ( i = 0; i < MAX_NUM_OPERANDS; i++ )
			{
				EmitOperandInfo( & ruleData->operands[i] );
				if ( i < MAX_NUM_OPERANDS-1 ) {
					Emit( ", " );
				}
			}
		}
		Emit( " }, " );

		// ?) emit operand constraints

		// ?) emit side effects
		Emit( "{ " );
		{
			side_effects_t	side_effects;
			ParseSideEffects( ruleData->side_effects, &side_effects );
			
			fprintf( g_outputFile, "%d", side_effects.clobberedRegs );
			fprintf( g_outputFile, ", %d", side_effects.modifiedFlags );
		}
		Emit( " }, " );//END side effects

		// ?) emit attributes
		Emit( "{ " );
		{
			attributes_t   tempAttributes;
			ParseAttributes( ruleData->attributes, &tempAttributes );
			fprintf( g_outputFile, "%d, %d", tempAttributes.flags, tempAttributes.data );
		}
		Emit( " }" );//END emit attributes

		

		fprintf( g_outputFile, ", %d", ruleData->numOperandsInTemplate );

		//------------------------------------
		fprintf( g_outputFile, " },\n" );
		// end emitting instruction data

		rule = rule->link;

	}//while ( rule != NULL )
	Emit( "};\n\n" );
}
