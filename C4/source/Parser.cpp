/*
======================================
	Recursive-descent parser for C4,
	implementation file.
======================================
*/
#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4
{

// undefine this if it causes parse problems
#define	ALLOW_C_STYLE_POINTERS		// allow such things as E '->' m, *E, &E, etc.


Parser::Parser( const char * source /* =0 */ )
	: m_numErrors( 0 )
	, m_numWarnings( 0 )
	, m_braceCount( 0 )
	, m_inBrackets( 0 )
	, m_parenthCount( 0 )
{
	if ( source ) {
		Reset( source );
	}
}

Parser::~Parser() {
}

bool Parser::Reset( const char *source ) {
	Lexer::Reset( source );

	m_numErrors = 0;
	m_numWarnings = 0;

	m_braceCount = 0;
	m_inBrackets = 0;
	m_parenthCount = 0;

	return true;
}

int Parser::GetNumErrors() const {
	return m_numErrors;
}

int Parser::GetNumWarnings() const {
	return m_numWarnings;
}

AST_Node * Parser::Parse()
{
	c4::common->Print( "Parsing..." );

	AST_Node * rootNode = ParseCompilationUnit();

	if ( rootNode ) {
		c4::common->Print( "--------------- Parsing succeded! --------------------------------" );
	} else {
		c4::common->Print( "--------------- Parsed with errors. ------------------------------" );
	}

	return rootNode;
}

//******************************************

CompilationUnit * Parser::ParseCompilationUnit() 
{
	CompilationUnit * program = new CompilationUnit( "program" );

	while ( T_END_OF_FILE != PeekTokenType() )
	{
		if ( StartOfDeclaration() ) 
		{
			if ( Declaration *	decl = ParseDeclaration() ) {
				program->Add( decl );
			}
			continue;
		}

		break;
	}
	
	return program;
}

/*
====================
	Parser::ParseDeclaration()

	assumes that the parser is at the start of a declaration
====================
*/
Declaration * Parser::ParseDeclaration()
{
	Token * token = PeekToken();

	switch ( token->Value() )
	{
	case T_NAMESPACE:
		return ParseNamespace();

	case T_ENUM:
		return ParseEnum();

	case T_CLASS:
		return ParseClass();
/*
	case T_AUTO:
		ReadToken();
		Identifier * var = ExpectIdentifier();
		Expect( P_EQUAL );
		Expression * e = ExpectExpression();
		Expect( P_SEMICOLON );
		return new AutoDeclaration( var, e );
*/
	}

	//******************************************
	Declaration *	result = 0;
	STORAGE_CLASS	storageClass = SC_UNDEFINED;
	LINKAGE			linkage		= DEFAULT_LINKAGE;

	// temp. var to find redundant storage class declarations
	STORAGE_CLASS		stc; 
	//******************************************

	while ( true ) 
	{
		switch ( token->Value() ) 
		{
			case T_CONST:
				ReadToken();
				if ( P_LEFT_PARENTH == PeekTokenType() ) 
				{
					goto L_parse_type;
				}
				stc = SC_CONST;
				goto L_check_storage_class;

			case T_STATIC:
				ReadToken();
				stc = SC_STATIC;
				goto L_check_storage_class;

			case T_FINAL:
				ReadToken();
				stc = SC_FINAL;
				goto L_check_storage_class;

			case T_AUTO:
				ReadToken();
				stc = SC_AUTO;
				goto L_check_storage_class;

			case T_SCOPE:
				ReadToken();
				stc = SC_SCOPE;
				goto L_check_storage_class;

			case T_VIRTUAL:
				ReadToken();
				stc = SC_VIRTUAL;
				goto L_check_storage_class;

			case T_OVERRIDE:
				ReadToken();
				stc = SC_OVERRIDE;
				goto L_check_storage_class;

			case T_ABSTRACT:
				ReadToken();
				stc = SC_ABSTRACT;
				goto L_check_storage_class;

			case T_SYNCHRONIZED:
				ReadToken();
				stc = SC_SYNCHRONIZED;
				goto L_check_storage_class;

			case T_DEPRECATED:
				ReadToken();
				stc = SC_DEPRECATED;
				goto L_check_storage_class;

			case T_NOTHROW:
				ReadToken();
				stc = SC_NOTHROW;
				goto L_check_storage_class;

			case T_PURE:
				ReadToken();
				stc = SC_PURE;
				goto L_check_storage_class;

			case T_ENUM:
				ReadToken();
				stc = SC_MANIFEST;
				goto L_check_storage_class;

			case T_EXTERN:
				ReadToken();
				stc = SC_EXTERN;
				//goto L_check_storage_class;

L_check_storage_class:
				if ( storageClass & stc ) {
					Error( "redundant storage class '%s'", Text() );
				}
				storageClass = ( STORAGE_CLASS )(storageClass | stc);

				// TODO: find conflicting storage classes

				token = PeekToken();
				continue;

			default:
				break;
		}//switch curr. token

		break;
	}//while true

	//***************************************************************

	// TODO: parsing aggregates
	//token = PeekToken();

	//***************************************************************

L_parse_type:

	// every declaration begins with a type
	Type * type = ParseType();

	token = PeekToken();  // token should be an identifier - the name of the thing being declared
	
	if ( token->Value() == L_IDENTIFIER )
	{
		token = PeekNextToken( token );

		// if '(' follows then it's a function declaration
		if ( token->Value() == P_LEFT_PARENTH ) 
		{
			Identifier *	id = ExpectIdentifier( "name of function" );	// function name
			ArgumentList *	arguments = ParseFunctionArguments();
			Statement *		body = ParseBlockStatement();

			result = new FunctionDecl( type, id, arguments, body );
		}
		else {
			// variable declaration
			result = ParseVarDecls( type, storageClass );
		}
	}
	else {
		//Error( "identifier is expected in declaration" );
		Error( "type is expected in declaration" );
		ReadToken();
	}

	//********************
	if ( result ) {
		result->SetStorageClass( storageClass );
	}
	
	//********************
	//result->UpdateUIinfo();

	return result;
}

NamespaceDecl * Parser::ParseNamespace() {
	Expect( T_NAMESPACE );
	
	Identifier * name = NULL;

	int token = PeekTokenType();
	if ( L_IDENTIFIER == token ) {
		name = ExpectIdentifier( "name of namespace" );
	}

	NamespaceDecl * namespaceDecl = new NamespaceDecl( name );

	// TODO: replace the code fragment below with ParseBracedDeclarations().

	Expect( P_LEFT_BRACE );

	while ( true )
	{
		token = PeekTokenType();
		if ( T_END_OF_FILE == token || P_RIGHT_BRACE == token || !StartOfDeclaration() ) {
			break;
		}
		if ( Declaration *	decl = ParseDeclaration() ) {
			namespaceDecl->Add( decl );
		}
		continue;
	}

	Expect( P_RIGHT_BRACE );

	return namespaceDecl;
}

ClassDecl * Parser::ParseClass() {
	Expect( T_CLASS );

	Identifier * name = ExpectIdentifier( "name of class" );

	DeclarationList * classBody = ParseAggregateBody();

	ClassDecl * classDecl = new ClassDecl( name, classBody, GetLocation() );

	return classDecl;
}

// TODO:
DeclarationList * Parser::ParseClassBody()
{
	return ParseAggregateBody();
}

EnumDecl * Parser::ParseEnum() {
	Expect( T_ENUM );

	Identifier * name = NULL;

	int token = PeekTokenType();
	if ( L_IDENTIFIER == token ) {
		name = ExpectIdentifier( "name of enum" );
	}

	EnumDecl * enumDecl = new EnumDecl( name, GetLocation() );
	
	DeclarationList * enumBody = ParseAggregateBody();

	enumDecl->SetBody( enumBody );

	return enumDecl;
}

//--------------------------------------------------------------------
// Parses '{' declarations '}'
//  or	 '{' declarations '}' ';'
//--------------------------------------------------------------------
DeclarationList * Parser::ParseAggregateBody( int flags /* =ALLOW_ALL_MEMBERS */ )
{
	DeclarationList * declList = new DeclarationList();

	Expect( P_LEFT_BRACE );

	PROTECTION_LEVEL   protection = PL_PUBLIC;

	while ( true )
	{
		int token = PeekTokenType();
		if ( T_END_OF_FILE == token || P_RIGHT_BRACE == token ) {
			break;
		}

		switch ( token ) {
		// parse access modifiers
			case T_PRIVATE:
				ReadToken();	ReadOptional( P_COLON );
				protection = PL_PRIVATE;
				continue;

			case T_PACKAGE:
				ReadToken();	ReadOptional( P_COLON );
				protection = PL_PACKAGE;
				continue;

			case T_PROTECTED:
				ReadToken();	ReadOptional( P_COLON );
				protection = PL_PROTECTED;
				continue;

			case T_PUBLIC:
				ReadToken();	ReadOptional( P_COLON );
				protection = PL_PUBLIC;
				continue;

			case T_EXPORT:
				ReadToken();	ReadOptional( P_COLON );
				protection = PL_EXPORT;
				continue;

			case T_INTERNAL:
				ReadToken();	ReadOptional( P_COLON );
				protection = PL_INTERNAL;
				continue;
			
			default:
				/* nothing */
				;
		
		}//switch

		token = PeekTokenType();
		if ( P_RIGHT_BRACE == token ) {
			break;
		}

		// constructors 
		if ( flags & ALLOW_CONSTRUCTORS ) {
			// ctor -> 'this' '(' parameters ')'
			if ( T_THIS == token ) {
				ReadToken();
				ArgumentList *	arguments = ParseFunctionArguments();
				Statement *		body = ParseBlockStatement();

				declList->Add( new CtorDecl( arguments, body ) );
				continue;
			}
		}

		// destructors
		if ( flags & ALLOW_DESTRUCTORS ) {
			// dtor -> ~this()
			if ( P_TILDE == token ) { 
				ReadToken();
				Expect( T_THIS );
				Expect( P_LEFT_PARENTH );
				Expect( P_RIGHT_PARENTH );
				Statement *		body = ParseBlockStatement();

				declList->Add( new DtorDecl( body ) );
				continue;
			}
		}

		Declaration *	decl = ParseDeclaration();
		if ( decl ) {
			decl->SetProtection( protection );
			declList->Add( decl );
			continue;
		}

		break;
	}//while ( true )

	Expect( P_RIGHT_BRACE );

	ReadOptional( P_SEMICOLON );	// check for optional semicolon

	return declList;
}

//--------------------------------------------------------------------
// Parses '(' arguments ')'
// For example: ( int a, int b, const int^ p = 0 )
//--------------------------------------------------------------------
ArgumentList * Parser::ParseFunctionArguments() {
	Expect( P_LEFT_PARENTH );

	ArgumentList * arguments = new ArgumentList();

	while ( true ) 
	{
		int token = PeekTokenType();
		
		if ( P_RIGHT_PARENTH == token ) {
			break;
		}

		Type *	type = ParseType();
		Identifier * name = ExpectIdentifier( "type" );
		Expression * initializer = 0;

		// look for default initializer
		token = PeekTokenType();

		if ( P_EQUAL == token ) {
			ReadToken();
			initializer = ExpectExpression();
		}

		Argument * a = new Argument( type, name, initializer );
		arguments->Add( a );

		token = PeekTokenType();
		if ( P_COMMA == token ) {
			ReadToken();
			continue;
		}

		break;
	}

	Expect( P_RIGHT_PARENTH );

	return arguments;
}

//--------------------------------------------------------------------
// Parses 
//  var1, var2, ..., varN ;  or
//	var = expr, var2( ctor ), var3, ..., varN = initExpr ;
// until the closing token ( other than ',', usually ';' ) is found.
// (Assumed that we've read ',' or type.)
//--------------------------------------------------------------------
DeclarationList * Parser::ParseVarDecls( Type* type, STORAGE_CLASS stc /* =SC_STATIC */ )
{
	DeclarationList * declarators = new DeclarationList;
	Declaration *	var;

	while ( true )
	{
		Identifier * id = ExpectIdentifier( "type" );

		Expression * initializer = 0;

		int token = PeekTokenType();

		// if there is an initializer
		if ( P_EQUAL == token ) {
			ReadToken();
			initializer = ExpectExpression();
		}

		var = new VarDecl( type, id, initializer );
		var->SetStorageClass( stc );
		
		declarators->Add( var );
	
		// ',' or ';'
		token = PeekTokenType();

		if ( P_COMMA == token ) {
			ReadToken();
			continue;
		}
		
		Expect( P_SEMICOLON );
		break;

	}//while

	return declarators;
}

/*
==================================================================
	Parser::ParseType()

// Examples:
// int^^ is a pointer to a pointer to int
// const int^^ is a pointer to a pointer to const int
// const(int^)^ is a pointer to a const pointer to const int
// SomeType::MemberType^.AnotherType
==================================================================
*/
Type * Parser::ParseType() {
	Type *	result = 0;

	result = ParseBasicType();
	result = ParseBasicType2( result );

	return result;
}

//-------------------------------------------------
//	Parser::ParseBasicType
//
//	Never returns NULL; will return erroneous type
//	in case of error
//-------------------------------------------------
//
Type * Parser::ParseBasicType() {
	int		token;
	Type *				resultType;
	TypeQualified *		qualifiedId;

	// parse type modifiers
	int  typeModifiers = NO_MODIFIERS;

	while ( true ) {
		token = PeekTokenType();
		switch ( token ) {
		case T_CONST:
			ReadToken();
			typeModifiers |= MOD_CONST;
			
			if ( typeModifiers & MOD_CONST ) {
				Error( "redundant 'const'" );
			}
			continue;

		case T_UNSIGNED:
			ReadToken();
			typeModifiers |= UNSIGNED;
			
			if ( typeModifiers & SIGNED ) {
				Error( "signed/unsigned keywords mutually exclusive" );
			}
			continue;

		case T_SIGNED:
			ReadToken();
			typeModifiers |= SIGNED;
			if ( typeModifiers & UNSIGNED ) {
				Error( "signed/unsigned keywords mutually exclusive" );
			}
			continue;

		default:
			goto L_DONE;
		}
	}

L_DONE:

	token = PeekTokenType();
	switch ( token )
	{
		// primitive types

		case T_CHAR :
			ReadToken();
			resultType = new Type_Char();
			break;

		//case T_WCHAR:
		//case T_DCHAR:

		case T_BOOL :
			ReadToken();
			resultType = new Type_Bool();
			break;

		case T_INT8 :
		case T_BYTE :
			ReadToken();
			resultType = new Type_Int8();
			break;
		
		case T_UINT8 :
		case T_UBYTE :
			ReadToken();
			resultType = new Type_Int8();
			resultType->SetModifiers( UNSIGNED );
			break;

		case T_INT16:
			ReadToken();
			resultType = new Type_Int16();
			break;

		case T_SHORT:
			ReadToken();
			if ( PeekTokenType() == T_INT ) {
				ReadToken();
			}
			resultType = new Type_Int16();
			break;

		case T_UINT16:
			ReadToken();
			resultType = new Type_Int16();
			resultType->SetModifiers( UNSIGNED );
			break;

		case T_INT :
			ReadToken();
			resultType = new Type_Int32();
			break;

		case T_UINT :
			ReadToken();
			resultType = new Type_Int32();
			resultType->SetModifiers( UNSIGNED );
			break;

#pragma region Long integer
/* UNDONE: in my implementation, sizeof ( long int ) == 4
		case T_INT64:
		case T_LONG:
			ReadToken();
			resultType = new Type_Int64();
			break;

		case T_UINT64:
		case T_ULONG:
			ReadToken();
			resultType = new Type_Int64();
			resultType->SetModifiers( UNSIGNED );
			break;
*/
		case T_LONG :
#ifdef IMPLEMENTATION_WARNINGS
			::c4::compiler->Warning( "in my implementation, sizeof( long ) == 4" );
#endif
			ReadToken();
			if ( PeekTokenType() == T_LONG ) {
				::c4::compiler->Warning( "long long int is not implemented properly, currently, sizeof( long long ) == 4" );
				ReadToken();
			}
			if ( PeekTokenType() == T_INT ) {
				ReadToken();
			}
			resultType = new Type_Int32();
			break;

		case T_ULONG :
#ifdef IMPLEMENTATION_WARNINGS
			::c4::compiler->Warning( "in my implementation, sizeof( long ) == 4" );
#endif
			ReadToken();
			resultType = new Type_Int32();
			resultType->SetModifiers( UNSIGNED );
			break;

		case T_INT64:
		case T_UINT64:
			c4::compiler->Error( "64-bit integers ( int64, uint64, etc. ) are not implemented" );
			ReadToken();
			resultType = new Type_Error( "unimplemented type", Lexer::GetLocation() );
			break;

#pragma endregion


		case T_FLOAT :
			ReadToken();
			resultType = new Type_Float32();
			break;

		case T_DOUBLE :
		case T_FLOAT64 :
			ReadToken();
			resultType = new Type_Float64();
			break;

		case T_STRING :		Unimplemented();
			ReadToken();
			resultType = new Type_String();
			break;

		//---------- user-defined types ------------

		// some type ( Id.id.id )

		case T_VOID:
			ReadToken();
			resultType = new Type_Void();
			break;

		case L_IDENTIFIER:
			{
				Identifier * id = ExpectIdentifier();

				qualifiedId = new TypeQualified( id, GetLocation() );

				while ( true )
				{
					token = PeekTokenType();

					// id.id.id ... id
					// id::id::id ... id
					if ( token == P_DOT || token == P_COLON_COLON ) {
						ReadToken();

						Identifier * nextId = ExpectIdentifier();
						qualifiedId->Add( nextId );
						
						continue;
					}

					break;
				}//while

				resultType = qualifiedId;
			}
			break;

			// const ( type )
		case T_CONST:
			{
				ReadToken();
				Expect( P_LEFT_PARENTH );
				resultType = ParseType();
				Expect( P_RIGHT_PARENTH );

				resultType->MakeConst();
			}
			break;

		default:
			Error( "basic type expected, not %s", Text() );
			ReadToken();
			resultType = new Type_Error( "Parser::ParseBasicType(): type expected", GetLocation() );
			break;
	}

	SetLocation( resultType );

	resultType->SetModifiers( typeModifiers );

	return resultType;
}

//-------------------------------------------------
//	Parser::ParseBasicType2
//
//-------------------------------------------------
//
Type * Parser::ParseBasicType2( Type * prevType ) {
	Type *	result;

	while ( true ) {
		int token = PeekTokenType();

		switch ( token ) {
			// '*' - pointer
			//case P_ASTERISK:

			// '^' - pointer
			case P_CARET:
				{
					ReadToken();
					prevType = new TypePointer( prevType );
				}
				continue;

			// '[' ... ']' - array
			// example: "int[3][1] a;" is (array[1] of array[3] of int)
			case P_LEFT_BRACKET:
				{
					ReadToken();
					++m_inBrackets;
					// '[' expr ']'
					Expression * e = ExpectExpression();
					Expect( P_RIGHT_BRACKET );

					--m_inBrackets;

					prevType = new TypeStaticArray( prevType, e );
				}
				continue;

			default:
				result = prevType;
				break;
		}
		
		break;
	}//while true

	return result;
}

bool Parser::StartOfDeclaration() {
	Token * token = PeekToken();

	switch ( token->Value() )
	{
		case T_AUTO:
		case T_ALIAS:
		case T_CONST:
		case T_EXTERN:
		case T_INVARIANT:
		case T_TYPEOF:
		case T_TYPEDEF:
		case T_FINAL:

		//***
		case T_NAMESPACE:
		case T_CLASS:
		case T_INTERFACE:
		case T_STRUCT:

		case T_VOID:
		case T_BOOL:
		case T_BYTE:
		case T_UBYTE:
		case T_CHAR:
		case T_WCHAR:
		case T_DCHAR:
		case T_SHORT:
		case T_INT:
		case T_UINT:
		case T_LONG:
		case T_ULONG:
		case T_FLOAT:
		case T_DOUBLE:
		case T_STRING:

		case T_SIGNED:
		case T_UNSIGNED:
			return true;

		case L_IDENTIFIER:
			{
				// peek over identifier
				token = PeekNextToken( token );

				// it is a label
				if ( token->Value() == P_COLON ) {
					return false;
				}

				// idType  idVar
				if ( token->Value() == L_IDENTIFIER ) {
					return true;
				}

				while ( true )
				{
					// identifier, followed by '^' ( or '*' )
					if ( token->Value() == P_CARET ) //||
						// token->Value() == P_ASTERISK )
					{
						token = PeekNextToken( token );

						// id^ id
						if ( token->Value() == L_IDENTIFIER ) {
							return true;
						}
						continue;
					}
					
					// id.id ( or id :: id )
					if ( token->Value() == P_DOT || 
						 token->Value() == P_COLON_COLON )
					{
						// peek over '.'
						token = PeekNextToken( token );

						if ( token->Value() == L_IDENTIFIER ) {
							token = PeekNextToken( token );
							if ( token->Value() == L_IDENTIFIER ) {
								return true;
							}
						}
						
						continue;
					}

					// id [ expr ]
					if ( token->Value() == P_LEFT_BRACKET )
					{
						m_inBrackets = 1;

						while ( m_inBrackets && !EndOfFile() )
						{
							token = PeekNextToken( token );

							if ( token->Value() == P_LEFT_BRACKET ) {
								++m_inBrackets;
							} 
							else if ( token->Value() == P_RIGHT_BRACKET ) {
								--m_inBrackets;
							}
							if ( m_inBrackets > MAX_BRACKET_NEST_LEVEL ) {
								Error( "too many nested square brackets: %d", m_inBrackets );
								return false;
							}
						}

						// peek over ']'
						token = PeekNextToken( token );
						continue;
					}

					break;
				}//while true
			
			}//case identifier

	}//switch curr. token

	return false;
}

Block * Parser::ParseBlockStatement() 
{
	Expect( P_LEFT_BRACE );
	StatementList * stmts = ParseStatementList();
	Expect( P_RIGHT_BRACE );

	SetLocation( stmts );

	Block * s = new Block( stmts );
	SetLocation( s );

	return s;
}

// never returns NULL
StatementList * Parser::ParseStatementList() 
{
	StatementList * sList = new StatementList;
	
	while ( true ) {
		Statement * s = ParseStatement();
		if ( s ) {
			SetLocation( s );
			sList->Add( s );
			continue;
		}

		break;
	}

	SetLocation( sList );
	return sList;
}


// returns NULL, if no statements were parsed
Statement * Parser::ParseStatement( int flags /* =ALLOW_ANY_STATEMENT */ ) {
	Statement *	s = 0;

	if ( flags & ALLOW_DECLARATIONS ) 
	{
		if ( StartOfDeclaration() 
			//HACK: auto declarations will be parsed as statements
			&& PeekTokenType() != T_AUTO )
		{
			Declaration * decl = ParseDeclaration();
			if ( decl ) {
				s = new DeclStatement( decl );
				return s;
			}
			else {
				return NULL;
			}
		}
	}//end case of declarations

	int tok = PeekTokenType();

	if ( IsStartOfExpression( tok ) )
	{
		Expression * e = ParseExpression();

		tok = PeekTokenType();

		// if it is a label
		if ( P_COLON == tok )
		{
			ReadToken();

			if ( ID_IDENTIFIER != e->GetId() ) {
				Error( "labels should start with a single identifier!" );
				return new ErrorStatement( "labels should start with a single identifier!", GetLocation() );
			}
			IdExp * idExp = static_cast< IdExp* >( e );

			s = ParseStatement();
			SetLocation( s );

			s = new LabeledStatement( idExp->GetIdentifier(), s );
			return s;
		}

		// otherwise, it's an expression statement

		// if it is a list of comma-separated expressions
		if ( P_COMMA == tok ) 
		{
			ExpressionList * eList = new ExpressionList;
			eList->Add( e );

			while ( P_COMMA == tok ) {
				ReadToken();

				Expression * eTmp = ExpectExpression();
				eList->Add( eTmp );
				
				tok = PeekTokenType();
			}
			
			e = eList;
		}

		Expect( P_SEMICOLON );
		s = new ExpressionStatement( e );
		return s;

	}//end case of expressions

	// see what follows based on the type of the next token
	switch ( tok )
	{
		// auto declaration: 'auto' id = nonVoidInitializer ';'
		case T_AUTO:
			{
				ReadToken();
				Identifier * var = ExpectIdentifier();
				Expect( P_EQUAL );
				Expression * e = ExpectExpression();
				Expect( P_SEMICOLON );

				s = new AutoDeclStatement( var, e );
			}
			break;

		if ( flags & ALLOW_BLOCK_STATEMENT ) 
		{
			case P_LEFT_BRACE:
				s = ParseBlockStatement();
				break;
		}

			//------------------------------
			// selection statements
			//------------------------------

		case T_IF:
			{
				ReadToken();
				Expect( P_LEFT_PARENTH );
				Expression * condition = ExpectExpression();
				Expect( P_RIGHT_PARENTH );

				Statement * thenStmt = ParseStatement();

				Statement * elseStmt = 0;
				
				tok = PeekTokenType();
				if ( T_ELSE == tok ) {
					ReadToken();
					elseStmt = ParseStatement();
				}
	
				s = new IfStatement( condition, thenStmt, elseStmt );
			}
			break;

			//------------------------------
			// iteration statements
			//------------------------------
		case T_WHILE:
			{
				ReadToken();
				Expect( P_LEFT_PARENTH );
				Expression * condition = ExpectExpression();
				Expect( P_RIGHT_PARENTH );

				Statement * body = ExpectStatement();

				s = new WhileStatement( condition, body );
			}
			break;

		case T_DO:
			{
				ReadToken();

				Statement * body = ExpectStatement();
				
				Expect( T_WHILE );

				Expect( P_LEFT_PARENTH );
				Expression * condition = ExpectExpression();
				Expect( P_RIGHT_PARENTH );

				Expect( P_SEMICOLON );

				s = new DoStatement( body, condition );
			}
			break;

		case T_FOR:
			s = ParseForStatement();
			break;

			//------------------------------
			//	Control statements
			//------------------------------
		case T_BREAK:
			{
				ReadToken();
				
				Identifier * label = 0;

				tok = PeekTokenType();
				if ( L_IDENTIFIER == tok ) {
					label = ExpectIdentifier();
				}
				Expect( P_SEMICOLON );

				s = new BreakStatement( label );
			}
			break;

		case T_CONTINUE:
			{
				ReadToken();
				
				Identifier * label = 0;

				tok = PeekTokenType();
				if ( L_IDENTIFIER == tok ) {
					label = ExpectIdentifier();
				}
				Expect( P_SEMICOLON );

				s = new ContinueStatement( label );
			}
			break;

		case T_DEFAULT:
			{
				ReadToken();
				Statement * stmt = ExpectStatement();
				
				s = new DefaultStatement( stmt );
			}
			break;

		case T_RETURN:
			{
				ReadToken();
				
				Expression * returnExp = 0;

				tok = PeekTokenType();
				if ( P_SEMICOLON != tok ) {
					returnExp = ExpectExpression();
				}
				Expect( P_SEMICOLON );
				
				s = new ReturnStatement( returnExp );
			}
			break;

			//------------------------------
			// With Statement
			//------------------------------
		case T_WITH:
			{
				ReadToken();

				Expect( P_LEFT_PARENTH );
				Expression * E = ExpectExpression();
				Expect( P_RIGHT_PARENTH );

				Statement * body = ParseBlockStatement();

				s = new WithStatement( E, body );
			}
			break;
		

		if ( flags & ALLOW_EMPTY_STATEMENT ) 
		{
			// Empty Statement
			case P_SEMICOLON:
				ReadToken();
				s = new EmptyStatement();
				break;
		}

		default:
			return NULL;
	}

	return s;
}

ForLoopStatement * Parser::ParseForStatement() {
	Expect( T_FOR );

	Expression *	condition = 0;
	ExpressionList * update = 0;
	Statement *		body = 0;

	Expect( P_LEFT_PARENTH );

	Statement * initStmt = ParseStatement( ALLOW_DECLARATIONS | ALLOW_EMPTY_STATEMENT );

	Token * token = PeekToken();
	
	// if there is no condition
	if ( P_SEMICOLON == token->Value() ) {
		ReadToken();
	} 
	else {
		condition = ExpectExpression();
		Expect( P_SEMICOLON );
	}

	// if there's no update expressions
	if ( P_RIGHT_PARENTH != PeekTokenType() ) {
		update = ParseExpressionList();
	}
	
	Expect( P_RIGHT_PARENTH );

/*	
	Token * token = PeekToken();
	// if there are no initializers
	if ( P_SEMICOLON == token->Value() ) {
		ReadToken();
	}
	else {
		initializers = ParseStatement();
	}

	tok = PeekTokenType();
	if ( P_SEMICOLON == tok ) {
		ReadToken();
	}
	else {
		condition = ExpectExpression();
		Expect( P_SEMICOLON );
	}

	tok = PeekTokenType();
	if ( P_RIGHT_PARENTH == tok ) {
		ReadToken();
	}
	else {
		update = ParseExpressionList();
		Expect( P_RIGHT_PARENTH );
	}
*/
	body = ParseStatement();

	return new ForLoopStatement( initStmt, condition, update, body );
}

//******************************************

//-------------------------------------------------------
//		Expressions
//-------------------------------------------------------

Expression * Parser::ParseExpression() {
	Expression * E = ParseCommaExp();
	SetLocation( E );
	return E;
}

Expression * Parser::ParseCommaExp() {
	Expression  *e, *e2;

	e = ParseAssignment();
	SetLocation( e );

	while ( P_COMMA == PeekTokenType() ) {
		ReadToken();
		e2 = ParseAssignment();
		SetLocation( e2 );
		e = new CommaExpression( e, e2 );
		SetLocation( e );
	}
	return e;
}

Expression * Parser::ParseAssignment() {
	Expression * e, *e2;

	e = ParseConditional();

	SetLocation( e );

	while ( true ) {
		int token = PeekTokenType();

		switch ( token ) {
			// e += e
			case P_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				e = new AssignmentExp( e, e2 );
				SetLocation( e );
				continue;

				// e += e
			case P_PLUS_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new AddAssignExp( e, e2 );
				SetLocation( e );
				continue;

				// e -= e
			case P_MINUS_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new SubAssignExp( e, e2 );
				SetLocation( e );
				continue;

				// e /= e
			case P_SLASH_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new DivAssignExp( e, e2 );
				SetLocation( e );
				continue;

			// e *= e
			case P_ASTERISK_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new MulAssignExp( e, e2 );
				SetLocation( e );
				continue;

				// e %= e
			case P_PERCENT_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new ModAssignExp( e, e2 );
				SetLocation( e );
				continue;

				// e &= e 
			case P_AMPERSAND_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new BitAndAssignExp( e, e2 );
				SetLocation( e );
				continue;

				// e ^= e 
			case P_CARET_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new BitXorAssignExp( e, e2 );
				SetLocation( e );
				continue;

				// e |= e 
			case P_VERTICAL_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new BitOrAssignExp( e, e2 );
				SetLocation( e );
				continue;

				// e >>= e 
			case P_GREATER_GREATER_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new ShrAssignExp( e, e2 );
				SetLocation( e );
				continue;

				// e <<= e 
			case P_LESS_LESS_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new ShlAssignExp( e, e2 );
				SetLocation( e );
				continue;

				// e >>>= e 
			case P_UNSIGNED_RIGHT_SHIFT_EQUAL:
				ReadToken();
				e2 = ParseAssignment();
				SetLocation( e2 );
				
				e = new UShrAssignExp( e, e2 );
				SetLocation( e );
				continue;
		}

		break;
	}

	return e;
}

// ?:
Expression * Parser::ParseConditional() {
	Expression * e, *e1, *e2;

	e = ParseLogicalOr();

	int tok = PeekTokenType();
	if ( P_QUESTION == tok ) {
		ReadToken();

		e1 = ParseExpression();
		Expect( P_COLON );
		SetLocation( e1 );

		e2 = ParseConditional();
		SetLocation( e2 );

		e = new CondExp( e, e1, e2 );
		SetLocation( e );
	}
	return e;
}

// || .
Expression * Parser::ParseLogicalOr() {
	Expression * L = ParseLogicalAnd();

	Expression  * R, *E;

	while ( true ) {
		int token = PeekTokenType();

		if ( P_VERTICAL_VERTICAL == token ) {
			ReadToken();
			R = ParseLogicalAnd();
			SetLocation( R );
			E = new OrOrExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}

		break;
	}
	return L;
}

// && .
Expression * Parser::ParseLogicalAnd() {
	Expression * L = ParseBitOr();

	Expression  * R, *E;

	while ( true ) {
		int token = PeekTokenType();

		if ( P_AMPERSAND_AMPERSAND == token ) {
			ReadToken();
			R = ParseBitOr();
			SetLocation( R );
			E = new AndAndExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}

		break;
	}
	return L;
}

// E | E ( E or E )
Expression * Parser::ParseBitOr() {
	Expression * e = ParseBitXor();
	SetLocation( e );

	int token = PeekTokenType();
	while ( P_VERTICAL == token || T_OR == token ) {
		ReadToken();
		
		Expression * e2 = ParseBitXor();
		SetLocation( e2 );

		e = new BitOrExp( e, e2 );
		SetLocation( e );

		token = PeekTokenType();
	}
	return e;
}

// E ^ E ( E xor E )
Expression * Parser::ParseBitXor() {
	Expression * e = ParseBitAnd();
	SetLocation( e );

	int token = PeekTokenType();
	while ( P_CARET == token || T_XOR == token ) {
		ReadToken();
		
		Expression * e2 = ParseBitAnd();
		SetLocation( e2 );

		e = new BitXorExp( e, e2 );
		SetLocation( e );

		token = PeekTokenType();
	}
	return e;
}

// E & E ( E and E )
Expression * Parser::ParseBitAnd() {
	Expression * e = ParseEquality();
	SetLocation( e );

	int token = PeekTokenType();
	while ( P_AMPERSAND == token || T_AND == token ) {
		ReadToken();
		
		Expression * e2 = ParseEquality();
		SetLocation( e2 );

		e = new BitAndExp( e, e2 );
		SetLocation( e );

		token = PeekTokenType();
	}
	return e;
}

// ==, != .
Expression * Parser::ParseEquality() {
	Expression * L = ParseRelational();

	Expression  * R, *E;

	while ( true ) {
		int token = PeekTokenType();

		// E -> E == E
		if ( P_EQUAL_EQUAL == token ) {
			ReadToken();
			R = ParseRelational();
			SetLocation( R );
			E = new EquExp( L, R, EquExp::Equal );
			SetLocation( E );
			L = E;
			continue;
		}
		// E -> E != E
		if ( P_NON_EQUAL == token ) {
			ReadToken();
			R = ParseRelational();
			SetLocation( R );
			E = new EquExp( L, R, EquExp::NotEqual );
			SetLocation( E );
			L = E;
			continue;
		}

		break;
	}
	return L;
}

// >, >=, <, <= .
Expression * Parser::ParseRelational() {
	Expression * L = ParseShifts();

	Expression  * R, *E;

	while ( true ) {
		int token = PeekTokenType();
		
		// E -> E < E
		if ( P_LESS == token ) {
			ReadToken();
			R = ParseShifts();
			SetLocation( R );
			E = new RelExp( L, R, RelExp::LT );
			SetLocation( E );
			L = E;
			continue;
		}
		// E -> E <= E
		if ( P_LESS_EQUAL == token ) {
			ReadToken();
			R = ParseShifts();
			SetLocation( R );
			E = new RelExp( L, R, RelExp::LE );
			SetLocation( E );
			L = E;
			continue;
		}
		// E -> E > E
		if ( P_GREATER == token ) {
			ReadToken();
			R = ParseShifts();
			SetLocation( R );
			E = new RelExp( L, R, RelExp::GT );
			SetLocation( E );
			L = E;
			continue;
		}
		// E -> E >= E
		if ( P_GREATER_EQUAL == token ) {
			ReadToken();
			R = ParseShifts();
			SetLocation( R );
			E = new RelExp( L, R, RelExp::GE );
			SetLocation( E );
			L = E;
			continue;
		}
		if ( T_IN == token ) {
			ReadToken();
			R = ParseShifts();
			SetLocation( R );
			E = new InExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}

		break;
	}
	return L;
}

// <<, >>, >>> .
Expression * Parser::ParseShifts() {
	Expression * L = ParseTerms();

	Expression  * R, *E;

	while ( true ) {
		int token = PeekTokenType();

		// E -> E << E
		if ( P_LESS_LESS == token ) {
			ReadToken();
			R = ParseTerms();
			SetLocation( R );
			E = new ShlExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}
		// E -> E >> E
		if ( P_GREATER_GREATER == token ) {
			ReadToken();
			R = ParseTerms();
			SetLocation( R );
			E = new ShrExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}
		// E -> E >>> E
		if ( P_UNSIGNED_RIGHT_SHIFT == token ) {
			ReadToken();
			R = ParseTerms();
			SetLocation( R );
			E = new UShrExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}
		break;
	}
	return L;
}

// +, - .
Expression * Parser::ParseTerms() {
	Expression * L = ParseFactors();

	Expression  * R, *E;

	while ( true ) {
		int token = PeekTokenType();
		
		// E -> E + E
		if ( P_PLUS == token ) {
			ReadToken();
			R = ParseFactors();
			SetLocation( R );
			E = new AddExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}
		// E -> E - E
		if ( P_MINUS == token ) {
			ReadToken();
			R = ParseFactors();
			SetLocation( R );
			E = new SubExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}
		break;		 
	}
	return L;
}

// *, /, % .
Expression * Parser::ParseFactors() {
	Expression * L = ParsePostfixExp();

	Expression  * R, *E;

	while ( true ) {
		int token = PeekTokenType();

		// E -> E * E
		if ( P_ASTERISK == token ) {
			ReadToken();
			R = ParsePostfixExp();
			SetLocation( R );
			E = new MulExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}
		// E -> E / E
		if ( P_SLASH == token ) {
			ReadToken();
			R = ParsePostfixExp();
			SetLocation( R );
			E = new DivExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}
		// E -> E % E
		if ( P_PERCENT == token ) {
			ReadToken();
			R = ParsePostfixExp();
			SetLocation( R );
			E = new ModExp( L, R );
			SetLocation( E );
			L = E;
			continue;
		}
		break;		 
	}
	return L;
}

Expression * Parser::ParsePostfixExp() {
	Expression * e, *e1;

	e = ParseUnaryExp();

	int tok = PeekTokenType();

	switch ( tok ) {
	// E -> E++
	case P_PLUS_PLUS:
		ReadToken();
		e = new PostIncExp( e );
		SetLocation( e );
		break;

	// E -> E--
	case P_MINUS_MINUS:
		ReadToken();
		e = new PostDecExp( e );
		SetLocation( e );
		break;

	// E -> E [ e1 ]
	case P_LEFT_BRACKET:
		ReadToken();
		e1 = ParseUnaryExp();
		Expect( P_RIGHT_BRACKET );
		SetLocation( e1 );
		e = new ArrayAccessExp( e, e1 );
		SetLocation( e );
		break;

	// E -> e1 is e2
	case T_IS:
		ReadToken();
		e1 = ParseUnaryExp();
		e = new IsExp( e, e1 );
		break;

	// E -> e1 as e2
	case T_AS:
		ReadToken();
		e1 = ParseUnaryExp();
		e = new AsExp( e, e1 );
		break;

#ifdef ALLOW_C_STYLE_POINTERS		
	// E -> ptr '->' member
	case P_MINUS_GREATER:	
		ReadToken();
		e1 = ParseUnaryExp();
		e = new PtrExp( e, e1 );
		break;
#endif

	// E -> E ( parameters )
	case P_LEFT_PARENTH:
		ExpressionList * args = ParseArguments();
		e = new CallExp( e, args );
		SetLocation( e );
		break;
	}

	return e;
}

Expression * Parser::ParseUnaryExp() {
	Expression		* e, *e2;
	Location  loc = GetLocation();

	int  token = PeekTokenType();

	switch ( token ) 
	{

#ifdef ALLOW_C_STYLE_POINTERS		
		// E -> &E
		case P_AMPERSAND:	
#endif
		// E -> @E
		case P_AT:
			ReadToken();
			e = ParseUnaryExp();
			SetLocation( e );
			e = new AddrOfExp( e );
			SetLocation( e );
			break;

		//------------------------------------------

#ifdef ALLOW_C_STYLE_POINTERS
			// C-style pointer dereferencing operator ( remove if it causes problems )
			// E -> *E
		case P_ASTERISK:
			ReadToken();
			e = ParseUnaryExp();
			e = new PtrIndirectionExp( e );
			break;
#endif
		//------------------------------------------

		// E -> +E
		case P_PLUS :
			ReadToken();
			e = ParseUnaryExp();
			e = new UnaryPlusExp( e );
			SetLocation( e );
			break;

		// E -> -E
		case P_MINUS :
			ReadToken();
			e = ParseUnaryExp();
			SetLocation( e );
			e = new NegationExp( e );
			SetLocation( e );
			break;

		// E -> ++E
		case P_PLUS_PLUS :
			ReadToken();
			e = ParseUnaryExp();
			SetLocation( e );
			e = new PreIncExp( e );
			SetLocation( e );
			break;

		// E -> --E
		case P_MINUS_MINUS :
			ReadToken();
			e = ParseUnaryExp();
			SetLocation( e );
			e = new PreDecExp( e );
			SetLocation( e );
			break;

		// E -> !E
		case P_EXCLAMATION :
			ReadToken();
			e = ParseUnaryExp();
			SetLocation( e );
			e = new NotExp( e );
			SetLocation( e );
			break;

		// E -> ~E
		case P_TILDE :
			ReadToken();
			e = ParseUnaryExp();
			SetLocation( e );
			e = new ComplExp( e );
			SetLocation( e );
			break;

		// E -> cast( Etype ) E
		// E -> cast< Etype > E
		case T_CAST :
			ReadToken();
		
			// E -> cast( Etype ) E
			if ( P_LEFT_PARENTH == PeekTokenType() ) {
				ReadToken();
				e = ParseUnaryExp();
				Expect( P_RIGHT_PARENTH );
			}
			else {	// E -> cast< Etype > E
				Expect( P_LESS );
				e = ParseUnaryExp();
				Expect( P_GREATER );
			}
			SetLocation( e );

			e2 = ParseUnaryExp();
			SetLocation( e2 );

			e = new CastExp( e, e2 );
			SetLocation( e );
			break;

		default:
			e = ParsePrimaryExp();
			SetLocation( e );
	}

	return e;
}

Expression * Parser::ParsePrimaryExp() {
	Expression * e = ParseAtom();

	while ( true ) {
		int token = PeekTokenType();

		// E -> E^
		if ( P_CARET == token ) {
			ReadToken();
			e = new PtrIndirectionExp( e );
			SetLocation( e );
			continue;
		}
		// E -> E.id
		if ( P_DOT == token ) {
			ReadToken();
			Identifier * id = ExpectIdentifier();

			e = new DotExp( e, id );
			SetLocation( e );
			continue;
		}

		break;
	}

	return e;
}

Expression * Parser::ParseAtom() {
	Expression *	e;
	Identifier *	id;
//	Type *			type;
	Location		loc = GetLocation();

	int  token = PeekTokenType();

	switch ( token ) {
		// E -> Identifier
		case L_IDENTIFIER :
			ReadToken();
			id = new Identifier( Text() );
			SetLocation( id );

			e = new IdExp( id );
			SetLocation( e );

			token = PeekTokenType();

			if ( P_CARET == token ) {
				ReadToken();
				e = new PtrIndirectionExp( e );
				SetLocation( e );
			}
			break;

		// E -> :: Identifier
		case P_COLON_COLON :
			ReadToken();
			id = ExpectIdentifier();
			e = new IdExp( id );
			e->location = loc;
			break;

		//----------------------------------------
		// type expressions used in type casts
		// for example:  f = (float)n;  r = cast< int >( ptr );

		case T_CHAR :
			ReadToken();
			return new CharConstant( '\0' );

		case T_BOOL :
			ReadToken();
			return new BooleanConstant( true );

		case T_INT :
			ReadToken();
			return new IntegerConstant( 0 );

		case T_FLOAT :
			ReadToken();
			return new FloatingConstant( 0.0f );

		case T_STRING :
			ReadToken();
			return new StringLiteral( "" );
		//----------------------------------------

		// 'this'
		case T_THIS :
			ReadToken();
			e = new ThisExpression( loc );
			break;

		// Literals

		case L_CHAR :
			ReadToken();
			e = new CharConstant( CurrentTokenChar() );
			break;

		// Boolean constants
		case T_TRUE :
			ReadToken();
			e = new BooleanConstant( true );
			SetLocation( e );
			break;

		case T_FALSE :
			ReadToken();
			e = new BooleanConstant( false );
			SetLocation( e );
			break;

		// Numerical constants
		case L_INT32 :
			ReadToken();
			e = new IntegerConstant( Text() );
			SetLocation( e );
			break;

		case L_FLOAT32 :
			ReadToken();
			e = new FloatingConstant( Text() );
			SetLocation( e );
			break;

		// String constants
		case L_STRING :
			ReadToken();
			e = new StringLiteral( Text() );
			break;

		// E -> ( E )
		case P_LEFT_PARENTH :
			ReadToken();
			e = ParseExpression();
			Expect( P_RIGHT_PARENTH );
			SetLocation( e );
			break;

		// E -> assert( e1 )
		// E -> assert( e1, e2, ..., e3 )
		// E -> assert( e1, e2, ..., e3 : eMessage )
		case T_ASSERT :
			{
				ReadToken();
				Expect( P_LEFT_PARENTH );
				ExpressionList * eList = ParseExpressionList();

				Expression * eMsg = 0;

				int token = PeekTokenType();
				if ( P_COLON == token )
				{
					ReadToken();
					eMsg = ParseExpression();
					SetLocation( eMsg );
				}
				
				Expect( P_RIGHT_PARENTH );

				e = new AssertExpression( eList, eMsg );
				SetLocation( e );
				break;
			}

		default:
			Error( "expression expected, not '%s'", Text() );

			// anything != NULL to keep the parser running
			e = new ErroneousExpression( "expression_expected", GetLocation() );
			break;
	}

	return e;
}

ExpressionList * Parser::ParseExpressionList() {
	ExpressionList * expList = new ExpressionList;

	while ( true ) {
		Expression * e = ExpectExpression();
		SetLocation( e );
		expList->Add( e );

		if ( P_COMMA == PeekTokenType() ) {
			ReadToken();
			continue;
		}
		
		break;
	}
	SetLocation( expList );
	return expList;
}

ExpressionList * Parser::ParseArguments() {
	Expect( P_LEFT_PARENTH );

	ExpressionList * args = new ExpressionList;

	while ( true ) 
	{
		if ( IsStartOfExpression( PeekTokenType() ) ) 
		{
			Expression * e = ParseAssignment();
			SetLocation( e );
			args->Add( e );
			continue;
		} 
		
		if ( P_COMMA == PeekTokenType() ) {
			ReadToken();
			continue;
		}

		break;
	}
	
	Expect( P_RIGHT_PARENTH );
	SetLocation( args );

	return args;
}

//******************************************

bool Parser::IsStartOfExpression( int tokenType ) const {
	switch ( tokenType ) {
		case L_CHAR :
		case L_WCHAR :
		case L_DCHAR :
		case L_INT32 :
		case L_INT64 :
		case L_UINT32 :
		case L_UINT64 :
		case L_FLOAT32 :
		case L_FLOAT64 :
		case L_FLOAT80 :
		case L_STRING :

		case L_IDENTIFIER:
		
		case T_NULL:	case T_NIL:

		case T_CAST:

		case T_FUNCTION:
		case T_DELEGATE:

		case T_TRUE:	case T_FALSE:
		
		case T_NEW:		case T_DELETE:

		case T_TYPEID:

		case T__FILE__:
		case T__DATE__:
		case T__LINE__:

		case T_THIS:	case T_SUPER:

		case P_PLUS:	case P_PLUS_PLUS:
		case P_MINUS:	case P_MINUS_MINUS:

#ifdef ALLOW_C_STYLE_POINTERS
			case P_ASTERISK:
#endif

		case P_LEFT_PARENTH:	
		case P_LEFT_BRACKET:

		case T_ASSERT:
		case P_COLON_COLON:
			return true;
	}
	return false;
}

Identifier * Parser::ExpectIdentifier( const char *message /* =0 */ ) {
	const Token * token = PeekToken();

	if ( L_IDENTIFIER != token->Value() ) {
		Error( "expected '%s' after '%s'.", 
			message ? message : "identifier",
			Text() );
	} else {
		ReadToken();
	}

	Identifier * id = new Identifier( Text() );
	SetLocation( id );
	return id;
}

Expression * Parser::ExpectExpression() {
	Expression * e = ParseExpression();
	if ( !e ) {
		Error( "expression expected!" );
		return new ErroneousExpression( "expression_expected", GetLocation() );
	}
	SetLocation( e );
	return e;
}

Statement * Parser::ExpectStatement() {
	Statement * s = ParseStatement();
	if ( !s ) {
		Error( "statement expected!" );
		return new ErrorStatement( "statement_expected", GetLocation() );
	}
	SetLocation( s );
	return s;
}

void Parser::SetLocation( AST_Node * node ) const {
	node->location = GetLocation();
}

void Parser::ReadOptional( int token ) {
	if ( PeekTokenType() == token ) {
		ReadToken();
	}
}

void Parser::Expect( int tokenType ) {
	const Token * token = PeekToken();

	// TODO: these should be updated whenever a new token is read
	switch ( token->Value() ) 
	{
		case P_LEFT_BRACE:
			++m_braceCount;
			break;

		case P_RIGHT_BRACE:
			--m_braceCount;
			break;

		case P_LEFT_PARENTH:
			++m_parenthCount;
			break;

		case P_RIGHT_PARENTH:
			--m_parenthCount;
			break;

		case P_LEFT_BRACKET:
			++m_inBrackets;
			break;

		case P_RIGHT_BRACKET:
			--m_inBrackets;
			break;
	}

	if ( tokenType != token->Value() ) {
		Error( "expected '%s' after '%s'.", Token::ToChars( tokenType ), Text() );
	} else {
		ReadToken();
	}
}

void Parser::Check( int token ) {
	if ( token != TokenType() ) {
		Error( "expected '%s', but got '%s'.", Token::ToChars( token ), Text() );
		ReadToken();
	}
}

//******************************************

//******************************************

void Parser::Print( const char* format, ... ) {
	c4::common->Print( "\tParser(%d,%d): message:", GetLineNum(), CurrentColumn() );

	va_list	 argptr;

	va_start( argptr, format );
	vsprintf_s( m_tmp_buf, BUFFER_SIZE, format, argptr );
	va_end( argptr );

	c4::common->Print( m_tmp_buf );
}

void Parser::Warning( const char *message, ... ) {
	c4::common->Print( "Warning(%d,%d): ", GetLineNum(), CurrentColumn() );

	++m_numWarnings;

	va_list	 argptr;

	va_start( argptr, message );
	vsprintf_s( m_tmp_buf, BUFFER_SIZE, message, argptr );
	va_end( argptr );

	c4::compiler->Warning( m_tmp_buf );
}

void Parser::Error( const char *message, ... ) {
	c4::common->Print( "Syntax error(%d,%d): ", GetLineNum(), CurrentColumn() );

	++m_numErrors;

	va_list	 argptr;

	va_start( argptr, message );
	vsprintf_s( m_tmp_buf, BUFFER_SIZE, message, argptr );
	va_end( argptr );

	c4::compiler->Error( m_tmp_buf );
}

}//end namespace c4