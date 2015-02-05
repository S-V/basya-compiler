#ifndef __BASIC_TYPES_H__
#define __BASIC_TYPES_H__

/*
==========================================================================
	Built-in types.
==========================================================================
*/

namespace c4 {

class IntegralType : public PrimitiveType {
public:
	// overrides
	bool	IsIntegral() const	{ return true; }
	bool	IsFloating() const	{ return false; }
};

class Type_Char : public IntegralType {
public:
	CLASS_PROTOTYPE( Type_Char );

				Type_Char();
				~Type_Char();

	// overrides
	const char *	ToChars() const;
	void			ToString( String & s );

	ulong		Size() const;
};

//==========================================

// ??? should Type_Bool inherit some BooleanType ???
//
class Type_Bool : public IntegralType 
{
public:
	CLASS_PROTOTYPE( Type_Bool );

				Type_Bool();
				~Type_Bool();

	// overrides
	bool		IsBoolean() const { return true; }

	const char *	ToChars() const;
	void			ToString( String & s );
	ulong			Size() const;

	bool	SetModifiers( int32 mods );
};

//==========================================

class Type_Int8 : public IntegralType {
public:
	CLASS_PROTOTYPE( Type_Int8 );

				Type_Int8();
				~Type_Int8();

	// overrides
	const char *	ToChars() const;
	void			ToString( String & s );
	ulong			Size() const;
};

//==========================================

class Type_Int16 : public IntegralType {
public:
	CLASS_PROTOTYPE( Type_Int16 );

				Type_Int16();
				~Type_Int16();

	// overrides
	const char *	ToChars() const;
	void			ToString( String & s );
	ulong			Size() const;
};

//==========================================

class Type_Int32 : public IntegralType {
public:
	CLASS_PROTOTYPE( Type_Int32 );

				Type_Int32();
				~Type_Int32();

	// overrides
	const char *	ToChars() const;
	void			ToString( String & s );
	ulong			Size() const;
};

//==========================================

class Type_Int64 : public IntegralType {
public:
	CLASS_PROTOTYPE( Type_Int64 );

				Type_Int64();
				~Type_Int64();

	// overrides
	const char *	ToChars() const;
	void			ToString( String & s );
	ulong			Size() const;
};

//==========================================

class FloatingPointType : public PrimitiveType {
public:
	// overrides
	bool	IsIntegral() const	{ return false; }
	bool	IsFloating() const	{ return true; }

	bool	SetModifiers( int32 mods );
};

// float
//
class Type_Float32 : public FloatingPointType {
public:
	CLASS_PROTOTYPE( Type_Float32 );

				Type_Float32();
				~Type_Float32();

	// overrides
	const char *	ToChars() const;
	void			ToString( String & s );
	ulong			Size() const;
};

//==========================================

// double
//
class Type_Float64 : public FloatingPointType {
public:
	CLASS_PROTOTYPE( Type_Float64 );

				Type_Float64();
				~Type_Float64();

	// overrides
	const char *	ToChars() const;
	void			ToString( String & s );
	ulong			Size() const;
};

//==========================================
/*
class Type_Float80 : public FloatingPointType 
{
	friend class Type;

				Type_Float80();
				~Type_Float80();

public:
	// overrides
	const char *	ToChars() const;
	void			ToString( String & s );
	ulong			Size() const;
};
*/
//==========================================

class Type_String : public PrimitiveType {
public:
	CLASS_PROTOTYPE( Type_String );

				Type_String();
				~Type_String();

	// overrides
	const char *	ToChars() const;
	void			ToString( String & s );
	ulong			Size() const;

	bool			IsIntegral() const;
	bool			IsFloating() const;
};

}//end namespace c4

#endif // !__BASIC_TYPES_H__