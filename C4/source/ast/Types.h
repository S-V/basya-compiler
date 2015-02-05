/*
======================================
	classes, representing types.
======================================
*/
#ifndef __TYPES_H__
#define __TYPES_H__

namespace c4 {

enum TYPE_KIND
{
	STATIC_ARRAY,
	//DYNAMIC_ARRAY,
	//ASSOCIATIVE_ARRAY,
	POINTER,
	//REFERENCE,
	//FUNCTION,
	//IDENTIFIER,
	CLASS,
	//STRUCT,
	ENUM,
	//TYPEDEF,
	//DELEGATE,

	TYPE_NONE,		// a pseudo-type used where no actual type is appropriate
	TYPE_VOID,		// the pseudo-type corresponding to the keyword "void"
	TYPE_NULL,		// the null type
	TYPE_INT8,
	TYPE_INT16,
	TYPE_INT32,
	TYPE_INT64,
	TYPE_FLOAT32,
	TYPE_FLOAT64,
	FLOAT80,
	STRING,

	BIT,
	BOOL,
	CHAR,
	WCHAR,
	//DCHAR,

//	INSTANCE,
//	TYPEOF,
//	TUPLE,
//	SLICE,
//	RETURN,

//	WILDCARD,	// a wildcard type argument

	TYPE_ERROR,		// a class or interface type that could not be resolved; for error recovery
	//TYPE_UNKNOWN,	// an implementation-reserved type; should not be used

	NUM_OF_TYPES,
};

enum MODIFIERS {
	NO_MODIFIERS = 0,
	MOD_CONST	= BIT( 1 ),
	INVARIANT	= BIT( 2 ),
	UNSIGNED	= BIT( 3 ),	
};

enum { SIGNED = BIT( 4 ) };

//-------------------------------------------------------

// forward declarations
class Scope;
class Semantics;

class ClassDecl;

//-------------------------------------------------------

class Type : public AST_Node {
protected:
				Type();	
	virtual		~Type();

public:
				CLASS_PROTOTYPE( Type );

	//TODO: remove this in the future
	static void		StaticInit();

	//-- semantic resolution ---------------------------

	virtual Type *		ResolveInto( Semantics* semantics );

public:
	override const char *	ToChars() const;
	override void			ToString( String & s );
	override bool			Equals( const Object *other ) const;

public: // common type interface

						// returns the size of this type in bytes
	virtual ulong		Size() const;

	virtual bool		IsBuiltIn() const;
	virtual bool		IsPrimitive() const;
	virtual bool		IsBoolean() const; // <= TODO: remove this
	virtual bool		IsIntegral() const;
	virtual bool		IsFloating() const;
	virtual bool		IsSigned() const;
	virtual bool		IsUnsigned() const;
	virtual bool		IsAuto() const;
	virtual bool		IsAssignable() const;
	virtual bool		IsConst() const;
	virtual bool		IsInvariant() const;
	virtual bool		IsMutable() const;

						// returns true if this type can be converted to _boolean value
	virtual bool		CheckBoolean() const;

	virtual Type *		IsConstOf() const;
	virtual Type *		IsInvariantOf() const;
	virtual Type *		IsMutableOf() const;
	virtual Type *		IsPointerTo() const;
	virtual Type *		IsReferenceTo() const;
	virtual Type *		IsArrayOf() const;
	virtual ClassDecl *	IsClassHandle() const;

						// if this is a type of something, return that something
	virtual Type *		TypeOf();

	virtual Expression *	DefaultInitializer() const;

					// returns true if this type has pointers
					// that need to be scanned by the garbage collector
	virtual bool	HasPointers() const;

						// if this is a shell around another type, get that other type
	virtual Type *		ToBaseType();

	//-------------------------------------------
			void		MakeConst();
			void		MakeInvariant();

					// set type modifiers ( const, signed / unsigned, etc. )
					// can be overriden in derived classes to validate new modifiers;
					// returns true if new modifiers were accepted, returns false in case of error
	virtual bool	SetModifiers( int32 mods );
	//-------------------------------------------

	virtual TYPE_KIND	GetTypeKind() const;

						// do an implicit conversion between the specified types;
						// returns a new type node or returns NULL if no such conversion exists.
						// For example, the following widening conversions are possible:
						//				( int, float ) => float
						//				( int, byte ) => int
						//				( long, pointer ) => long
	static Type *		GetCommonType( Type* t1, Type* t2 );

protected:
	TYPE_KIND		m_typeKind;
	int32			m_mods;	//MODIFIERS
	String			m_name;

public:
	String			mInfo;  // for storing additional information, for example, "pointer to pointer to int"

	bool		operator == ( TYPE_KIND	 typeKind ) const;

private:
	// these tables are for implicit conversion of binary operations;
	// the indices are the type of operand one, followed by operand two
	static TYPE_KIND	m_convTable[ NUM_OF_TYPES ][ NUM_OF_TYPES ];
};

//=======================================================

class BuiltInType : public Type {
protected:
	CLASS_PROTOTYPE( BuiltInType );

						BuiltInType();
						~BuiltInType();
public:
	override bool	IsBuiltIn() const { return true; }
	void			ToString( String & s );
};

//-------------------------------------------------------

class PrimitiveType : public BuiltInType {
public:				
	CLASS_PROTOTYPE( PrimitiveType );

	override bool	IsPrimitive() const { return true; }

	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;
};

//-------------------------------------------------------

class Type_Void : public PrimitiveType {
public:
	CLASS_PROTOTYPE( Type_Void );

		Type_Void();
		~Type_Void();

	const char *	ToChars() const;
	const char *	uiToString() const;

	TYPE_KIND		GetTypeKind() const;
};

inline TYPE_KIND Type_Void::GetTypeKind() const {
	return TYPE_VOID;
}

//=======================================================

class Type_Null : public PrimitiveType 
{
	friend class Type;

				Type_Null();
				~Type_Null();

public:
	CLASS_PROTOTYPE( Type_Null );

	// overrides
	const char *	ToChars() const;
	ulong			Size() const;

	bool			IsIntegral() const;
	bool			IsFloating() const;
	bool			IsReal() const;

	TYPE_KIND		GetTypeKind() const;
	
	void			ToString( String & s );
	const char *	uiToString() const;
};

inline TYPE_KIND Type_Null::GetTypeKind() const {
	return TYPE_NULL;
}

//-------------------------------------------------------
//	class NonPrimitiveType
//
//	represents types that should be resolved, for example, pointers to and 
//	arrays of some non-primitive types, user-defined data types 
//	( interfaces, classes, structs, enums, etc. ).
//-------------------------------------------------------
//
class NonPrimitiveType : public Type {
public:
	CLASS_PROTOTYPE( NonPrimitiveType );

						NonPrimitiveType();
						NonPrimitiveType( const Location & loc );
	virtual				~NonPrimitiveType();

	virtual Type *		ResolveInto( Semantics* semantics );
	
	const char *		ToChars() const;
	void				ToString( String & s );

	const Location &	GetLocation() const;

	bool				IsPrimitive() const { return false; }

protected:
	Location		m_location;
};

INLINE const Location & NonPrimitiveType::GetLocation() const {
	return m_location;
}

/*
==========================================================================
	Pointer
		represents a type of pointer
==========================================================================
*/
class TypePointer : public NonPrimitiveType {
public:
	CLASS_PROTOTYPE( TypePointer );

					TypePointer( Type * target );
					~TypePointer();

	Type *			ResolveInto( Semantics* semantics );
	Type *			GetTargetType();
	
	ulong			Size() const;
	
	const char *	ToChars() const;
	void			ToString( String & s );
	
	TYPE_KIND		GetTypeKind() const;

	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;
	const char *	uiToString() const;

protected:
	Type *		m_targetType;
};

INLINE Type * TypePointer::GetTargetType() {
	return m_targetType;
}

inline TYPE_KIND TypePointer::GetTypeKind() const {
	return POINTER;
}

//-------------------------------------------------------

// forward declarations
class Expression;
class Identifier;

/*
==========================================================================
	Static Array ( with a fixed dimension )
==========================================================================
*/
class TypeStaticArray : public TypePointer {
public:
	CLASS_PROTOTYPE( TypeStaticArray );

					TypeStaticArray( Type* targetType, Expression* dim );
					~TypeStaticArray();

	Type *			ResolveInto( Semantics* semantics );
	Expression *	GetExpression();

	const char *	ToChars() const;
	void			ToString( String & s );

	TYPE_KIND		GetTypeKind() const;

	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;
	const char *	uiToString() const;

protected:
	Expression *	m_dimension;
};

INLINE Expression * TypeStaticArray::GetExpression() {
	return m_dimension;
}

inline TYPE_KIND TypeStaticArray::GetTypeKind() const {
	return STATIC_ARRAY;
}

//-------------------------------------------------------
//		class TypeQualified
//
// examples: CType::InnerType, SomeType.MemberType.YetAnotherMemberType
// MyNamespace.MyClass
//-------------------------------------------------------
//
class TypeQualified : public NonPrimitiveType {
public:
	CLASS_PROTOTYPE( TypeQualified );

					TypeQualified( Identifier* id, const Location & loc );
					~TypeQualified();

	Type *			ResolveInto( Semantics* semantics );

	void			Add( Identifier * id );

	const char *	ToChars() const;
	void			ToString( String & s );

	TYPE_KIND		GetTypeKind() const;

	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;
	const char *	uiToString() const;

private:
	LinkedList< Identifier* >	m_idList;  // array of Identifier's representing ident.ident.ident etc.
};

//-------------------------------------------------------

class TypeEnum : public NonPrimitiveType {
public:
	CLASS_PROTOTYPE( TypeEnum );

					TypeEnum( Identifier* id, const Location & loc );
					~TypeEnum();

	Type *		ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	TYPE_KIND		GetTypeKind() const;

	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;
	const char *	uiToString() const;
};

inline TYPE_KIND TypeEnum::GetTypeKind() const {
	return ENUM;
}

//-------------------------------------------------------

class TypeClass : public NonPrimitiveType {
public:
	CLASS_PROTOTYPE( TypeClass );

					TypeClass( Identifier* id, const Location & loc );
					~TypeClass();

	Type *			ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	TYPE_KIND		GetTypeKind() const;

	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;
	const char *	uiToString() const;
};

inline TYPE_KIND TypeClass::GetTypeKind() const {
	return CLASS;
}

//-------------------------------------------------------

class Type_Error : public PrimitiveType {
public:
			CLASS_PROTOTYPE( Type_Error );

		Type_Error( const char* message, const Location & loc );
		~Type_Error();

	const char *	GetMessage() const;
	Location &		GetLocation();

	const char *	ToChars() const;
	void			ToString( String & s );

	TYPE_KIND		GetTypeKind() const;

	Type *			ResolveInto( Semantics* semantics );

private:
	String			m_message;
	Location		m_location;
};

inline TYPE_KIND Type_Error::GetTypeKind() const {
	return TYPE_ERROR;
}

}//end namespace c4

#endif // !__TYPES_H__