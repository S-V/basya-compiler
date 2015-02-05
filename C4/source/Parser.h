#ifndef __PARSER_H__
#define __PARSER_H__

namespace c4 {

/*
==========================================================================
	class Parser

	is a handcrafted recursive-descent parser;
	builds a tree during parsing.
==========================================================================
*/

class Parser : public Lexer {
public:
					Parser( const char * source = 0 );
					~Parser();

	bool			Reset( const char * source );

	AST_Node *		Parse();

	int			GetNumErrors() const;
	int			GetNumWarnings() const;

	//------------------------------------------

	enum PARSER_FLAGS {
		MAX_NUM_OF_ERRORS			= 128,
		MAX_BRACKET_NEST_LEVEL		= 1024	// []
	};

	//******************************************

	CompilationUnit *		ParseCompilationUnit();

	Declaration *			ParseDeclaration();

	NamespaceDecl *			ParseNamespace();

	enum PARSE_AGGREGATE_FLAGS {
		ALLOW_CONSTRUCTORS	= BIT( 0 ),
		ALLOW_DESTRUCTORS	= BIT( 1 ),
		ALLOW_INDEXERS		= BIT( 2 ),

		ALLOW_ALL_MEMBERS	=	ALLOW_CONSTRUCTORS | 
								ALLOW_DESTRUCTORS  |	
								ALLOW_INDEXERS
	};
							// parses declarations enclosed in curly braces '{'...'}'
	DeclarationList *		ParseAggregateBody( int flags = ALLOW_ALL_MEMBERS );

	ClassDecl *				ParseClass();
	DeclarationList *		ParseClassBody();
	DeclarationList *		ParseInterfaceBody();

	//UnionDecl *				ParseUnion();

	EnumDecl *				ParseEnum();
	DeclarationList *		ParseEnumBody();

	ArgumentList *			ParseFunctionArguments();

							// returns Error_Type in case of error
	Type *					ParseType();

							// examples: int, void, SomeType.MemberType, X, float, etc..
	Type *					ParseBasicType();

							// parses '^', [], etc.
							// examples: void^, X[5]^, float[67], etc..
	Type *					ParseBasicType2( Type * prevType );

							// for parsing variable declarations
							// example: int x = 0, y(8), z=1; => declarators: int x = 0; int y(8); int z = 1
	DeclarationList *		ParseVarDecls( Type* type, STORAGE_CLASS stc = SC_STATIC );

	//---- statements ---------------------------------------------------------------

							// '{' stmt1, stmt2, ..., stmtN '}'
	Block *					ParseBlockStatement();

							// never returns NULL
	StatementList *			ParseStatementList();

	ForLoopStatement *		ParseForStatement();

	// these control how ParseStatement() works
	enum PARSE_STATEMENT_FLAGS
	{
		ALLOW_EMPTY_STATEMENT	= BIT( 0 ),	// empty ';' statements are allowed
		EXPECT_SCOPE_STATEMENT	= BIT( 1 ),	// start a new scope
		ALLOW_DECLARATIONS		= BIT( 3 ),
		ALLOW_BLOCK_STATEMENT	= BIT( 4 ),

		ALLOW_ANY_STATEMENT		= ALLOW_EMPTY_STATEMENT |
								  ALLOW_BLOCK_STATEMENT | 
								  ALLOW_DECLARATIONS
	};

						// returns NULL if no statements were parsed;
						// 'flags' must be a member of the enum PARSE_STATEMENT_FLAGS
	Statement *			ParseStatement( int flags = ALLOW_ANY_STATEMENT );

						// never returns NULL, will return some erroneous statement in case of error
	Statement *			ExpectStatement();


	//---- expressions ---------------------------------------------------------------
	
	ExpressionList *	ParseExpressionList();
						// '(' param1, param2, ..., paramN ')'
	ExpressionList *	ParseArguments();
						// E, E, ..., E
	Expression *		ParseCommaExp();

						// never returns NULL; will return some error expression
	Expression *		ParseExpression();
	Expression *		ExpectExpression();

	Expression *		ParseAssignment();


						// ?: .
	Expression *		ParseConditional();

						// || .
	Expression *		ParseLogicalOr();
						// && .
	Expression *		ParseLogicalAnd();

						// E | E
	Expression *		ParseBitOr();
						// E ^ E
	Expression *		ParseBitXor();
						// E & E
	Expression *		ParseBitAnd();

						// ==, !=
	Expression *		ParseEquality();

						// >, >=, <, <=, E in E .
	Expression *		ParseRelational();

						// <<, >>, >>>
	Expression *		ParseShifts();

						// +, -
	Expression *		ParseTerms();
						// *, /
	Expression *		ParseFactors();

	Expression *		ParseNewExp();
	Expression *		ParseDeleteExp();
	
						// E++, E--
						// E[ expr ], E ( params )
						// E '->' e
						// E is e, E as e, E in e
	Expression *		ParsePostfixExp();

						// @E, &E
						// +E, -E, ++E, --E, !E, cast(E)E
	Expression *		ParseUnaryExp();

						// E^, E.E. ... E
	Expression *		ParsePrimaryExp();
	
						// id,  :: id, id^, id^.id
						// ( E ), literal, assert ( E )
	Expression *		ParseAtom();

	//***************************************

	bool			IsStartOfExpression( int tokenType ) const;

					// returns true if if the scanner is sitting on the start of a declaration
	bool			StartOfDeclaration();

	Identifier *	ExpectIdentifier( const char *message = 0 );
	void			ReadOptional( int token );
	void			Expect( int token );
	void			Check( int token );
	void			SetLocation( AST_Node * node ) const;

protected:
	void		Print( const char* format, ... );
	void		Warning( const char *message, ... );
	void		Error( const char *message, ... );

private:
	int			m_numErrors;
	int			m_numWarnings;

	int			m_braceCount;	// {}
	int			m_inBrackets;	// bracket counter ( >0 inside [] of array index )
	int			m_parenthCount; // ()

private:
	enum { BUFFER_SIZE = 512 };
	char		m_tmp_buf[ BUFFER_SIZE ];
};

}//end namespace c4 

#endif // !__PARSER_H__

