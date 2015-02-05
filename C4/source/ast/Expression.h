/*
======================================
	ast nodes, representing expressions.
======================================
*/

#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

namespace c4 {

//----------------------------------------------------------------------
// A tree node used as the base class
// for the different types of expressions.
//----------------------------------------------------------------------
class Expression : public AST_Node {
protected:
	CLASS_PROTOTYPE( Expression );

				Expression();
				~Expression();

public:
	//-- overrides ------------------------------------

						// write expression out to string
	void				ToString( String & s );

	int					GetId() const { return ID_EXPRESSION; }

	//--- semantic analysis -----------------------------------------
							// returns a new, resolved expression, or 'this'
							// must NOT return a NULL pointer.
	virtual Expression *	ResolveInto( Semantics* semantics );

	virtual bool		IsConst() const;

	// check for expressions that have no use
	enum E_SideEffectFlag
	{ 
		// not going to use the result, so issue error message if no side effects
		RESULT_IS_NOT_USED,

		// the result of the expression is used, but still check for useless subexpressions
		RESULT_IS_USED,
		
		// do not issue error messages, just return 'false' if expression has side effects
		DO_NOT_ISSUE_ERRORS
	};
	virtual bool		CheckSideEffects( E_SideEffectFlag  flag );

	// check that expression can tested for true or false
	virtual Expression *	CheckToBoolean();

	virtual bool		CheckLValue() { return true; }
	virtual bool		CheckRValue() { return true; }

						// returns the type of this expression;
						// will be NULL if we haven't done semantic checks yet
	Type *				GetType() const;

							// do an explicit cast ( returns this; may return a new expression )
							// must never return a NULL pointer, becase it can be used as "pRight = pRight->CastTo( pType );"
							// useful for converting types of constants
	virtual Expression *	CastTo( Type * pType );

	//-- visitor -----------------------------------
	void		Accept( ITreeVisitor * visitor );

	//-- user interface -----------------------------------------
	int					uiNumChildren() const;
	UINode *			uiChildAt( int index );
	int					uiIndexOf( const UINode *child ) const;
	const char *		uiToString() const;

protected:
	Type *		m_type;	// type of this expression; will be set during semantic resolution

public:
	// Operator precedence - greater values mean higher precedence.
	//	This is used to wrap expressions with lower precedence in parentheses '(' ')'
	//	when writing them out.
	//
	enum PRECEDENCE {
		prec_zero,
		prec_expr,
		prec_assign,	// =
		prec_cond,		// ?:
		prec_oror,		// ||
		prec_andand,	// &&
		prec_or,		// |
		prec_xor,		// ^
		prec_and,		// &
		prec_equal,		// ==
		prec_rel,		// relational operators
		prec_shift,		// <<, >>, >>>
		prec_add,		// +, -
		prec_mul,		// *, /
		prec_unary,
		prec_primary,
		prec_highest
	};

	virtual PRECEDENCE		GetPrecedence() const { return prec_expr; }

				// write expression out to string, but wrap it in '(' ')'
				// if it's precedence is less that 'parentPrec'
	void		Write( Expression* pExp, PRECEDENCE eParentPrec, String & s );

public:
	//TODO: find a better solution
	// for avoiding 'dynamic_cast'
	//virtual ExprType	GetOperator() const = 0; <- leads to type codes, redundancy
	//TOKEN_TYPE		tokenType;
	virtual TOKEN_TYPE	GetOperator() const { return ( TOKEN_TYPE ) 0; }
};

//==================================================================

class ExpressionList : public Expression {
public:
	CLASS_PROTOTYPE( ExpressionList );
	VISITABLE();

				ExpressionList();
				~ExpressionList();

	void				Add( Expression * e );

	Expression *		ResolveInto( Semantics* semantics );

	const char *		ToChars() const;
	void				ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

	LinkedList< Expression* > &		GetList();

protected:
	LinkedList< Expression* >	m_expList;
};

INLINE LinkedList< Expression* > & ExpressionList::GetList() {
	return m_expList;
}

/*
=============================================
	Tree nodes for literal expressions.
=============================================
*/

class Literal : public Expression {
public:
		CLASS_PROTOTYPE( Literal );
		VISITABLE();

					Literal();

	const char *	ToChars() const;
	void			ToString( String & s );

	bool			IsConst() const;

	PRECEDENCE	GetPrecedence() const { return prec_primary; }

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;
};

INLINE bool Literal::IsConst() const {
	return true;
}

//----------------------------------------------------------------------
// 'true' and 'false'
//----------------------------------------------------------------------
class BooleanConstant : public Literal {
public:
		CLASS_PROTOTYPE( BooleanConstant );
		VISITABLE();
		
				BooleanConstant( bool value );
				~BooleanConstant();

	bool			Value() const;

	const char *	ToChars() const;
	void			ToString( String & s );

	Expression *	CastTo( Type * pType );
	Expression *	Clone();

protected:
	bool		m_value;
};

INLINE bool BooleanConstant::Value() const {
	return m_value;
}

//==================================================================

class IntegerConstant : public Literal {
public:
		CLASS_PROTOTYPE( IntegerConstant );
		VISITABLE();

				IntegerConstant( int value );
				IntegerConstant( const char* number );
				~IntegerConstant();

	bool			IsBool( bool result ) const;
	bool			Equals( const Object* other ) const;

	const char *	ToChars() const;
	void			ToString( String & s );

	int64			Value() const;

	Expression *	CastTo( Type * pType );

protected:
	int64		m_value;
};

INLINE int64 IntegerConstant::Value() const {
	return m_value;
}

//----------------------------------------------------------------------
// Examples:	3.14159265358f, 0.666, 77.748234E-67.
//----------------------------------------------------------------------
class FloatingConstant : public Literal {
public:
	CLASS_PROTOTYPE( FloatingConstant );
	VISITABLE();

				FloatingConstant( float value );
				FloatingConstant( const char* number );
				~FloatingConstant();

	bool			IsBool( bool result ) const;
	bool			Equals( const Object* other ) const;

	const char *	ToChars() const;
	void			ToString( String & s );

	double			Value() const;

	Expression *	CastTo( Type * pType );

protected:
	double		m_value;
};

INLINE double FloatingConstant::Value() const {
	return m_value;
}

//----------------------------------------------------------------------
// Examples:	'a', '\0', '!', '\n'
//----------------------------------------------------------------------
class CharConstant : public Literal {
public:
	CLASS_PROTOTYPE( CharConstant );
	VISITABLE();

				CharConstant( char c );
				~CharConstant();

	char			Value() const;
	Expression *	CastTo( Type * pType );
	Expression *	Clone();

	const char *	ToChars() const;
	void			ToString( String & s );

protected:
	char		m_value;
};

INLINE char CharConstant::Value() const {
	return m_value;
}

//----------------------------------------------------------------------
// Examples: "Hello, world!\n", "Alexander"
//----------------------------------------------------------------------
class StringLiteral : public Literal {
public:
	CLASS_PROTOTYPE( StringLiteral );
	VISITABLE();

				StringLiteral( const char* text );
				~StringLiteral();

	bool			IsConst() const;

	const char *	ToChars() const;
	void			ToString( String & s );

	Expression *	ResolveInto( Semantics* semantics );

	String &		Value();
	Expression *	CastTo( Type * pType );
	Expression *	Clone();

protected:
	String		m_value;
};

INLINE String & StringLiteral::Value() {
	return m_value;
}

//----------------------------------------------------------------------
// A tree node for an identifier expression.
// E -> id
//
//  Notes:
//		this node can be resolved into a new, completely different, node
//	( for example, VarAccessExp )
//----------------------------------------------------------------------
class IdExp : public Expression {
public:
	CLASS_PROTOTYPE( IdExp );
	VISITABLE();

				IdExp( Identifier *id );
				IdExp( const char * text, const Location & location );
				~IdExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	Identifier *	GetIdentifier();
	int				GetId() const;

	Expression *	CastTo( Type * pType );
	Expression *	Clone();

	// ui
	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;
	
	PRECEDENCE	GetPrecedence() const { return prec_primary; }

protected:
	Identifier *	m_identifier;
};

class VarSymbol;

//----------------------------------------------------------------------
// A tree node for a variable access.
//
//	Usually such nodes are result of resolved IdExp or DotExp nodes.
//----------------------------------------------------------------------
class VarExp : public Expression {
public:
	CLASS_PROTOTYPE( VarExp );
	VISITABLE();

				VarExp( VarSymbol * symbol );
				~VarExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	Expression *	CastTo( Type * pType );
	VarSymbol *		GetSymbol();

	PRECEDENCE	GetPrecedence() const { return prec_primary; }
private:
	VarSymbol *		m_symbol;	// will be set during semantic resolution
};

INLINE VarSymbol * VarExp::GetSymbol() {
	return m_symbol;
}

//----------------------------------------------------------------------
// A tree node for postfix and unary expressions.
//----------------------------------------------------------------------
class UnaryExpression : public Expression 
{
	CLASS_PROTOTYPE( UnaryExpression );
	VISITABLE();

protected:
				UnaryExpression( Expression *e );
				~UnaryExpression();
public:
	Expression *	ResolveInto( Semantics* semantics );
	
	const char *	ToChars() const;
	void			ToString( String & s );

	Expression *	GetExp() { return m_expression; }

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

	virtual PRECEDENCE	Precedence() const { return prec_unary; }

	bool		CheckSideEffects( E_SideEffectFlag  flag );
	bool		CheckLValue();

protected:
	Expression *	m_expression;
};

//----------------------------------------------------------------------
// A tree node for a binary expression.
//----------------------------------------------------------------------
class BinaryExpression : public Expression 
{
	CLASS_PROTOTYPE( BinaryExpression );
	VISITABLE();

protected:
				BinaryExpression( Expression *left, Expression *right );
				~BinaryExpression();

public:
	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	Expression *	Left();
	Expression *	Right();

				// bring leaves to common type					
	void		CombineTypes( Semantics *semantics );

	bool		CheckSideEffects( E_SideEffectFlag  flag );
	bool		CheckLValue();

	bool		IsUnsigned() const;

	// ui
	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

protected:
	Expression *	m_left;
	Expression *	m_right;
};

INLINE Expression * BinaryExpression::Left() {
	return m_left;
}
INLINE Expression * BinaryExpression::Right() {
	return m_right;
}

//----------------------------------------------------------------------
// A tree node for an 'assert' expression.
// E -> assert ( e1, ..., eN )
// E -> assert ( e1, ..., eN : "message" )
//----------------------------------------------------------------------
class AssertExpression : public Expression {
public:
	CLASS_PROTOTYPE( AssertExpression );
	VISITABLE();

			AssertExpression( Expression* e, Expression* message = 0 );
			AssertExpression( ExpressionList* eList, Expression* message = 0 );
			~AssertExpression();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

	PRECEDENCE	GetPrecedence() const { return prec_primary; }

private:
	ExpressionList *	m_expList;
	Expression *		m_message;
};

//----------------------------------------------------------------------
// E -> & E
// E -> @ E
//----------------------------------------------------------------------
class AddrOfExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( AddrOfExp );
	VISITABLE();

				AddrOfExp( Expression *e );
				~AddrOfExp();

	Expression *   ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	Expression *	CastTo( Type * pType );
};

//----------------------------------------------------------------------
// A tree node for an array access expression.
// E -> E [ index ]
//----------------------------------------------------------------------
class ArrayAccessExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( ArrayAccessExp );
	VISITABLE();

				ArrayAccessExp( Expression *e1, Expression *e2 );
				~ArrayAccessExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE	GetPrecedence() const { return prec_unary; }
};

//----------------------------------------------------------------------
// E -> *E
// E -> E^
//	represents the object pointed to by a pointer;
//	the operand is the pointer being dereferenced.
//----------------------------------------------------------------------
class PtrIndirectionExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( PtrIndirectionExp );
	VISITABLE();

				PtrIndirectionExp( Expression *e );
				~PtrIndirectionExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );
};

//----------------------------------------------------------------------
// E -> ePtr '->' member
//	represents the address_32 of an object
//----------------------------------------------------------------------
class PtrExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( PtrExp );
	VISITABLE();

				PtrExp( Expression *e1, Expression *e2 );
				~PtrExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE	GetPrecedence() const { return prec_unary; }
};

//----------------------------------------------------------------------
// MemberSelectExp -- A tree node for a member access expression. 
// E -> E.E.E. ... .E
//----------------------------------------------------------------------
class DotExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( DotExp );
	VISITABLE();

				DotExp( Expression *left, Expression *right );
				DotExp( Expression *left, Identifier *right );
				~DotExp();

	Expression *	ResolveInto( Semantics* semantics );
	
	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE	GetPrecedence() const { return prec_primary; }
};

//----------------------------------------------------------------------
// A tree node for a type cast expression.
// E -> cast( EType ) E
//----------------------------------------------------------------------
class CastExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( CastExp );
	VISITABLE();

				CastExp( Expression* targetExp, Expression* sourceExp );
				CastExp( Type *targetType, Expression *sourceExp );
				~CastExp();

	Expression *	ResolveInto( Semantics* semantics );

	Expression *	GetSourceExp() { return m_expression; }
	Expression *	GetTargetExp() { return m_targetExp; }
	Type *			GetTargetType() { return m_targetType; }

	Expression *	CastTo( Type * pType );

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

protected:
	Type *			m_targetType;
	Expression *	m_targetExp;
};

//----------------------------------------------------------------------
// E -> -E
//----------------------------------------------------------------------
class NegationExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( NegationExp );
	VISITABLE();

				NegationExp( Expression *e );
				~NegationExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );
};

//----------------------------------------------------------------------
// E -> +E
//----------------------------------------------------------------------
class UnaryPlusExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( UnaryPlusExp );
	VISITABLE();

				UnaryPlusExp( Expression *e );
				~UnaryPlusExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );
};

//----------------------------------------------------------------------
// E -> ++E
//----------------------------------------------------------------------
class PreIncExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( PreIncExp );
	VISITABLE();

				PreIncExp( Expression *e );
				~PreIncExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	TOKEN_TYPE	GetOperator() const { return P_PLUS_PLUS; }

	bool	CheckSideEffects( Expression::E_SideEffectFlag  flag ) { return true; }
};

//----------------------------------------------------------------------
// E -> --E
//----------------------------------------------------------------------
class PreDecExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( PreDecExp );
	VISITABLE();

				PreDecExp( Expression *e );
				~PreDecExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	TOKEN_TYPE	GetOperator() const { return P_MINUS_MINUS; }

	bool	CheckSideEffects( Expression::E_SideEffectFlag  flag ) { return true; }
};

//----------------------------------------------------------------------
// E -> E++
//----------------------------------------------------------------------
class PostIncExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( PostIncExp );
	VISITABLE();

				PostIncExp( Expression *e );
				~PostIncExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	TOKEN_TYPE	GetOperator() const { return P_PLUS_PLUS; }

	bool	CheckSideEffects( Expression::E_SideEffectFlag  flag ) { return true; }
};

//----------------------------------------------------------------------
// E -> E--
//----------------------------------------------------------------------
class PostDecExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( PostDecExp );
	VISITABLE();

				PostDecExp( Expression *e );
				~PostDecExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	TOKEN_TYPE	GetOperator() const { return P_MINUS_MINUS; }

	bool	CheckSideEffects( Expression::E_SideEffectFlag  flag ) { return true; }
};

//----------------------------------------------------------------------
// E -> ~E
//	This tree node represents bitwise complement.
//  The only operand is the value to be complemented.
//	This node will always have integral type.
//----------------------------------------------------------------------
class ComplExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( ComplExp );
	VISITABLE();

				ComplExp( Expression *e );
				~ComplExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );
};

//----------------------------------------------------------------------
// E -> !E
//----------------------------------------------------------------------
class NotExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( NotExp );
	VISITABLE();

				NotExp( Expression *e );
				~NotExp();

	Expression *	ResolveInto( Semantics* semantics );
	
	const char *	ToChars() const;
	void			ToString( String & s );
};

//----------------------------------------------------------------------
// E -> E1 , E2
//----------------------------------------------------------------------
class CommaExpression : public BinaryExpression {
public:
	CLASS_PROTOTYPE( CommaExpression );
	VISITABLE();

				CommaExpression( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );
	
	const char *	ToChars() const;
	void			ToString( String & s );

	bool		CheckSideEffects( E_SideEffectFlag  flag );
};

//----------------------------------------------------------------------
// A tree node for an assignment expression.
// E -> e1 = e2
//----------------------------------------------------------------------
class AssignmentExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( AssignmentExp );
	VISITABLE();

				AssignmentExp( Expression *e1, Expression *e2 );
				~AssignmentExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	Expression *	CastTo( Type * pType );

	Expression *	CheckToBoolean();

	TOKEN_TYPE	GetOperator() const { return P_EQUAL; }
};

//----------------------------------------------------------------------
// E -> e1 + e2
//----------------------------------------------------------------------
class AddExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( AddExp );
	VISITABLE();

				AddExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_add; }
};

//----------------------------------------------------------------------
// E -> e1 - e2
//----------------------------------------------------------------------
class SubExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( SubExp );
	VISITABLE();

				SubExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_add; }
};

//----------------------------------------------------------------------
// E -> e1 * e2
//----------------------------------------------------------------------
class MulExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( MulExp );
	VISITABLE();

				MulExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_mul; }
};

//----------------------------------------------------------------------
// E -> e1 / e2
//----------------------------------------------------------------------
class DivExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( DivExp );
	VISITABLE();

				DivExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_mul; }
};

//----------------------------------------------------------------------
// E -> e1 % e2
//----------------------------------------------------------------------
class ModExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( ModExp );
	VISITABLE();

				ModExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_mul; }
};

//----------------------------------------------------------------------
// E -> e1 << e2
//----------------------------------------------------------------------
class ShlExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( ShlExp );
	VISITABLE();

				ShlExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_shift; }
};

//----------------------------------------------------------------------
// E -> e1 >> e2
//----------------------------------------------------------------------
class ShrExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( ShrExp );
	VISITABLE();

				ShrExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_shift; }
};

//----------------------------------------------------------------------
// E -> e1 >>> e2
//----------------------------------------------------------------------
class UShrExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( UShrExp );
	VISITABLE();

				UShrExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_shift; }
};

//----------------------------------------------------------------------
// E -> e1 ^ e2
// E -> e1 xor e2
//----------------------------------------------------------------------
class XorExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( XorExp );
	VISITABLE();

				XorExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_xor; }
};

//----------------------------------------------------------------------
// E -> e1 && e2
//----------------------------------------------------------------------
class AndAndExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( AndAndExp );
	VISITABLE();

				AndAndExp( Expression *e1, Expression *e2 );
				~AndAndExp();

	Expression *	ResolveInto( Semantics* semantics );
	
	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_andand; }
};

//----------------------------------------------------------------------
// E -> e1 || e2
//----------------------------------------------------------------------
class OrOrExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( OrOrExp );
	VISITABLE();

				OrOrExp( Expression *e1, Expression *e2 );
				~OrOrExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_oror; }
};

//----------------------------------------------------------------------
// E -> e1 in e2
//----------------------------------------------------------------------
class InExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( InExp );
	VISITABLE();

				InExp( Expression *e1, Expression *e2 );
				~InExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	//PRECEDENCE		GetPrecedence() const { return prec_expr; }
};

//----------------------------------------------------------------------
// E -> e1 is e2
//----------------------------------------------------------------------
class IsExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( IsExp );
	VISITABLE();

				IsExp( Expression *e1, Expression *e2 );
				~IsExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	//PRECEDENCE		GetPrecedence() const { return prec_expr; }
};

//----------------------------------------------------------------------
// E -> e1 as e2
//----------------------------------------------------------------------
class AsExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( AsExp );
	VISITABLE();

				AsExp( Expression *e1, Expression *e2 );
				~AsExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );
};

//----------------------------------------------------------------------
// A tree node for the conditional operator ? :.
// E -> ECond ? e1 : e2
//----------------------------------------------------------------------
class CondExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( CondExp );
	VISITABLE();

				CondExp( Expression *condition, Expression *e1, Expression *e2 );
				~CondExp();

	Expression *	GetCondition() { return m_condition; }

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_cond; }

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

private:
	Expression *	m_condition;
};


//----------------------------------------------------------------------
// E -> e1 == e2
// E -> e1 != e2
//----------------------------------------------------------------------
class EquExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( EquExp );
	VISITABLE();

	enum OperatorKind {
		Equal,
		NotEqual
	};

				EquExp( Expression *e1, Expression *e2, OperatorKind op );

	OperatorKind	GetKind() const { return m_op; }

	const char *	ToChars() const;
	void			ToString( String & s );

	Expression *	ResolveInto( Semantics* semantics );

	PRECEDENCE		GetPrecedence() const { return prec_equal; }

private:
	mutable OperatorKind	m_op;
};

// TODO: make 6 separate classes of relational expressions?

//----------------------------------------------------------------------
// ==, !=, >, >=, <, <= .
//----------------------------------------------------------------------
class RelExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( RelExp );
	VISITABLE();

	enum OperatorKind {
		//	Equ,	  // ==, equality
		//  Neq,	  // !=, inequality
		LT,	      // <, less than ( lower than comparison )
		GT,		  // >, greater than ( greater than comparison )
		LE,		  // <=, less than or equal ( lower or equal comparison )
		GE,		  // >=, greater than or equal ( greater or equal comparison )
	};

				RelExp( Expression *e1, Expression *e2, OperatorKind op );

	OperatorKind	GetKind() const { return m_op; }

	const char *	ToChars() const;
	void			ToString( String & s );

	Expression *	ResolveInto( Semantics* semantics );
	
	PRECEDENCE		GetPrecedence() const { return prec_rel; }

private:
	mutable OperatorKind	m_op;
};

//----------------------------------------------------------------------
// E -> e1 & e2
//----------------------------------------------------------------------
class BitAndExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( BitAndExp );
	VISITABLE();

				BitAndExp( Expression *e1, Expression *e2 );
				~BitAndExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_and; }
};

//----------------------------------------------------------------------
// E -> e1 ^ e2
//----------------------------------------------------------------------
class BitXorExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( BitXorExp );
	VISITABLE();

				BitXorExp( Expression *e1, Expression *e2 );
				~BitXorExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_xor; }
};

//----------------------------------------------------------------------
// E -> e1 | e2
//----------------------------------------------------------------------
class BitOrExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( BitOrExp );
	VISITABLE();

				BitOrExp( Expression *e1, Expression *e2 );
				~BitOrExp();

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_or; }
};

//----------------------------------------------------------------------
// E -> e1 += e2
//----------------------------------------------------------------------
class AddAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( AddAssignExp );
	VISITABLE();

				AddAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_PLUS_EQUAL; }
};

//----------------------------------------------------------------------
// E -> e1 -= e2
//----------------------------------------------------------------------
class SubAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( SubAssignExp );
	VISITABLE();

				SubAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_MINUS_EQUAL; }
};

//----------------------------------------------------------------------
// E -> e1 *= e2
//----------------------------------------------------------------------
class MulAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( MulAssignExp );
	VISITABLE();

				MulAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );
	
	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_ASTERISK_EQUAL; }
};

//----------------------------------------------------------------------
// E -> e1 /= e2
//----------------------------------------------------------------------
class DivAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( DivAssignExp );
	VISITABLE();

				DivAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );
	
	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_SLASH_EQUAL; }
};

//----------------------------------------------------------------------
// E -> e1 %= e2
//----------------------------------------------------------------------
class ModAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( ModAssignExp );
	VISITABLE();

				ModAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char * 	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_PERCENT_EQUAL; }
};

//----------------------------------------------------------------------
// E -> e1 &= e2
//----------------------------------------------------------------------
class BitAndAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( BitAndAssignExp );
	VISITABLE();

				BitAndAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_AMPERSAND_EQUAL; }
};

//----------------------------------------------------------------------
// E -> e1 ^= e2
//----------------------------------------------------------------------
class BitXorAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( BitXorAssignExp );
	VISITABLE();

				BitXorAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_CARET_EQUAL; }
};

//----------------------------------------------------------------------
// E -> e1 |= e2
//----------------------------------------------------------------------
class BitOrAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( BitOrAssignExp );
	VISITABLE();

				BitOrAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );
	
	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_VERTICAL_EQUAL; }
};

//----------------------------------------------------------------------
// E -> e1 <<= e2
//----------------------------------------------------------------------
class ShlAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( ShlAssignExp );
	VISITABLE();

				ShlAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_LESS_LESS_EQUAL; }
};

//----------------------------------------------------------------------
// E -> e1 >>= e2
//----------------------------------------------------------------------
class ShrAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( ShrAssignExp );
	VISITABLE();

				ShrAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );
	
	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_GREATER_GREATER_EQUAL; }
};

//----------------------------------------------------------------------
// Unsigned right shift
// E -> e1 >>>= e2
//----------------------------------------------------------------------
class UShrAssignExp : public BinaryExpression {
public:
	CLASS_PROTOTYPE( UShrAssignExp );
	VISITABLE();

				UShrAssignExp( Expression *e1, Expression *e2 );

	Expression *	ResolveInto( Semantics* semantics );

	const char *	ToChars() const;
	void			ToString( String & s );

	PRECEDENCE		GetPrecedence() const { return prec_assign; }

	TOKEN_TYPE	GetOperator() const { return P_UNSIGNED_RIGHT_SHIFT_EQUAL; }
};


class FunctionSymbol;

//----------------------------------------------------------------------
// A tree node for a method invocation expression.
// E -> E ( params )
//----------------------------------------------------------------------
class CallExp : public UnaryExpression {
public:
	CLASS_PROTOTYPE( CallExp );
	VISITABLE();

				CallExp( Expression *e, ExpressionList *args );
				~CallExp();

	ExpressionList *	GetArguments() { return m_args; }

	Expression *	ResolveInto( Semantics* semantics );	

	const char *	ToChars() const;
	void			ToString( String & s );

	int			uiNumChildren() const;
	UINode *	uiChildAt( int index );
	int			uiIndexOf( const UINode *child ) const;

	FunctionSymbol *	GetSymbol() { return m_symbol; }

	bool		CheckSideEffects( E_SideEffectFlag  flag );

private:
	ExpressionList *	m_args;
	FunctionSymbol *	m_symbol;
};

//----------------------------------------------------------------------
// A tree node to stand in for a malformed expression. 
//----------------------------------------------------------------------
class ErroneousExpression : public Expression {
public:
	CLASS_PROTOTYPE( ErroneousExpression );
	VISITABLE();

				ErroneousExpression( const char* message, const Location & location );
				~ErroneousExpression();

	const char *	GetMessage() const;

	const char *	ToChars() const;
	void			ToString( String & s );

	Expression *	ResolveInto( Semantics* semantics );

private:
	String		m_message;
};

//==================================================================
class ThisExpression : public Expression {
public:
	CLASS_PROTOTYPE( ThisExpression );
	VISITABLE();

				ThisExpression( const Location & loc );
				~ThisExpression();

	const char *	ToChars() const;
	void			ToString( String & s );

	Expression *	ResolveInto( Semantics* semantics );
	
	PRECEDENCE		GetPrecedence() const { return prec_primary; }
};

}//end namespace c4 

#endif // !__EXPRESSION_H__