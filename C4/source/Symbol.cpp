/*
======================================
	implementation of symbols, 
	which determine semantic behaviour of ast nodes.
======================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

/*
======================================
	implementations of SymbolTable
	and Scope
======================================
*/
#pragma region "Symbol Table"
SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
	m_hash.Clear(); // remove, but do not delete my symbols!
}

Symbol * SymbolTable::Lookup( const Identifier * id ) {
	Symbol *	value = 0;
	Symbol **	p = & value;

	if ( m_hash.Get( id->Text(), &p ) ) {
		return *p;
	}
	return 0;
}

Symbol * SymbolTable::Lookup( const char * text ) {
	Symbol *	value = 0;
	Symbol **	p = & value;

	if ( m_hash.Get( text, &p ) ) {
		return *p;
	}
	return NULL;
}

// inserts a symbol in table; returns NULL if already there
//
Symbol * SymbolTable::Insert( Symbol * s ) 
{
	Identifier * id = s->GetIdentifier();

	if ( m_hash.Get( id->Text()) ) {
		return NULL;
	}

	m_hash.Set( id->Text(), s );

	return s;
}

const char * SymbolTable::ToChars() const {
	return "Symbol Table";
}
#pragma endregion

/*
===========================================
	Scope::Scope( Scope * parent )

	constructor for non-global scopes
	( i.e. parent != NULL )
===========================================
*/
Scope::Scope( Scope * parent )
	: m_parent( parent )
	, mSymbol		( NULL )
	, storageClass	( SC_UNDEFINED )
	, linkage		( DEFAULT_LINKAGE )
	, protection	( PL_UNDEFINED )
	, enclosingFunction( NULL )
{
	if ( m_parent ) {
		m_parent->AddChild( this );
	}

	m_symTable = new SymbolTable;
	m_symTable->Grab();
}

Scope::~Scope() {
	m_symTable->Drop();

	DeleteChildren();
}

Symbol * Scope::Search( const char * name )
{
	for ( Scope * sc = this; sc; sc = sc->m_parent ) 
	{
		Symbol * sym = sc->m_symTable->Lookup( name );
		if ( sym ) {
			return sym;
		}
	}
	return NULL;
}

Symbol * Scope::Search( const Identifier* id ) {
	return Search( id->Text() );
}

Symbol * Scope::Insert( Symbol * s ) {
	return m_symTable->Insert( s );
}

void Scope::AddChild( Scope * sc ) {
	m_children.Append( sc );
}

void Scope::DeleteChildren() {
	for ( int i = 0; i < m_children.Num(); i++ )
	{
		delete m_children[ i ];
	}
	m_children.Clear();
}

bool Scope::IsGlobal() const {
	return m_parent == NULL;
}

#pragma region "Symbol"

DEFINE_TYPE( Symbol, Object );

Symbol::Symbol( Identifier * id /* =0 */ ) 
	: m_id( id )
{
	GRAB( m_id );
	
	m_iUsageCount = 0;
}

Symbol::~Symbol() {
	DROP( m_id );
}

const char * Symbol::ToChars() const {
	return "Symbol";
}

const char * Symbol::Name() const {
	return m_id ? m_id->Text() : "[anonymous]";
}

bool Symbol::Equals( const Object* other ) const 
{
	if ( ID_SYMBOL != other->GetId() ) {
		return false;
	}

	if ( this == other ) {
		return true;
	}

	Symbol * s = ( Symbol* ) other;
	if ( !m_id && !( s->m_id ) ) {
		return true;
	}
	if ( m_id && m_id->Equals( s->m_id ) ) {
		return true;
	}

	return false;
}

int Symbol::GetId() const {
	return ID_SYMBOL;
}

//Symbol * Symbol::GetParent() {
//	return m_parent;
//}

Location & Symbol::GetLocation() {
	// TODO: remove all methods, returning "const Location&";
	// outsiders won't be able to modify locations anyway.
	return const_cast< Location& >( m_id->GetLocation() ); // <== TODO: remove this kludge
}

Identifier * Symbol::GetIdentifier() {
	return m_id;
}

void Symbol::Resolve( Scope * sc ) {
	c4::common->Print( "symbol '%s' has no semantic routine.", Name() );
}

//uint Symbol::GetSize() const {
//	c4::common->Print( "symbol '%s' has no size.", ToChars() );
//	return 0;
//}
#pragma endregion

#pragma region "Scope Symbol"

DEFINE_TYPE( ScopeSymbol, Symbol );

ScopeSymbol::ScopeSymbol( Scope * enclosing ) 
{
	m_scope = new Scope( enclosing );
}

ScopeSymbol::~ScopeSymbol() {
}

const char * ScopeSymbol::ToChars() const {
	return "Scope Symbol";
}

#pragma endregion

#pragma region "Var Symbol"

DEFINE_TYPE( VarSymbol, Symbol );

VarSymbol::VarSymbol( VarDecl * decl )
	: Symbol( decl->GetName() )
	, m_decl( decl )
	, m_stackOffset( 0 )
{
}

VarSymbol::~VarSymbol() {
}

const char * VarSymbol::ToChars() const {
	return "Var Symbol";
}

#pragma endregion

#pragma region "Function symbol"

DEFINE_TYPE( FunctionSymbol, ScopeSymbol );

FunctionSymbol::FunctionSymbol( Identifier* name, Scope* enclosing )
	: ScopeSymbol( enclosing )
	, m_sizeOfLocals( 0 )

	, m_isBuiltIn( false )
	, m_isVarArg( false )

	, m_iNumArguments( 0 )
	, m_returnType( NULL )

	, m_bHasSideEffects( false )
{
	m_id = name;
	m_id->Grab();

	m_nameInAsm = String("_") + m_id->Text();
}

FunctionSymbol::~FunctionSymbol() {
	DROP( m_returnType );
}

const char * FunctionSymbol::ToChars() const {
	return "Function Symbol";
}

void FunctionSymbol::IncSizeOfLocals( uint32 bytes ) {
	m_sizeOfLocals += bytes;
}

uint32 FunctionSymbol::GetSizeOfLocals() const {
	return m_sizeOfLocals;
}

void FunctionSymbol::SetReturnType( Type * pType ) {
	DROP( m_returnType );
	m_returnType = pType;
	m_returnType->Grab();
}

#pragma endregion

//====================================================

void StringEntry::GetMangledName( String & s ) const 
{
	s += "str_";
	s += m_number;
	s += "_";
	
	s += m_originalName;

	// replace all whitespaces with '_', '!' with '?', etc..

	char * p = s.GetBufferPtr();
	while ( *p )
	{
		switch ( *p )
		{
		case ' ':	
			*p = '_';	break;

		case ',':	
		case '.':	
		case ':':
		case '!':	
			*p = '?';	break;
		}

		++p;
	}
}

//====================================================

DEFINE_TYPE( ModuleSymbol, ScopeSymbol );

ModuleSymbol::ModuleSymbol( const char * name, Scope* enclosing ) 
	: m_name( name )
	, ScopeSymbol( enclosing )
{
}

ModuleSymbol::~ModuleSymbol() {
	m_constData.Clear();
}

const char * ModuleSymbol::ToChars() const {
	return "ModuleSymbol";
}

StringEntry * ModuleSymbol::Add_string_to_data_segment( const String & s )
{
	StringEntry	* pEntry = new StringEntry( s, m_constData.Num() );
	m_constData.Append( pEntry );

	return pEntry;
}

//====================================================

Semantics::Semantics()
	: m_currentFunction( NULL )
{
	m_globalScope = new Scope( NULL );

	m_currentScope = m_globalScope;

	//---------------
	Init();
}

Semantics::~Semantics()
{
	delete m_globalScope;
}

Scope * Semantics::SetContext( Scope* scope ) {
	Scope * prevScope = m_currentScope;
	m_currentScope = scope;
	return prevScope;
}

void Semantics::SetFunction( FunctionDecl * function ) {
	m_currentFunction = function;
}

FunctionDecl * Semantics::CurrentFunction() {
	return m_currentFunction;
}

//===========================================================
#include <common/x86_codegen.h>

void Semantics::Init() 
{
	// getchar()
	{
		FunctionSymbol * pSymbol = new FunctionSymbol( new Identifier("getchar"), m_globalScope );
		pSymbol->Grab();

		pSymbol->m_isBuiltIn = true;
		pSymbol->m_isVarArg = false;

		pSymbol->m_nameInAsm = "__imp__getchar";
		pSymbol->m_clobberedRegs = BIT( rEAX );
		pSymbol->m_iNumArguments = 0;

		pSymbol->m_returnType = new Type_Int32();
		pSymbol->m_returnType->Grab();

		pSymbol->m_bHasSideEffects = true;

		m_globalScope->Insert( pSymbol );
	}

	// printf
	{
		FunctionSymbol * pSymbol = new FunctionSymbol( new Identifier("printf"), m_globalScope );
		pSymbol->Grab();

		pSymbol->m_isBuiltIn = true;
		pSymbol->m_isVarArg = true;

		pSymbol->m_nameInAsm = "__imp__printf";
		pSymbol->m_clobberedRegs = BIT( rEAX );
		//pSymbol->m_iNumArguments = 0;
		pSymbol->m_returnType = new Type_Int32();
		pSymbol->m_returnType->Grab();

		pSymbol->m_bHasSideEffects = true;

		m_globalScope->Insert( pSymbol );
	}
}

}//end namespace c4