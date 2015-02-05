/*
======================================
ast nodes, representing statements.
======================================
*/
#ifndef __STATEMENT_H__
#define __STATEMENT_H__

namespace c4 {

class Expression;
class ExpressionList;

//-------------------------------------------------------
// A tree node used as the base class
// for different kinds of statements.
//-------------------------------------------------------
class Statement : public AST_Node {
public:
	CLASS_PROTOTYPE( Statement );
	VISITABLE();

				Statement();
				~Statement();

	const char *	ToChars() const;
	void			ToString( String & s );

	virtual void	Resolve( Semantics* semantics );

	// for avoiding dynamic cast
	virtual ReturnStatement * IsReturnStatement() { return NULL; }
};

//-------------------------------------------------------
// A tree node for an empty (skip) statement.
//-------------------------------------------------------
class EmptyStatement : public Statement {
public:
	CLASS_PROTOTYPE( EmptyStatement );
	VISITABLE();

				EmptyStatement();
				~EmptyStatement();

	void			ToString( String & s );
	const char *	ToChars() const;

	void		Resolve( Semantics* semantics );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;
};

//-------------------------------------------------------
// for error recovery
//-------------------------------------------------------
class ErrorStatement : public EmptyStatement {
public:
	CLASS_PROTOTYPE( ErrorStatement );
	VISITABLE();

				ErrorStatement( const char * message, const Location & loc );
				~ErrorStatement();

	const char *	GetMessage() const;

	const char *	ToChars() const;
	void			ToString( String & s );

	void		Resolve( Semantics* semantics );
};

class StatementList : public Statement {
public:
	CLASS_PROTOTYPE( StatementList );
	VISITABLE();

				StatementList();
				~StatementList();

	void			Add( Statement * s );

	LinkedList< Statement* > &	GetActualList();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	// ui
	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

protected:
	LinkedList< Statement* >	m_stmtList;
};

INLINE LinkedList< Statement* > & StatementList::GetActualList() {
	return m_stmtList;
}

//-------------------------------------------------------
// A tree node for an expression statement.
//-------------------------------------------------------
class ExpressionStatement : public Statement {
public:
	CLASS_PROTOTYPE( ExpressionStatement );
	VISITABLE();

				ExpressionStatement( Expression* e );
				~ExpressionStatement();

	Expression *	GetExpression();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	// ui
	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

protected:
	Expression *	m_expression;
};

//-------------------------------------------------------
// A tree node for a labeled statement.
//-------------------------------------------------------
class LabeledStatement : public Statement {
public:
	CLASS_PROTOTYPE( LabeledStatement );
	VISITABLE();

				LabeledStatement( Identifier *name, Statement* s );
				~LabeledStatement();

	Identifier *	GetIdentifier();
	Statement *		GetStatement();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	// ui
	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

private:
	Identifier *	m_id;
	Statement *		m_stmt;
};

//-------------------------------------------------------
//	A tree node for a declaration statement.
//-------------------------------------------------------
class DeclStatement : public Statement {
public:
	CLASS_PROTOTYPE( DeclStatement );
	VISITABLE();

				DeclStatement( Declaration* decl );
				~DeclStatement();

	Declaration *	GetDeclaration();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	Declaration *		m_decl;
};

INLINE Declaration * DeclStatement::GetDeclaration() {
	return m_decl;
}

//-------------------------------------------------------
// A tree node for a statement block.
//-------------------------------------------------------
class Block : public Statement {
public:
	CLASS_PROTOTYPE( Block );
	VISITABLE();

				Block( Statement * s = 0 );
				Block( StatementList * statements );
				~Block();

	StatementList *		GetStatements();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	// ui
	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

protected:
	StatementList *		m_stmtList;
};

//-------------------------------------------------------
// auto id = expr ;
//-------------------------------------------------------
class AutoDeclStatement : public Statement {
public:
	CLASS_PROTOTYPE( AutoDeclStatement );
	VISITABLE();

				AutoDeclStatement( Identifier * id, Expression* initializer );
				~AutoDeclStatement();

	Identifier *	GetVariable();
	Expression *	GetInitializer();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

private:
	Identifier *	m_variable;
	Expression *	m_initializer;
};

//-------------------------------------------------------
// A tree node for an 'if' statement.
//-------------------------------------------------------
class IfStatement : public Statement {
public:
	CLASS_PROTOTYPE( IfStatement );
	VISITABLE();

				IfStatement( Expression* condition, Statement* thenStmt, Statement* elseStmt = 0 );
				IfStatement( AutoDeclStatement* condition, Statement* thenStmt, Statement* elseStmt = 0 );
				~IfStatement();

	Expression *	GetCondition();
	Statement *		GetThenStatement();
	Statement *		GetElseStatement();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

private:
	Expression *	m_condition;
	Statement *		m_thenStmt;
	Statement *		m_elseStmt;

	StatementList *		m_declarations;  // if ( int x = a + b, y = -1, z ) { ...
};

//-------------------------------------------------------
// the base class for all loop statements.
//-------------------------------------------------------
class LoopStatement : public Statement {
public:
	CLASS_PROTOTYPE( LoopStatement );
	VISITABLE();

				LoopStatement( Expression* condition, Statement* body );
				~LoopStatement();

	// returns the end-loop test expression
	Expression *	GetCondition();
	Statement *		GetBody();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

protected:
	Expression *	m_condition;
	Statement *		m_body;
};

//-------------------------------------------------------
// A tree node for a 'while' loop statement.
//-------------------------------------------------------
class WhileStatement : public LoopStatement {
public:
	CLASS_PROTOTYPE( WhileStatement );
	VISITABLE();

				WhileStatement( Expression* condition, Statement* body );
				~WhileStatement();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );
};

//-------------------------------------------------------
// A tree node for a 'do' statement.
//-------------------------------------------------------
class DoStatement : public LoopStatement {
public:
	CLASS_PROTOTYPE( DoStatement );
	VISITABLE();

				DoStatement( Statement* body, Expression* condition );
				~DoStatement();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );
};

//-------------------------------------------------------
// with ( expr ) { ... }
//-------------------------------------------------------
class WithStatement : public Statement {
public:
	CLASS_PROTOTYPE( WithStatement );
	VISITABLE();

				WithStatement( Expression* expr, Statement* body );
				~WithStatement();

	Expression *	GetExpression();
	Statement *		GetBody();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

private:
	Expression *	m_expression;
	Statement *		m_body;
};

//-------------------------------------------------------
// return ;
// return expr ;
//-------------------------------------------------------
class ReturnStatement : public Statement {
public:
	CLASS_PROTOTYPE( ReturnStatement );
	VISITABLE();

				ReturnStatement( Expression* returnExp = 0 );
				~ReturnStatement();

	Expression *	GetExpression();
	void			Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

	ReturnStatement * IsReturnStatement() { return this; }

protected:
	Expression *	m_expression;
};

//-------------------------------------------------------
//	A tree node for a basic 'for' loop statement:
//
//   for ( initializer ; condition ; update )
//       statement
//-------------------------------------------------------
class ForLoopStatement : public LoopStatement {
public:
	CLASS_PROTOTYPE( ForLoopStatement );
	VISITABLE();

				// any of these pointers can be null
				ForLoopStatement(	Statement *			init,
									Expression *		condition,
									ExpressionList *	update,
									Statement *			body  );

				~ForLoopStatement();

						// returns the init statements
	Statement *			GetInitStatement();

						// returns the update statements
	ExpressionList *	GetUpdate();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

protected:
	Statement *			m_initStmt;
	ExpressionList *	m_updateExp;
};

//-------------------------------------------------------
// A tree node for a 'case' in a 'switch' statement.
//
//	CaseStatement:
//		case ExpressionList : Statement
//-------------------------------------------------------
class CaseStatement : public Statement {
public:
	CLASS_PROTOTYPE( CaseStatement );
	VISITABLE();

				CaseStatement( ExpressionList * expressions );
				~CaseStatement();

	ExpressionList *	GetExpressions();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

protected:
	ExpressionList *	m_expList;
};

//-------------------------------------------------------
//	DefaultStatement:
//		default: Statement
//-------------------------------------------------------
class DefaultStatement : public Statement {
public:
	CLASS_PROTOTYPE( DefaultStatement );
	VISITABLE();

				DefaultStatement( Statement * stmt );
				~DefaultStatement();

	Statement *		GetStatement();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

protected:
	Statement *		m_stmt;	
};

//-------------------------------------------------------
//	ContinueStatement:
//	    continue;
//	    continue Identifier ;
//-------------------------------------------------------
class ContinueStatement : public Statement {
public:
	CLASS_PROTOTYPE( ContinueStatement );
	VISITABLE();

				ContinueStatement( Identifier *label = 0 );
				~ContinueStatement();

	Identifier *	GetLabel();

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

protected:
	Identifier *	m_label;
};

//-------------------------------------------------------
//	BreakStatement:
//	    break;
//	    break Identifier ;
//-------------------------------------------------------
class BreakStatement : public ContinueStatement {
public:
	CLASS_PROTOTYPE( BreakStatement );
	VISITABLE();

				BreakStatement( Identifier *label = 0 );
				~BreakStatement();	

	void		Resolve( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );
};

#if 0
//-------------------------------------------------------
// TODO: create 4 separate classes for gotos.
//	GotoStatement:
//	    goto Identifier ;
//	    goto default ;
//	    goto case ;
//	    goto case Expression ;
//-------------------------------------------------------
class GotoStatement : public Statement {
public:
	// goto Identifier ;
	GotoStatement( Identifier * label );

	// goto case Expression ;
	GotoStatement( Expression * expr );

	enum GotoType {
		GOTO_CASE,
		GOTO_DEFAULT
	};

	// goto case ;
	// goto default ;
	GotoStatement( GotoType type );

protected:
};
#endif

}//end namespace c4

#endif // !__STATEMENT_H__