/*
======================================
	implementation of ast nodes,
	representing expressions.
======================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

//#define CONSTANT_FOLDING

namespace c4 {

#pragma region Expression

DEFINE_TYPE( Expression, AST_Node );

Expression::Expression()
	: m_type( NULL )
{
}

Expression::~Expression() {
	DROP( m_type );
}

void Expression::ToString( String & s ) {
	s += "[Expression]";
}

bool Expression::IsConst() const {
	return false;
}

bool Expression::CheckSideEffects( E_SideEffectFlag  flag ) { 
	String  strExp;
	this->ToString( strExp );

	if ( flag != RESULT_IS_NOT_USED )
	{
		Error( "'%s' has no effect in expression (%s)", this->ToChars(), strExp.c_str() );
	}
	else //if ( flag == RESULT_IS_NOT_USED )
	{
		//Error( "expression '%s' has no effect", strExp.c_str() );
		Error( "result of expression '%s' is not used", strExp.c_str() );
	}
	return false; 
}

Expression * Expression::CheckToBoolean() {
	if ( ! m_type->CheckBoolean() )
	{
		String  strExp, strType;
		this->ToString( strExp );
		m_type->ToString( strType );

		Error( "expression '%s' of type '%s' doesn't have a boolean value",
			strExp.c_str(), strType.c_str() );
	}
	return this;
}

void Expression::Accept( ITreeVisitor * visitor ) {
	Unimplemented();
}

Type * Expression::GetType() const {
	return m_type;
}

/*
============================================================
	Expression::Write

	Needed to correctly write trees as text strings.
	For example,

       *
	  /  \
	 +    \		should be written as "( a + b ) * x ".
	/  \   \
   a    b   x

	Precedence of binary '+' is less than that of binary '*',
	so "a + b" is enclosed in paretheses.
============================================================
*/
void Expression::Write( Expression* pExp, PRECEDENCE eParentPrec, String & s )
{
	if ( pExp->GetPrecedence() <= eParentPrec ) 
	{
		s += "(";
		pExp->ToString( s );
		s += ")";
	}
	else 
	{
		pExp->ToString( s );
	}
}

#pragma region "UI"

const char * Expression::uiToString() const {
	return ToChars();
}

int Expression::uiNumChildren() const {
	return 0;
}

UINode * Expression::uiChildAt( int index ) {
	return 0;
}

int Expression::uiIndexOf( const UINode *child ) const {
	return -1;
}
#pragma endregion

/*
============================
	Expression::ResolveInto

	semantically analyze this expression 
	( determine types, fold constants, etc. )
============================
*/
Expression * Expression::ResolveInto( Semantics* semantics ) {
	//c4::common->Print( "Resolving expression: '%s'", ToChars() );
	if ( m_type ) {
		c4::Resolve< Type >( &m_type, semantics );
	} else {
		m_type = new Type_Void();
		m_type->Grab();
	}
	return this;
}

Expression * Expression::CastTo( Type * pType ) {
	if ( m_type->GetTypeKind() != pType->GetTypeKind() ) {
		return new CastExp( pType, this );
	}
	return this;
}

#pragma endregion 

#pragma region "ExpressionList ::= e1 , e2, ... , eN"

DEFINE_TYPE( ExpressionList, Expression );

ExpressionList::ExpressionList()
{
}

ExpressionList::~ExpressionList() {
	LinkedList< Expression* >::Iterator  iter = m_expList.Begin();

	for ( ; iter != m_expList.End(); iter++ ) 
	{
		( *iter )->Drop();
	}

	m_expList.RemoveAll();
}

void ExpressionList::Add( Expression* e ) {
	e->Grab();
	e->uiSetParent( this );
	m_expList.Append( e );
}

const char * ExpressionList::ToChars() const {
	return "Expr.List: E, E, ..., E";
}

void ExpressionList::ToString( String & s )
{
	LinkedList< Expression* >::Iterator  iter = m_expList.Begin();

	for ( ; iter != m_expList.End(); iter++ ) 
	{
		( *iter )->ToString( s );
		
		if ( iter.HasNext() ) {
			s += ", ";
		}
	}
}

#pragma region "UI"
int ExpressionList::uiNumChildren() const {
	return m_expList.Num();
}

UINode * ExpressionList::uiChildAt( int index )
{
	if ( index >= m_expList.Num() ) {
		return 0;
	}

	LinkedList< Expression* >::Iterator  iter = m_expList.Begin();

	for ( int i = 0; iter != m_expList.End(); iter++, i++ )
	{
		if ( i == index ) {
			return *iter;
		}
	}

	return 0;
}

int ExpressionList::uiIndexOf( const UINode *child ) const
{
	LinkedList< Expression* >::Iterator  iter = m_expList.Begin();

	for ( int i = 0; iter != m_expList.End(); iter++, i++ ) {
		if ( child == *iter ) {
			return i;
		}
	}

	return -1;
}
#pragma endregion

Expression * ExpressionList::ResolveInto( Semantics* semantics ) {
	LinkedList< Expression* >::Iterator  iter = m_expList.Begin();

	for ( int i = 0; iter != m_expList.End(); iter++, i++ ) {
		c4::Resolve< Expression >( &( *iter ), semantics );
	}
	return this;
}

#pragma endregion

#pragma region "Literal"

DEFINE_TYPE( Literal, Expression );

Literal::Literal()
{
}

const char* Literal::ToChars() const {
	return "Literal";
}

void Literal::ToString( String & s )
{
}

#pragma region "UI"
int Literal::uiNumChildren() const {
	return 0;
}

UINode * Literal::uiChildAt( int index ) {
	return 0;
}

int Literal::uiIndexOf( const UINode *child ) const {
	return -1;
}
#pragma endregion

#pragma region BooleanConstant

DEFINE_TYPE( BooleanConstant, Literal );

BooleanConstant::BooleanConstant( bool value ) 
	: m_value( value )
{
	m_type = new Type_Bool();
}

BooleanConstant::~BooleanConstant() {
}

const char * BooleanConstant::ToChars() const {
	return m_value ? "true" : "false";
}

void BooleanConstant::ToString( String & s )
{
	m_value ? s += "true" : s += "false";
}

Expression * BooleanConstant::Clone() {
	return new BooleanConstant( m_value );
}

Expression * BooleanConstant::CastTo( Type * pType ) {
	switch ( pType->GetTypeKind() ) {
		case CHAR :
			return new CharConstant( m_value );

		case TYPE_INT8 :
		case TYPE_INT32 :
		case TYPE_INT64 :
			return new IntegerConstant( m_value );

		case TYPE_FLOAT32 : 
		case TYPE_FLOAT64 :
			return new FloatingConstant( m_value );

		default:	/* nothing */	;
	}
	return this;
}
#pragma endregion 

#pragma region IntegerConstant

DEFINE_TYPE( IntegerConstant, Literal );

IntegerConstant::IntegerConstant( int value )
	: m_value( value ) 
{
	m_type = new Type_Int32;
}

IntegerConstant::IntegerConstant( const char* number )
	: m_value( atol( number ) )
{
	m_type = new Type_Int32;
}

IntegerConstant::~IntegerConstant() {
}

bool IntegerConstant::IsBool( bool result ) const {
	return result ? (m_value != 0) : (m_value == 0);
}

bool IntegerConstant::Equals( const Object* other ) const {
	return false;
}

const char * IntegerConstant::ToChars() const {
	static String	str;
	str = "Integer constant:\t";
	str += (int) m_value;
	return (const char* )str;
}

void IntegerConstant::ToString( String & s )
{
	char  buf[ 32 ];
	_ltoa_s( m_value, buf, 32, 10 );
	s += buf;
}

Expression * IntegerConstant::CastTo( Type * pType ) {
	switch ( pType->GetTypeKind() ) {
		case BOOL :
			return new BooleanConstant( m_value != 0 );

		case CHAR :
			return new CharConstant( m_value );

		case TYPE_FLOAT32 : 
		case TYPE_FLOAT64 :
			return new FloatingConstant( m_value );

		default:	/* nothing */	;
	}
	return this;
}

#pragma endregion

#pragma region FloatingConstant

DEFINE_TYPE( FloatingConstant, Literal );

FloatingConstant::FloatingConstant( float value )
	: m_value( value )
{
	m_type = new Type_Float32;
}

FloatingConstant::FloatingConstant( const char* number )
	: m_value( atof( number ) )
{
	m_type = new Type_Float32;
}

FloatingConstant::~FloatingConstant() {
}

bool FloatingConstant::IsBool( bool result ) const {
	return result ? (m_value != 0) : (m_value == 0);
}

bool FloatingConstant::Equals( const Object* other ) const {
	return false;
}

const char * FloatingConstant::ToChars() const {
	static String	str;
	str = "Floating-point constant:\t";
	str += ( float )m_value;
	return (const char* )str;
}

void FloatingConstant::ToString( String & s )
{
	char  buf[ 32 ];
	_gcvt_s( buf, 32, m_value, 12 );
	s += buf;
}

Expression * FloatingConstant::CastTo( Type * pType ) {
	switch ( pType->GetTypeKind() ) {
		case BOOL :
			return new BooleanConstant( m_value != 0 );

		case CHAR :
			return new CharConstant( m_value );

		case TYPE_INT8 :
		case TYPE_INT32 :
		case TYPE_INT64 :
			return new IntegerConstant( m_value );

		default:	/* nothing */	;
	}
	return this;
}

#pragma endregion

#pragma region CharConstant

DEFINE_TYPE( CharConstant, Literal );

CharConstant::CharConstant( char c )
	: m_value( c ) 
{
	m_type = new Type_Char();
}

CharConstant::~CharConstant() {
}

const char * CharConstant::ToChars() const {
	static String	str;
	str = "Character constant:\t";
	str += m_value;
	return (const char* )str;
}

void CharConstant::ToString( String & s ) {
	s += '\'';
	s += m_value;
	s += '\'';
}

Expression * CharConstant::Clone() {
	return new CharConstant( m_value );
}

Expression * CharConstant::CastTo( Type * pType ) {
	switch ( pType->GetTypeKind() ) {
		case BOOL :
			return new BooleanConstant( m_value != 0 );

		case TYPE_INT32 :
		case TYPE_INT64 :
			return new IntegerConstant( m_value );

		case TYPE_FLOAT32 :
		case TYPE_FLOAT64 :
			return new FloatingConstant( m_value );

		default:	/* nothing */	;
	}
	return this;
}

#pragma endregion

#pragma region StringLiteral

DEFINE_TYPE( StringLiteral, Literal );

StringLiteral::StringLiteral( const char* text )
	: m_value( text ) 
{
	m_type = new Type_String();
}

StringLiteral::~StringLiteral() {
}

bool StringLiteral::IsConst() const {
	return true;
}

const char * StringLiteral::ToChars() const {
	static String	str;
	str = "String Literal:\t";
	str += m_value;
	return (const char* )str;
}

void StringLiteral::ToString( String & s )
{
	s += "\"";
	s += m_value;
	s += "\"";
}

Expression * StringLiteral::Clone() {
	return new StringLiteral( m_value.c_str() );
}

Expression * StringLiteral::ResolveInto( Semantics* semantics ) 
{
	return NULL;
}

Expression * StringLiteral::CastTo( Type * pType ) {
	return this;
}

#pragma endregion

#pragma region IdExp

DEFINE_TYPE( IdExp, Expression );

IdExp::IdExp( Identifier *id )
	: m_identifier( id )
{
	m_identifier->Grab();
	m_identifier->uiSetParent( this );
}

IdExp::IdExp( const char * text, const Location & location )
{
	m_identifier = new Identifier( text );
	m_identifier->Grab();
	m_identifier->uiSetParent( this );
	m_identifier->location = location;
}

IdExp::~IdExp() {
	m_identifier->Drop();
}

const char* IdExp::ToChars() const {
	return m_identifier->Text();
}

void IdExp::ToString( String & s ) {
	s += m_identifier->Text();
}

Identifier * IdExp::GetIdentifier() {
	return m_identifier;
}

#pragma region "UI"
int IdExp::uiNumChildren() const {
	return 0;
}

UINode * IdExp::uiChildAt( int index ) {
	return 0;
}

int IdExp::uiIndexOf( const UINode *child ) const {
	return -1;
}
#pragma endregion

int IdExp::GetId() const {
	return ID_IDENTIFIER;
}

Expression * IdExp::Clone() {
	Identifier * id = new Identifier( m_identifier->Text() );
	return new IdExp( id );
}

Expression *  IdExp::ResolveInto( Semantics* semantics ) 
{
	Scope * scope = semantics->GetContext();

	Symbol * pSymbol = scope->Search( m_identifier );

	if ( pSymbol ) 
	{
		// if it's a variable access
		if ( pSymbol->GetTypeInfo()->Is( & VarSymbol::typeInfo ) )
		{
			VarSymbol * varSymbol = static_cast< VarSymbol* >( pSymbol );
			return new VarExp( varSymbol );
		}
		// if it's a function call
		else if ( pSymbol->GetTypeInfo()->Is( & FunctionSymbol::typeInfo ) )
		{
			FunctionSymbol * funcSymbol = static_cast< FunctionSymbol* >( pSymbol );
			//return new CallExp( funcSymbol );
			m_type = funcSymbol->GetReturnType();
			m_type->Grab();
			return this; // ??? is this right
		}
		// TODO: other cases
		else {
			Unimplemented();
		}

		return this;
	}

	Error( "undefined identifier: '%s'", m_identifier->Text() );
	m_type = new Type_Error( "undefined identifier", location );

	return this;
}

Expression * IdExp::CastTo( Type * pType ) {
	return this;
}

#pragma endregion

#pragma region VarExp

DEFINE_TYPE( VarExp, Expression );

VarExp::VarExp( VarSymbol * symbol )
	: m_symbol( symbol )
{
	m_symbol->Grab();

	m_type = m_symbol->GetVarDecl()->GetType();
}

VarExp::~VarExp() {
	m_symbol->Drop();
}

const char* VarExp::ToChars() const {
	static String	str;
	str = "Variable access: \t";
	str += m_symbol->Name();
	return (const char* )str;
}

void VarExp::ToString( String & s ) {
	s += m_symbol->Name();
}

Expression *  VarExp::ResolveInto( Semantics* semantics ) 
{
	return NULL;
}

Expression * VarExp::CastTo( Type * pType ) {
	// TODO:
	if ( m_type->GetTypeKind() != pType->GetTypeKind() ) {
		return new CastExp( pType, this );
	}
	return this;
}

#pragma endregion

#pragma region UnaryExpression

DEFINE_TYPE( UnaryExpression, Expression );

UnaryExpression::UnaryExpression( Expression *e )
	: m_expression( e )
{
	m_expression->Grab();
	m_expression->uiSetParent( this );
}

UnaryExpression::~UnaryExpression() {
	m_expression->Drop();
}

const char * UnaryExpression::ToChars() const { 
	return "Unary Expression";
}

void UnaryExpression::ToString( String & s )
{
	s += "[Unary Expression]";
}

#pragma region "UI"
int UnaryExpression::uiNumChildren() const {
	return 1;
}

UINode * UnaryExpression::uiChildAt( int index ) {
	return m_expression;
}

int UnaryExpression::uiIndexOf( const UINode *child ) const {
	if ( child == m_expression ) {
		return 0;
	}
	return -1;
}
#pragma endregion

Expression *  UnaryExpression::ResolveInto( Semantics* semantics ) {
	c4::Resolve< Expression >( & m_expression, semantics );
	
	m_type = m_expression->GetType();
	Assert( m_type );
	//UNDONE: ??? should we call m_type->Grab(); ???
	m_type->Grab();

	return this;
}

bool UnaryExpression::CheckSideEffects( E_SideEffectFlag  flag ) {
	//return m_expression->CheckSideEffects( flag ); 
	return Expression::CheckSideEffects( flag ) 
		&& m_expression->CheckSideEffects( flag );
}

bool UnaryExpression::CheckLValue() {
	String  strExp;
	this->ToString( strExp );
	Error( "expression '%s' is not an lvalue", strExp.c_str() );
	return false;
}

#pragma endregion

#pragma region BinaryExpression

DEFINE_TYPE( BinaryExpression, Expression );

BinaryExpression::BinaryExpression( Expression *left, Expression *right ) 
	: m_left( left ), m_right( right )
{
	m_left->Grab();
	m_left->uiSetParent( this );

	m_right->Grab();
	m_right->uiSetParent( this );
}

BinaryExpression::~BinaryExpression() {
	m_left->Drop();
	m_right->Drop();
}

const char * BinaryExpression::ToChars() const { 
	return "Binary Expression";
}

void BinaryExpression::ToString( String & s )
{
	//s += "[Binary Expression]";
	s += this->ToChars();
}

#pragma region "UI"
int BinaryExpression::uiNumChildren() const {
	return 2;
}

UINode * BinaryExpression::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_left;
	}
	if ( 1 == index ) {
		return m_right;
	}
	return 0;
}

int BinaryExpression::uiIndexOf( const UINode *child ) const {
	if ( child == m_left ) {
		return 0;
	}
	if ( child == m_right ) {
		return 1;
	}
	return -1;
}
#pragma endregion

Expression *  BinaryExpression::ResolveInto( Semantics* semantics ) {
	c4::Resolve< Expression >( & m_left, semantics );
	c4::Resolve< Expression >( & m_right, semantics );

	//Assert2( m_type == NULL, "Semantics hasn't been done" );
	if ( m_type == NULL ) {
		CombineTypes( semantics );
		Assert( m_type );
	}

	// remove unnecessary computations
	// example:
	//	int i, a, b;
	//	i++ + i++;
	//  (i=a)*(i=b)


	return this;
}

// bring the leaves to their common type
// performs only widening conversions, returns the common type of children;
void BinaryExpression::CombineTypes( Semantics* semantics )
{
	Type * commonType = Type::GetCommonType( m_left->GetType(), m_right->GetType() );
	
	Expression * e1 = m_left->CastTo( commonType );
	Expression * e2 = m_right->CastTo( commonType );
	
	if ( e1 && e1 != m_left ) {
		m_left->Drop();
		m_left = e1;
		m_left->Grab();
	}
	if ( e2 && e2 != m_right ) {
		m_right->Drop();
		m_right = e2;
		m_right->Grab();
	}

	m_type = commonType;
	m_type->Grab();
}

bool BinaryExpression::CheckSideEffects( E_SideEffectFlag  flag )
{
	int op = this->GetOperator();
	if ( op == P_EQUAL ||
		
		op == P_PLUS_EQUAL ||
		op == P_MINUS_EQUAL ||
		
		op == P_ASTERISK_EQUAL ||
		op == P_SLASH_EQUAL ||
		op == P_PERCENT_EQUAL ||
		
		op == P_LESS_LESS_EQUAL ||
		op == P_GREATER_GREATER_EQUAL ||
		op == P_UNSIGNED_RIGHT_SHIFT_EQUAL ||
		
		op == P_PLUS_PLUS ||
		op == P_MINUS_MINUS ||

		op == P_AT_EQUAL ||
		op == P_CARET_EQUAL ||
		op == P_AMPERSAND_EQUAL ||
		op == P_VERTICAL_EQUAL )
	{
		return true;
	}

//	return m_left->CheckSideEffects( flag ) 
//		|| m_right->CheckSideEffects( flag ); 
	return Expression::CheckSideEffects( flag );
}

bool BinaryExpression::CheckLValue() {
	String  strExp;
	this->ToString( strExp );
	Error( "expression '%s' is not an lvalue", strExp.c_str() );
	return false;
}

bool BinaryExpression::IsUnsigned() const
{
	return m_left->GetType()->IsUnsigned() 
		|| m_right->GetType()->IsUnsigned();
}

#pragma endregion

#pragma region AssertExpression

DEFINE_TYPE( AssertExpression, Expression );

AssertExpression::AssertExpression( Expression* e, Expression* message /* =0 */ ) 
	:  m_message( message )
{
	m_expList = new ExpressionList;
	m_expList->Grab();
	m_expList->uiSetParent( this );
	m_expList->Add( e );

	if ( m_message ) {
		m_message->Grab();
		m_message->uiSetParent( this );
	}
}

AssertExpression::AssertExpression( ExpressionList* eList, Expression* message /* =0 */ ) 
	: m_expList( eList )
	, m_message( message )
{
	m_expList->Grab();
	m_expList->uiSetParent( this );

	if ( m_message ) {
		m_message->Grab();
		m_message->uiSetParent( this );
	}
}

AssertExpression::~AssertExpression() {
	m_expList->Drop();
	DROP( m_message );
}

const char * AssertExpression::ToChars() const { 
	return "Assert Expression";
}

void AssertExpression::ToString( String & s )
{
}

#pragma region "UI"
int AssertExpression::uiNumChildren() const {
	return m_message ? 2 : 1;
}

UINode * AssertExpression::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_expList;
	}
	if ( 1 == index ) {
		return m_message;
	}
	return 0;
}

int AssertExpression::uiIndexOf( const UINode *child ) const {
	if ( child == m_expList ) {
		return 0;
	}
	if ( child && child == m_message ) {
		return 1;
	}
	return -1;
}
#pragma endregion

Expression *  AssertExpression::ResolveInto( Semantics* semantics ) {
	return this;
}

#pragma endregion

#pragma region "AddrOfExp"

DEFINE_TYPE( AddrOfExp, UnaryExpression );

AddrOfExp::AddrOfExp( Expression* e ) 
	: UnaryExpression( e )
{
}

AddrOfExp::~AddrOfExp() {
}

const char* AddrOfExp::ToChars() const {
	return "& E";
}

void AddrOfExp::ToString( String & s )
{
	s += '&';
	m_expression->ToString( s );
}

Expression * AddrOfExp::CastTo( Type * pType ) {
	return this;
}

Expression *  AddrOfExp::ResolveInto( Semantics* semantics ) {
	UnaryExpression::ResolveInto( semantics );

	return this;
}

#pragma endregion

#pragma region "ArrayAccessExp ::= E [ E ]"

DEFINE_TYPE( ArrayAccessExp, BinaryExpression );

ArrayAccessExp::ArrayAccessExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

ArrayAccessExp::~ArrayAccessExp() {
}

const char * ArrayAccessExp::ToChars() const {
	return "E [ E ]";
}

void ArrayAccessExp::ToString( String & s ) {
	//s += "( ";
	m_left->ToString( s );
	s += "[ ";
	m_right->ToString( s );
	s += " ]";
	//s += " )";
}

Expression *  ArrayAccessExp::ResolveInto( Semantics* semantics ) {
	return NULL;
}

#pragma endregion

#pragma region "Pointer Dereferencing ( Expression '^' )"

DEFINE_TYPE( PtrIndirectionExp, UnaryExpression );

PtrIndirectionExp::PtrIndirectionExp( Expression* e ) 
	: UnaryExpression( e )
{
}

PtrIndirectionExp::~PtrIndirectionExp() {
}

const char* PtrIndirectionExp::ToChars() const {
	return "E^";
}

void PtrIndirectionExp::ToString( String & s ) {
	s += '*';
	m_expression->ToString( s );
}

Expression *  PtrIndirectionExp::ResolveInto( Semantics* semantics ) {
	//if ( m_expression->IsLiteralConstant() ) {
	//	Error( "pointer-dereferencing cannot be applied to constants!" );
	//	return NULL;
	//}
	return NULL;
}

#pragma endregion

//===========================================================================================
#pragma region PtrExp: '->'
DEFINE_TYPE( PtrExp, BinaryExpression );

PtrExp::PtrExp( Expression *e1, Expression *e2 )
	: BinaryExpression( e1, e2 )
{
}

PtrExp::~PtrExp() {
}

const char* PtrExp::ToChars() const {
	return "E '->' e ( E^.e )";
}

void PtrExp::ToString( String & s ) {
	m_left->ToString( s );
	s += "->";
	m_right->ToString( s );
}

Expression *  PtrExp::ResolveInto( Semantics* semantics ) {
	return NULL;
}

#pragma endregion
//===========================================================================================

#pragma region DotExp

DEFINE_TYPE( DotExp, BinaryExpression );

DotExp::DotExp( Expression *left, Expression *right )
	: BinaryExpression( left, right )
{
}

DotExp::DotExp( Expression *left, Identifier *right )
	: BinaryExpression( left, new IdExp( right ) )
{
}

DotExp::~DotExp() {
}

const char* DotExp::ToChars() const {
	return "E '.' E";
}

void DotExp::ToString( String & s ) {
	m_left->ToString( s );
	s += '.';
	m_right->ToString( s );
}

Expression *  DotExp::ResolveInto( Semantics* semantics ) {
	return NULL;
}

#pragma endregion

#pragma region "Cast Expression"

DEFINE_TYPE( CastExp, UnaryExpression );

CastExp::CastExp( Expression* targetExp, Expression* sourceExp ) 
	: m_targetExp( targetExp )
	, m_targetType( NULL )
	, UnaryExpression( sourceExp )
{
	m_targetExp->Grab();
	m_targetExp->uiSetParent( this );

	m_type = m_targetExp->GetType();
	//UNDONE: ??? should we call m_type->Grab(); ???
	m_type->Grab();
}

CastExp::CastExp( Type *targetType, Expression *sourceExp )
	: m_targetType( targetType )
	, m_targetExp( NULL )
	, UnaryExpression( sourceExp )
{
	m_targetType->Grab();

	m_type = m_targetType;
	//UNDONE: ??? should we call m_type->Grab(); ???
	m_type->Grab();
}

CastExp::~CastExp() {
	DROP( m_targetType );
	DROP( m_targetExp );
}

const char* CastExp::ToChars() const {
	return "cast< type >( exp )";
}

void CastExp::ToString( String & s ) {
	s += "cast< ";
	if ( m_targetType ) {
		m_targetType->ToString( s );
	} else {
		m_targetExp->ToString( s );
	}
	s += " >( ";
	m_expression->ToString( s );
	s += " )";
}

#pragma region "UI"
int CastExp::uiNumChildren() const {
	return 2;
}

UINode * CastExp::uiChildAt( int index ) {
	if ( 0 == index ) {
		//return m_targetType ? m_targetType : m_targetExp;
	// BUG:
		return m_targetExp;
	}
	if ( 1 == index ) {
		return m_expression;
	}
	return 0;
}

int CastExp::uiIndexOf( const UINode *child ) const {
	if ( m_targetType && child == m_targetType ) {
		return 0;
	} else if ( child == m_targetExp ) {
		return 0;
	}
	if ( child == m_expression ) {
		return 1;
	}
	return -1;
}
#pragma endregion

Expression * CastExp::CastTo( Type * pType ) {
	Unimplemented();
	return this;
}

Expression *  CastExp::ResolveInto( Semantics* semantics ) {
	// TODO: various checks
	Unimplemented();
	return this;
}

#pragma endregion

#pragma region "NegationExp"

DEFINE_TYPE( NegationExp, UnaryExpression );

NegationExp::NegationExp( Expression* e ) 
	: UnaryExpression( e )
{
}

NegationExp::~NegationExp() {
}

const char* NegationExp::ToChars() const {
	return "-E";
}

void NegationExp::ToString( String & s ) {
	s += "-( ";
	m_expression->ToString( s );
	s += " )";
}

Expression *  NegationExp::ResolveInto( Semantics* semantics ) {
	Expression * e = UnaryExpression::ResolveInto( semantics );
	return e;
}

#pragma endregion

#pragma region "UnaryPlusExp"

DEFINE_TYPE( UnaryPlusExp, UnaryExpression );

UnaryPlusExp::UnaryPlusExp( Expression* e ) 
	: UnaryExpression( e )
{
}

UnaryPlusExp::~UnaryPlusExp() {
}

const char* UnaryPlusExp::ToChars() const {
	return "+E";
}

void UnaryPlusExp::ToString( String & s ) {
	//s += '+';
	m_expression->ToString( s );
}

Expression *  UnaryPlusExp::ResolveInto( Semantics* semantics ) {
	Expression * e = UnaryExpression::ResolveInto( semantics );
	return e;
}

#pragma endregion

#pragma region "++E"

DEFINE_TYPE( PreIncExp, UnaryExpression );

PreIncExp::PreIncExp( Expression* e ) 
	: UnaryExpression( e )
{
}

PreIncExp::~PreIncExp() {
}

const char* PreIncExp::ToChars() const {
	return "++E";
}

void PreIncExp::ToString( String & s ) {
	s += "++";
	m_expression->ToString( s );
}

Expression *  PreIncExp::ResolveInto( Semantics* semantics ) {
	Expression * e = UnaryExpression::ResolveInto( semantics );

	if ( e->IsConst() ) {
		Error( "'--E': cannot modify constant expression" );
	}
	if ( m_type->IsBoolean() ) {
		Error( "operation '%s' is not allowed on bool", this->ToChars() );
	}
	return e;
}

#pragma endregion

#pragma region "--E"

DEFINE_TYPE( PreDecExp, UnaryExpression );

PreDecExp::PreDecExp( Expression* e ) 
	: UnaryExpression( e )
{
}

PreDecExp::~PreDecExp() {
}

const char* PreDecExp::ToChars() const {
	return "--E";
}

void PreDecExp::ToString( String & s ) {
	s += "--";
	m_expression->ToString( s );
}

Expression *  PreDecExp::ResolveInto( Semantics* semantics ) {
	Expression * e = UnaryExpression::ResolveInto( semantics );

	if ( e->IsConst() ) {
		Error( "'--E': cannot modify constant expression" );
	}
	if ( m_type->IsBoolean() ) {
		Error( "operation '%s' is not allowed on bool", this->ToChars() );
	}
	return e;
}

#pragma endregion

#pragma region "E++"

DEFINE_TYPE( PostIncExp, UnaryExpression );

PostIncExp::PostIncExp( Expression* e ) 
	: UnaryExpression( e )
{
}

PostIncExp::~PostIncExp() {
}

const char* PostIncExp::ToChars() const {
	return "E++";
}

void PostIncExp::ToString( String & s ) {
	m_expression->ToString( s );
	s += "++";
}

Expression *  PostIncExp::ResolveInto( Semantics* semantics ) 
{
	UnaryExpression::ResolveInto( semantics );

	if ( m_expression->IsConst() ) 
	{
		Error( "'E++': cannot modify constant expression" );
	}
	if ( m_type->IsBoolean() ) {
		Error( "operation '%s' is not allowed on bool", this->ToChars() );
	}

	return NULL;
}

#pragma endregion

#pragma region "E--"

DEFINE_TYPE( PostDecExp, UnaryExpression );

PostDecExp::PostDecExp( Expression* e ) 
	: UnaryExpression( e )
{
}

PostDecExp::~PostDecExp() {
}

const char* PostDecExp::ToChars() const {
	return "E--";
}

void PostDecExp::ToString( String & s ) {
	m_expression->ToString( s );
	s += "--";
}

Expression *  PostDecExp::ResolveInto( Semantics* semantics ) 
{
	Expression * e = UnaryExpression::ResolveInto( semantics );
	if ( e->IsConst() ) {
		Error( "'E--': cannot modify constant expression" );
	}
	if ( m_type->IsBoolean() ) {
		Error( "operation '%s' is not allowed on bool", this->ToChars() );
	}
	return e;
}

#pragma endregion

#pragma region "~E"

DEFINE_TYPE( ComplExp, UnaryExpression );

ComplExp::ComplExp( Expression* e ) 
	: UnaryExpression( e )
{
}

ComplExp::~ComplExp() {
}

const char* ComplExp::ToChars() const {
	return "~E";
}

void ComplExp::ToString( String & s ) {
	s += "~(";
	m_expression->ToString( s );
	s += ')';
}

Expression *  ComplExp::ResolveInto( Semantics* semantics ) {
	Expression * e = UnaryExpression::ResolveInto( semantics );
	if ( m_type->IsFloating() ) {
		Error( "operator '~' cannot be applied to floats!" );
	}
	return e;
}

#pragma endregion

#pragma region "!E"

DEFINE_TYPE( NotExp, UnaryExpression );

NotExp::NotExp( Expression* e ) 
	: UnaryExpression( e )
{
}

NotExp::~NotExp() {
}

const char* NotExp::ToChars() const {
	return "!E";
}

void NotExp::ToString( String & s ) {
	s += "!(";
	m_expression->ToString( s );
	s += ')';
}

Expression * NotExp::ResolveInto( Semantics* semantics ) {
	Expression * e = UnaryExpression::ResolveInto( semantics );

	//TODO: remove this kludge
//	Type_Bool * tmp = new Type_Bool();
//	Expression * e2 = e->CastTo( tmp );

	return e;
}

#pragma endregion

#pragma region "CommaExpression ::= E, E, ... , E"

DEFINE_TYPE( CommaExpression, BinaryExpression );

CommaExpression::CommaExpression( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * CommaExpression::ToChars() const {
	return "CommaExpression: E, E, ..., E";
}

void CommaExpression::ToString( String & s ) {
/*
	PRECEDENCE  myPrec = this->GetPrecedence();
	s += '(';
	Write( m_left, myPrec, s );
	s += ", ";
	Write( m_right, myPrec, s );
	s += ')';
*/
	s += '(';
	m_left->ToString( s );
	s += ", ";
	m_right->ToString( s );
	s += ')';
}

Expression *  CommaExpression::ResolveInto( Semantics* semantics ) {
	c4::Resolve< Expression >( & m_left, semantics );

	m_left->CheckSideEffects( RESULT_IS_NOT_USED );

	c4::Resolve< Expression >( & m_right, semantics );

	CombineTypes( semantics );

	return this;
}

bool CommaExpression::CheckSideEffects( E_SideEffectFlag  flag )
{
	if ( flag == DO_NOT_ISSUE_ERRORS )
	{
		return m_left->CheckSideEffects( DO_NOT_ISSUE_ERRORS )
			|| m_right->CheckSideEffects( DO_NOT_ISSUE_ERRORS );
	}
	else {
		return m_right->CheckSideEffects( flag );
	}
}

#pragma endregion

#pragma region "E + E"

DEFINE_TYPE( AddExp, BinaryExpression );

AddExp::AddExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

const char * AddExp::ToChars() const {
	return "E + E";
}

void AddExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " + ";
	Write( m_right, myPrec, s );
}

Expression *  AddExp::ResolveInto( Semantics* semantics ) {
	return BinaryExpression::ResolveInto( semantics );
}

#pragma endregion

#pragma region "E - E"

DEFINE_TYPE( SubExp, BinaryExpression );

SubExp::SubExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

const char * SubExp::ToChars() const {
	return "E - E";
}

void SubExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " - ";
	Write( m_right, myPrec, s );
}

Expression *  SubExp::ResolveInto( Semantics* semantics ) {
	return BinaryExpression::ResolveInto( semantics );
}

#pragma endregion

#pragma region "E * E"

DEFINE_TYPE( MulExp, BinaryExpression );

MulExp::MulExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

const char * MulExp::ToChars() const {
	return "E * E";
}

void MulExp::ToString( String & s ) 
{
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " * ";
	Write( m_right, myPrec, s );
}

Expression *  MulExp::ResolveInto( Semantics* semantics ) {
	return BinaryExpression::ResolveInto( semantics );
}

#pragma endregion

#pragma region "E / E"

DEFINE_TYPE( DivExp, BinaryExpression );

DivExp::DivExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

const char * DivExp::ToChars() const {
	return "E / E";
}

void DivExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " / ";
	Write( m_right, myPrec, s );
}

Expression *  DivExp::ResolveInto( Semantics* semantics ) {
	return BinaryExpression::ResolveInto( semantics );
}

#pragma endregion

#pragma region "E % E"

DEFINE_TYPE( ModExp, BinaryExpression );

ModExp::ModExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

const char * ModExp::ToChars() const {
	return "E % E";
}

void ModExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " % ";
	Write( m_right, myPrec, s );
}

Expression *  ModExp::ResolveInto( Semantics* semantics ) {
	Expression * e = BinaryExpression::ResolveInto( semantics );
	if ( m_type->IsFloating() ) {
		Error( "operator '%%' cannot be applied to floats!" );
	}
	return e;
}

#pragma endregion

#pragma region "E << E"

DEFINE_TYPE( ShlExp, BinaryExpression );

ShlExp::ShlExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

const char * ShlExp::ToChars() const {
	return "E << E";
}

void ShlExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " << ";
	Write( m_right, myPrec, s );
}

Expression *  ShlExp::ResolveInto( Semantics* semantics ) {
	Expression * e = BinaryExpression::ResolveInto( semantics );
	if ( m_type->IsFloating() ) {
		Error( "operator '<<' cannot be applied to floats!" );
	}
	return e;
}

#pragma endregion

#pragma region "E >> E"

DEFINE_TYPE( ShrExp, BinaryExpression );

ShrExp::ShrExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

const char * ShrExp::ToChars() const {
	return "E >> E";
}

void ShrExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " >> ";
	Write( m_right, myPrec, s );
}

Expression *  ShrExp::ResolveInto( Semantics* semantics ) {
	Expression * e = BinaryExpression::ResolveInto( semantics );
	if ( m_type->IsFloating() ) {
		Error( "operator '>>' cannot be applied to floats!" );
	}
	return e;
}

#pragma endregion

#pragma region "E >>> E"

DEFINE_TYPE( UShrExp, BinaryExpression );

UShrExp::UShrExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

const char * UShrExp::ToChars() const {
	return "E >>> E";
}

void UShrExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " >>> ";
	Write( m_right, myPrec, s );
}

Expression *  UShrExp::ResolveInto( Semantics* semantics ) {
	Expression * e = BinaryExpression::ResolveInto( semantics );
	if ( m_type->IsFloating() ) {
		Error( "operator '>>>' cannot be applied to floats!" );
	}
	return e;
}

#pragma endregion

#pragma region "E == E, E != E"

DEFINE_TYPE( EquExp, BinaryExpression );

EquExp::EquExp( Expression *e1, Expression *e2, OperatorKind op ) 
	: BinaryExpression( e1, e2 )
	, m_op( op ) 
{
}

const char * EquExp::ToChars() const {
	return Equal == m_op ? "E == E" : "E != E";
}

void EquExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	
	Equal == m_op ? (s += " == ") : (s += " != ");
	
	Write( m_right, myPrec, s );
}

Expression *  EquExp::ResolveInto( Semantics* semantics ) {
	return BinaryExpression::ResolveInto( semantics );
}

#pragma endregion

#pragma region "E < E, E > E, E <= E, E >= E"

DEFINE_TYPE( RelExp, BinaryExpression );

RelExp::RelExp( Expression *e1, Expression *e2, OperatorKind op ) 
	: BinaryExpression( e1, e2 )
	, m_op( op ) 
{
}

const char * RelExp::ToChars() const {
	switch ( m_op ) {
		case LT:	return "E < E";
		case GT:	return "E > E";
		case LE:	return "E <= E";
		case GE:	return "E >= E";
		default:	Unreachable();
	}
	return NULL; // prevent compiler from complaining ( "not all control paths return a value" )
}

void RelExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	
	switch ( m_op ) {
		case LT:	s += " < "; break;
		case GT:	s += " > "; break;
		case LE:	s += " <= "; break;
		case GE:	s += " >= "; break;
		default:	Unreachable();
	}
	
	Write( m_right, myPrec, s );
}

Expression *  RelExp::ResolveInto( Semantics* semantics ) {
	return BinaryExpression::ResolveInto( semantics );
}

#pragma endregion

#pragma region "BitAndExp ::= e1 & e2"

DEFINE_TYPE( BitAndExp, BinaryExpression );

BitAndExp::BitAndExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

BitAndExp::~BitAndExp() {
}

const char * BitAndExp::ToChars() const {
	return "E & E";
}

void BitAndExp::ToString( String & s ) 
{
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " & ";
	Write( m_right, myPrec, s );
}

Expression * BitAndExp::ResolveInto( Semantics* semantics ) {
	Expression * e = BinaryExpression::ResolveInto( semantics );
	if ( m_type->IsFloating() ) {
		Error( "operator '&' cannot be applied to floats!" );
	}
	return e;
}

#pragma endregion

#pragma region "BitXorExp ::= e1 ^ e2"

DEFINE_TYPE( BitXorExp, BinaryExpression );

BitXorExp::BitXorExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

BitXorExp::~BitXorExp() {
}

const char * BitXorExp::ToChars() const {
	return "E ^ E";
}

void BitXorExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " ^ ";
	// s+= " xor ";
	Write( m_right, myPrec, s );
}

Expression *  BitXorExp::ResolveInto( Semantics* semantics ) {
	Expression * e = BinaryExpression::ResolveInto( semantics );
	if ( m_type->IsFloating() ) {
		Error( "operator 'xor' cannot be applied to floats!" );
	}
	return e;
}

#pragma endregion

#pragma region "BitOrExp ::= e1 | e2"

DEFINE_TYPE( BitOrExp, BinaryExpression );

BitOrExp::BitOrExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

BitOrExp::~BitOrExp() {
}

const char * BitOrExp::ToChars() const {
	return "E | E";
}

void BitOrExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " | ";
	Write( m_right, myPrec, s );
}

Expression *  BitOrExp::ResolveInto( Semantics* semantics ) {
	Expression * e = BinaryExpression::ResolveInto( semantics );
	if ( m_type->IsFloating() ) {
		Error( "operator '|' cannot be applied to floats!" );
	}
	return e;
}

#pragma endregion

#pragma region "AndAndExp ::= e1 && e2"

DEFINE_TYPE( AndAndExp, BinaryExpression );

AndAndExp::AndAndExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

AndAndExp::~AndAndExp() {
}

const char * AndAndExp::ToChars() const {
	return "E && E";
}

void AndAndExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " && ";
	Write( m_right, myPrec, s );
}

Expression *  AndAndExp::ResolveInto( Semantics* semantics ) {
	return BinaryExpression::ResolveInto( semantics );
}

#pragma endregion

#pragma region "OrOrExp ::= e1 || e2"

DEFINE_TYPE( OrOrExp, BinaryExpression );

OrOrExp::OrOrExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

OrOrExp::~OrOrExp() {
}

const char * OrOrExp::ToChars() const {
	return "E || E";
}

void OrOrExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " || ";
	Write( m_right, myPrec, s );
}

Expression *  OrOrExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );
	return NULL;
}

#pragma endregion

#pragma region "InExp ::= e1 in e2"

DEFINE_TYPE( InExp, BinaryExpression );

InExp::InExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

InExp::~InExp() {
}

const char * InExp::ToChars() const {
	return "E in E";
}

void InExp::ToString( String & s ) {
	PRECEDENCE  myPrec = this->GetPrecedence();

	Write( m_left, myPrec, s );
	s += " in ";
	Write( m_right, myPrec, s );
}

Expression *  InExp::ResolveInto( Semantics* semantics ) {
	return BinaryExpression::ResolveInto( semantics );
}

#pragma endregion

#pragma region "IsExp ::= e1 is e2"

DEFINE_TYPE( IsExp, BinaryExpression );

IsExp::IsExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

IsExp::~IsExp() {
}

const char * IsExp::ToChars() const {
	return "E is E";
}

void IsExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " is ";
	m_right->ToString( s );
}

Expression *  IsExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return NULL;
}

#pragma endregion

#pragma region "AsExp ::= e1 as e2"

DEFINE_TYPE( AsExp, BinaryExpression );

AsExp::AsExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 ) 
{
}

AsExp::~AsExp() {
}

const char * AsExp::ToChars() const {
	return "E as E";
}

void AsExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " as ";
	m_right->ToString( s );
}

Expression *  AsExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return NULL;
}

#pragma endregion

#pragma region "CondExp ::= ECond ? e1 : e2"

DEFINE_TYPE( CondExp, BinaryExpression );

CondExp::CondExp( Expression *condition, Expression *e1, Expression *e2 )
	: m_condition( condition )
	, BinaryExpression( e1, e2 ) 
{
	m_condition->Grab();
	m_condition->uiSetParent( this );
}

CondExp::~CondExp() {
	m_condition->Drop();
}

const char * CondExp::ToChars() const {
	return "Econd ? E1 : E2";
}

#pragma region "UI"
void CondExp::ToString( String & s ) {
	m_condition->ToString( s );
	s += " ? ";
	m_left->ToString( s );
	s += " : ";
	m_right->ToString( s );
}

int CondExp::uiNumChildren() const {
	return 3;
}

UINode * CondExp::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_condition;
	}
	if ( 1 == index ) {
		return m_left;
	}
	if ( 2 == index ) {
		return m_right;
	}
	return 0;
}

int CondExp::uiIndexOf( const UINode *child ) const {
	if ( child == m_condition ) {
		return 0;
	}
	if ( child == m_left ) {
		return 1;
	}
	if ( child == m_right ) {
		return 2;
	}
	return -1;
}
#pragma endregion

Expression *  CondExp::ResolveInto( Semantics* semantics ) 
{
	c4::Resolve< Expression >( & m_condition, semantics );

	//TODO: remove this kludge
	Type_Bool * tmp = new Type_Bool();
	Expression * testExp = m_condition->CastTo( tmp );
	delete tmp;
	if ( testExp && testExp != m_condition ) {
		m_condition->Drop();
		m_condition = testExp;
		m_condition->Grab();
	}

	Expression * e = BinaryExpression::ResolveInto( semantics );
	return e;
}

#pragma endregion

#pragma region "AssignmentExp ::= e1 = e2"

DEFINE_TYPE( AssignmentExp, BinaryExpression );

AssignmentExp::AssignmentExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

AssignmentExp::~AssignmentExp() {
}

const char * AssignmentExp::ToChars() const {
	return "Assignment: E = E";
}

void AssignmentExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " = ";
	m_right->ToString( s );
}

Expression *  AssignmentExp::ResolveInto( Semantics* semantics ) {
	c4::Resolve< Expression >( & m_left, semantics );
	c4::Resolve< Expression >( & m_right, semantics );

	Assert2( m_left->GetType(), "expression must be resolved!" );
	Assert2( m_right->GetType(), "expression must be resolved!" );

	m_left->CheckLValue();
	m_left->CheckRValue();

	Expression * e = m_right->CastTo( m_left->GetType() );
	if ( e && e != m_right ) {
		m_right->Drop();
		m_right = e;
		m_right->Grab();
	}

	Assert( m_left->GetType()->GetTypeKind() == m_right->GetType()->GetTypeKind() );

	m_type = m_right->GetType();
	//UNDONE: ??? should we call m_type->Grab(); ???
	m_type->Grab();

	return this;
}

Expression * AssignmentExp::CastTo( Type * pType ) {
	if ( m_type->GetTypeKind() != pType->GetTypeKind() ) {
		return new CastExp( pType, this );
	}
	return this;
}

Expression * AssignmentExp::CheckToBoolean() {
	Error( "'=' does not give a boolean result" );
	return this;
}

#pragma endregion

#pragma region "AddAssignExp ::= e1 += e2"

DEFINE_TYPE( AddAssignExp, BinaryExpression );

AddAssignExp::AddAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * AddAssignExp::ToChars() const {
	return "E += E";
}

void AddAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " += ";
	m_right->ToString( s );
}

Expression *  AddAssignExp::ResolveInto( Semantics* semantics ) {
	//Expression * e = BinaryExpression::ResolveInto( semantics );

	AddExp * addExp = new AddExp( m_left, m_right );
	AssignmentExp * e = new AssignmentExp( m_left, addExp );

	return e->ResolveInto( semantics );
}

#pragma endregion

#pragma region "SubAssignExp ::= e1 -= e2"

DEFINE_TYPE( SubAssignExp, BinaryExpression );

SubAssignExp::SubAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * SubAssignExp::ToChars() const {
	return "E -= E";
}

void SubAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " -= ";
	m_right->ToString( s );
}

Expression *  SubAssignExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return this;
}

#pragma endregion

#pragma region "MulAssignExp ::= e1 *= e2"

DEFINE_TYPE( MulAssignExp, BinaryExpression );

MulAssignExp::MulAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * MulAssignExp::ToChars() const {
	return "E *= E";
}

void MulAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " *= ";
	m_right->ToString( s );
}

Expression *  MulAssignExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return this;
}

#pragma endregion

#pragma region "DivAssignExp ::= e1 /= e2"

DEFINE_TYPE( DivAssignExp, BinaryExpression );

DivAssignExp::DivAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * DivAssignExp::ToChars() const {
	return "E /= E";
}

void DivAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " /= ";
	m_right->ToString( s );
}

Expression *  DivAssignExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return this;
}

#pragma endregion

#pragma region "ModAssignExp ::= e1 %= e2"

DEFINE_TYPE( ModAssignExp, BinaryExpression );

ModAssignExp::ModAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * ModAssignExp::ToChars() const {
	return "E %= E";
}

void ModAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " %= ";
	m_right->ToString( s );
}

Expression *  ModAssignExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return NULL;
}

#pragma endregion

#pragma region "BitAndAssignExp ::= e1 &= e2"

DEFINE_TYPE( BitAndAssignExp, BinaryExpression );

BitAndAssignExp::BitAndAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * BitAndAssignExp::ToChars() const {
	return "E &= E";
}

void BitAndAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " &= ";
	m_right->ToString( s );
}

Expression *  BitAndAssignExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return NULL;
}

#pragma endregion

#pragma region "BitXorAssignExp ::= e1 ^= e2"

DEFINE_TYPE( BitXorAssignExp, BinaryExpression );

BitXorAssignExp::BitXorAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * BitXorAssignExp::ToChars() const {
	return "E ^= E";
}

void BitXorAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " ^= ";
	m_right->ToString( s );
}

Expression *  BitXorAssignExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return NULL;
}

#pragma endregion

#pragma region "BitOrAssignExp ::= e1 |= e2"

DEFINE_TYPE( BitOrAssignExp, BinaryExpression );

BitOrAssignExp::BitOrAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * BitOrAssignExp::ToChars() const {
	return "E |= E";
}

void BitOrAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " |= ";
	m_right->ToString( s );
}

Expression *  BitOrAssignExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return NULL;
}

#pragma endregion

#pragma region "ShlAssignExp ::= e1 <<= e2"

DEFINE_TYPE( ShlAssignExp, BinaryExpression );

ShlAssignExp::ShlAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * ShlAssignExp::ToChars() const {
	return "E <<= E";
}

void ShlAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " <<= ";
	m_right->ToString( s );
}

Expression *  ShlAssignExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return NULL;
}

#pragma endregion

#pragma region "ShlAssignExp ::= e1 >>= e2"

DEFINE_TYPE( ShrAssignExp, BinaryExpression );

ShrAssignExp::ShrAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * ShrAssignExp::ToChars() const {
	return "E >>= E";
}

void ShrAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " >>= ";
	m_right->ToString( s );
}

Expression *  ShrAssignExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return NULL;
}

#pragma endregion

#pragma region "UShrAssignExp ::= e1 >>>= e2"

DEFINE_TYPE( UShrAssignExp, BinaryExpression );

UShrAssignExp::UShrAssignExp( Expression *e1, Expression *e2 ) 
	: BinaryExpression( e1, e2 )
{
}

const char * UShrAssignExp::ToChars() const {
	return "E >>>= E";
}

void UShrAssignExp::ToString( String & s ) {
	m_left->ToString( s );
	s += " >>>= ";
	m_right->ToString( s );
}

Expression *  UShrAssignExp::ResolveInto( Semantics* semantics ) {
	BinaryExpression::ResolveInto( semantics );

	return NULL;
}

#pragma endregion

#pragma region "CallExp ::= E ( params )"

DEFINE_TYPE( CallExp, UnaryExpression );

CallExp::CallExp( Expression *e, ExpressionList *args ) 
	: UnaryExpression( e )
	, m_args( args )
	, m_symbol( NULL )
{
	args->Grab();
	args->uiSetParent( this );
}

CallExp::~CallExp() {
	m_args->Drop();
}

const char* CallExp::ToChars() const {
	return "CallExp: E ( params )";
}

void CallExp::ToString( String & s ) {
	m_expression->ToString( s );
	s += "(";
	m_args->ToString( s );
	s += ")";
}

#pragma region "UI"
int CallExp::uiNumChildren() const {
	return 2;
}

UINode * CallExp::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_expression;
	}
	if ( 1 == index ) {
		return m_args;
	}
	return 0;
}

int CallExp::uiIndexOf( const UINode *child ) const {
	if ( child == m_expression ) {
		return 0;
	}
	if ( child == m_args ) {
		return 1;
	}
	return -1;
}
#pragma endregion

Expression * CallExp::ResolveInto( Semantics* semantics ) 
{
	c4::Resolve< Expression >( & m_expression, semantics );

	// 03.08.2008.
	// Only identifiers in function call expressions are supported:
	if ( ! m_expression->GetTypeInfo()->Is( & IdExp::typeInfo ) )
	{
		String  strTmp;
		m_expression->ToString( strTmp );
		Error( "cannot call '%s'", strTmp.c_str() );
		goto L_error;
	}

	Identifier * pIdent = (( IdExp* )( m_expression ))->GetIdentifier();

	//-------------------------
	Scope * scope = semantics->GetContext();
	Symbol * pSymbol = scope->Search( pIdent );

	if ( pSymbol ) 
	{
		if ( ! pSymbol->GetTypeInfo()->Is( & FunctionSymbol::typeInfo ) )
		{
			Error( "'%s' is not a function: '%s'", pSymbol->Name() );
			goto L_resolve_params;
		}
		else {
			m_symbol = ( FunctionSymbol* )pSymbol;
		}

		if ( ! m_symbol->IsVarArg() )
		{
			int  numParams = m_args->GetList().Num();
			if ( numParams != m_symbol->GetNumOfArguments() )
			{
				Error( "function '%s' doesn't take %d argument(s)", m_symbol->Name(), numParams );
			}
		}

		m_type = m_symbol->GetReturnType();
		m_type->Grab();
	}
	else {
L_error:
		//Error( "undefined function: '%s'", pIdent->Text() );
		m_type = new Type_Error( "undefined identifier", location );
	}
//==============
L_resolve_params:
	// resolve our parameters
	LinkedList< Expression* > &	rList = m_args->GetList();
	LinkedList< Expression* >::Iterator  iter = rList.Begin();

	for ( ; iter != rList.End(); iter++ ) 
	{
		c4::Resolve< Expression >( &( *iter ), semantics );
	}

	return this;
}

bool CallExp::CheckSideEffects( E_SideEffectFlag  flag ) {
	Assert( m_type );
	return m_symbol->CheckSideEffects();
}

#pragma endregion

#pragma region "Erroneous Expression"

DEFINE_TYPE( ErroneousExpression, Expression );

ErroneousExpression::ErroneousExpression( const char* message, const Location & location )
	: m_message( message )
{
	this->location = location;
}

ErroneousExpression::~ErroneousExpression() {
}

const char * ErroneousExpression::GetMessage() const {
	return m_message.c_str();
}

const char * ErroneousExpression::ToChars() const {
	return "Erroneous Expression";
}

void ErroneousExpression::ToString( String & s ) {
	s += "malformed expression";
}

Expression *  ErroneousExpression::ResolveInto( Semantics* semantics ) {
	c4::compiler->Error( "erroneous expression" );
	return this;
}

#pragma endregion


#pragma region "'This' Expression"

DEFINE_TYPE( ThisExpression, Expression );

ThisExpression::ThisExpression( const Location & loc )
{
	location = loc;
}

ThisExpression::~ThisExpression() {
}

const char* ThisExpression::ToChars() const {
	return "'this' expression";
}

void ThisExpression::ToString( String & s ) {
	s += "this";
}

Expression *  ThisExpression::ResolveInto( Semantics* semantics ) {
	return NULL;
	c4::common->Print( "Resolving 'this' expression: '%s'", ToChars() );
}

#pragma endregion

}//end namespace c4