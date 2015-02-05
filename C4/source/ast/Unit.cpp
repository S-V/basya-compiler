#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

DEFINE_TYPE( CompilationUnit, AST_Node );

CompilationUnit::CompilationUnit( const char * name )
	: m_name( name )
	, m_symbol( NULL )
{
}

CompilationUnit::~CompilationUnit() 
{
	for ( int i = 0; i < m_decls.Num(); i++ )
	{
		m_decls[i]->Drop();
	}
	m_decls.Clear();

	DROP( m_symbol );
}

void CompilationUnit::Add( Declaration * decl ) 
{
	m_decls.Append( decl );
	decl->Grab();

	decl->uiSetParent( this );
}

const char * CompilationUnit::ToChars() const {
	return "Compilation Unit";
}

#pragma region "UI"
int CompilationUnit::uiNumChildren() const {
	return m_decls.Num();
}

UINode * CompilationUnit::uiChildAt( int index )
{
	if ( index < 0 || index >= m_decls.Num() ) {
		return 0;
	}
	return m_decls[ index ];
}

int CompilationUnit::uiIndexOf( const UINode *child ) const
{
	Declaration * decl = ( Declaration* )( child );
	return m_decls.FindIndex( decl );
}
#pragma endregion

void CompilationUnit::Resolve( Semantics * semantics ) {
	if ( m_decls.Num() == 0 ) {
		c4::compiler->Error( "Invalid input" );
		return;
	}

	if ( m_symbol ) {
		c4::common->Print( "File '%s': semantic checking has already been done.", m_name.c_str() );
		return;
	}

	// create root scope
	m_symbol = new ModuleSymbol( "test.c", semantics->GetGlobal() );
	m_symbol->Grab();

	// resolve each declaration
	for ( int i = 0; i < m_decls.Num(); i++ )
	{
		m_decls[ i ]->Resolve( semantics );
	}
}

}//end namespace c4