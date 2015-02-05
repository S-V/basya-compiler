#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

Identifier::Identifier( const char * text )
	: m_text( text )
{
}

Identifier::~Identifier() {
}

bool Identifier::Equals( const Object * other ) const {
	return ( this == other ) || ( m_text == other->ToChars() );
}

hash_t Identifier::GetHash() const {
	return String::Hash( m_text );
}

int Identifier::Compare( const Object * other ) const {
	return m_text.Cmp( other->ToChars() );
}

int Identifier::GetId() const {
	return ID_IDENTIFIER;
}

const char * Identifier::ToChars() const { 
	return m_text.c_str(); 
}

#pragma region "UI"
int Identifier::uiNumChildren() const {
	return 0;
}

UINode * Identifier::uiChildAt( int index ) {
	return 0;
}

int Identifier::uiIndexOf( const UINode *child ) const {
	return -1;
}
#pragma endregion

const char * Identifier::Text() const {
	return m_text.c_str();
}

}//end namespace c4