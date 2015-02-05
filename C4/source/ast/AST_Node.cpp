/*
======================================
	Implementation of AST_Node,
	the base class for all ast nodes.
======================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

DEFINE_TYPE( AST_Node, Object );

AST_Node::AST_Node( UINode* parent /* =0 */ )
	: UINode( parent )
	, m_detailedInfo( "AST_Node, the base class for all ast nodes" )
{
	++numASTnodes;
}

AST_Node::AST_Node( const Location &location, UINode* parent /* =0 */ ) 
	: UINode( parent )
	, location( location )
{
	this->location = location;

	++numASTnodes;
}

AST_Node::~AST_Node()
{
	--numASTnodes;
}

int AST_Node::GetId() const {
	return ID_PARSE_TREE;
}

const char * AST_Node::uiToString() const {
	return ToChars();
}

void AST_Node::UpdateUIinfo() {
}

const char * AST_Node::uiGetInfo() const {
	return m_detailedInfo.c_str();
}

//void AST_Node::ResolveInto( Semantics* semantics ) {
//}

void AST_Node::Generate( CodeGenerator * cg ) {
}

void AST_Node::Accept( ITreeVisitor * visitor ) {
	visitor->Visit( this );
}

void AST_Node::Print( const char * format, ... )
{
	c4::common->Print( "Line: %d, column: %d :", location.GetLine(), location.GetColumn() );

	va_list	 argptr;
	char  buf[ 512 ];

	va_start( argptr, format );
	vsprintf_s( buf, 512, format, argptr );
	va_end( argptr );

	c4::common->Print( buf );
}

void AST_Node::Error( const char * format, ... )
{
	// TODO: fix this later ( when location is initialized in ctor during parsing )
	c4::common->Print( "Error( %d, %d ): ", location.GetLine(), location.GetColumn() );

	va_list	 argptr;
	char  buf[ 512 ];

	va_start( argptr, format );
	vsprintf_s( buf, 512, format, argptr );
	va_end( argptr );

	c4::compiler->Error( buf );
}

void AST_Node::Warning( const char * format, ... )
{
	// TODO: fix this later ( when location is initialized in ctor during parsing )
	c4::common->Print( "Warning( %d, %d ): ", location.GetLine(), location.GetColumn() );
	
	va_list	 argptr;
	char  buf[ 512 ];

	va_start( argptr, format );
	vsprintf_s( buf, 512, format, argptr );
	va_end( argptr );

	c4::compiler->Warning( buf );
}

//==========================================================================
// statics
//
ulong	AST_Node::numASTnodes = 0;

ulong AST_Node::GetNumNodes() {
	return numASTnodes;
}

void AST_Node::PrintStatistics() {
	c4::common->Print( "************ Total num. of ast nodes: %u ************************", numASTnodes );
}

}//end namespace c4