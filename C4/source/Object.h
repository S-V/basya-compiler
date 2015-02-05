/*
======================================
	Object, the base class for most objects.

	All objects are reference-counted and linked into the list 
	which is a static member of the class Object. 
	Objects with zero reference counter can be deleted from the list.
	Reference counting is done through the methods Grab() and Drop().
		
======================================
*/
#ifndef __ROOT_OBJECT_H__
#define __ROOT_OBJECT_H__

namespace c4 {

typedef uint	hash_t;

/*
==========================================================================
	class List
		TODO:
==========================================================================
*/
template< class T >
class List : public idList< T > {
};


enum ID_CODE		// useful for questioning an object's general type
{
	ID_OBJECT		= 0,
	ID_IDENTIFIER,
	ID_SYMBOL,
	ID_EXPRESSION,
	ID_TYPE,
	ID_TUPLE,
	ID_PARSE_TREE
};

class TypeInfo;

/*
=========================================
	class Object

	The root of our class hierarchy.
=========================================
*/

class Object {
protected:
	virtual					~Object();

public:
							Object();

	virtual bool			Equals( const Object * other ) const;

							// returns a hash code, useful for things like building hash tables of Objects.
	virtual hash_t			GetHash() const;

							// returns <0, 0, or >0, useful for sorting Objects
	virtual int				Compare( const Object * other ) const;

	virtual const char *	ToChars() const = 0;

							// returns a unique number defined by the user. For Object, the return value is 0.
	virtual int				GetId() const;

							// for dumping this object's contents ( for example, expressions may write themselves as text )
	virtual void			ToString( String & s );

	//-- run-time type information ----------------------------------------
	virtual TypeInfo *			GetTypeInfo( void ) const;
	virtual unsigned int		IdCode() const;


	//-- reference counting ------------------------------------------

			void	Grab() const;
			bool	Drop() const;	// returns true if the object has been deleted

	uint		GetReferenceCount() const;
/*
					// delete all non-referenced objects ( those with num. of ref. == 0 )
	static void		Clear();

	//------------------------------------------

	// debugging
	//
	static int			GetNumObjects();	// useful for detecting memory leaks

						// shows all existing objects through the common
	static void			DisplayObjects();
						// make sure no one's pointing at Object before calling this!
	static void			DeleteAllObjects();
*/
private:
	mutable uint	referenceCounter;

	// TODO: replace with a doubly-linked list
	static idList< Object* >	allObjects;

public:
	static TypeInfo		typeInfo;
};

INLINE TypeInfo * Object::GetTypeInfo( void ) const {
	return & typeInfo;
}

INLINE unsigned int Object::IdCode() const {
	return 0;
}

INLINE void Object::Grab() const {
	++referenceCounter;
}

INLINE bool Object::Drop() const
{
	BREAK_IF( referenceCounter <= 0 )

	--referenceCounter;
	if ( !referenceCounter ) {
		delete this;
		return true;
	}
	return false;
}

INLINE uint Object::GetReferenceCount() const {
	return referenceCounter;
}

//--------------------------------------------------------------------------------

/*
=========================================
	class TypeInfo

	for providing dynamic type information

	All our class will have TypeInfo as a static member.
	This is done with macros.
=========================================
*/

// should be included in .h
#define DECLARE_TYPE( nameOfType )							\
public:/*private:*/											\
	static	TypeInfo			typeInfo;					\
public:														\
	virtual	TypeInfo *			GetTypeInfo( void ) const;	\
	virtual unsigned int		IdCode() const;

//--------------------------------------------------------------------------------

// should be included in every objects's interface ( *.h )
//#define DECLARE_ABSTRACT( nameOfType )						\
//public:														\
//	virtual	TypeInfo *			GetType( void ) const = 0;	\
//	virtual unsigned int		IdCode() const = 0;


//--------------------------------------------------------------------------------

// should be included in every object's implementation ( *.cpp )
#define DEFINE_TYPE( nameOfClass, nameOfSuperClass )									\
	TypeInfo	nameOfClass::typeInfo( #nameOfClass, & nameOfSuperClass::typeInfo );	\
	TypeInfo *	nameOfClass::GetTypeInfo( void ) const { return &typeInfo; }			\
	unsigned int	nameOfClass::IdCode() const {										\
		return Hashing::hashpjw( typeInfo.Name() );										\
	}

//--------------------------------------------------------------------------------

class TypeInfo {
public:
					TypeInfo( const char* name, TypeInfo* parentType );
					~TypeInfo();

	const char *		Name() const;
	TypeInfo *			ParentType();
	bool				Is( TypeInfo * type );
	//bool				Is( const char* classname ) const;

private:
	mutable String		m_name;		// the name of this type
	mutable int			m_hashCode;
	TypeInfo *			m_parentType;	// parent of this type

private:
					TypeInfo( const TypeInfo & );	// prevent copying
	TypeInfo &		operator = ( const TypeInfo & );	// prevent assignment
};

INLINE const char * TypeInfo::Name() const {
	return m_name;
}

INLINE TypeInfo * TypeInfo::ParentType() {
	return m_parentType;
}

}//end namespace c4

#endif // !__ROOT_OBJECT_H__