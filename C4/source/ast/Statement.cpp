/*
======================================
	implementation of ast nodes,
	representing statements.
======================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

#pragma region "Statement"

DEFINE_TYPE( Statement, AST_Node );

Statement::Statement()
{
}

Statement::~Statement() {
}

const char * Statement::ToChars() const {
	return "Statement";
}

void Statement::ToString( String & s ) {
}

void Statement::Resolve( Semantics* semantics ) {
}

#pragma endregion

#pragma region "Empty Statement"

DEFINE_TYPE( EmptyStatement, Statement );

EmptyStatement::EmptyStatement()
{
}

EmptyStatement::~EmptyStatement() {
}

const char* EmptyStatement::ToChars() const {
	return "Empty Statement";
}

void EmptyStatement::ToString( String & s ) {
}

int EmptyStatement::uiNumChildren() const {
	return 0;
}

UINode * EmptyStatement::uiChildAt( int index ) {
	return 0;
}

int EmptyStatement::uiIndexOf( const UINode *child ) const {
	return -1;
}

void EmptyStatement::Resolve( Semantics* semantics ) {
}

#pragma endregion

#pragma region "Error Statement"

DEFINE_TYPE( ErrorStatement, Statement );

ErrorStatement::ErrorStatement( const char * message, const Location & loc )
{
	m_detailedInfo = message;
	location = loc;
}

ErrorStatement::~ErrorStatement() {
}

const char * ErrorStatement::GetMessage() const {
	return m_detailedInfo;
}

const char* ErrorStatement::ToChars() const {
	return "Error Statement";
}

void ErrorStatement::ToString( String & s ) {
}

void ErrorStatement::Resolve( Semantics* semantics ) {
}

#pragma endregion

#pragma region "Statement List"

DEFINE_TYPE( StatementList, Statement );

StatementList::StatementList()
{
}

StatementList::~StatementList()
{
	LinkedList< Statement* >::Iterator  iter = m_stmtList.Begin();

	for ( int i = 0; iter != m_stmtList.End(); iter++, i++ ) 
	{
		( *iter )->Drop();
	}
	m_stmtList.RemoveAll();
}

void StatementList::Add( Statement* s ) {
	s->Grab();

	s->uiSetParent( this );
	m_stmtList.Append( s );
}

const char * StatementList::ToChars() const {
	return "Statement List";
}

void StatementList::ToString( String & s ) {
}

#pragma region "UI"
int StatementList::uiNumChildren() const {
	return m_stmtList.Num();
}

UINode * StatementList::uiChildAt( int index )
{
	if ( index >= m_stmtList.Num() ) {
		return 0;
	}

	LinkedList< Statement* >::Iterator  iter = m_stmtList.Begin();

	for ( int i = 0; iter != m_stmtList.End(); iter++, i++ ) {
		if ( i == index ) {
			return *iter;
		}
	}

	return 0;
}

int StatementList::uiIndexOf( const UINode *child ) const
{
	LinkedList< Statement* >::Iterator  iter = m_stmtList.Begin();

	for ( int i = 0; iter != m_stmtList.End(); iter++, i++ ) {
		if ( child == *iter ) {
			return i;
		}
	}

	return -1;
}
#pragma endregion

void StatementList::Resolve( Semantics* semantics )
{
	LinkedList< Statement* >::Iterator  iter = m_stmtList.Begin();

	// for removing unreachable code after we've seen a return statement
	LinkedList< Statement* >::Iterator  & rIterRet( m_stmtList.End() );
	
	for ( ; iter != m_stmtList.End(); iter++ ) 
	{
		Statement * s = *iter;
		s->Resolve( semantics );
		
		// check for unreachable code
		if ( s->IsReturnStatement() && iter.HasNext() )
		{
			Warning( "unreachable code after return statement" );
			rIterRet = iter + 1;
		}
	}

	// remove dead code
	while ( rIterRet.HasCurrent() )
	{
		rIterRet = m_stmtList.Remove( rIterRet );
	}
}

#pragma endregion

#pragma region "ExpressionStatement"

DEFINE_TYPE( ExpressionStatement, Statement );

ExpressionStatement::ExpressionStatement( Expression* e )
	: m_expression( e )
{
	m_expression->Grab();

	m_expression->uiSetParent( this );
}

ExpressionStatement::~ExpressionStatement() {
	m_expression->Drop();
}

Expression * ExpressionStatement::GetExpression() {
	return m_expression;
}

const char* ExpressionStatement::ToChars() const {
	return "Expression Statement";
}

void ExpressionStatement::ToString( String & s ) {
	m_expression->ToString( s );
	s += ';';
}

#pragma region "UI"
int ExpressionStatement::uiNumChildren() const {
	return 1;
}

UINode * ExpressionStatement::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_expression;
	}
	return 0;
}

int ExpressionStatement::uiIndexOf( const UINode *child ) const {
	if ( child == m_expression ) {
		return 0;
	}
	return -1;
}
#pragma endregion

void ExpressionStatement::Resolve( Semantics* semantics ) {
	c4::Resolve< Expression >( & m_expression, semantics );

	m_expression->CheckSideEffects( Expression::RESULT_IS_NOT_USED );
}

#pragma endregion

#pragma region "Labelled Statement"

DEFINE_TYPE( LabeledStatement, Statement );

LabeledStatement::LabeledStatement( Identifier *name, Statement* s )
	: m_id( name ), m_stmt( s )
{
	m_id->Grab();

	if ( m_stmt ) {
		m_stmt->Grab();
		m_stmt->uiSetParent( this );
	}
}

LabeledStatement::~LabeledStatement() {
	m_id->Drop();
	m_stmt->Drop();
}

Identifier * LabeledStatement::GetIdentifier() {
	return m_id;
}

Statement * LabeledStatement::GetStatement() {
	return m_stmt;
}

const char* LabeledStatement::ToChars() const {
	static String  name( "Labelled Statement: " + String( m_id->ToChars() ) );
	return name.c_str();
}

void LabeledStatement::ToString( String & s ) {
}

#pragma region "UI"
int LabeledStatement::uiNumChildren() const {
	return 1;
}

UINode * LabeledStatement::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_stmt;
	}
	return 0;
}

int LabeledStatement::uiIndexOf( const UINode *child ) const {
	if ( child == m_stmt ) {
		return 0;
	}
	return -1;
}
#pragma endregion

void LabeledStatement::Resolve( Semantics* semantics ) {
	m_stmt->Resolve( semantics );
}

#pragma endregion

#pragma region "Declaration Statement"

DEFINE_TYPE( DeclStatement, Statement );

DeclStatement::DeclStatement( Declaration* decl )
	: m_decl( decl )
{
	m_decl->Grab();
	m_decl->uiSetParent( this );
}

DeclStatement::~DeclStatement() {
	m_decl->Drop();
}

const char * DeclStatement::ToChars() const {
		return "Declaration Statement";
}

void DeclStatement::ToString( String & s ) {
	m_decl->ToString( s );
}

#pragma region "UI"
const char * DeclStatement::uiToString() const {
	return "Declaration Statement";
}

int DeclStatement::uiNumChildren() const {
	return 1;
}

UINode * DeclStatement::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_decl;
	}
	return 0;
}

int DeclStatement::uiIndexOf( const UINode *child ) const {
	if ( child = m_decl ) {
		return 0;
	}
	return -1;
}
#pragma endregion

void DeclStatement::Resolve( Semantics* semantics ) {
	m_decl->Resolve( semantics );
}

#pragma endregion

#pragma region "Block Statement"

DEFINE_TYPE( Block, Statement );

Block::Block( StatementList * statements )
	: m_stmtList( statements )
{
	m_stmtList->Grab();
	m_stmtList->uiSetParent( this );
}

Block::~Block() {
	m_stmtList->Drop();
}

StatementList * Block::GetStatements() {
	return m_stmtList;
}

const char* Block::ToChars() const {
	return "{ Block Statement }";
}

void Block::ToString( String & s ) {
}

#pragma region "UI"
int Block::uiNumChildren() const {
	return 1;
}

UINode * Block::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_stmtList;
	}
	return 0;
}

int Block::uiIndexOf( const UINode *child ) const {
	if ( child == m_stmtList ) {
		return 0;
	}
	return -1;
}
#pragma endregion

void Block::Resolve( Semantics* semantics ) {
	// get the current scope
	Scope * prevScope = semantics->GetContext();//Semantics::GetContext();

	// create a new scope for this block
	Scope * newScope = new Scope( prevScope );
	semantics->SetContext( newScope );

	// resolve all the statements within this block
	m_stmtList->Resolve( semantics );

	// restore the previous scope
	semantics->SetContext( prevScope );
}

#pragma endregion

#pragma region "Auto Declaration"

DEFINE_TYPE( AutoDeclStatement, Statement );

AutoDeclStatement::AutoDeclStatement( Identifier * id, Expression* initializer )
	: m_variable( id )
	, m_initializer( initializer )
{
	m_variable->Grab();
	m_variable->uiSetParent( this );

	if ( m_initializer ) {
		m_initializer->Grab();
		m_initializer->uiSetParent( this );
	}
}

AutoDeclStatement::~AutoDeclStatement() {
	m_variable->Drop();
	DROP( m_initializer );
}

Identifier * AutoDeclStatement::GetVariable() {
	return m_variable;
}

Expression * AutoDeclStatement::GetInitializer() {
	return m_initializer;
}

const char * AutoDeclStatement::ToChars() const {
	return "Auto Declaration";
}

void AutoDeclStatement::ToString( String & s ) {
}

#pragma region "UI"
int AutoDeclStatement::uiNumChildren() const {
	return 2;
}

UINode * AutoDeclStatement::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_variable;
	}
	if ( 1 == index ) {
		return m_initializer;
	}
	return 0;
}

int AutoDeclStatement::uiIndexOf( const UINode *child ) const {
	if ( child == m_variable ) {
		return 0;
	}
	if ( child == m_initializer ) {
		return 1;
	}
	return -1;
}
#pragma endregion

void AutoDeclStatement::Resolve( Semantics* semantics ) {
}

#pragma endregion

#pragma region "If Statement"

DEFINE_TYPE( IfStatement, Statement );

IfStatement::IfStatement( Expression* condition, Statement* thenStmt, Statement* elseStmt /* =0 */ )
	: m_condition( condition )
	, m_thenStmt( thenStmt )
	, m_elseStmt( elseStmt )
	, m_declarations( 0 )
{
	m_condition->Grab();
	m_condition->uiSetParent( this );

	m_thenStmt->Grab();
	m_thenStmt->uiSetParent( this );

	if ( m_elseStmt ) {
		m_elseStmt->Grab();
		m_elseStmt->uiSetParent( this );
	}
}

IfStatement::~IfStatement() {
	DROP( m_condition );
	DROP( m_thenStmt );
	DROP( m_elseStmt );
}

Expression * IfStatement::GetCondition() {
	return m_condition;
}

Statement * IfStatement::GetThenStatement() {
	return m_thenStmt;
}

Statement * IfStatement::GetElseStatement() {
	return m_elseStmt;
}
const char * IfStatement::ToChars() const {
	return "If Statement";
}

void IfStatement::ToString( String & s ) {
}

#pragma region "UI"
int IfStatement::uiNumChildren() const {
	return m_elseStmt ? 3 : 2;
}

UINode * IfStatement::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_condition;
	}
	if ( 1 == index ) {
		return m_thenStmt;
	}
	if ( m_elseStmt && 2 == index ) {
		return m_elseStmt;
	}
	return 0;
}

int IfStatement::uiIndexOf( const UINode *child ) const {
	if ( child == m_condition ) {
		return 0;
	}
	if ( child == m_thenStmt ) {
		return 1;
	}
	if ( child == m_elseStmt ) {
		return 2;
	}
	return -1;
}
#pragma endregion

void IfStatement::Resolve( Semantics* semantics ) 
{
	c4::Resolve< Expression >( & m_condition, semantics );
/*	if ( ! m_condition->GetType()->IsBoolean() )
	{
		Type_Bool * temp = new Type_Bool();

		CastExp * castExp = new CastExp( temp, m_condition );
		m_condition->Drop();
		m_condition = castExp;
		m_condition->Grab();
	}
*/
	m_thenStmt->Resolve( semantics );
	
	if ( m_elseStmt ) {
		m_elseStmt->Resolve( semantics );
	}
}

#pragma endregion

#pragma region "Loop Statement"

DEFINE_TYPE( LoopStatement, Statement );

LoopStatement::LoopStatement( Expression* condition, Statement* body )
	: m_condition( condition )
	, m_body( body )
{
	if ( m_condition ) {
		m_condition->Grab();
		m_condition->uiSetParent( this );
	}

	if ( m_body ) {
		m_body->Grab();
		m_body->uiSetParent( this );
	}
}

LoopStatement::~LoopStatement() {
	DROP( m_condition );
	DROP( m_body );
}

Expression * LoopStatement::GetCondition() {
	return m_condition;
}

Statement * LoopStatement::GetBody() {
	return m_body;
}

const char * LoopStatement::ToChars() const {
	return "Loop Statement";
}

void LoopStatement::ToString( String & s ) {
}

#pragma region "UI"
int LoopStatement::uiNumChildren() const {
	return (int)m_condition + (int)m_body;
}

UINode * LoopStatement::uiChildAt( int index ) {
	if ( 0 == index && m_condition ) {
		return m_condition;
	}
	if ( 1 == index && m_body ) {
		return m_body;
	}
	return 0;
}

int LoopStatement::uiIndexOf( const UINode *child ) const {
	if ( !child ) {
		return -1;
	}
	if ( child == m_condition ) {
		return 0;
	}
	if ( child == m_body ) {
		return 1;
	}
	return -1;
}
#pragma endregion

void LoopStatement::Resolve( Semantics* semantics ) {
}

#pragma endregion

#pragma region "While Statement"

DEFINE_TYPE( WhileStatement, LoopStatement );

WhileStatement::WhileStatement( Expression* condition, Statement* body )
	: LoopStatement( condition, body )
{
}

WhileStatement::~WhileStatement() {
}

const char * WhileStatement::ToChars() const {
	return "While Statement ( condition + body )";
}

void WhileStatement::ToString( String & s ) {
}

void WhileStatement::Resolve( Semantics* semantics ) {
	c4::Resolve< Expression >( & m_condition, semantics );
	m_body->Resolve( semantics );
}

#pragma endregion 

#pragma region "Do Statement"

DEFINE_TYPE( DoStatement, LoopStatement );

DoStatement::DoStatement( Statement* body, Expression* condition )
	: LoopStatement( condition, body )
{
}

DoStatement::~DoStatement() {
}

const char * DoStatement::ToChars() const {
	return "Do Statement ( body + condition )";
}

void DoStatement::ToString( String & s ) {
}

void DoStatement::Resolve( Semantics* semantics ) {
	m_body->Resolve( semantics );
	c4::Resolve< Expression >( & m_condition, semantics );
}

#pragma endregion

#pragma region "With Statement"

DEFINE_TYPE( WithStatement, Statement );

WithStatement::WithStatement( Expression* expr, Statement* body )
	: m_expression( expr )
	, m_body( body )
{
	m_expression->Grab();
	m_expression->uiSetParent( this );

	m_body->Grab();
	m_body->uiSetParent( this );
}

WithStatement::~WithStatement() {
	DROP( m_expression );
	DROP( m_body );
}

Expression * WithStatement::GetExpression() {
	return m_expression;
}

Statement * WithStatement::GetBody() {
	return m_body;
}

const char * WithStatement::ToChars() const {
	return "With Statement ( expression + body )";
}

void WithStatement::ToString( String & s ) {
}

#pragma region "UI"
int WithStatement::uiNumChildren() const {
	return 2;
}

UINode * WithStatement::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_expression;
	}
	if ( 1 == index ) {
		return m_body;
	}
	return 0;
}

int WithStatement::uiIndexOf( const UINode *child ) const {
	if ( child == m_expression ) {
		return 0;
	}
	if ( child == m_body ) {
		return 1;
	}
	return -1;
}
#pragma endregion

void WithStatement::Resolve( Semantics* semantics ) {
	c4::Resolve< Expression >( & m_expression, semantics );
	m_body->Resolve( semantics );
}

#pragma endregion

#pragma region "Return Statement"

DEFINE_TYPE( ReturnStatement, Statement );

ReturnStatement::ReturnStatement( Expression* returnExp /* =0 */ )
	: m_expression( returnExp )
{
	if ( m_expression ) {
		m_expression->Grab();
		m_expression->uiSetParent( this );
	}
}

ReturnStatement::~ReturnStatement() {
	DROP( m_expression );
}

Expression * ReturnStatement::GetExpression() {
	return m_expression;
}

const char* ReturnStatement::ToChars() const {
	return "Return Statement";
}

void ReturnStatement::ToString( String & s ) {
	s += "return";
	if ( m_expression ) {
		s += ' ';
		m_expression->ToString( s );
	}
	s += ';';
}

#pragma region "UI"
int ReturnStatement::uiNumChildren() const {
	return m_expression ? 1 : 0;
}

UINode * ReturnStatement::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_expression;
	}
	return 0;
}

int ReturnStatement::uiIndexOf( const UINode *child ) const {
	if ( child == m_expression ) {
		return 0;
	}
	return -1;
}
#pragma endregion

void ReturnStatement::Resolve( Semantics* semantics ) {
	if ( m_expression ) {
		c4::Resolve< Expression >( & m_expression, semantics );
		if ( m_expression->GetType()->IsFloating() )
		{
			Type_Int32 * temp = new Type_Int32();

			CastExp * castExp = new CastExp( temp, m_expression );
			m_expression->Drop();
			m_expression = castExp;
			m_expression->Grab();
		}
	}
}

#pragma endregion

#pragma region "For Statement"

DEFINE_TYPE( ForLoopStatement, LoopStatement );

ForLoopStatement::ForLoopStatement( 
	  Statement *		init,
	  Expression *		condition,
	  ExpressionList *	update,
	  Statement *		body  
)
	: LoopStatement( condition, body )
	, m_initStmt( init )
	, m_updateExp( update )
{
	if ( m_initStmt ) {
		m_initStmt->Grab();
		m_initStmt->uiSetParent( this );
	}
	if ( m_updateExp ) {
		m_updateExp->Grab();
		m_updateExp->uiSetParent( this );
	}
}

ForLoopStatement::~ForLoopStatement() {
	DROP( m_initStmt );
	DROP( m_updateExp );
}

Statement * ForLoopStatement::GetInitStatement() {
	return m_initStmt;
}

ExpressionList * ForLoopStatement::GetUpdate() {
	return m_updateExp;
}

const char* ForLoopStatement::ToChars() const {
	return "For Statement";
}

void ForLoopStatement::ToString( String & s ) {
}

#pragma region "UI"
int ForLoopStatement::uiNumChildren() const {
	return ( (int)m_initStmt + (int)m_updateExp + LoopStatement::uiNumChildren() );
}

UINode * ForLoopStatement::uiChildAt( int index ) {
	switch ( index ) {
		case 0:
			return m_initStmt;

		case 1:
			return m_condition;

		case 2:
			return m_updateExp;

		case 3:
			return m_body;
	}
	return 0;
}

int ForLoopStatement::uiIndexOf( const UINode *child ) const {
	if ( !child ) {
		return -1;
	}
	if ( child == m_initStmt ) {
		return 0;
	}
	if ( child == m_condition ) {
		return 1;
	}
	if ( child == m_updateExp ) {
		return 2;
	}
	if ( child == m_body ) {
		return 3;
	}
	return -1;
}
#pragma endregion

void ForLoopStatement::Resolve( Semantics* semantics ) {
}

#pragma endregion

#pragma region "Case Statement"

DEFINE_TYPE( CaseStatement, Statement );

CaseStatement::CaseStatement( ExpressionList * expressions )
	: m_expList( expressions )
{
	m_expList->Grab();
	m_expList->uiSetParent( this );
}

CaseStatement::~CaseStatement() {
	m_expList->Drop();
}

ExpressionList * CaseStatement::GetExpressions() {
	return m_expList;
}

override const char* CaseStatement::ToChars() const {
	return "Case Statement";
}

void CaseStatement::ToString( String & s ) {
}

#pragma region "UI"
int CaseStatement::uiNumChildren() const {
	return 1;
}

UINode * CaseStatement::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_expList;
	}
	return 0;
}

int CaseStatement::uiIndexOf( const UINode *child ) const {
	if ( child == m_expList ) {
		return 0;
	}
	return -1;
}
#pragma endregion

void CaseStatement::Resolve( Semantics* semantics ) {
}

#pragma endregion

#pragma region "Default Statement"

DEFINE_TYPE( DefaultStatement, Statement );

DefaultStatement::DefaultStatement( Statement * stmt )
	: m_stmt( stmt )
{
	m_stmt->Grab();
	m_stmt->uiSetParent( this );
}

DefaultStatement::~DefaultStatement() {
	m_stmt->Drop();
}

Statement * DefaultStatement::GetStatement() {
	return m_stmt;
}

const char* DefaultStatement::ToChars() const {
	return "Default Statement";
}

void DefaultStatement::ToString( String & s ) {
}

#pragma region "UI"
int DefaultStatement::uiNumChildren() const {
	return 1;
}

UINode * DefaultStatement::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_stmt;
	}
	return 0;
}

int DefaultStatement::uiIndexOf( const UINode *child ) const {
	if ( child == m_stmt ) {
		return 0;
	}
	return -1;
}
#pragma endregion

void DefaultStatement::Resolve( Semantics* semantics ) {
}

#pragma endregion

#pragma region "ContinueStatement"

DEFINE_TYPE( ContinueStatement, Statement );

ContinueStatement::ContinueStatement( Identifier *label /* =0 */ )
	: m_label( label )
{
	if ( m_label ) {
		m_label->Grab();
		m_label->uiSetParent( this );
	}
}

ContinueStatement::~ContinueStatement() {
	DROP( m_label );
}

Identifier * ContinueStatement::GetLabel() {
	return m_label;
}

const char* ContinueStatement::ToChars() const {
	return "Continue Statement";
}

void ContinueStatement::ToString( String & s ) {
}

#pragma region "UI"
int ContinueStatement::uiNumChildren() const {
	return 1;
}

UINode * ContinueStatement::uiChildAt( int index ) {
	if ( 0 == index ) {
		return m_label;
	}
	return 0;
}

int ContinueStatement::uiIndexOf( const UINode *child ) const {
	if ( child == m_label ) {
		return 0;
	}
	return -1;
}
#pragma endregion

void ContinueStatement::Resolve( Semantics* semantics ) {
}

#pragma endregion

#pragma region "Break Statement"

DEFINE_TYPE( BreakStatement, Statement );

BreakStatement::BreakStatement( Identifier *label /* =0 */ )
	: ContinueStatement( label )
{
}

BreakStatement::~BreakStatement() {
}

const char* BreakStatement::ToChars() const {
	return "Break Statement";
}

void BreakStatement::ToString( String & s ) {
}

void BreakStatement::Resolve( Semantics* semantics ) {
}

#pragma endregion

}//end namespace c4