/*
======================================
	ast nodes for declarations
======================================
*/
#ifndef __DECLARATION_H__
#define __DECLARATION_H__

namespace c4 {
	
enum STORAGE_CLASS {
	SC_UNDEFINED			= 0,		// this enum constant must be 0 ( undefined storage class = 0 )
	SC_STATIC				= BIT( 0 ),
	SC_EXTERN				= BIT( 1 ),
	SC_CONST				= BIT( 2 ),
	SC_FINAL				= BIT( 3 ),
	SC_ABSTRACT				= BIT( 4 ),
	SC_PARAMETER			= BIT( 5 ),
	SC_FIELD				= BIT( 6 ),
	SC_OVERRIDE				= BIT( 7 ),
	SC_AUTO					= BIT( 8 ),
	SC_SYNCHRONIZED			= BIT( 9 ),
	SC_DEPRECATED			= BIT( 10 ),
	SC_IN					= BIT( 11 ),	// in parameter
	SC_OUT					= BIT( 12 ),	// out parameter
	SC_LAZY					= BIT( 13 ),	// lazy parameter
	SC_FOREACH				= BIT( 14 ),	// variable for foreach loop
	SC_COMDAT				= BIT( 15 ),	// should go into COMDAT record
	SC_VARIADIC				= BIT( 16 ),	// variadic function argument
	SC_CTORINIT				= BIT( 17 ),	// can only be set inside constructor
	SC_TEMPLATE_PARAMETER	= BIT( 18 ),	// template parameter
	SC_SCOPE				= BIT( 19 ),
	SC_INVARIANT			= BIT( 20 ),
	SC_REF					= BIT( 21 ),	// ref parameter
	SC_INIT					= BIT( 22 ),	// has explicit initializer
	SC_MANIFEST				= BIT( 23 ),	// manifest constant
	SC_NODTOR				= BIT( 24 ),	// don't run destructor
	SC_NOTHROW				= BIT( 25 ),	// never throws exceptions
	SC_PURE					= BIT( 26 ),	// pure function
	SC_THREAD_LOCAL			= BIT( 27 ),	// thread local
	SC_VIRTUAL				= BIT( 28 )
};

enum LINKAGE {
	DEFAULT_LINKAGE,
	LINK_C,
	LINK_C4,
	LINK_CPP,
	LINK_STDCALL,
	LINK_PASCAL
};

enum PROTECTION_LEVEL {
	PL_NONE,			// no access
	PL_PRIVATE,
	PL_INTERNAL,
	PL_PACKAGE,
	PL_PROTECTED,
	PL_PUBLIC,
	PL_EXPORT,
	PL_UNDEFINED
};

//==================================================================

const char * ToStr( STORAGE_CLASS );
void		 ToStr( STORAGE_CLASS, String & outStr );

const char * ToStr( LINKAGE );
void		 ToStr( LINKAGE, String & outStr );

const char * ToStr( PROTECTION_LEVEL );
void		 ToStr( PROTECTION_LEVEL, String & outStr );

//==================================================================

class Scope;

//-------------------------------------------------------
//	class Declaration
//
//		is the base class for all ast nodes,
//	representing declarations ( variable declarations,
//	class, interface or enum decls, etc. ).
//-------------------------------------------------------
//
class Declaration : public AST_Node {
public:
	CLASS_PROTOTYPE( Declaration );
	VISITABLE();

				Declaration();
				Declaration( Type* type, Identifier* name );
				~Declaration();

	virtual void		Resolve( Semantics* semantics );

	FASTBOOL			IsStatic() const;
	FASTBOOL			IsCtorInit() const;
	FASTBOOL			IsExtern() const;
	FASTBOOL			IsFinal() const;
	FASTBOOL			IsAbstract() const;
	FASTBOOL			IsConst() const;
	FASTBOOL			IsInvariant() const;
	FASTBOOL			IsAuto() const;
	FASTBOOL			IsScope() const;
	FASTBOOL			IsSynchronized() const;
	FASTBOOL			IsParameter() const;
	FASTBOOL			IsVirtual() const;
	FASTBOOL			IsOverride() const;
	FASTBOOL			IsDeprecated() const;
	FASTBOOL			IsIn() const;
	FASTBOOL			IsOut() const;
	FASTBOOL			IsRef() const;

	virtual FASTBOOL	IsInDataSegment() const;
	virtual FASTBOOL	IsInCodeSegment() const;

	Declaration *		IsDeclaration();

	Type *				GetType();
	Identifier *		GetName();

	STORAGE_CLASS		GetStorageClass() const;
	PROTECTION_LEVEL	GetProtection() const;
	LINKAGE				GetLinkage() const;

						// adds the specified storage class to this declaration ( bitwise 'or' )
	void				SetStorageClass( STORAGE_CLASS sc );
	void				SetProtection( PROTECTION_LEVEL pl );
	void				SetLinkage( LINKAGE linkage );

	// ui
	void		UpdateUIinfo();
	void		ToString( String & s );

protected:
	Type *				m_type;	// type of the thing being declared
	Identifier *		m_id;	// name of the thing being declared

protected: 
	unsigned int		m_storageClass;	 // enum STORAGE_CLASS
	unsigned int		m_protection;	 // enum PROTECTION_LEVEL
	unsigned int		m_linkage;		 // enum LINKAGE
	//STORAGE_CLASS		m_storageClass;
	//PROTECTION_LEVEL	m_protection;
	//LINKAGE				m_linkage;
};

INLINE Type * Declaration::GetType() {
	return m_type;
}

INLINE Identifier * Declaration::GetName() {
	return m_id;
}

inline STORAGE_CLASS Declaration::GetStorageClass() const {
	return ( STORAGE_CLASS )m_storageClass;
}

inline PROTECTION_LEVEL Declaration::GetProtection() const {
	return ( PROTECTION_LEVEL )m_protection;
}

inline LINKAGE Declaration::GetLinkage() const {
	return ( LINKAGE )m_linkage;
}

INLINE void Declaration::SetStorageClass( STORAGE_CLASS sc ) {
	m_storageClass |= sc;
}

INLINE void Declaration::SetProtection( PROTECTION_LEVEL pl ) {
	m_protection |= pl;
}

INLINE void Declaration::SetLinkage( LINKAGE linkage ) {
	m_linkage |= linkage;
}

INLINE FASTBOOL Declaration::IsStatic() const {
	return m_storageClass & SC_STATIC;
}

INLINE FASTBOOL Declaration::IsCtorInit() const {
	return m_storageClass & SC_CTORINIT;
}

INLINE FASTBOOL Declaration::IsExtern() const {
	return m_storageClass & SC_EXTERN;
}

INLINE FASTBOOL Declaration::IsFinal() const {
	return m_storageClass & SC_FINAL;
}

INLINE FASTBOOL Declaration::IsAbstract() const {
	return m_storageClass & SC_ABSTRACT;
}

INLINE FASTBOOL Declaration::IsConst() const {
	return m_storageClass & SC_CONST;
}

INLINE FASTBOOL Declaration::IsInvariant() const {
	return m_storageClass & SC_INVARIANT;
}

INLINE FASTBOOL Declaration::IsAuto() const {
	return m_storageClass & SC_AUTO;
}

INLINE FASTBOOL Declaration::IsScope() const {
	return m_storageClass & SC_SCOPE;
}

INLINE FASTBOOL Declaration::IsSynchronized() const {
	return m_storageClass & SC_SYNCHRONIZED;
}

INLINE FASTBOOL Declaration::IsParameter() const {
	return m_storageClass & SC_PARAMETER;
}

INLINE FASTBOOL Declaration::IsVirtual() const {
	return m_storageClass & SC_VIRTUAL;
}

INLINE FASTBOOL Declaration::IsOverride() const {
	return m_storageClass & SC_OVERRIDE;
}

INLINE FASTBOOL Declaration::IsDeprecated() const {
	return m_storageClass & SC_DEPRECATED;
}

INLINE FASTBOOL Declaration::IsIn() const {
	return m_storageClass & SC_IN;
}

INLINE FASTBOOL Declaration::IsOut() const {
	return m_storageClass & SC_OUT;
}

INLINE FASTBOOL Declaration::IsRef() const {
	return m_storageClass & SC_REF;
}

INLINE FASTBOOL Declaration::IsInDataSegment() const {
	return false;
}

INLINE FASTBOOL Declaration::IsInCodeSegment() const {
	return false;
}

INLINE Declaration * Declaration::IsDeclaration() {
	return this;
}

//-------------------------------------------------------
//	List of declarations
//-------------------------------------------------------
//
class DeclarationList : public Declaration {
public:
	CLASS_PROTOTYPE( DeclarationList );
	VISITABLE();

			DeclarationList();
			~DeclarationList();

	void			Add( Declaration * decl );

	idList< Declaration* > & GetActualList();

	// overrides
	void			Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	idList< Declaration* >		m_decls;
};

INLINE idList< Declaration* > & DeclarationList::GetActualList() {
	return m_decls;
}

//-------------------------------------------------------

class Expression;
class ExpressionList;
class Statement;

class Symbol;
class VarSymbol;
class FunctionSymbol;

//-------------------------------------------------------
// Variable declaration
// 		Type id
// 		Type id = initializer
//
//	represents a declared variable
//-------------------------------------------------------
class VarDecl : public Declaration {
public:
	CLASS_PROTOTYPE( VarDecl );
	VISITABLE();

			VarDecl( Type* type, Identifier* name, Expression* init = 0 ); 
			~VarDecl();

	Expression *		GetInitializer();
	VarSymbol *			GetSymbol();

	void			Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;
	void			UpdateUIinfo();

protected:
	Expression *	m_initializer;

	VarSymbol *		m_symbol; // != NULL => semantics has been done
};

INLINE Expression * VarDecl::GetInitializer() {
	return m_initializer;
}

INLINE VarSymbol * VarDecl::GetSymbol() {
	return m_symbol;
}

//-------------------------------------------------------

enum ARGUMENT_FLOW {
	AF_NONE,
	AF_LAZY,
	AF_IN,
	AF_OUT,
	AF_REF
};

//-------------------------------------------------------
// Function argument:
// 		Type id
// 		Type id = initializer
//-------------------------------------------------------
class Argument : public VarDecl {
public:
	CLASS_PROTOTYPE( Argument );
	VISITABLE();

					Argument( Type* type, Identifier* name, Expression* defaultInitializer = 0 );
					~Argument();

	ARGUMENT_FLOW	GetFlowType() const;

	void			Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );
	const char *	uiToString() const;

protected:
	ARGUMENT_FLOW		m_flow;
};

INLINE ARGUMENT_FLOW Argument::GetFlowType() const {
	return m_flow;
}

//-------------------------------------------------------
// List of function arguments
//-------------------------------------------------------
class ArgumentList : public Declaration {
public:
	CLASS_PROTOTYPE( ArgumentList );
	VISITABLE();

					ArgumentList();
					~ArgumentList();

	void				Add( Argument * a );
	
	LinkedList< Argument* > &	GetArguments();

	void			Resolve( Semantics* semantics );

	const char* 	ToChars() const;
	void			ToString( String & s );

	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	LinkedList< Argument* >		m_argList;
};

INLINE LinkedList< Argument* > & ArgumentList::GetArguments() {
	return m_argList;
}

//-------------------------------------------------------
//	Function Declaration
//-------------------------------------------------------
class FunctionDecl : public Declaration {
public:
	CLASS_PROTOTYPE( FunctionDecl );
	VISITABLE();

			FunctionDecl( Type* returnType, Identifier *name, ArgumentList* args, Statement* body );
			~FunctionDecl();

	void			Resolve( Semantics* semantics );

	Type *			GetReturnType() { return m_type; }
	Identifier *	GetName()		{ return m_id; }
	ArgumentList *	GetArguments()	{ return m_args; }
	Statement *		GetBody()		{ return m_body; }

	FunctionSymbol *	GetSymbol()		{ return m_symbol; }

	const char *	ToChars() const;
	void			ToString( String & s );
	void			HeaderToString( String & s );	// pretty-print the function header

	bool			IsMain() const;

	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	ArgumentList *		m_args;
	Statement *			m_body;

	FunctionSymbol *	m_symbol;
};

//-------------------------------------------------------
//	Namespace Declaration
//-------------------------------------------------------
class NamespaceDecl : public Declaration {
public:
	CLASS_PROTOTYPE( NamespaceDecl );
	VISITABLE();
					NamespaceDecl( Identifier* name = 0 );
					~NamespaceDecl();

	void				Add( Declaration* decl );
	DeclarationList *	GetDeclarations();

	void			Resolve( Semantics* semantics );

	const char *		ToChars() const;
	void				ToString( String & s );

	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	DeclarationList *	m_declList;
};

INLINE DeclarationList * NamespaceDecl::GetDeclarations() {
	return m_declList;
}

//-------------------------------------------------------
//	Typedef Declaration
//-------------------------------------------------------
class TypedefDecl : public Declaration {
public:
	CLASS_PROTOTYPE( TypedefDecl );
	VISITABLE();

			TypedefDecl( Identifier* name, Type* basetype, Expression* init = 0 );
			~TypedefDecl();

	void			Resolve( Semantics* semantics );

	Type *			GetBaseType();
	
	const char *	ToChars() const;
	void			ToString( String & s );

	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	Type *			m_baseType;
	Expression *	m_init;		// initializer
};

INLINE Type * TypedefDecl::GetBaseType() {
	return m_baseType;
}

//=======================================================

//-------------------------------------------------------
//	class DeclarationList_Owner_Base
//
//	is the base class for all ast nodes,
// containing list of declarations
// ( for example, enums, classes, namespaces, etc. ).
// Example of an anonymous declaration:
//	
//		enum {
//			ENUM_MEMBER_NO_1,
//			ENUM_MEMBER_NO_2,
//			/* .... */
//		};
//-------------------------------------------------------
class DeclarationList_Owner_Base : public Declaration {
public:
	CLASS_PROTOTYPE( DeclarationList_Owner_Base );
	VISITABLE();

		DeclarationList_Owner_Base( Type* type, Identifier* name = 0 ); // name == 0 => anonymous declaration
		~DeclarationList_Owner_Base();

	void		Add( Declaration* decl );
	void		SetBody( DeclarationList* declList );
	
	DeclarationList *	GetDeclarations();

	//void			Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	DeclarationList *	m_declList;
};

INLINE DeclarationList * DeclarationList_Owner_Base::GetDeclarations() {
	return m_declList;
}

//-------------------------------------------------------
//	Enum Declaration
//-------------------------------------------------------
class EnumDecl : public DeclarationList_Owner_Base {
public:
	CLASS_PROTOTYPE( EnumDecl );
	VISITABLE();

				EnumDecl( Identifier* name, const Location & loc );
				~EnumDecl();

	void			Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	const char *	uiToString() const;
};

//-------------------------------------------------------
//	Class Declaration
//-------------------------------------------------------
class ClassDecl : public Declaration {
public:
	CLASS_PROTOTYPE( ClassDecl );
	VISITABLE();

				ClassDecl( Identifier* name, DeclarationList* decl, const Location & loc );
				~ClassDecl();

	void		Resolve( Semantics* semantics );

	void				SetBody( DeclarationList* declList );
	DeclarationList *	GetDeclarations();

	const char *	ToChars() const;
	void			ToString( String & s );

	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	DeclarationList *	m_declList;
};

INLINE DeclarationList * ClassDecl::GetDeclarations() {
	return m_declList;
}

//-------------------------------------------------------
//	Constructor
//-------------------------------------------------------
class CtorDecl : public Declaration {
public:
	CLASS_PROTOTYPE( CtorDecl );
	VISITABLE();

				CtorDecl( ArgumentList* args, Statement* body );
				~CtorDecl();

	ArgumentList *		GetArguments()	{ return m_args; }
	Statement *			GetBody()		{ return m_body; }

	void			Resolve( Semantics* semantics );

	const char *	ToChars() const;
	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	ArgumentList *		m_args;
	Statement *			m_body;
};

//-------------------------------------------------------
//	Destructor
//-------------------------------------------------------
class DtorDecl : public Declaration {
public:
	CLASS_PROTOTYPE( DtorDecl );
	VISITABLE();

				DtorDecl( Statement* body );
				~DtorDecl();

	Statement *			GetBody()		{ return m_body; }

	void			Resolve( Semantics* semantics );

	const char *	ToChars() const;
	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	Statement *			m_body;
};

}//end namespace c4 

#endif // !__DECLARATION_H__