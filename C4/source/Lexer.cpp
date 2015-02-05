/*
======================================
	Lexer, implementation.
======================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

/*
=========================================
	Lexer::Lexer()
=========================================
*/
Lexer::Lexer() 
	: m_source( 0 )
	, m_size( 0 )
	, m_isLoaded( false )
	, m_p( m_source )
	, m_prev( 0 )
	, m_token( 0 )
	, m_line( 0 )
	, m_lastLine( 0 )
	, m_column( 0 )
	, m_lastColumn( 0 )
	, m_flags( 0 )
	, m_eof( false )

	, hashTable( keywords )
{
	ResetBuffer();

	StaticInit();
}

Lexer::Lexer( const char * source ) 
	: hashTable( keywords )
{
	StaticInit();
}

Lexer::~Lexer() {
}

void Lexer::Reset( const char * source ) {
	m_tokenList.DeleteAll();
	m_token = new Token; // dummy token to be linked into the token list
	m_tokenList.Append( m_token );

	m_source = source;

	m_p		= m_source;
	m_prev	= m_source;

	m_size		= strlen( source );
	m_isLoaded	= true;

	m_line		= 1;
	m_lastLine	= 1;
	m_column	= 0;
	m_lastColumn = 0;

	m_eof = false;

	ResetBuffer();

	m_flags = 0;
}

bool Lexer::LoadFile( const char *filename ) {
	c4::common->Warning( "Lexer: LoadFile( const char* filename ) - not implemented yet!" );
	return false;
}

void Lexer::FreeSource() {
	c4::common->Warning( "Lexer: FreeSource() - not implemented yet!" );
}

void Lexer::Init()
{
	m_tokenList.DeleteAll();
	m_token = new Token; // dummy token to be linked into the token list
	m_tokenList.Append( m_token );

	m_source = NULL;

	m_p		= m_source;
	m_prev	= NULL;

	m_size		= 0;
	m_isLoaded	= false;

	m_line		= 1;
	m_lastLine	= 1;
	m_column	= 0;
	m_lastColumn = 0;

	m_eof = false;

	ResetBuffer();

	m_flags = 0;
}

Token * Lexer::ReadNextToken() {
	if ( m_token->m_next ) {
		 m_token = m_token->m_next;
	}
	else {
		Token * token = new Token;
		Scan( token );
		Update( token );
		m_tokenList.Append( token );

		m_token = token;
	}

	return m_token;
}

int Lexer::ReadToken() {
	return ReadNextToken()->Value();
}

/*
======================================================
	Lexer::Scan( Token * token )

	The main worker function. Reads the next token
	from the source file into the passed token.
======================================================
*/
void Lexer::Scan( Token * token ) {
	UpdateLexerState();

	while ( true )
	{
		m_buffer.Clear();

#pragma region "Comments and division operator"
// TODO: function "ReadCommentOrSlash()".
		SkipComments();

#pragma endregion

#pragma region Identifiers and keywords

		if ( IsFirstIdChar( CurrentChar() ) )	// TODO: re-work this: return ReadKeywordOrIdentifier();
		{
			ReadName();
			token->m_text = m_buffer.ToChar();

			int		value;
			int *	p = & value;

			// if we got a keyword
			if ( hashTable.Get( m_buffer, &p ) ) {
				token->m_type = *p;
			}
			else {
				token->m_type = L_IDENTIFIER;
			}

			return;
		}
#pragma endregion

		else

#pragma region	Numbers
		
		if ( IsDigit( CurrentChar() ) ) {
			token->m_type = ReadNumber();
			token->m_text = m_buffer.Ptr();
			return;
		}

#pragma endregion

		else

#pragma region Characters

		if ( '\'' == CurrentChar() ) {
			token->m_type = ReadCharConstant();
			token->m_text = m_buffer.Ptr();
			return;
		}

#pragma endregion

		else

#pragma region String literals

		if ( '\"' == CurrentChar() ) {
			token->m_type = ReadStringConstant();
			token->m_text = m_buffer.Ptr();
			return;
		}

#pragma endregion

		else

		switch ( CurrentChar() )
		{
			case 0:
			case 0x1A:
				token->m_text = "<end of file>";
				token->m_type = T_END_OF_FILE;
				return;

			case ' ':
			case '\v':
			case '\f':
				++m_p;
				++m_column;
				continue;	// skip white space

			case '\t':
				++m_p;
				m_column += tabWidth;
				continue;

			case '\r':
				++m_p;
				m_column = 0;
				if ( '\n' != CurrentChar() ) {	// if CR stands by itself	
					++m_line;
				}
				continue;	// skip white space

			case '\n':
				++m_p;
				++m_line;
				m_column = 0;
				continue;	// skip white space



#pragma region Punctuation & Operators

			// Punctuation

#define GET_TOKEN( ch, the_type )		case ch:						\
											m_buffer.Reset();			\
											ReadChar();					\
											token->m_type = the_type;	\
											token->m_text = m_buffer;	\
											return;

		GET_TOKEN( '(',		P_LEFT_PARENTH 		);
		GET_TOKEN( ')',		P_RIGHT_PARENTH	 	);
		GET_TOKEN( '{',		P_LEFT_BRACE 		);
		GET_TOKEN( '}',		P_RIGHT_BRACE 		);
		GET_TOKEN( '[',		P_LEFT_BRACKET 		);
		GET_TOKEN( ']',		P_RIGHT_BRACKET	 	);
		GET_TOKEN( ';',		P_SEMICOLON 		);
		GET_TOKEN( ',',		P_COMMA 			);
		GET_TOKEN( '.',		P_DOT				);
		GET_TOKEN( '?',		P_QUESTION			);
		GET_TOKEN( '$',		P_DOLLAR			);
		GET_TOKEN( '\\',	P_BACKSLASH			);

#undef GET_TOKEN

		// Arithmetic operators

#define GET_TOKEN2( ch1, ch2, type1, type2 )	case ch1:							\
													m_buffer.Reset();				\
													ReadChar();						\
													if ( ch2 == *m_p ) {			\
														ReadChar();					\
														token->m_type = type2;		\
														token->m_text = m_buffer;	\
														return;						\
													}								\
													token->m_type = type1;			\
													token->m_text = m_buffer;		\
													return;


		GET_TOKEN2( '@', '=',	P_AT,			P_AT_EQUAL			);
		GET_TOKEN2( '^', '=',	P_CARET,		P_CARET_EQUAL		);
		GET_TOKEN2( '*', '=',	P_ASTERISK,		P_ASTERISK_EQUAL	);
		GET_TOKEN2( '/', '=',	P_SLASH,		P_SLASH_EQUAL		);
		GET_TOKEN2( '%', '=',	P_PERCENT,		P_PERCENT_EQUAL		);

		GET_TOKEN2( '=', '=',	P_EQUAL,		P_EQUAL_EQUAL		);
		GET_TOKEN2( '!', '=',	P_EXCLAMATION,	P_NON_EQUAL			);

		GET_TOKEN2( '~', '=',	P_TILDE,		P_TILDE_EQUAL		);

		GET_TOKEN2( ':', ':',	P_COLON,		P_COLON_COLON		);
		GET_TOKEN2( '#', '#',	P_SHARP,		P_SHARP_SHARP		);

#undef GET_TOKEN2



// Ex.:
// +
// +=
// ++

#define GET_TOKEN3( ch1, ch2, ch3, type1, type2, type3 )	case ch1:									\
																m_buffer.Reset();						\
																ReadChar();								\
																if ( ch2 == *m_p ) {					\
																	ReadChar();							\
																	token->m_type = type2;				\
																}										\
																else if ( ch3 == *m_p ) {				\
																	ReadChar();							\
																	token->m_type = type3;				\
																} else {								\
																	token->m_type = type1;				\
																}										\
																token->m_text = m_buffer;				\
																return;


	GET_TOKEN3( '+', '=', '+',	P_PLUS,			P_PLUS_EQUAL,			P_PLUS_PLUS			);
//	GET_TOKEN3( '-', '=', '-',	P_MINUS,		P_MINUS_EQUAL,			P_MINUS_MINUS		);

//------------------------------------------------------------
case '-':						
	m_buffer.Reset();			
	ReadChar();					
	if ( '>' == *m_p ) {		
		ReadChar();				
		token->m_type = P_MINUS_GREATER;	
	}							
	else if ( '-' == *m_p ) {	
		ReadChar();				
		token->m_type = P_MINUS_MINUS;	
	}
	else if ( '=' == *m_p ) {	
		ReadChar();				
		token->m_type = P_MINUS_EQUAL;	
	} 
	else {					
		token->m_type = P_MINUS;	
	}							
	token->m_text = m_buffer;	
	return;
//------------------------------------------------------------


	GET_TOKEN3( '&', '&', '=',	P_AMPERSAND,	P_AMPERSAND_AMPERSAND,	P_AMPERSAND_EQUAL	);
	GET_TOKEN3( '|', '=', '|',	P_VERTICAL,		P_VERTICAL_EQUAL,		P_VERTICAL_VERTICAL );

#undef GET_TOKEN3


// Shifts & Relational operators
// Example:
// <
// <=
// <<
// <<=

#define GET_TOKEN4( ch1, ch2, ch3, ch4, type1, type2, type3, type4 )	\
	case ch1:															\
		m_buffer.Reset();												\
		ReadChar();														\
		if ( ch2 == *m_p ) {											\
			ReadChar();													\
			token->m_type = type2;										\
		}																\
		else if ( ch3 == *m_p ) {										\
			ReadChar();													\
			if ( ch4 == *m_p ) {										\
				ReadChar();												\
				token->m_type = type4;									\
			} else {													\
				token->m_type = type3;									\
			}															\
			token->m_text = m_buffer;									\
			return;														\
		} else {														\
			token->m_type = type1;										\
		}																\
		token->m_text = m_buffer;										\
		return;															


	GET_TOKEN4( '<', '=', '<', '=', 
		P_LESS,		P_LESS_EQUAL,		P_LESS_LESS,		P_LESS_LESS_EQUAL		
	);
	
	//GET_TOKEN4( '>', '=', '>', '=', 
	//	P_GREATER,	P_GREATER_EQUAL,	P_GREATER_GREATER,	P_GREATER_GREATER_EQUAL	
	//);

#undef GET_TOKEN4


// Unsigned right shift: >>>, >>>=
//
	case '>':
		ReadChar();
		if ( '>' == *m_p ) {
			ReadChar();
			if ( '>' == *m_p ) {
				ReadChar();
				if ( '=' == *m_p ) {
					ReadChar();
					token->m_type = P_UNSIGNED_RIGHT_SHIFT_EQUAL;
				}
				else {
					token->m_type = P_UNSIGNED_RIGHT_SHIFT;
				}
				token->m_text = Text();
				return;
			} 
			else if ( '=' == *m_p ) {
				ReadChar();
				token->m_type = P_GREATER_GREATER_EQUAL;
				token->m_text = Text();
				return;
			}
			token->m_type = P_GREATER_GREATER;
			token->m_text = Text();
			return;
		}
		else if ( '=' == *m_p ) {
			ReadChar();
			token->m_type = P_GREATER_EQUAL;
			token->m_text = Text();
			return;
		}
		token->m_type = P_GREATER;
		token->m_text = Text();
		return;

#pragma endregion

	default:
		//Warning( "unknown token: %s; stopping parsing...", m_buffer );
		token->m_text = '\0';
		token->m_type = T_END_OF_FILE;
		return;

		}//switch ( curr. char )

	}//while ( true )
}

void Lexer::UnreadToken() {
	m_p = m_prev;
	m_line = m_lastLine;
	m_column = m_lastColumn;

	m_token = m_token->m_prev;
}

//****************************************************************

TokenList & Lexer::BuildTokenList() {
	while ( T_END_OF_FILE != ReadToken() )
		/* nothing */ 
		;
	return m_tokenList;
}

/*
================================================
	Lexer::ReadNumber

	Reads a number and returns its type ( L_INT32, L_UINT32, ... ).
	Integers can be decimal, octal or hex.
	Handles the suffixes U, UL, LU, L, etc.
================================================
*/
int Lexer::ReadNumber() {
	while ( IsDigit( *m_p ) ) {
		m_buffer += *m_p++;

		++m_column;
	}

	String  intPart = m_buffer;

	// if this is a floating-point number
	if ( '.' != *m_p ) {
		return L_INT32;
	}

	// skip '.'
	m_buffer += *m_p++;
	++m_column;	

	// read the fractional part
	while ( IsDigit( *m_p ) ) {
		m_buffer += *m_p++;
		++m_column;
	}

	return L_FLOAT32;
}

/*
================================================
	Lexer::ReadReal

	Reads in characters, converting them to real
================================================
*/
int Lexer::ReadReal() {
	Error( "ReadReal(): not implemented!" );
	return 0;
}

/*
================================================
	Lexer::ReadName()

	Reads an identifier or a keyword
	into buffer and return false 
	if started reading at the end of file.

	TODO:
		Checks for buffer overrun,
		ignore last chars in long identifiers,
		etc..
================================================
*/
bool Lexer::ReadName() {
	while ( IsIdChar( *m_p ) )
	{
		m_buffer += *m_p++;

		++m_column;
	}
	//c4::common->Print( "Lexer::ReadName(): read %s\n", m_buffer );
	return true;
}

//--------------------------------------------------------------------------
//	Reads spaces, tabs, C-like comments, etc.
//	When a newline character is found the line counter is increased.
//--------------------------------------------------------------------------
bool Lexer::ReadWhiteSpace() {
	while ( true ) {
		// skip white space
		while ( CurrentChar() <= ' ' )
		{
			if ( EndOfFile() ) {
				return false;
			}
			if ( '\n' == CurrentChar() ) {
				++m_line;
			}
			if ( '\t' == CurrentChar() ) {
				++m_p;
				m_column += tabWidth;
			}

			SkipChar();
		}
		// skip comments
		if ( '/' == CurrentChar() )
		{
			// comments //
			if ( '/' == PeekChar() )
			{
				SkipChar();
				do {
					SkipChar();
					if ( EndOfFile() ) {
						return false;
					}
				} while ( '\n' != CurrentChar() );
				
				++m_line;
				SkipChar();

				if ( EndOfFile() ) {
					return false;
				}
				continue;
			}
			// comments /* */
			else if ( '*' == PeekChar() )
			{
				SkipChar();
				while ( true )
				{
					SkipChar();
					if ( EndOfFile() ) {
						return false;
					}
					if ( '\n' == CurrentChar() ) {
						++m_line;
					}
					else if ( '/' == CurrentChar() )
					{
						if ( '*' == PreviousChar() ) {
							break;
						}
						if ( '*' == PeekChar() ) {
							Warning( "nested comment" );
						}
					}
				}
				
				SkipChar();

				if ( EndOfFile() ) {
					return false;
				}
				continue;
			}
		}
		break;

	}//while

	return true;
}

/*
=====================================
	Lexer::SkipComments()

	Notes:
	When a newline character is found the line counter is increased.
=====================================
*/
void Lexer::SkipComments() {
	int nestLevel = 0;

	if ( '/' == CurrentChar() )
	{
		// single-line comments //
		// line comments terminate at the end of the line
		if ( '/' == PeekChar() )
		{
			SkipChar();
			do {
				SkipChar();
				if ( EndOfFile() ) {
					return;
				}
			} while ( '\n' != CurrentChar() );

			++m_line;
		}
		// multi-line comments /* ... */
		// these can span multiple lines, but do not nest
		else if ( '*' == PeekChar() )
		{
			SkipChar();
			while ( true )
			{
				SkipChar();
				if ( EndOfFile() ) {
					return;
				}
				if ( '\n' == CurrentChar() ) {
					++m_line;
				}
				else if ( '/' == CurrentChar() )
				{
					if ( '*' == PreviousChar() ) {
						break;
					}
//					if ( '*' == PeekChar() ) {
//						Warning( "nested comment" );
//					}
				}
			}

			SkipChar();
		}
		// nesting comments /+ ... +/
		// these can span multiple lines and can nest
		else if ( '+' == PeekChar() ) 
		{
			SkipChar();
			++nestLevel;		
			
			while ( nestLevel )
			{
				SkipChar();
				if ( EndOfFile() ) {
					if ( nestLevel ) {
						Error( "unterminated /+ +/ comment; end of file reached." );
					}
					return;
				}
				if ( '\n' == CurrentChar() ) {
					++m_line;
				}
				else if ( '/' == CurrentChar() )
				{
					if ( '+' == PreviousChar() ) {
						--nestLevel;
					}
				}
				else if( '+' == CurrentChar() )
				{
					if ( '/' == PreviousChar() ) {
						++nestLevel;
					}
				}
			}

			SkipChar();
		}
	}
}

/*
=====================================
	Lexer::ReadCharConstant()
=====================================
*/
int Lexer::ReadCharConstant() {
	ReadChar();	// eat '\''

	char c = CurrentChar();

	if ( '\'' != ReadChar() ) {
		Error( "unterminated character constant" );
		return T_END_OF_FILE;
	}

	ReadChar();	// eat '\''

	m_buffer.Reset();
	m_buffer += c;     // now buffer[0] is the char

	return L_CHAR;
}

int Lexer::ReadStringConstant() {
	ReadChar();	  // read '\"'

	m_buffer.Reset();

	while ( '\"' != CurrentChar() && !EndOfFile() ) {
		ReadChar();
	}

	SkipChar();		// read the closing '\"'

	return L_STRING;
}

#if 0

bool Lexer::CheckTokenString( const char * str ) {
	for ( int i = 0; m_p[i]; i++ ) {
		if ( str[i] != m_p[i] ) {
			return false;
		}
	}
	return true;
}

#endif

int Lexer::ReadEscapeSequence() {
	return 0;
}

int Lexer::PeekTokenType() {
	return PeekToken()->Value();
}

Token * Lexer::PeekToken() {
	if ( m_token->m_next ) {
		return m_token->m_next;
	}
	else {
		Token * token = new Token;
		Scan( token );
		Update( token );
		m_tokenList.Append( token );

		return token;
	}
}

Token * Lexer::PeekNextToken( Token *prevToken /* =0 */ )
{
	if ( prevToken )
	{
		// peek the token following the given token
		if ( prevToken->m_next ) {
			return prevToken->m_next;
		}
		else {
			Token * token = new Token;
			Scan( token );
			Update( token );
			m_tokenList.Append( token );
			return token;
		}
	}
	else {
		// peek the token following the current token
		//return m_token->m_next;
		return PeekToken();
	}
}

bool Lexer::ExpectToken( int expectedType ) {
	if ( expectedType != ReadToken() )
	{
		Error( "expected '%s', but got '%s'", Token::ToChars( expectedType ), Text() );
	}
	return true;
}

const char * Lexer::Text() const {
	return m_token->Text();
}

char Lexer::CurrentTokenChar() const {
	return m_token->m_text[ 0 ];
}

const Location & Lexer::GetLocation() const {
	return m_token->m_location;
}

int Lexer::GetLineNum() const {
	return m_token->m_location.line;
}

int Lexer::CurrentColumn() const {
	return m_token->m_location.column;
}

void Lexer::Update( Token * token )
{
	token->m_location.line		= m_line;
	token->m_location.endLine	= m_lastLine;
	token->m_location.column	= m_column;
	token->m_location.endColumn = m_lastColumn;
}

void Lexer::UpdateLexerState() {
	m_prev		= m_p;
	m_lastLine	= m_line;
	m_lastColumn = m_column;
}

void Lexer::SaveState( LexerState & state ) const {
	state.p = m_p;
	state.prev = m_prev;
	state.token = m_token;
}

void Lexer::RestoreState( const LexerState & savedState ) {
	m_p = savedState.p;
	m_prev = savedState.prev;
	m_token = savedState.token;
}

void Lexer::ResetBuffer() {
	m_buffer.Clear();
}

void Lexer::Print( const char *message, ... ) const {
	va_list   ap;

	char	buffer[ 256 ];
	va_start( ap, message );
	sprintf( buffer, message, ap );
	va_end( ap );
	
	c4::common->Print( "Lexer(%d,%d): %s\n", m_line, m_column, buffer );
}

void Lexer::Warning( const char *message, ... ) const {
	va_list   ap;

	char	buffer[ 256 ];
	va_start( ap, message );
	sprintf( buffer, message, ap );
	va_end( ap );
	
	c4::common->Print( "Warning: Lexer(%d,%d): %s\n", m_line, m_column, buffer );
}

void Lexer::Error( const char *message, ... ) {
	m_flags &= LEXER_FLAGS::HAD_ERRORS;

	va_list   ap;

	char	buffer[ 256 ];
	va_start( ap, message );
	sprintf( buffer, message, ap );
	va_end( ap );

	c4::common->Print( "Lexer(%d,%d): %s\n", m_line, m_column, buffer );
}

//--------------------------------------------------------------------------------

/*
=========================
	statics
=========================
*/

void Lexer::StaticInit() {
	InitCharMap();
	InitKeywords();

	// TODO: init. __DATE__, __VERSION__, etc.
	*__time = 0;
	*__date = 0;
	//__version = globals::versionNumber;
	//strcpy( __vendor, globals::vendor );
}

//--------------------------------------------------------------------------------
unsigned char	Lexer::charMap[ 256 ];
const int		Lexer::cmOctal  = 0x1;
const int		Lexer::cmHex	= 0x2;
const int		Lexer::cmIdChar = 0x4;

unsigned char Lexer::IsOctal( unsigned char c ) {
	return charMap[ c ] & cmOctal;
}

unsigned char Lexer::IsHex( unsigned char c ) {
	return charMap[ c ] & cmHex;
}

unsigned char Lexer::IsIdChar( unsigned char c ) {
	return charMap[ c ] & cmIdChar;
}

void Lexer::InitCharMap()
{
	for ( unsigned c = 0; c < ARRAY_SIZE( charMap ); c++ )
	{
		if ( '0' <= c && c <= '7' ) {
			charMap[ c ] |= cmOctal;
		}
		if ( IsDigit( c ) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F') )
		{
			charMap[ c ] |= cmHex;
		}
		if ( isalnum( c ) || c == '_' ) {
			charMap[ c ] |= cmIdChar;
		}
	}
}

//--------------------------------------------------------------------------------

c4::Hash< int >		Lexer::keywords;

char	Lexer::__time[ 10 ];	// __TIME__, "HH:MM:SS"
char	Lexer::__date[ 12 ];	// __DATE__, "MMM DD YYYY"

//int		Lexer::__version; 
//char	Lexer::__vendor[ 32 ]; 

int	Lexer::tabWidth = 4;

/*
=========================================
	Lexer::InitKeywords()

	fill the hash table of keywords
=========================================
*/
void Lexer::InitKeywords() {
	int		value;

	value = T_ALIAS;
	keywords.Set( "alias" , value );

#define	ADD( keyword, code )		value = T_##code;				\
									keywords.Set( #keyword, value );

	ADD(	abstract,			ABSTRACT			)	
	ADD(	align,				ALIGN				)
	ADD(	and,				AND					)
	ADD(	as,					AS					)
	ADD(	asm,				ASM					)
	ADD(	assert,				ASSERT				)
	ADD(	auto,				AUTO				)

	ADD(	body,				BODY				)
	ADD(	bool,				BOOL				)
	ADD(	break,				BREAK				)
	ADD(	byte,				BYTE				)

	ADD(	case,				CASE				)
	ADD(	cast,				CAST				)
	ADD(	catch,				CATCH				)
	ADD(	cent,				CENT				)	
	ADD(	char,				CHAR				)
	ADD(	class,				CLASS				)	
	ADD(	const,				CONST				)
	ADD(	const_cast,			CONST_CAST			)
	ADD(	continue,			CONTINUE			)

	ADD(	dchar,				DCHAR				)
	ADD(	debug,				DEBUG				)
	ADD(	default,			DEFAULT				)
	ADD(	delegate,			DELEGATE			)
	ADD(	delete,				DELETE				)
	ADD(	deprecated,			DEPRECATED			)
	ADD(	do,					DO					)	
	ADD(	double,				DOUBLE				)

	ADD(	elif,				ELIF				)
	ADD(	end,				END					)
	ADD(	endif,				ENDIF				)
	ADD(	else,				ELSE				)
	ADD(	enum,				ENUM				)
	ADD(	export,				EXPORT				)
	ADD(	extern,				EXTERN				)

	ADD(	false,				FALSE				)
	ADD(	final,				FINAL				)
	ADD(	finally,			FINALLY				)
	ADD(	float,				FLOAT				)
	ADD(	for,				FOR					)
	ADD(	foreach,			FOREACH				)
	ADD(	foreach_reverse,	FOREACH_REVERSE		)
	ADD(	friend,				FRIEND				)
	ADD(	function,			FUNCTION			)

	ADD(	goto,				GOTO				)

	ADD(	if,					IF					)
	ADD(	implicit,			IMPLICIT			)
	ADD(	import,				IMPORT				)
	ADD(	in,					IN					)
	ADD(	inout,				INOUT				)
	ADD(	int,				INT					)
	ADD(	interface,			INTERFACE			)
	ADD(	internal,			INTERNAL			)
	ADD(	invariant,			INVARIANT			)
	ADD(	is,					IS					)

	ADD(	lazy,				LAZY				)
	ADD(	local,				LOCAL				)
	ADD(	long,				LONG				)

	ADD(	macro,				MACRO				)
	ADD(	mixin,				MIXIN				)
	ADD(	module,				MODULE				)
	ADD(	mutable,			MUTABLE				)

	ADD(	namespace,			NAMESPACE			)
	ADD(	new,				NEW					)
	ADD(	nil,				NIL					)
	ADD(	nothrow,			NOTHROW				)
	ADD(	null,				NULL				)

	ADD(	operator,			OPERATOR			)
	ADD(	or,					OR					)
	ADD(	out,				OUT					)
	ADD(	override,			OVERRIDE			)

	ADD(	package,			PACKAGE				)
	ADD(	partial,			PARTIAL				)
	ADD(	pragma,				PRAGMA				)
	ADD(	private,			PRIVATE				)
	ADD(	program,			PROGRAM				)
	ADD(	protected,			PROTECTED			)
	ADD(	public,				PUBLIC				)
	ADD(	pure,				PURE				)

	ADD(	raise,				RAISE				)
	ADD(	real,				REAL				)
	ADD(	ref,				REF					)
	ADD(	reinterpret_cast,	REINTERPRET_CAST	)
	ADD(	return,				RETURN				)

	ADD(	script,				SCRIPT				)
	ADD(	scope,				SCOPE				)
	ADD(	sealed,				SEALED				)
	ADD(	select,				SELECT				)
	ADD(	short,				SHORT				)
	ADD(	signed,				SIGNED				)
	ADD(	sizeof,				SIZEOF				)
	ADD(	static,				STATIC				)
	ADD(	static_cast,		STATIC_CAST			)
	ADD(	string,				STRING				)
	ADD(	struct,				STRUCT				)
	ADD(	super,				SUPER				)
	ADD(	switch,				SWITCH				)
	ADD(	synchronized,       SYNCHRONIZED		)

	ADD(	template,			TEMPLATE			)
	ADD(	this,				THIS				)
	ADD(	throw,				THROW				)
	ADD(	true,				TRUE				)
	ADD(	try,				TRY					)
	ADD(	type,				TYPE				)
	ADD( 	typedef,			TYPEDEF				)
	ADD(	typeid,				TYPEID				)
	ADD(	typeof,				TYPEOF				)

	ADD(	ubyte,				UBYTE				)
//11.22.2008 - added "uchar"
	ADD(	uchar,				UBYTE				)
//
	ADD(	ucent,				UCENT				)
	ADD(	uint,				UINT				)
	ADD(	ulong,				ULONG				)
	ADD(	union,				UNION				)
	ADD(	unit,				UNIT				)
	ADD(	unittest,			UNITTEST			)
	ADD(	unsigned,			UNSIGNED			)
	ADD(	uses,				USES				)
	ADD(	using,				USING				)
	ADD(	ushort,				USHORT				)

	ADD(	version,			VERSION				)
	ADD(	virtual,			VIRTUAL				)
	ADD(	void,				VOID				)
	ADD(	volatile,			VOLATILE			)

	ADD(	wchar,				WCHAR				)
	ADD(	while,				WHILE				)
	ADD(	with,				WITH				)

	ADD(	xor,				XOR					)

	keywords[ "__traits" ]	= T__TRAITS;

#undef ADD

	/*
	-----------------------------------------
		Predefined constants
	-----------------------------------------
	*/
	keywords[ "__DATE__" ]		= T__DATE__;
	keywords[ "__FILE__" ]		= T__FILE__;
	keywords[ "__LINE__" ]		= T__LINE__;
	keywords[ "__TIME__" ]		= T__TIME__;
	keywords[ "__TIMESTAMP__" ]	= T__TIMESTAMP__;
	keywords[ "__VENDOR__" ]	= T__VENDOR__;
	keywords[ "__VERSION__" ]	= T__VERSION__;


	/*
	-----------------------------------------
		Preprocessor directives
	-----------------------------------------
	*/
	keywords[ "#define" ]	= PP_DEFINE;
	keywords[ "#include" ]	= PP_INCLUDE;
	keywords[ "#if" ]		= PP_IF;
	keywords[ "#ifdef" ]	= PP_IFDEF;
	keywords[ "#ifndef" ]	= PP_IFNDEF;
	keywords[ "#elif" ]		= PP_ELIF;
	keywords[ "#else" ]		= PP_ELSE;
	keywords[ "#endif" ]	= PP_ENDIF;
	keywords[ "#undef" ]	= PP_UNDEF;
}

//--------------------------------------------------------------------------------

bool Lexer::IsKeyword( const char * text ) {
	return keywords.Get( text );
}

}//end namespace c4