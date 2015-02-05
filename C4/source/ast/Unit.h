#ifndef __COMPILATION_UNIT_H__
#define __COMPILATION_UNIT_H__

namespace c4 {

class Declaration;
class UnitTestDecl;
class ModuleSymbol;
class Semantics;

/*
==========================================================================
	class CompilationUnit

	Represents the abstract syntax tree 
	for compilation units ( source files ).
==========================================================================
*/
class CompilationUnit : public AST_Node {
public:
	CLASS_PROTOTYPE( CompilationUnit );

				CompilationUnit( const char * name );
				~CompilationUnit();

	void			Add( Declaration * decl );

	idList< Declaration* > & GetActualList();

	//-- semantic analysis -------------------------------

	void				Resolve( Semantics * semantics );

	ModuleSymbol *		GetSymbol();

	// overrides
	const char *		ToChars() const;
	void		Accept( ITreeVisitor * visitor );

	//-- ui -------------------------------
	int					uiNumChildren() const;
	UINode *			uiChildAt( int index );
	int					uiIndexOf( const UINode *child ) const;

	//-- code generation -------------------------------
	
protected:
	idList< Declaration* >	m_decls;	// top-level declarations
	
	String			m_name;

	ModuleSymbol *	m_symbol;	// != NULL => semantics has been done
};

INLINE idList< Declaration* > & CompilationUnit::GetActualList() {
	return m_decls;
}

INLINE ModuleSymbol * CompilationUnit::GetSymbol() {
	return m_symbol;
}

INLINE void CompilationUnit::Accept( ITreeVisitor * visitor ) {
	visitor->Visit( this );
}

}//end namespace c4 

#endif // !__COMPILATION_UNIT_H__