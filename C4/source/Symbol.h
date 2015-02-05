#ifndef __SYMBOL_H__
#define __SYMBOL_H__

namespace c4 {

class Symbol;
class Identifier;

/*
======================================
	class SymbolTable

	never deletes its symbols, 
	only stores them in a hash table;
	symbols are destroyed by ast nodes.
======================================
*/
class SymbolTable : public Object {
public:
				SymbolTable();
				~SymbolTable();

					// returns NULL, if symbol not found
	Symbol *		Lookup( const Identifier * id );
	Symbol *		Lookup( const char * text );

					// inserts a symbol in table; returns NULL if it's already there
	Symbol *		Insert( Symbol * s );

	const char *	ToChars() const;

protected:
	Hash< Symbol* >		m_hash;
};

class ScopeSymbol;

/*
======================================
	class Scope

	Parent scopes delete their children.
	Thus, all scopes will be deleted by the root ( global ) scope
	which in turn will be deleted by Semantics.
======================================
*/
class Scope {
public:				
			Scope( Scope * parent );

				// lookup a symbol in this scope and all parent scopes
	Symbol *	Search( const Identifier* id );
	Symbol *	Search( const char * name );

				// returns NULL if such name already exists
	Symbol *	Insert( Symbol * s );

	bool		IsGlobal() const;

				// returns the lexical parent of this scope ( the enclosing scope )
	Scope *		GetParentScope() { return m_parent; }

public:
	ScopeSymbol *		mSymbol;		// parent symbol which has this scope

	LINKAGE				linkage;
	STORAGE_CLASS		storageClass;
	PROTECTION_LEVEL	protection;

	FunctionDecl *		enclosingFunction;	// function we are in

protected:
	Scope *				m_parent;
	SymbolTable *		m_symTable;
	List< Scope* >		m_children;

private:
	friend class Semantics;

				~Scope();
			
	void		AddChild( Scope * sc );

				// called in dtor
	void		DeleteChildren();
};

/*
==========================================================================
	class Semantics
==========================================================================
*/
class Semantics {
public:
			Semantics();
			~Semantics();

					// returns the global scope
	Scope *			GetGlobal() { return m_globalScope; }

					// returns the current context
	Scope *			GetContext() { return m_currentScope; }

					// returns the previous context
	Scope *			SetContext( Scope* scope );

	void			SetFunction( FunctionDecl * function );
	FunctionDecl *	CurrentFunction();

private:
	Scope *			m_globalScope;
	Scope *			m_currentScope;

	FunctionDecl *	m_currentFunction;

private:
	void	Init();
};


/*
==========================================================================
	class Symbol

		determines semantics of ast nodes.
==========================================================================
*/
class Symbol : public Object {
public:
	CLASS_PROTOTYPE( Symbol );

					// if id == 0 then it's anonymous
					Symbol( Identifier * id = 0 );
					~Symbol();

	Symbol *			GetParent();
	Location &			GetLocation();
	Identifier *		GetIdentifier();

	const char *		Name() const;

	virtual void		Resolve( Scope * sc );

	//virtual uint		GetSize() const;

	void	IncUsageCount() { ++m_iUsageCount; }
	int		GetUsageCount() const { return m_iUsageCount; }

	// overrides
	const char *	ToChars() const;
	bool			Equals( const Object* other ) const;
	int				GetId() const;

protected:
	Identifier *	m_id;	// == NULL => this symbol is anonymous
	int		m_iUsageCount;	// how many times has this symbol been used?
};

/*
==========================================================================
	class ScopeSymbol
		
	is a symbol that generates a scope.
	Doesn't delete its scope, all scopes
	are deleted by their enclosing scopes
	( lexical parent scopes ).
==========================================================================
*/
class ScopeSymbol : public Symbol {
public:
	CLASS_PROTOTYPE( ScopeSymbol );

				ScopeSymbol( Scope * enclosing );
				~ScopeSymbol();

	void		SetScope( Scope * sc );
	Scope *		GetScope();

	const char *	ToChars() const;

protected:
	Scope *		m_scope;
};

INLINE void ScopeSymbol::SetScope( Scope * sc ) {
	m_scope = sc;
}

INLINE Scope * ScopeSymbol::GetScope() {
	return m_scope;
}

/*
==========================================================================
	class VarSymbol
		
	symbol for variable declarations
==========================================================================
*/
class VarSymbol : public Symbol {
	
	friend class VarDecl;

public:
	CLASS_PROTOTYPE( VarSymbol );

				VarSymbol( VarDecl * decl );
				~VarSymbol();
	
	const char *	ToChars() const;

	VarDecl *	GetVarDecl();
	uint32		StackOffset() const;

protected:
	VarDecl *	m_decl;	 // the var. decl. that created us

	uint32		m_stackOffset;
};

INLINE VarDecl * VarSymbol::GetVarDecl() {
	return m_decl;
}

INLINE uint32 VarSymbol::StackOffset() const {
	return m_stackOffset;
}

//=========================================================================

//class ArgumentInfo {
//public:
//	Type *		m_type;
//};

class FunctionDecl;

class FunctionSymbol : public ScopeSymbol {
public:
	CLASS_PROTOTYPE( FunctionSymbol );

				FunctionSymbol( Identifier* name, Scope* enclosing );
				~FunctionSymbol();
	
	void		IncSizeOfLocals( uint32 bytes );

	uint32		GetSizeOfLocals() const;
	int			GetNumOfArguments() const;
	void		SetNumOfArguments( int num );

	const char *	ToChars() const;

	bool		IsVarArg() const;
	bool		IsBuiltIn() const;	

	void		SetReturnType( Type * pType );
	Type *		GetReturnType();

	int32	GetClobberedRegs() const;

	const char *	GetAsmName() const;

protected:
	uint32		m_sizeOfLocals;

	friend class Semantics;

	bool	m_isBuiltIn;
	bool	m_isVarArg;

	// the name that will be written in assembly listings
	// call 'm_nameInAsm'
	String	m_nameInAsm;

	// registers, which are changed by this function
	int32	m_clobberedRegs;

//	LinkedList< ArgumentInfo* >		m_arguments;
	int		m_iNumArguments;

	Type *	m_returnType;

	bool	m_bHasSideEffects;

public:
	bool		CheckSideEffects() const { return m_bHasSideEffects; }
};

INLINE bool FunctionSymbol::IsVarArg() const {
	return m_isVarArg;
}

INLINE bool FunctionSymbol::IsBuiltIn() const {
	return m_isBuiltIn;
}

INLINE int FunctionSymbol::GetNumOfArguments() const {
	return m_iNumArguments;
}

INLINE void FunctionSymbol::SetNumOfArguments( int num ) {
	m_iNumArguments = num;
}

INLINE const char * FunctionSymbol::GetAsmName() const {
	return m_nameInAsm.c_str();
}

INLINE Type * FunctionSymbol::GetReturnType() {
	return m_returnType;
}

INLINE int32  FunctionSymbol::GetClobberedRegs() const {
	return m_clobberedRegs;
}

/*
=============================================
	Example:

CONST	SEGMENT

str_1_Hello_World?		db	'Hello, world!', 0

CONST	ENDS
=============================================
*/
class StringEntry {
public:
				StringEntry( const String & name, int number );

	const String &	OriginalName() const;
	int			Number() const;

	void	GetMangledName( String & s ) const;

private:
	String		m_originalName;
	int			m_number;
};

INLINE StringEntry::StringEntry( const String & name, int number ) 
	: m_originalName( name )
	, m_number( number )
{
}

INLINE const String & StringEntry::OriginalName() const {
	return m_originalName;
}

INLINE int StringEntry::Number() const {
	return m_number;
}

/*
=========================================================================
	Symbol for a compilation unit
=========================================================================
*/
class ModuleSymbol : public ScopeSymbol {
public:
	CLASS_PROTOTYPE( ModuleSymbol );

				ModuleSymbol( const char* name, Scope* enclosing );
				~ModuleSymbol();

	const char *	ToChars() const;

	StringEntry *	Add_string_to_data_segment( const String & s );

private:
	String		m_name;

	// for back-end
	LinkedList< StringEntry* >	m_constData;
};



//===========================================================

/*
=============================================================
	class Resolve< T >

	T  is the type of the node being resolved,
	T2 is the type of the resolved node.

	T must inherit Object and UINode and must implement the method
		
		T2 * ResolveInto( Semantics * );

	, which may return NULL, 'this' or a new, completely different 
	and resolved, node of type T2.
=============================================================
*/
template< class T, class T2 = T >
class Resolve {
public:
	Resolve( T ** node, Semantics* semantics )
	{
		T2 * resolvedNode = ( *node )->ResolveInto( semantics );

		if ( NULL != resolvedNode && resolvedNode != *node )
		{
			UINode * uiParent = ( *node )->uiGetParent();
			( *node )->Drop();
			*node = resolvedNode;
			( *node )->Grab();
			( *node )->uiSetParent( uiParent );
		}
	}
};

}//end namespace c4

#endif // !__SYMBOL_H__