#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

/*
=========================
	Object::Object()

	referenceCounter == 0 => no one needs the object
	and the object can be safely deleted.
=========================
*/
Object::Object()
	//: referenceCounter( 1 )
	: referenceCounter( 0 )		
{
	allObjects.Append( this );
}

Object::~Object() {
	allObjects.Remove( this );
}

bool Object::Equals( const Object * other ) const {
	return ( this == other );
}

hash_t Object::GetHash() const {
	return ( hash_t ) this;
}

int Object::Compare( const Object * other ) const {
	return ( this - other );
}

//const char * Object::ToChars() const {
//	return "Object";
//}

int Object::GetId() const {
	return ID_OBJECT;
}

// TODO:
void Object::ToString( String & s ) {
	s += "Object";
}

//==========================================================================
/*
// BUG: Object::Clear()
// TODO: replace dynamic array with a doubly-linked list
void Object::Clear() {
	//c4::common->Print( "Object::Clear(): deleting all non-referenced objects..." );
	
	for ( int i = 0; i < allObjects.Num(); i++ ) 
	{
		if ( 0 == allObjects[i]->GetReferenceCount() ) {
			delete allObjects[i];
			//allObjects.RemoveIndex( i );
			allObjects[i] = NULL;
		}
	}

	//DisplayObjects();
}

/*
=========================
	statics
=========================
*/

idList< Object* > Object::allObjects( 256 );

TypeInfo	Object::typeInfo( "Object", NULL );
/*
int Object::GetNumObjects() {
	return allObjects.Num();
}

void Object::DeleteAllObjects() {
	allObjects.Clear();
}

void Object::DisplayObjects() {
	c4::common->Print( "-----------------------------------------------------" );
	for ( int i = 0; i < allObjects.Num(); i++ ) {
		if ( allObjects[ i ] ) {
			c4::common->Print( allObjects[ i ]->ToChars() );
		}
	}
	c4::common->Print( "======== %d Objects ================================", allObjects.Num() );
}
*/
//============================================================

TypeInfo::TypeInfo( const char* name, TypeInfo* parentType )
	: m_name( name )
	, m_hashCode( Hashing::hashpjw( name ) )	
	, m_parentType( parentType )
{
}

TypeInfo::~TypeInfo() {
}

bool TypeInfo::Is( TypeInfo* type )
{
	TypeInfo * startType = this;

	while ( startType ) 
	{
		if ( startType == type ) {
			return true;
		}
		startType = startType->ParentType();
	}
	return false;
}

//bool TypeInfo::Is( const char* classname ) const {
//	return m_hashCode == Hashing::hashpjw( classname );	
//}

}//end namespace c4