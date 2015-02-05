/*
======================================
	implementation of classes, 
	representing types.
======================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

#pragma region Type

DEFINE_TYPE( Type, AST_Node );

Type::Type()
	: m_name( "type" )
	, m_typeKind( TYPE_NONE )
	, m_mods( NO_MODIFIERS )
{
}

Type::~Type() {
}

ulong Type::Size() const {
	c4::common->Print( "Type::Size(): no size for the type '%s'", ToChars() );
	return 0;
}

const char * Type::ToChars() const {
	return "Type";
}

void Type::ToString( String & s ) {
	s += this->ToChars();
}

bool Type::Equals( const Object *other ) const {
	Type * t = ( Type* )other;

	if ( this == t ) {
		return true;
	}
	return false;
}

bool Type::operator == ( TYPE_KIND  typeKind ) const {
	return m_typeKind == typeKind;
}

Type * Type::ResolveInto( Semantics* semantics )
{
	//c4::common->Print( "Resolving type: '%s'", ToChars() );
	return this;
}

bool Type::SetModifiers( int32 mods ) {
	if ( m_mods & UNSIGNED )
	{
		if ( mods & SIGNED )
		{
			Error( "signed/unsigned mismatch" );
			return false;
		}
		if ( mods & UNSIGNED )
		{
			Error( "redundant 'unsigned'" );
			return false;
		}
	}

	m_mods |= mods;
	return true;
}

//****************************************************

/*
=========================
	statics
=========================
*/

TYPE_KIND  Type::m_convTable[ NUM_OF_TYPES ][ NUM_OF_TYPES ];

void Type::StaticInit()
{
	// setup conversion tables
	for ( int i = 0; i < NUM_OF_TYPES; i++ ) {
		for ( int j = 0; j < NUM_OF_TYPES; j++ ) {
			m_convTable[ i ][ j ] = TYPE_ERROR;
		}
	}

	m_convTable[ CHAR ][ CHAR ]				= CHAR;
	m_convTable[ CHAR ][ BOOL ]				= CHAR;
	m_convTable[ CHAR ][ TYPE_INT8 ]		= TYPE_INT8;
	m_convTable[ CHAR ][ TYPE_INT32 ]		= TYPE_INT32;
	m_convTable[ CHAR ][ TYPE_INT64 ]		= TYPE_INT64;
	m_convTable[ CHAR ][ TYPE_FLOAT32 ]		= TYPE_FLOAT32;
	m_convTable[ CHAR ][ FLOAT80 ]			= FLOAT80;
	m_convTable[ CHAR ][ STRING ]			= STRING;

	m_convTable[ BOOL ][ CHAR ]				= CHAR;
	m_convTable[ BOOL ][ BOOL ]				= BOOL;
	m_convTable[ BOOL ][ TYPE_INT8 ]		= TYPE_INT8;
	m_convTable[ BOOL ][ TYPE_INT32 ]		= TYPE_INT32;
	m_convTable[ BOOL ][ TYPE_INT64 ]		= TYPE_INT64;
	m_convTable[ BOOL ][ TYPE_FLOAT32 ]		= TYPE_FLOAT32;
	m_convTable[ BOOL ][ FLOAT80 ]			= FLOAT80;
	m_convTable[ BOOL ][ STRING ]			= STRING;

	m_convTable[ TYPE_INT8 ][ CHAR ]			= TYPE_INT8;
	m_convTable[ TYPE_INT8 ][ BOOL ]			= TYPE_INT8;
	m_convTable[ TYPE_INT8 ][ TYPE_INT8 ]		= TYPE_INT8;
	m_convTable[ TYPE_INT8 ][ TYPE_INT32 ]		= TYPE_INT32;
	m_convTable[ TYPE_INT8 ][ TYPE_INT64 ]		= TYPE_INT64;
	m_convTable[ TYPE_INT8 ][ TYPE_FLOAT32 ]	= TYPE_FLOAT32;
	m_convTable[ TYPE_INT8 ][ TYPE_FLOAT64 ]	= TYPE_FLOAT64;
	m_convTable[ TYPE_INT8 ][ FLOAT80 ]			= FLOAT80;
	m_convTable[ TYPE_INT8 ][ STRING ]			= STRING;

	m_convTable[ TYPE_INT16 ][ CHAR ]			= TYPE_INT16;
	m_convTable[ TYPE_INT16 ][ BOOL ]			= TYPE_INT16;
	m_convTable[ TYPE_INT16 ][ TYPE_INT8 ]		= TYPE_INT16;
	m_convTable[ TYPE_INT16 ][ TYPE_INT32 ]		= TYPE_INT32;
	m_convTable[ TYPE_INT16 ][ TYPE_INT64 ]		= TYPE_INT64;
	m_convTable[ TYPE_INT16 ][ TYPE_FLOAT32 ]	= TYPE_FLOAT32;
	m_convTable[ TYPE_INT16 ][ TYPE_FLOAT64 ]	= TYPE_FLOAT64;
	m_convTable[ TYPE_INT16 ][ FLOAT80 ]		= FLOAT80;
	m_convTable[ TYPE_INT16 ][ STRING ]			= STRING;

	m_convTable[ TYPE_INT32 ][ CHAR ]			= TYPE_INT32;
	m_convTable[ TYPE_INT32 ][ BOOL ]			= TYPE_INT32;
	m_convTable[ TYPE_INT32 ][ TYPE_INT8 ]		= TYPE_INT32;
	m_convTable[ TYPE_INT32 ][ TYPE_INT32 ]		= TYPE_INT32;
	m_convTable[ TYPE_INT32 ][ TYPE_INT64 ]		= TYPE_INT64;
	m_convTable[ TYPE_INT32 ][ TYPE_FLOAT32 ]	= TYPE_FLOAT32;
	m_convTable[ TYPE_INT32 ][ TYPE_FLOAT64 ]	= TYPE_FLOAT64;
	m_convTable[ TYPE_INT32 ][ FLOAT80 ]		= FLOAT80;
	m_convTable[ TYPE_INT32 ][ STRING ]			= STRING;

	m_convTable[ TYPE_FLOAT32 ][ CHAR ]			= TYPE_FLOAT32;
	m_convTable[ TYPE_FLOAT32 ][ BOOL ]			= TYPE_FLOAT32;
	m_convTable[ TYPE_FLOAT32 ][ TYPE_INT8 ]	= TYPE_FLOAT32;
	m_convTable[ TYPE_FLOAT32 ][ TYPE_INT32 ]	= TYPE_FLOAT32;
	m_convTable[ TYPE_FLOAT32 ][ TYPE_INT64 ]	= TYPE_FLOAT32;
	m_convTable[ TYPE_FLOAT32 ][ TYPE_FLOAT32 ]	= TYPE_FLOAT32;
	m_convTable[ TYPE_FLOAT32 ][ TYPE_FLOAT64 ]	= TYPE_FLOAT64;
	m_convTable[ TYPE_FLOAT32 ][ FLOAT80 ]		= FLOAT80;
	m_convTable[ TYPE_FLOAT32 ][ STRING ]		= STRING;

	m_convTable[ TYPE_FLOAT64 ][ CHAR ]			= TYPE_FLOAT64;
	m_convTable[ TYPE_FLOAT64 ][ BOOL ]			= TYPE_FLOAT64;
	m_convTable[ TYPE_FLOAT64 ][ TYPE_INT8 ]	= TYPE_FLOAT64;
	m_convTable[ TYPE_FLOAT64 ][ TYPE_INT32 ]	= TYPE_FLOAT64;
	m_convTable[ TYPE_FLOAT64 ][ TYPE_INT64 ]	= TYPE_FLOAT64;
	m_convTable[ TYPE_FLOAT64 ][ TYPE_FLOAT32 ]	= TYPE_FLOAT64;
	m_convTable[ TYPE_FLOAT64 ][ TYPE_FLOAT64 ]	= TYPE_FLOAT64;
	m_convTable[ TYPE_FLOAT64 ][ FLOAT80 ]		= FLOAT80;
	m_convTable[ TYPE_FLOAT64 ][ STRING ]		= STRING;
}

Type * Type::GetCommonType( Type* t1, Type* t2 ) {
	TYPE_KIND  commonType = 
		m_convTable [ t1->GetTypeKind() ] [ t2->GetTypeKind() ];

	Assert( TYPE_ERROR != commonType );

	Type * newType = NULL;

	switch ( commonType )
	{
	case CHAR :	
	case TYPE_INT8 :
		newType = new Type_Int8();
		break;

	case BOOL :
		newType = new Type_Bool();
		break;

	case TYPE_INT16 :
		newType = new Type_Int16();
		break;

	case TYPE_INT32 :
		newType = new Type_Int32();
		break;

	case TYPE_FLOAT32 :
		newType = new Type_Float32();
		break;

	case TYPE_FLOAT64 :
		newType = new Type_Float64();
		break;

	default : Unreachable();
	}

	if ( newType->IsIntegral() && newType->GetTypeKind() != BOOL )
	{
		if ( t1->IsUnsigned() || t2->IsUnsigned() )
		{
			newType->SetModifiers( UNSIGNED );
		}
	}

	return newType;
}

//--------------------------------------------------------

//========================================================

bool Type::IsBuiltIn() const {
	return false;
}

bool Type::IsPrimitive() const {
	return false;
}

bool Type::IsBoolean() const {
	return false;
}

bool Type::IsIntegral() const {
	return false;
}

bool Type::IsFloating() const {
	return false;
}

bool Type::IsSigned() const {
	return ! IsUnsigned();
}

bool Type::IsUnsigned() const {
	return m_mods & UNSIGNED;
}

bool Type::IsAuto() const {
	return false;
}

bool Type::IsAssignable() const {
	return false;
}

bool Type::IsConst() const {
	return MOD_CONST == m_mods;
}

bool Type::IsInvariant() const {
	return INVARIANT == m_mods;
}

bool Type::IsMutable() const {
	return false;
}

// returns true if this type can be converted to _boolean value
bool Type::CheckBoolean() const {
	return false;
}

Type * Type::IsConstOf() const {
	return 0;
}

Type * Type::IsInvariantOf() const {
	return 0;
}

Type * Type::IsMutableOf() const {
	return 0;
}

Type * Type::IsPointerTo() const {
	return 0;
}

Type * Type::IsReferenceTo() const {
	return 0;
}

Type * Type::IsArrayOf() const {
	return 0;
}

ClassDecl * Type::IsClassHandle() const {
	return 0;
}

Type * Type::TypeOf() {
	return 0;
}

Expression * Type::DefaultInitializer() const {
	return 0;
}

bool Type::HasPointers() const {
	return false;
}

Type * Type::ToBaseType() {
	return NULL;
}

TYPE_KIND Type::GetTypeKind() const {
	return m_typeKind;
}

void Type::MakeConst() {
	m_mods = MOD_CONST;
}

void Type::MakeInvariant() {
	m_mods = INVARIANT;
}

#pragma endregion

#pragma region "Built-in Type"

DEFINE_TYPE( BuiltInType, Type );

BuiltInType::BuiltInType()
{
	Grab();	// built-in types must not be deleted!
	// all Objects which are not referenced will be deleted.
}

BuiltInType::~BuiltInType()
{
}

void BuiltInType::ToString( String & s )
{
}
#pragma endregion


#pragma region "Primitive Type"

DEFINE_TYPE( PrimitiveType, BuiltInType );

void PrimitiveType::ToString( String & s )
{
}

#pragma region "UI"
int PrimitiveType::uiNumChildren() const {
	return 0;
}

UINode * PrimitiveType::uiChildAt( int index ) {
	return 0;
}

int PrimitiveType::uiIndexOf( const UINode *child ) const {
	return -1;
}
#pragma endregion

#pragma endregion


#pragma region "Type_Void"

DEFINE_TYPE( Type_Void, PrimitiveType );

Type_Void::Type_Void()
{
	m_name = "void";

	m_typeKind = TYPE_VOID;
	m_mods = NO_MODIFIERS;

	mInfo = "void";
}

Type_Void::~Type_Void()
{
}

const char* Type_Void::ToChars() const {
	return "'void'";
}

const char * Type_Void::uiToString() const {
	return "void";
}
#pragma endregion

#pragma region "Type NULL"

DEFINE_TYPE( Type_Null, PrimitiveType );

Type_Null::Type_Null()
{
	m_name = "NULL";
	m_typeKind = TYPE_FLOAT32;
	m_mods = NO_MODIFIERS;

	mInfo = "NULL";
}

Type_Null::~Type_Null() {
}

const char * Type_Null::ToChars() const {
	return "'null'";
}

const char * Type_Null::uiToString() const {
	return "null";
}

void Type_Null::ToString( String & s ) {
	s += "null";
}

ulong Type_Null::Size() const {
	return sizeof( float );
}

bool Type_Null::IsIntegral() const {
	return false;
}

bool Type_Null::IsFloating() const {
	return false;
}

bool Type_Null::IsReal() const {
	return false;
}
#pragma endregion

#pragma region "NonPrimitiveType"

DEFINE_TYPE( NonPrimitiveType, Type );

NonPrimitiveType::NonPrimitiveType()
{
}

NonPrimitiveType::NonPrimitiveType( const Location & loc )
	: m_location( loc )
{
}

NonPrimitiveType::~NonPrimitiveType()
{
}

const char* NonPrimitiveType::ToChars() const {
	return "NonPrimitiveType";
}

void NonPrimitiveType::ToString( String & s )
{
}

Type * NonPrimitiveType::ResolveInto( Semantics* semantics ) {
	c4::common->Warning( "!!!!!!!!!!!!!! - NonPrimitiveType::Resolve() - !!!!!!!!!!!!!!" );
	return this;
}
#pragma endregion

#pragma region "Pointer Type"

DEFINE_TYPE( TypePointer, NonPrimitiveType );

TypePointer::TypePointer( Type * target )
	: m_targetType( target )
{
	m_targetType->Grab();
	m_targetType->uiSetParent( this );

	m_name = "pointer";

	m_typeKind = POINTER;
	m_mods = NO_MODIFIERS;

	mInfo = "pointer to ";
	mInfo += m_targetType->mInfo;
}

TypePointer::~TypePointer() {
	m_targetType->Drop();
}

const char* TypePointer::ToChars() const {
	return "Pointer Type";
}

void TypePointer::ToString( String & s )
{
}

ulong TypePointer::Size() const {
	return sizeof( void* );
}

Type * TypePointer::ResolveInto( Semantics* semantics ) {
	m_targetType->ResolveInto( semantics );
	return this;
}

#pragma region "UI"
int TypePointer::uiNumChildren() const {
	return 1;
}
UINode * TypePointer::uiChildAt( int index ) {
	return index == 0 ? m_targetType : NULL;
}

int TypePointer::uiIndexOf( const UINode *child ) const {
	return child == m_targetType ? 0 : -1;
}

const char * TypePointer::uiToString() const {
	return "pointer";
}
#pragma endregion

#pragma endregion


#pragma region "Static Array Type"

DEFINE_TYPE( TypeStaticArray, TypePointer);

TypeStaticArray::TypeStaticArray( Type* targetType, Expression* dim )
	: TypePointer( targetType )
	, m_dimension( dim )
{
	m_dimension->Grab();
	m_dimension->uiSetParent( this );

	m_name = "array";

	m_typeKind = STATIC_ARRAY;
	m_mods = NO_MODIFIERS;

	mInfo = "array of ";
	mInfo += m_targetType->mInfo;
}

TypeStaticArray::~TypeStaticArray() {
	m_dimension->Drop();
}

const char* TypeStaticArray::ToChars() const {
	return "Static Array Type";
}


void TypeStaticArray::ToString( String & s )
{
}

Type * TypeStaticArray::ResolveInto( Semantics* semantics ) {
	m_targetType->ResolveInto( semantics );
	m_dimension->ResolveInto( semantics );
	return this;
}

#pragma region "UI"
int TypeStaticArray::uiNumChildren() const {
	return 2;
}
UINode * TypeStaticArray::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_targetType;
	}
	if ( 1 == index ) {
		return m_dimension;
	}
	return NULL;
}

int TypeStaticArray::uiIndexOf( const UINode *child ) const {
	if ( child == m_targetType ) {
		return 0;
	}
	if ( child == m_dimension ) {
		return 1;
	}
	return -1;
}

const char * TypeStaticArray::uiToString() const {
	return "Static Array: Type + [ Dimension ]";
}
#pragma endregion

#pragma endregion

#pragma region "Qualified Type"

DEFINE_TYPE( TypeQualified, NonPrimitiveType );

TypeQualified::TypeQualified( Identifier* id, const Location & loc )
	: NonPrimitiveType( loc )
{
	Add( id );
}

TypeQualified::~TypeQualified() {
	LinkedList< Identifier* >::Iterator  iter = m_idList.Begin();

	for ( int i = 0; iter != m_idList.End(); iter++, i++ ) 
	{
		( *iter )->Drop();
	}

	m_idList.RemoveAll();
}

void TypeQualified::Add( Identifier * id ) {
	id->Grab();
	id->uiSetParent( this );
	m_idList.Append( id );

	mInfo += id->Text();
	mInfo += '.';
}

const char* TypeQualified::ToChars() const {
	return "Qualified Type";
}

void TypeQualified::ToString( String & s ) {
}

#pragma region "UI"
int TypeQualified::uiNumChildren() const {
	return m_idList.Num();
}

UINode * TypeQualified::uiChildAt( int index )
{
	LinkedList< Identifier* >::Iterator	  iter = m_idList.Begin();
	
	for ( int i = 0; iter != m_idList.End(); iter++, i++ ) 
	{
		if ( i == index ) {
			return *iter;
		}
	}
	return NULL;
}

int TypeQualified::uiIndexOf( const UINode *child ) const {
	LinkedList< Identifier* >::Iterator	  iter = m_idList.Begin();
	
	for ( int i = 0; iter != m_idList.End(); iter++, i++ ) 
	{
		if ( child == *iter ) {
			return i;
		}
	}
	return -1;
}

const char * TypeQualified::uiToString() const {
	return "TypeQualified";
}
#pragma endregion

TYPE_KIND TypeQualified::GetTypeKind() const {
	c4::common->Print( "Not implemented: TypeQualified::GetTypeKind()" );
	return TYPE_ERROR;
}

Type * TypeQualified::ResolveInto( Semantics* semantics ) {
	// ...
	return this;
}
#pragma endregion

#pragma region "Type Enum"

DEFINE_TYPE( TypeEnum, NonPrimitiveType );

TypeEnum::TypeEnum( Identifier* id, const Location & loc )
	: NonPrimitiveType( loc )
{
	m_name = "enum ";
	m_name += id->Text();
}

TypeEnum::~TypeEnum() {
}

const char * TypeEnum::ToChars() const {
	return "Enum";
}


void TypeEnum::ToString( String & s )
{
}

#pragma region "UI"
int TypeEnum::uiNumChildren() const {
	return 0;
}
UINode * TypeEnum::uiChildAt( int index ) {
	return NULL;
}

int TypeEnum::uiIndexOf( const UINode *child ) const {
	return -1;
}

const char * TypeEnum::uiToString() const {
	return "TypeEnum";
}
#pragma endregion

Type * TypeEnum::ResolveInto( Semantics* semantics )
{
	return NULL;
}

#pragma endregion

#pragma region "Type Class"

DEFINE_TYPE( TypeClass, NonPrimitiveType );

TypeClass::TypeClass( Identifier* id, const Location & loc )
	: NonPrimitiveType( loc )
{
	m_name = "class ";
	m_name += id->Text();
}

TypeClass::~TypeClass() {
}

const char * TypeClass::ToChars() const {
	return "Class";
}

#pragma region "UI"
int TypeClass::uiNumChildren() const {
	return 0;
}
UINode * TypeClass::uiChildAt( int index ) {
	return NULL;
}

int TypeClass::uiIndexOf( const UINode *child ) const {
	return -1;
}

const char * TypeClass::uiToString() const {
	return "TypeClass";
}
#pragma endregion

void TypeClass::ToString( String & s )
{
}

Type * TypeClass::ResolveInto( Semantics* semantics )
{
	return NULL;
}

#pragma endregion


#pragma region "Type_Error"

DEFINE_TYPE( Type_Error, BuiltInType );

Type_Error::Type_Error( const char* message, const Location & loc )
	: m_message( message )
	, m_location( loc )
{
}

Type_Error::~Type_Error()
{
}

const char * Type_Error::GetMessage() const {
	return m_message.c_str();
}

Location & Type_Error::GetLocation() {
	return m_location;
}

const char* Type_Error::ToChars() const {
	return "[Type: Error]";
}

void Type_Error::ToString( String & s )
{
}

Type * Type_Error::ResolveInto( Semantics* semantics ) {
	c4::common->Warning( "Type_Error::Resolve()" );
	return this;
}

#pragma endregion

}//end namespace c4