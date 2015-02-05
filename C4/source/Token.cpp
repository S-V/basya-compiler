#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

//===================================================
Token::Token()
	: m_type( 0 )
	, m_next( 0 )
	, m_prev( 0 )
{
}

Token::Token( const Token & other )
	: m_type( other.m_type )
	, m_text( other.m_text )
	, m_next( 0 )
	, m_prev( 0 )
{
}

Token::Token( const char *text, int type )
	: m_text( text )
	, m_type( type )
	, m_next( 0 )
	, m_prev( 0 )
{
}

Token::~Token() {
}

bool Token::IsKeyword() {
	return Lexer::IsKeyword( m_text.c_str() );
}

const char * Token::ToChars() const {
	return m_text.c_str();
}

Token * Token::NextToken() {
	return m_next;
}

Token * Token::PrevToken() {
	return m_prev;
}

int Token::Value() const {
	return m_type;
}

const char * Token::Text() const {
	return m_text.c_str();
}

Location & Token::GetLocation() {
	return m_location;
}

//===================================================

TokenList::TokenList()
	: m_num( 0 )
	, m_head( 0 )
	, m_tail( 0 )
{
}

TokenList::~TokenList() {
	DeleteAll();
}

void TokenList::Append( Token * token ) {
	if ( !m_head ) {
		m_head = token;
	}
	
	token->m_prev = m_tail;

	if ( m_tail ) {
		m_tail->m_next = token;
	}
	m_tail = token;

	++m_num;
}

int TokenList::Num() const {
	return m_num;
}

void TokenList::DeleteAll() {
	Token * token = m_head;
	while ( token ) {
		Token * next = token->m_next;
		delete token;
		token = next;
	}

	m_head = NULL;
	m_tail = NULL;
	m_num = 0;
}

Token * TokenList::Head() {
	return m_head;
}

Token * TokenList::Tail() {
	return m_tail;
}

/*
=========================
	statics
=========================
*/
const char* Token::token_data[ NUM_OF_TOKENS ] = {
	"<EOF>",

	"int32 literal",
	"uint32 literal" ,
	"int64 literal" ,
	"uint64 literal" ,
	"float32 literal" ,
	"float64 literal" ,
	"float80 literal" ,
	
	"char literal" ,
	"wchar literal" ,
	"dchar literal" ,
	
	"identifier" ,
	"string leaf operator" ,
	"this leaf operator" ,
	"super leaf operator" ,
	"halt leaf operator" ,
	"tuple leaf operator" ,

	// basic types
	"basic type: void" ,
	"basic type: bit" ,
	"basic type: bool" ,
	"basic type: char" ,
	"basic type: wchar" ,
	"basic type: dchar" ,	//22

	"basic type: int8" ,
	"basic type: uint8" ,
	"basic type: int16" ,
	"basic type: uint16" ,
	"basic type: int64" ,
	"basic type: uint64" ,
	"basic type: float32" ,
	"basic type: float64" ,
	"basic type: float80" ,
	"basic type: cent" ,
	"basic type: ucent" ,	//33

	// keywords
	//
	"keyword: abstract" ,
	"keyword: alias" ,
	"keyword: align" ,
	"keyword: and" ,
	"keyword: as" ,
	"keyword: asm" ,
	"keyword: assert" ,
	"keyword: auto" ,

	"keyword: body" ,
	"keyword: bool" ,
	"keyword: break" ,
	"keyword: byte" ,

	"keyword: case" ,
	"keyword: cast" ,
	"keyword: catch" ,
	"keyword: cent" ,
	"keyword: char" ,
	"keyword: class" ,
	"keyword: const" ,
	"keyword: const_cast" ,
	"keyword: continue" ,

	"keyword: dchar" ,
	"keyword: debug" ,
	"keyword: default" ,
	"keyword: delegate" ,
	"keyword: delete" ,
	"keyword: deprecated" ,
	"keyword: do" ,
	"keyword: double" ,

	"keyword: elif" ,		//63
	"keyword: end" ,
	"keyword: endif" ,
	"keyword: else" ,
	"keyword: enum" ,
	"keyword: export" ,
	"keyword: extern" ,

	"keyword: false" ,
	"keyword: final" ,
	"keyword: finally" ,
	"keyword: float" ,
	"keyword: for" ,
	"keyword: foreach" ,
	"keyword: foreach_reverse" ,
	"keyword: friend" ,
	"keyword: function" ,

	"keyword: goto" ,

	"keyword: if" ,
	"keyword: implicit" ,
	"keyword: import" ,
	"keyword: in" ,
	"keyword: inout" ,
	"keyword: int" ,
	"keyword: interface" ,
	"keyword: internal" ,
	"keyword: invariant" ,
	"keyword: is" ,

	"keyword: lazy" ,
	"keyword: local" ,
	"keyword: long" ,		//92

	"keyword: macro" ,
	"keyword: mixin" ,
	"keyword: module" ,
	"keyword: mutable" ,

	"keyword: namespace" ,
	"keyword: new" ,
	"keyword: nothrow" ,
	"keyword: nil" ,
	"keyword: null" ,

	"keyword: operator" ,
	"keyword: or" ,
	"keyword: out" ,
	"keyword: override" ,

	"keyword: package" ,
	"keyword: partial" ,
	"keyword: pragma" ,
	"keyword: private" ,
	"keyword: program" ,
	"keyword: protected" ,
	"keyword: public" ,
	"keyword: pure" ,

	"keyword: raise" ,
	"keyword: real" ,
	"keyword: ref" ,
	"keyword: reinterpret_cast" ,
	"keyword: return" ,

	"keyword: script" ,
	"keyword: scope" ,
	"keyword: sealed" ,
	"keyword: select" ,
	"keyword: short" ,
	"keyword: signed" ,
	"keyword: sizeof" ,
	"keyword: static" ,
	"keyword: static_cast" ,
	"keyword: string" ,
	"keyword: struct" ,
	"keyword: super" ,
	"keyword: switch" ,
	"keyword: synchronized" ,

	"keyword: template" ,
	"keyword: this" ,
	"keyword: throw" ,
	"keyword: true" ,
	"keyword: try" ,
	"keyword: type" ,
	"keyword: typedef" ,
	"keyword: typeid" ,
	"keyword: typeof" ,

	"keyword: ubyte" ,
	"keyword: ucent" ,
	"keyword: uint" ,
	"keyword: ulong" ,
	"keyword: union" ,
	"keyword: unit" ,
	"keyword: unittest" ,
	"keyword: unsigned" ,
	"keyword: uses" ,
	"keyword: using" ,
	"keyword: ushort" ,

	"keyword: version" ,
	"keyword: virtual" ,
	"keyword: void" ,
	"keyword: volatile" ,

	"keyword: wchar" ,
	"keyword: while" ,
	"keyword: with" ,

	"keyword: xor" ,

	"keyword: __traits" ,



	"special token: __DATE__"				,
	"special token: __FILE__"				,
	"special token: __LINE__"				,
	"special token: __TIME__"				,
	"special token: __TIMESTAMP__"			,
	"special token: __VENDOR__"				,
	"special token: __VERSION__"			,

	
	
	"preprocessor: #define"					,
	"preprocessor: #include"				,
	"preprocessor: #if"						,
	"preprocessor: #ifdef"					,
	"preprocessor: #ifndef"					,
	"preprocessor: #elif"					,
	"preprocessor: #else"					,
	"preprocessor: #endif"					,
	"preprocessor: #undef"					,

	
	
	"punctuation: '&'"			,
	"punctuation: '&&'"			,
	"punctuation: '&='"			,
	"punctuation: '*'"			,
	"punctuation: '*='"			,
	"punctuation: '@'" 			,
	"punctuation: '@='" 		,
	"punctuation: '\\'" 		,
	"punctuation: '^'" 			,
	"punctuation: '^='" 		,
	"punctuation: ':'" 			,
	"punctuation: '::'"			,
	"punctuation: ','"			,
	"punctuation: '$'" 			,
	"punctuation: '.'" 			,
	"punctuation: '...'" 		,
	"punctuation: '='" 			,
	"punctuation: '=='" 		,
	"punctuation: '!'" 			,
	"punctuation: '>'"			,
	"punctuation: '>='" 		,
	"punctuation: '>>'" 		,
	"punctuation: '>>='" 		,
	"punctuation: '{'" 			,
	"punctuation: '['" 			,
	"punctuation: '('" 			,
	"punctuation: '<'" 			,
	"punctuation: '<='" 		,
	"punctuation: '<<'" 		,
	"punctuation: '<<='" 		,
	"punctuation: '-'" 			,
	"punctuation: '-='" 		,
	"punctuation: '->'" 		,
	"punctuation: '--'" 		,
	"punctuation: '!='" 		,
	"punctuation: '%'" 			,
	"punctuation: '%='" 		,
	"punctuation: '+'" 			,
	"punctuation: '+='" 		,
	"punctuation: '++'" 		,
	"punctuation: '?'" 			,
	"punctuation: '[]'" 		,
	"punctuation: '}'" 			,
	"punctuation: ']'" 			,
	"punctuation: ')'"			,
	"punctuation: ';'" 			,
	"punctuation: '#'" 			,
	"punctuation: '##'" 		,
	"punctuation: '/'"			,
	"punctuation: '/='"			,
	"punctuation: '~'"			,
	"punctuation: '~='"			,
	"punctuation: '>>>'"		,
	"punctuation: '>>>='"		,
	"punctuation: '|'"			,
	"punctuation: '|='"			,
	"punctuation: '||'"			,
};	

const char * Token::ToChars( int token ) {
	return token_data[ token ];
}

//===================================================

StringBuffer::StringBuffer( int initialAlloc /* =1024 */ ) {
}

StringBuffer::~StringBuffer() {
}

void StringBuffer::Reset() {
	Clear();
}

char * StringBuffer::Ptr() {
	return const_cast< char* >( data );
}

}//end namespace c4
