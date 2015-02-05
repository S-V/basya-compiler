/*
======================================
	implementation of ast nodes,
	representing declarations
( var. decl, function decl., class declarations, etc.)
======================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

#pragma region "Utils to convert enum constants to strings"

const char * ToStr( STORAGE_CLASS sc )
{
	switch ( sc ) {
		case SC_UNDEFINED:			return "undefined";
		case SC_STATIC:				return "static";
		case SC_EXTERN:				return "extern";
		case SC_CONST:				return "const";
		case SC_FINAL:				return "final";
		case SC_ABSTRACT:			return "abstract";
		case SC_PARAMETER:			return "parameter";
		case SC_FIELD:				return "field";
		case SC_OVERRIDE:			return "override";
		case SC_AUTO:				return "auto";
		case SC_SYNCHRONIZED:		return "synchronized";
		case SC_DEPRECATED:			return "deprecated";
		case SC_IN:					return "in";
		case SC_OUT:				return "out";
		case SC_LAZY:				return "lazy";
		case SC_FOREACH:			return "foreach";
		case SC_COMDAT:				return "COMDAT";
		case SC_VARIADIC:			return "variadic";
		case SC_CTORINIT:			return "ctorinit";
		case SC_TEMPLATE_PARAMETER:	return "template_parameter";
		case SC_SCOPE:				return "scope";
		case SC_INVARIANT:			return "invariant";
		case SC_REF:				return "ref";
		case SC_INIT:				return "explicit_initializer";
		case SC_MANIFEST:			return "manifest";
		case SC_NODTOR:				return "nodtor";
		case SC_NOTHROW:			return "nothrow";
		case SC_PURE:				return "pure";
		case SC_THREAD_LOCAL:		return "threadlocal";
		case SC_VIRTUAL:			return "virtual";

		default:	Unreachable();
	}
	return NULL; // prevent compiler from complaining ( "not all control paths return a value" )
}

const char * ToStr( LINKAGE lk )
{
	switch ( lk ) {
		case DEFAULT_LINKAGE:	return "default_linkage";
		case LINK_C:		return "C";
		case LINK_C4:		return "C4";
		case LINK_CPP:		return "C++";
		case LINK_STDCALL:	return "stdcall";
		case LINK_PASCAL:	return "pascal";

		default:		Unreachable();
	}
	return NULL;
}

const char * ToStr( PROTECTION_LEVEL pl )
{
	switch ( pl ) {
		case PL_NONE:		return "no_access";
		case PL_PRIVATE:	return "private";
		case PL_INTERNAL:	return "internal";
		case PL_PACKAGE:	return "package";
		case PL_PROTECTED:	return "protected";
		case PL_PUBLIC:		return "public";
		case PL_EXPORT:		return "export";
		case PL_UNDEFINED:	return "undefined_protection_level";

		default:		Unreachable();
	}
	return NULL;
}

void ToStr( STORAGE_CLASS  m_storageClass, String & outStr ) 
{
	if ( m_storageClass == SC_UNDEFINED ) {
		outStr += "undefined_storage_class;";
		return;
	}
	if ( m_storageClass & SC_STATIC ) {
		outStr += "static;";
	}
	if ( m_storageClass & SC_EXTERN ) {
		outStr += "extern;";
	}
	if ( m_storageClass & SC_CONST ) {
		outStr += "const;";
	}
	if ( m_storageClass & SC_FINAL ) {
		outStr += "final;";
	}
	if ( m_storageClass & SC_ABSTRACT ) {
		outStr += "abstract;";
	}
	if ( m_storageClass & SC_PARAMETER ) {
		outStr += "parameter;";
	}
	if ( m_storageClass & SC_FIELD ) {
		outStr += "field;";
	}
	if ( m_storageClass & SC_OVERRIDE ) {
		outStr += "override;";
	}
	if ( m_storageClass & SC_AUTO ) {
		outStr += "auto;";
	}
	if ( m_storageClass & SC_SYNCHRONIZED ) {
		outStr += "synchronized;";
	}
	if ( m_storageClass & SC_DEPRECATED ) {
		outStr += "deprecated;";
	}
	if ( m_storageClass & SC_IN ) {
		outStr += "in;";
	}
	if ( m_storageClass & SC_OUT ) {
		outStr += "out;";
	}
	if ( m_storageClass & SC_LAZY ) {
		outStr += "lazy;";
	}
	if ( m_storageClass & SC_FOREACH ) {
		outStr += "foreach;";
	}
	if ( m_storageClass & SC_COMDAT ) {
		outStr += "COMDAT;";
	}
	if ( m_storageClass & SC_VARIADIC ) {
		outStr += "variadic;";
	}
	if ( m_storageClass & SC_CTORINIT ) {
		outStr += "ctorinit;";
	}
	if ( m_storageClass & SC_TEMPLATE_PARAMETER ) {
		outStr += "template_parameter;";
	}
	if ( m_storageClass & SC_SCOPE ) {
		outStr += "scope;";
	}
	if ( m_storageClass & SC_INVARIANT ) {
		outStr += "invariant;";
	}
	if ( m_storageClass & SC_REF ) {
		outStr += "ref;";
	}
	if ( m_storageClass & SC_INIT ) {
		outStr += "explicit_init;";
	}
	if ( m_storageClass & SC_MANIFEST ) {
		outStr += "manifest;";
	}
	if ( m_storageClass & SC_NODTOR ) {
		outStr += "nodtor;";
	}
	if ( m_storageClass & SC_NOTHROW ) {
		outStr += "nothrow;";
	}
	if ( m_storageClass & SC_PURE ) {
		outStr += "pure;";
	}
	if ( m_storageClass & SC_THREAD_LOCAL ) {
		outStr += "thread_local;";
	}
	if ( m_storageClass & SC_VIRTUAL ) {
		outStr += "virtual;";
	}
}

void ToStr( LINKAGE  m_linkage, String & outStr ) 
{
	switch ( m_linkage ) {
		case DEFAULT_LINKAGE :	outStr += "default_linkage;";	break;
		case LINK_C :			outStr += "C;";					break;
		case LINK_C4 :			outStr += "C4;";				break;
		case LINK_CPP :			outStr += "C++;";				break;
		case LINK_STDCALL :		outStr += "__stdcall;";			break;
		case LINK_PASCAL :		outStr += "PASCAL;";			break;
		default:			Unreachable();
	}
}

void ToStr( PROTECTION_LEVEL  m_protection, String & outStr ) 
{
	switch ( m_protection ) {
		case PL_UNDEFINED:
			outStr += "undefined_protection_level;";
			break;

		case PL_NONE:
			outStr += "no_access;";
			break;

		case PL_PRIVATE:
			outStr += "private;";
			break;

		case PL_INTERNAL:
			outStr += "internal;";
			break;

		case PL_PACKAGE:
			outStr += "package;";
			break;

		case PL_PROTECTED:
			outStr += "protected;";
			break;

		case PL_PUBLIC:
			outStr += "public;";
			break;

		case PL_EXPORT:
			outStr += "export;";
			break;

		default:
			Unreachable();
	}
}
#pragma endregion

#pragma region "Declaration"

DEFINE_TYPE( Declaration, AST_Node );

Declaration::Declaration()
	: m_type( NULL )
	, m_id( NULL )
	, m_storageClass( SC_UNDEFINED )
	, m_protection( PL_UNDEFINED )
	, m_linkage( DEFAULT_LINKAGE )
{
	m_detailedInfo = "null type";
}

Declaration::Declaration( Type* type, Identifier* name )
	: m_type( type )
	, m_id( name )
	, m_storageClass( SC_UNDEFINED )
	, m_protection( PL_UNDEFINED )
	, m_linkage( DEFAULT_LINKAGE )
{
	// reference counting
	m_type->Grab();
	m_id->Grab();

	// ui
	m_type->uiSetParent( this );
	m_id->uiSetParent( this );

	m_detailedInfo = "Type: ";
	m_detailedInfo += m_type->mInfo;
}

Declaration::~Declaration()
{
	DROP( m_type );
	DROP( m_id );
}

void Declaration::ToString( String & s )
{
}

//-------------------------------------------------------

void Declaration::Resolve( Semantics* semantics ) {
	Unreachable();
}

void Declaration::UpdateUIinfo() 
{
	// 1) storage
	m_detailedInfo = "Storage class: ";
	ToStr( ( STORAGE_CLASS )m_storageClass, m_detailedInfo );
	m_detailedInfo += '\n';

	// 2) m_linkage
	m_detailedInfo += "Linkage: ";
	ToStr( ( LINKAGE )m_linkage, m_detailedInfo );
	m_detailedInfo += '\n';

	// 3) access level
	m_detailedInfo += "Protection: ";
	ToStr( ( PROTECTION_LEVEL )m_protection, m_detailedInfo );
	m_detailedInfo += '\n';

	// 4) type
	m_detailedInfo += "Type: ";
	if ( m_type ) {
		m_detailedInfo += m_type->mInfo;
	} else {
		m_detailedInfo += "null type";
	}
	m_detailedInfo += '\n';
}

#pragma endregion

//--------------------------------------------------------------------------------

#pragma region "Declaration List"

DEFINE_TYPE( DeclarationList, Declaration );

DeclarationList::DeclarationList()
{
}

DeclarationList::~DeclarationList() {
	for ( int i = 0; i < m_decls.Num(); i++ ) 
	{
		m_decls[ i ]->Drop();
	}
	m_decls.Clear();
}

void DeclarationList::Add( Declaration * decl ) {
	m_decls.Append( decl );
	decl->Grab();

	// ui
	decl->uiSetParent( this );
}

const char * DeclarationList::ToChars() const {
	return "Declaration List";
}

void DeclarationList::ToString( String & s ) {
}

#pragma region "UI"
const char * DeclarationList::uiToString() const {
	return "Declaration List";
}

int DeclarationList::uiNumChildren() const {
	return m_decls.Num();
}

UINode * DeclarationList::uiChildAt( int index )
{
	if ( index < 0 || index > m_decls.Num() ) {
		return 0;
	}
	return m_decls[ index ];
}

int DeclarationList::uiIndexOf( const UINode *child ) const
{
	Declaration * decl = ( Declaration* )( child );
	return m_decls.FindIndex( decl );
}
#pragma endregion

void DeclarationList::Resolve( Semantics* semantics )
{
	for ( int i = 0; i < m_decls.Num(); i++ ) 
	{
		m_decls[ i ]->Resolve( semantics );
	}
}
#pragma endregion

#pragma region "Variable declaration"

DEFINE_TYPE( VarDecl, Declaration );

VarDecl::VarDecl( Type* type, Identifier* name, Expression* init /* =0 */ )
	: Declaration( type, name )
	, m_initializer( init )
	, m_symbol( NULL )
{
	if ( m_initializer ) {
		m_initializer->Grab();

		m_initializer->uiSetParent( this );
	}
	//----------------
	UpdateUIinfo();
}

VarDecl::~VarDecl() {
	DROP( m_initializer );
	DROP( m_symbol );
}

const char* VarDecl::ToChars() const {
	return "Variable Declaration";
}

void VarDecl::ToString( String & s ) {
}

#pragma region "UI"
const char* VarDecl::uiToString() const {
	return "Variable declaration ( type + name [+ initializer] )";
}

int VarDecl::uiNumChildren() const {
	return m_initializer ? 3 : 2;
}

UINode * VarDecl::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_type;
	}
	if ( 1 == index ) {
		return m_id;
	}
	if ( 2 == index ) {
		return m_initializer;
	}
	return 0;
}

int VarDecl::uiIndexOf( const UINode *child ) const {
	if ( child == m_type ) {
		return 0;
	}
	if ( child == m_id ) {
		return 1;
	}
	if ( child == m_initializer ) {
		return 2;
	}
	return -1;
}
#pragma endregion

void VarDecl::Resolve( Semantics* semantics ) {
	//c4::common->Print( "Resolving variable declaration: '%s'", m_id->Text() );

	// create our symbol
	m_symbol = new VarSymbol( this );
	m_symbol->Grab();

	Scope * scope = semantics->GetContext();

	// add ourselved to the scope
	if ( ! scope->Insert( m_symbol ) ) {
		c4::compiler->Error( "variable '%s' has already been declared", m_id->Text() );
	}
	
	//**********

	//m_storageClass = ( STORAGE_CLASS )( m_storageClass | scope->storageClass );
	m_storageClass |= scope->storageClass;

	if ( IsExtern() && m_initializer ) {
		Error( "extern symbols cannot have initializers" );
	}

	if ( IsAbstract() ) {
		Error( "'abstract' cannot be applied to variable" );
	}
	if ( IsVirtual() ) {
		Error( "'virtual' cannot be applied to variable" );
	}
	if ( IsOverride() ) {
		Error( "'override' cannot be applied to variable" );
	}
	if ( IsFinal() ) {
		Error( "'final' cannot be applied to variable" );
	}

	/// resolve our type
	c4::Resolve< Type >( & m_type, semantics );

	// inherit linkage and protection level from the scope
	m_linkage = scope->linkage;
	m_protection = scope->protection;

	// this type of variable declaraions is not an auto declaration
	m_storageClass = ( STORAGE_CLASS )( m_storageClass & ~SC_AUTO );

	if ( m_type->IsPrimitive() ) {
		if ( m_type->GetTypeKind() == TYPE_VOID ) {
			Error( "variables cannot have type 'void'" );
			//if ( m_initializer ) {
			//	Error( "variables declared as 'void' cannot have initializers" );
			//}
		}
	}

	// resolve our initializer
	if ( m_initializer ) {
		c4::Resolve< Expression >( &m_initializer, semantics );
	}
	else // provide a default initializer
	{
	//	c4::compiler->Warning( "variable '%s' is not initialized!", m_id->Text() );

		// TODO:
	}

	// check if we are in a function
	FunctionDecl * outerFunction = semantics->CurrentFunction();
	// increment the function's locals' size by our size
	if ( outerFunction ) {
		FunctionSymbol * functionSymbol = outerFunction->GetSymbol();
		
		uint32  ourSize = GetType()->Size();
		functionSymbol->IncSizeOfLocals( ourSize );

		// save our stack offset in our symbol
		m_symbol->m_stackOffset = functionSymbol->GetSizeOfLocals();
	}

	//**********
	UpdateUIinfo();
}

//FASTBOOL VarDecl::IsInDataSegment() const {
//	if ( m_storageClass & SC_MANIFEST ) {
//		return false;
//	}
//	return ( IsStatic() || IsExtern() );
//}

void VarDecl::UpdateUIinfo() {
	Declaration::UpdateUIinfo();

	if ( m_type->IsConst() ) {
		m_detailedInfo += "const type";
	}
	if ( m_type->IsInvariant() ) {
		m_detailedInfo += "invariant type";
	}
	m_detailedInfo += '\n';
}
#pragma endregion

//===========================================================================

#pragma region "Function Argument"

DEFINE_TYPE( Argument, VarDecl );

Argument::Argument( Type* type, Identifier* name, Expression* init /* =0 */ )
	: VarDecl( type, name, init )
	, m_flow( AF_NONE )
{
	m_storageClass |= SC_PARAMETER;
}

Argument::~Argument() {
}

const char* Argument::ToChars() const {
	return "Function Argument";
}

void Argument::ToString( String & s ) {
	m_type->ToString( s );
	s += " ";
	s += m_id->Text();
	s += ';';
}

const char* Argument::uiToString() const {
	return m_initializer ? "Function Argument ( name, default initializer )" : "Function Argument ( name )";
}

void Argument::Resolve( Semantics* semantics ) 
{
	VarDecl::Resolve( semantics );
}

#pragma endregion

//===========================================================================

#pragma region "ArgumentList ::= e1 , e2, ... , eN"

DEFINE_TYPE( ArgumentList, Declaration );

ArgumentList::ArgumentList()
{
}

ArgumentList::~ArgumentList() 
{
	LinkedList< Argument* >::Iterator  iter = m_argList.Begin();

	for ( ; iter != m_argList.End(); iter++ ) 
	{
		( *iter )->Drop();
	}

	m_argList.RemoveAll();
}

void ArgumentList::Add( Argument* e ) {
	e->Grab();
	e->uiSetParent( this );
	
	m_argList.Append( e );
}

const char * ArgumentList::ToChars() const {
	return "ArgumentList";
}

void ArgumentList::ToString( String & s ) {
	s += '(';
	LinkedList< Argument* >::Iterator  iter = m_argList.Begin();
	for ( ; iter != m_argList.End(); iter++ ) 
	{
		( *iter )->ToString( s );
		s += "; ";
	}

	s += ')';
}

#pragma region "UI"
const char* ArgumentList::uiToString() const {
	return "Function Arguments";
}

int ArgumentList::uiNumChildren() const {
	return m_argList.Num();
}

UINode * ArgumentList::uiChildAt( int index )
{
	if ( index >= m_argList.Num() ) {
		return 0;
	}

	LinkedList< Argument* >::Iterator  iter = m_argList.Begin();

	for ( int i = 0; iter != m_argList.End(); iter++, i++ ) {
		if ( i == index ) {
			return *iter;
		}
	}

	return 0;
}

int ArgumentList::uiIndexOf( const UINode *child ) const
{
	LinkedList< Argument* >::Iterator  iter = m_argList.Begin();

	for ( int i = 0; iter != m_argList.End(); iter++, i++ ) {
		if ( child == *iter ) {
			return i;
		}
	}

	return -1;
}
#pragma endregion

void ArgumentList::Resolve( Semantics* semantics ) 
{
	LinkedList< Argument* >::Iterator  iter = m_argList.Begin();

	for ( int i = 0; iter != m_argList.End(); iter++, i++ ) 
	{
		( *iter )->Resolve( semantics );
	}	
}

#pragma endregion

#pragma region "Function Declaration"

DEFINE_TYPE( FunctionDecl, Declaration );

FunctionDecl::FunctionDecl( Type* returnType, Identifier *name, ArgumentList* args, Statement* body )
	: Declaration( returnType, name )
	, m_args( args )
	, m_body( body )
	, m_symbol( NULL )
{
	m_args->Grab();
	m_body->Grab();

	// ui
	m_args->uiSetParent( this );
	m_body->uiSetParent( this );

	m_detailedInfo = "Return Type: ";
	m_detailedInfo += m_type->mInfo;
}

FunctionDecl::~FunctionDecl() {
	DROP( m_args );
	DROP( m_body );
	DROP( m_symbol );
}

const char* FunctionDecl::ToChars() const {
	static String  s;
	s = "Function Declaration ( name, params, body ): ";
	s.Append( m_id->ToChars() );
	return s.c_str();
}

void FunctionDecl::ToString( String & s ) {
}

// pretty-print the function header
void FunctionDecl::HeaderToString( String & s ) {
	m_type->ToString( s );
	s += ' ';
	s += m_id->Text();

	m_args->ToString( s );
}

#pragma region "UI"
const char * FunctionDecl::uiToString() const {
	return "Function Declaration";
}

int FunctionDecl::uiNumChildren() const {
	return 3;
}

UINode * FunctionDecl::uiChildAt( int index ) {
	switch ( index ) {
		case 0:	return m_id;
		case 1:	return m_args;
		case 2:	return m_body;
	}
	return 0;
}

int FunctionDecl::uiIndexOf( const UINode *child ) const {
	if ( !child ) {
		return -1;
	}
	if ( child == m_id ) {
		return 0;
	}
	if ( child == m_args ) {
		return 1;
	}
	if ( child == m_body ) {
		return 2;
	}
	return -1;
}
#pragma endregion

bool FunctionDecl::IsMain() const {
	return ! String::Cmp( m_id->Text(), "main" ); //"Main" );
}

void FunctionDecl::Resolve( Semantics* semantics ) {
	//c4::common->Print( "Resolving function: '%s'", m_id->Text() );

	// get the scope before entering this function
	Scope * scope = semantics->GetContext();

	Assert( scope->Search("getchar") );

	// resolve our return type
	c4::Resolve< Type >( & m_type, semantics );

	// create a function scope
	Scope * functionScope = new Scope( scope );
	functionScope->enclosingFunction = this;
	semantics->SetContext( functionScope );

	// create a symbol for this function
	m_symbol = new FunctionSymbol( m_id, functionScope );
	m_symbol->Grab();

	if ( ! scope->Insert( m_symbol ) ) {
		c4::compiler->Error( "function '%s' has already been declared", m_id->Text() );
	}

	semantics->SetFunction( this );

	m_linkage = scope->linkage;
	m_protection = scope->protection;

	// resolve our parameters' types and initializers
	// ( and add our parameters to this scope )
	m_args->Resolve( semantics );
	m_symbol->SetNumOfArguments( m_args->GetArguments().Num() );

	// resolve the function's body
	m_body->Resolve( semantics );

	// resolve our return type
	c4::Resolve< Type >( & m_type, semantics );
	m_symbol->SetReturnType( m_type );

	// restore the previous function
	semantics->SetFunction( functionScope->GetParentScope()->enclosingFunction );
		
	//**********
	UpdateUIinfo();
}

#pragma endregion

#pragma region "Namespace Declaration"

DEFINE_TYPE( NamespaceDecl, Declaration );

NamespaceDecl::NamespaceDecl( Identifier* name /* =0 */  )
{
	if ( name ) {
		m_id = name;
		m_id->Grab();
		m_id->uiSetParent( this );

		m_detailedInfo = "Namespace: ";
		m_detailedInfo += m_id->Text();
	}
	else {
		m_detailedInfo = "Anonymous namespace";
	}

	m_declList = new DeclarationList;
	m_declList->Grab();
	m_declList->uiSetParent( this );
}

NamespaceDecl::~NamespaceDecl()
{
	m_declList->Drop();
}

void NamespaceDecl::Add( Declaration* decl )
{
	m_declList->Add( decl );
}

const char * NamespaceDecl::ToChars() const {
	return "Namespace Declaration";
}

void NamespaceDecl::ToString( String & s ) {
}

#pragma region "UI"
const char * NamespaceDecl::uiToString() const {
	return "Namespace Declaration";
}

int NamespaceDecl::uiNumChildren() const {
	return 1;
}

UINode * NamespaceDecl::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_declList;
	}
	return 0;
}

int NamespaceDecl::uiIndexOf( const UINode *child ) const {
	if ( child == m_declList ) {
		return 0;
	}
	return -1;
}
#pragma endregion

void NamespaceDecl::Resolve( Semantics* semantics ) {
	c4::common->Print( "Resolving: '%s'", m_detailedInfo.c_str() );

	m_declList->Resolve( semantics );
}

#pragma endregion

#pragma region "DeclarationList_Owner_Base"

DEFINE_TYPE( DeclarationList_Owner_Base, Declaration );

DeclarationList_Owner_Base::DeclarationList_Owner_Base( Type* type, Identifier* name /* =0 */ )
	: Declaration( type, name )
{
	m_declList = new DeclarationList;
	m_declList->Grab();
	m_declList->uiSetParent( this );
}

DeclarationList_Owner_Base::~DeclarationList_Owner_Base() {
	m_declList->Drop();
}

void DeclarationList_Owner_Base::Add( Declaration * decl ) {
	m_declList->Add( decl );
}

const char * DeclarationList_Owner_Base::ToChars() const {
	return "DeclarationList_Owner_Base";
}

void DeclarationList_Owner_Base::ToString( String & s ) {
}

void DeclarationList_Owner_Base::SetBody( DeclarationList* declList )
{
	m_declList->Drop();
	m_declList = declList;
	m_declList->Grab();
	m_declList->uiSetParent( this );
}

#pragma region "UI"
const char * DeclarationList_Owner_Base::uiToString() const {
	return ToChars();
}

int DeclarationList_Owner_Base::uiNumChildren() const {
	return 1;
}

UINode * DeclarationList_Owner_Base::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_declList;
	}
	return 0;
}

int DeclarationList_Owner_Base::uiIndexOf( const UINode *child ) const {
	if ( child == m_declList ) {
		return 0;
	}
	return -1;
}
#pragma endregion

#pragma endregion

#pragma region "Enum Declaration"

DEFINE_TYPE( EnumDecl, DeclarationList_Owner_Base );

EnumDecl::EnumDecl( Identifier* name, const Location & loc )
	: DeclarationList_Owner_Base( new TypeEnum( name, loc ), name )
{
}

EnumDecl::~EnumDecl()
{
}

const char * EnumDecl::ToChars() const {
	static String  s;
	s = "Enum Declaration: ";
	s += m_id->Text();

	return s.c_str();
}

void EnumDecl::ToString( String & s ) {
}

const char * EnumDecl::uiToString() const {
	return ToChars();
}

void EnumDecl::Resolve( Semantics* semantics )
{
}

#pragma endregion

#pragma region "Class Declaration"

DEFINE_TYPE( ClassDecl, Declaration );

ClassDecl::ClassDecl( Identifier* name, DeclarationList* decl, const Location & loc )
	: Declaration( new TypeClass( name, loc ), name )
{
	m_declList = decl;
	m_declList->Grab();
	m_declList->uiSetParent( this );
}

ClassDecl::~ClassDecl() {
	m_declList->Drop();
}

const char * ClassDecl::ToChars() const {
	return "ClassDecl";
}

void ClassDecl::ToString( String & s ) {
}

void ClassDecl::SetBody( DeclarationList* declList )
{
	m_declList->Drop();
	m_declList = declList;
	m_declList->Grab();
	m_declList->uiSetParent( this );
}

#pragma region "UI"
const char * ClassDecl::uiToString() const {
	return ToChars();
}

int ClassDecl::uiNumChildren() const {
	return 1;
}

UINode * ClassDecl::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_declList;
	}
	return 0;
}

int ClassDecl::uiIndexOf( const UINode *child ) const {
	if ( child == m_declList ) {
		return 0;
	}
	return -1;
}
#pragma endregion

void ClassDecl::Resolve( Semantics* semantics )
{
}
#pragma endregion

//===================================================================================

DEFINE_TYPE( CtorDecl, Declaration );

CtorDecl::CtorDecl( ArgumentList* args, Statement* body )
	: m_args( args )
	, m_body( body )
{
	m_args->Grab();
	m_body->Grab();

	// ui
	m_args->uiSetParent( this );
	m_body->uiSetParent( this );
}

CtorDecl::~CtorDecl() {
	DROP( m_args );
	DROP( m_body );
}

void CtorDecl::Resolve( Semantics* semantics ) {
}

const char * CtorDecl::ToChars() const {
	return "Constructor";
}

//-------------------------------------------------
const char * CtorDecl::uiToString() const {
	return "Constructor";
}

int CtorDecl::uiNumChildren() const {
	return 2;
}

UINode * CtorDecl::uiChildAt( int index ) {
	switch ( index ) {
		case 0:	return m_args;
		case 1:	return m_body;
	}
	return 0;
}

int CtorDecl::uiIndexOf( const UINode* child ) const {
	if ( ! child ) {
		return -1;
	}
	if ( child == m_args ) {
		return 0;
	}
	if ( child == m_body ) {
		return 1;
	}
	return -1;
}

//===================================================================================

DEFINE_TYPE( DtorDecl, Declaration );

DtorDecl::DtorDecl( Statement* body )
	: m_body( body )
{
	m_body->Grab();
	m_body->uiSetParent( this );
}

DtorDecl::~DtorDecl() {
	DROP( m_body );
}

void DtorDecl::Resolve( Semantics* semantics ) {
}

const char * DtorDecl::ToChars() const {
	return "destructor";
}

//-------------------------------------------------
const char * DtorDecl::uiToString() const {
	return "Destructor";
}

int DtorDecl::uiNumChildren() const {
	return 1;
}

UINode * DtorDecl::uiChildAt( int index ) {
	switch ( index ) {
		case 0:	return m_body;
	}
	return 0;
}

int DtorDecl::uiIndexOf( const UINode* child ) const {
	if ( ! child ) {
		return -1;
	}
	if ( child == m_body ) {
		return 0;
	}
	return -1;
}

//===================================================================================

}//end namespace c4