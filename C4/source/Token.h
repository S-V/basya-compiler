#ifndef __TOKEN_H__
#define __TOKEN_H__

namespace c4 {

// enum of all the possible token types.
//
enum TOKEN_TYPE
{
	T_END_OF_FILE	= 0,

	/*
	-----------------------------------------
		Literals
	-----------------------------------------
	*/
	// numeric literals
	L_INT32,	L_UINT32,
	L_INT64,	L_UINT64,
	L_FLOAT32,	L_FLOAT64,	L_FLOAT80,

	// char constants
	L_CHAR,		L_WCHAR,	L_DCHAR,

	// leaf operators
	L_IDENTIFIER,		L_STRING,
	L_THIS,				L_SUPER,
	L_HALT,				L_TUPLE,

	// basic types
	TT_VOID,
	TT_BIT,		TT_BOOL,
	TT_CHAR,	TT_WCHAR,	TT_DCHAR, // dchar is unsigned 32-bit UTF value

	T_INT8,		T_UINT8,
	T_INT16,	T_UINT16,
	T_INT64,	T_UINT64,
	T_FLOAT32,	T_FLOAT64,	T_FLOAT80,

	TT_CENT,	TT_UCENT,	// cent is signed 128 bits

	/*
	-----------------------------------------
		Reserved keywords
	-----------------------------------------
	*/
	T_ABSTRACT,
	T_ALIAS,
	T_ALIGN,
	T_AND,
	T_AS,
	T_ASM,
	T_ASSERT,
	T_AUTO,

	T_BODY,
	T_BOOL,
	T_BREAK,
	T_BYTE,

	T_CASE,
	T_CAST,
	T_CATCH,
	T_CENT,
	T_CHAR,
	T_CLASS,
	T_CONST,
	T_CONST_CAST,
	T_CONTINUE,

	T_DCHAR,
	T_DEBUG,
	T_DEFAULT,
	T_DELEGATE,
	T_DELETE,
	T_DEPRECATED,
	T_DO,
	T_DOUBLE,

	T_ELIF,
	T_END,
	T_ENDIF,
	T_ELSE,
	T_ENUM,
	T_EXPORT,
	T_EXTERN,

	T_FALSE,
	T_FINAL,
	T_FINALLY,
	T_FLOAT,
	T_FOR,
	T_FOREACH,
	T_FOREACH_REVERSE,
	T_FRIEND,
	T_FUNCTION,

	T_GOTO,

	T_IF,
	T_IMPLICIT,
	T_IMPORT,
	T_IN,
	T_INOUT,
	T_INT,
	T_INTERFACE,
	T_INTERNAL,
	T_INVARIANT,
	T_IS,

	T_LAZY,
	T_LOCAL,
	T_LONG,

	T_MACRO,
	T_MIXIN,
	T_MODULE,
	T_MUTABLE,

	T_NAMESPACE,
	T_NEW,
	T_NOTHROW,
	T_NIL,
	T_NULL,

	T_OPERATOR,
	T_OR,
	T_OUT,
	T_OVERRIDE,

	T_PACKAGE,
	T_PARTIAL,
	T_PRAGMA,
	T_PRIVATE,
	T_PROGRAM,
	T_PROTECTED,
	T_PUBLIC,
	T_PURE,

	T_RAISE,
	T_REAL,
	T_REF,
	T_REINTERPRET_CAST,
	T_RETURN,

	T_SCRIPT,
	T_SCOPE,
	T_SEALED,
	T_SELECT,
	T_SHORT,
	T_SIGNED,
	T_SIZEOF,
	T_STATIC,
	T_STATIC_CAST,
	T_STRING,
	T_STRUCT,
	T_SUPER,
	T_SWITCH,
	T_SYNCHRONIZED,

	T_TEMPLATE,	
	T_THIS,		
	T_THROW,		
	T_TRUE,		
	T_TRY,			
	T_TYPE,		
	T_TYPEDEF,		
	T_TYPEID,		
	T_TYPEOF,		

	T_UBYTE,		
	T_UCENT,		
	T_UINT,		
	T_ULONG,		
	T_UNION,		
	T_UNIT,	
	T_UNITTEST,	
	T_UNSIGNED,
	T_USES,		
	T_USING,		
	T_USHORT,		

	T_VERSION,
	T_VIRTUAL,
	T_VOID,		
	T_VOLATILE,	

	T_WCHAR,	
	T_WHILE,		
	T_WITH,	

	T_XOR,

	T__TRAITS,

	/*
	-----------------------------------------
		Special tokens
	-----------------------------------------
	*/
	T__DATE__,			// string literal of the date of compilation "mmm dd yyyy"
	T__FILE__,			// string literal of the name of the compiled file
	T__LINE__,			// integer literal equal to the current line
	T__TIME__,			// string literal of the time of compilation "hh:mm:ss"
	T__TIMESTAMP__,		// string literal of the date and time of compilation "www mmm dd hh:mm:ss yyyy"
	T__VENDOR__,		// Compiler vendor string, such as "Student Software(R)"
	T__VERSION__,		// Compiler version as an integer, such as 2008

	/*
	-----------------------------------------
		Preprocessor directives
	-----------------------------------------
	*/
	PP_DEFINE,
	PP_INCLUDE,
	PP_IF,
	PP_IFDEF,
	PP_IFNDEF,
	PP_ELIF,
	PP_ELSE,
	PP_ENDIF,
	PP_UNDEF,

	/*
	-----------------------------------------
		Punctuation
	-----------------------------------------
	*/
	P_AMPERSAND,					//	"&" 
	P_AMPERSAND_AMPERSAND,			//	"&&" 
	P_AMPERSAND_EQUAL,				//	"&=" 
	P_ASTERISK,						//	"*" 
	P_ASTERISK_EQUAL,				//	"*=" 
	P_AT,							//	"@" 
	P_AT_EQUAL,						//	"@=" 
	P_BACKSLASH,					//	"\=" 
	P_CARET,						//	"^" 
	P_CARET_EQUAL,					//	"^=" 
	P_COLON,						//	":" 
	P_COLON_COLON,					//	"::" 
	P_COMMA,						//	"," 
	P_DOLLAR,						//  "$"
	P_DOT,							//	"." 
	P_DOT_DOT_DOT,					//	"..." 
	P_EQUAL,						//	"=" 
	P_EQUAL_EQUAL,					//	"==" 
	P_EXCLAMATION,					//	"!" 
	P_GREATER,						//	">" 
	P_GREATER_EQUAL,				//	">=" 
	P_GREATER_GREATER,				//	">>" 
	P_GREATER_GREATER_EQUAL,		//	">>=" 
	P_LEFT_BRACE,					//	"{" 
	P_LEFT_BRACKET,					//	"[" 
	P_LEFT_PARENTH,					//	"(" 
	P_LESS,							//	"<" 
	P_LESS_EQUAL,					//	"<=" 
	P_LESS_LESS,					//	"<<" 
	P_LESS_LESS_EQUAL,				//	"<<=" 
	P_MINUS,						//	"-" 
	P_MINUS_EQUAL,					//	"-=" 
	P_MINUS_GREATER,				//	"->" 
	P_MINUS_MINUS,					//	"--" 
	P_NON_EQUAL,					//	"!=" 
	P_PERCENT,						//	"%" 
	P_PERCENT_EQUAL,				//	"%=" 
	P_PLUS,							//	"+" 
	P_PLUS_EQUAL,					//	"+=" 
	P_PLUS_PLUS,					//	"++" 
	P_QUESTION,						//	"?" 
	P_RANK_SPECIFIER,				//	"[]"  // [,] [,,] ...
	P_RIGHT_BRACE,					//	"}" 
	P_RIGHT_BRACKET,				//	"]" 
	P_RIGHT_PARENTH,				//	"" 
	P_SEMICOLON,					//	";" 
	P_SHARP,						//	"#" 
	P_SHARP_SHARP,					//	"##" 
	P_SLASH,						//	"/" 
	P_SLASH_EQUAL,					//	"/=" 
	P_TILDE,						//	"~" 
	P_TILDE_EQUAL,					//	"~="
	P_UNSIGNED_RIGHT_SHIFT,			//  ">>>"
	P_UNSIGNED_RIGHT_SHIFT_EQUAL,	//  ">>>="
	P_VERTICAL,						//	"|" 
	P_VERTICAL_EQUAL,				//	"|=" 
	P_VERTICAL_VERTICAL,			//	"||" 

	NUM_OF_TOKENS
};

/*
======================================================
	class Location
		for tracking locations in a source file.
======================================================
*/

class Location {
public:
					Location();
					Location( const Location & other );
					~Location();

	int				GetLine() const;
	int				GetEndLine() const;
	int				GetColumn() const;
	int				GetEndColumn() const;

	const char *	Filename() const;

	Location &		operator = ( const Location &other );

private:
	friend class Lexer;

	int			line, endLine;
	int			column, endColumn;
	String		filename;
};

INLINE Location::Location()
	: line( 1 )
	, endLine( 1 )
	, column( 1 )
	, endColumn( 1 )
{
}

INLINE Location::Location( const Location & other )
	: line( other.line )
	, endLine( other.endLine )
	, column( other.column )
	, endColumn( other.endColumn )
	, filename( other.filename )
{
}

INLINE Location::~Location() {
}

INLINE int Location::GetLine() const {
	return line;
}

INLINE int Location::GetEndLine() const {
	return endLine;
}

INLINE int Location::GetColumn() const {
	return column;
}

INLINE int Location::GetEndColumn() const {
	return endColumn;
}

INLINE const char * Location::Filename() const {
	return filename.c_str();
}

INLINE Location & Location::operator = ( const Location & other ) {
	line	= other.line;
	endLine = other.endLine;
	column	= other.column;
	endColumn = other.endColumn;
	filename = other.filename;

	return *this;
}

/*
==========================================================================
	class Token
		
		is a single lexeme from a source file.
	These are produced by the lexer and are linked into the token list.
==========================================================================
*/
class Token {
				Token();
				Token( const Token & other );
				Token( const char *text, int type );
				~Token();
public:

	bool			IsKeyword();
	const char *	ToChars() const;
	
	static const char *	ToChars( int type );

	int				Value() const;
	const char *	Text() const;
	Location &		GetLocation();

	Token *			NextToken();
	Token *			PrevToken();

private:
	friend class Lexer;

	int				m_type;	    // value of this token ( enum TOKEN_TYPE )
	String			m_text;	    // raw text of this token
	Location		m_location; // position of this token in a source file

	friend class TokenList;

	Token *		m_next;
	Token *		m_prev;

private:
	static const char * token_data[ NUM_OF_TOKENS ];
};

//===================================================

class TokenList {
public:
				TokenList();
				~TokenList();

	void		Append( Token * token );
	int			Num() const;
	void		DeleteAll();

	Token *		Head();
	Token *		Tail();

private:
	int			m_num;
	Token *		m_head;
	Token *		m_tail;

	friend class Lexer;
};

/*
===============================================
	class StringBuffer

	used by the lexer to store string data.
===============================================
*/
class StringBuffer : public String {
public:
				StringBuffer( int initialAlloc = 1024 );
				~StringBuffer();

	void		Reset();
	char *		Ptr();
};

}//end namespace c4

#endif // !__TOKEN_H__