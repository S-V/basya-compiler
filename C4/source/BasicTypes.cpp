#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

#pragma region "Type Char"

DEFINE_TYPE( Type_Char, IntegralType );

Type_Char::Type_Char()
{
	m_name = "char";
	m_typeKind = CHAR;
	m_mods = NO_MODIFIERS;

	mInfo = "char";
}

Type_Char::~Type_Char() {
}

const char * Type_Char::ToChars() const {
	return "'char'";
}

void Type_Char::ToString( String & s ) {
	s += "char";
}

ulong Type_Char::Size() const {
	//return sizeof( char );
	return 1;
}

#pragma endregion

#pragma region "Type Bool"

DEFINE_TYPE( Type_Bool, IntegralType );

Type_Bool::Type_Bool()
{
	m_name = "bool";
	m_typeKind = BOOL;
	m_mods = NO_MODIFIERS;

	mInfo = "bool";
}

Type_Bool::~Type_Bool() {
}

const char * Type_Bool::ToChars() const {
	return "'bool'";
}

void Type_Bool::ToString( String & s ) {
	s += "bool";
}

ulong Type_Bool::Size() const {
	return 1;
}

bool Type_Bool::SetModifiers( int32 mods ) {
	if ( mods & SIGNED ) {
		Error( "'signed' : can not be used with type 'bool'" );
		return false;
	}
	if ( mods & UNSIGNED ) {
		Error( "'unsigned' : can not be used with type 'bool'" );
		return false;
	}
	PrimitiveType::SetModifiers( mods );
	return true;
}

#pragma endregion

#pragma region "Type int8"

DEFINE_TYPE( Type_Int8, IntegralType );

Type_Int8::Type_Int8()
{
	m_name = "int8";
	m_typeKind = TYPE_INT8;
	m_mods = NO_MODIFIERS;

	mInfo = "int8";

	//c4::common->Print( "Type int8 is initialized!" );
}

Type_Int8::~Type_Int8() {
}

const char * Type_Int8::ToChars() const {
	return "'int8'";
}

void Type_Int8::ToString( String & s ) {
	s += "char";
}

ulong Type_Int8::Size() const {
	return 1;
}

#pragma endregion


#pragma region "Type Int16"

DEFINE_TYPE( Type_Int16, IntegralType );

Type_Int16::Type_Int16()
{
	m_name = "int16";
	m_typeKind = TYPE_INT16;
	m_mods = NO_MODIFIERS;

	mInfo = "int16";

	//c4::common->Print( "Type Int16 is initialized!" );
}

Type_Int16::~Type_Int16() {
}

const char * Type_Int16::ToChars() const {
	return "'int16'";
}

void Type_Int16::ToString( String & s ) {
	s += "short";
}

ulong Type_Int16::Size() const {
	return 2;
}

#pragma endregion

#pragma region "Type Int32"

DEFINE_TYPE( Type_Int32, IntegralType );

Type_Int32::Type_Int32()
{
	m_name = "int32";
	m_typeKind = TYPE_INT32;
	m_mods = NO_MODIFIERS;

	mInfo = "int32";

	//c4::common->Print( "Type Int32 is initialized!" );
}

Type_Int32::~Type_Int32() {
}

const char * Type_Int32::ToChars() const {
	return "'int32'";
}

void Type_Int32::ToString( String & s ) {
	s += "int";
}

ulong Type_Int32::Size() const {
	return 4;
}

#pragma endregion

#pragma region "Type Int64"

DEFINE_TYPE( Type_Int64, IntegralType );

Type_Int64::Type_Int64()
{
	m_name = "int64";
	m_typeKind = TYPE_INT64;
	m_mods = NO_MODIFIERS;

	mInfo = "int64";
}

Type_Int64::~Type_Int64() {
}

const char * Type_Int64::ToChars() const {
	return "'int64'";
}

void Type_Int64::ToString( String & s ) {
	s += "long int";
}

ulong Type_Int64::Size() const {
	return 8;
}

#pragma endregion

bool FloatingPointType::SetModifiers( int32 mods ) {
	if ( mods & SIGNED || mods & UNSIGNED ) {
		Error( "invalid type specification: 'float' and 'double' cannot be signed / unsigned" );
		return false;
	}
	PrimitiveType::SetModifiers( mods );
	return true;
}

#pragma region "Type Float32"

DEFINE_TYPE( Type_Float32, FloatingPointType );

Type_Float32::Type_Float32()
{
	m_name = "float32";
	m_typeKind = TYPE_FLOAT32;
	m_mods = NO_MODIFIERS;

	mInfo = "float32";
}

Type_Float32::~Type_Float32() {
}

const char * Type_Float32::ToChars() const {
	return "'float32'";
}

void Type_Float32::ToString( String & s ) {
	s += "float";
}

ulong Type_Float32::Size() const {
	return sizeof( float );
}

#pragma endregion

#pragma region "Type Float64"

DEFINE_TYPE( Type_Float64, FloatingPointType );

Type_Float64::Type_Float64()
{
	m_name = "float64";
	m_typeKind = TYPE_FLOAT64;
	m_mods = NO_MODIFIERS;

	mInfo = "float64";
}

Type_Float64::~Type_Float64() {
}

const char * Type_Float64::ToChars() const {
	return "'float64'";
}

void Type_Float64::ToString( String & s ) {
	s += "double";
}

ulong Type_Float64::Size() const {
	return 8;
}

#pragma endregion

#pragma region "Type Float80"
/*
Type_Float80::Type_Float80()
{
	m_name = "float80";
	m_typeKind = TYPE_FLOAT32;
	m_mods = NO_MODIFIERS;

	mInfo = "float80";
}

Type_Float80::~Type_Float80() {
}

const char * Type_Float80::ToChars() const {
	return "'float80'";
}

void Type_Float80::ToString( String & s ) {
	s += "float80";
}

ulong Type_Float80::Size() const {
	return 10;
}

bool Type_Float80::IsIntegral() const {
	return false;
}

bool Type_Float80::IsFloating() const {
	return true;
}

bool Type_Float80::IsReal() const {
	return true;
}
*/
#pragma endregion

#pragma region "Type String"

DEFINE_TYPE( Type_String, PrimitiveType );

Type_String::Type_String()
{
	m_name = "string";
	m_typeKind = TYPE_FLOAT32;
	m_mods = NO_MODIFIERS;

	mInfo = "string";
}

Type_String::~Type_String() {
}

const char * Type_String::ToChars() const {
	return "'string'";
}

void Type_String::ToString( String & s ) {
	s += "string";
}

ulong Type_String::Size() const {
	c4::common->Warning( "Type_String::Size() cannot be determined!" );
	return sizeof( char );
}

bool Type_String::IsIntegral() const {
	return false;
}

bool Type_String::IsFloating() const {
	return false;
}

#pragma endregion

}//end namespace c4