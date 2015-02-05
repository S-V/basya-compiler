/*
==================================
	Intermediate representation
	code generation
==================================
*/
#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

#include "IR.h"

namespace c4 {

/*
================
	class IRCode
================
*/
IRCode::IRCode( ICode_t  opcode )
	: op1( NULL )
	, op2( NULL )
	, opcode( opcode )
{
	Init();
}

IRCode::IRCode( int i )
	: op1( NULL )
	, op2( NULL )
	, text( i )
{
	switch ( i ) {
		case 0:  opcode = iconst_0;  break;
		case 1:  opcode = iconst_1;  break;
		case 2:  opcode = iconst_2;  break;
		case 4:  opcode = iconst_4;  break;
		case 8:  opcode = iconst_8;  break;
		default:  opcode = iconst;  break;
	}

	Init();

	// TODO: an integer may occupy less than 4 bytes
	m_iSize = 4;
	m_int_value = i;
}

IRCode::IRCode( float f )
	: op1( NULL )
	, op2( NULL )
	, text( f )
	, opcode( fconst )
{
	Init();
	m_iSize = 4;
	m_double_value = f;
}
/*
IRCode::IRCode( const String & s )
	: op1( NULL )
	, op2( NULL )
	, text( s )
	, opcode( string_const )
{
	Init();
	m_iSize = s.Length() + 1;

	debugName = s;
}
*/
IRCode::IRCode( ICode_t  opcode, IRCode* op1, IRCode* op2 )
	: opcode( opcode )
	, op1( op1 ), op2( op2 )
{
	Init();

	switch ( opcode ) {
		case address_8 :
			m_iSize = 1;
			break;

		case address_16 :
			m_iSize = 2;
			break;

		case address_32 :
		case address_f32:
			m_iSize = 4;
			break;

		case address_64:
		case address_f64:
			m_iSize = 8;
			break;

		default: /* nothing */ ;
	}
}

const char* IRCode::Name() const {
	return text.c_str();
}

bool IRCode::IsLeaf() const {
	return !op1;// && !op2;
}

int IRCode::GetNumOperands() const {
	return int(op1 != NULL) + int(op2 != NULL );
}

void IRCode::Init() {
	m_pLabel = NULL;

	m_int_value = 0;
	m_double_value = 0;

	state = NULL;

	complexity = 0;

	m_iSize = 0;

	m_clobberedRegs = 0;

	m_bUnsigned = false;
}

/*
=========================
	statics
=========================
*/

const IRCode::ircode_data_s	IRCode::_irdata[ NUM_OPCODES ] = {
	// name, arity, cost
	{ "nop",		0,	0 },
	
	// Primitive ( leaf ) codes
	{ "iconst",		0,	0 },
	{ "iconst_0",	0,	0 },
	{ "iconst_1",	0,	0 },
	{ "iconst_2",	0,	0 },
	{ "iconst_4",	0,	0 },
	{ "iconst_8",	0,	0 },

	{ "fconst",		0,	0 },	

	{ "address_8",	0,	4 },
	{ "address_16",	0,	2 },
	{ "address_32",	0,	1 },
	{ "address_64",	0,	1 },

	{ "address_f32",	0,	4 },
	{ "address_f64",	0,	4 },

	{ "indir_i8",	1,	4 },
	{ "indir_u8",	1,	4 },
	{ "indir_i16",	1,	2 },
	{ "indir_u16",	1,	2 },
	{ "indir_i32",	1,	1 },
	{ "indir_i64",	1,	1 },
	{ "indir_f32",	1,	3 },
	{ "indir_f64",	1,	3 },

	{ "index",	2,	3 },

	{ "assign",	2,	2 },

	// Arithmetic instructions
	//
	{ "add",	2,	2 },
	{ "sub",	2,	2 },
	{ "mul",	2,	4 },

	{ "div",	2,	7 },
	{ "udiv",	2,	8 },
	{ "fdiv",	2,	8 },

	
	{ "rem",	2,	8 },
	{ "urem",	2,	8 },
	
	{ "neg",	1,	1 },
	{ "fneg",	1,	3 },

	{ "preinc",	1,	3 },
	{ "postinc",	1,	3 },
	{ "predec",	1,	3 },
	{ "postdec",	1,	3 },

	{ "and",	2,	2 },
	{ "or",	2,	2 },
	{ "xor",	2,	2 },
	{ "not",	1,	2 },
	{ "compl",	1,	2 },

	{ "shl",	2,	2 },
	{ "shr",	2,	2 },
	{ "ushr",	2,	2 },

	// Logical operators
	//
	{ "eq",	2,	2 },
	{ "neq",	2,	2 },
	{ "lt",	2,	2 },
	{ "gt",	2,	2 },
	{ "le",	2,	2 },
	{ "ge",	2,	2 },

	// Primitive type conversions
	//
	{ "f2i",	1,	0 },
	{ "i2f",	1,	0 },
	{ "i2b",	1,	0 },
	{ "f2b",	1,	0 },

	// Control flow instructions
	//
	{ "jmp",	1,	0 },

	// op1 -> exp, op2 -> label
	{ "br_true",	2,	0 },
	{ "br_false",	2,	0 },

	{ "je",	1,	0 },
	{ "jne",	1,	0 },
	{ "jge",	1,	0 },
	{ "jg",	1,	0 },
	{ "jle",	1,	0 },
	{ "jl",	1,	0 },
	

	{ "label",	0,	0 },

	{ "loop_start",	0,	0 },
	{ "loop_end",	0,	0 },

	// IR code of 'func_start' points to the end of the function
	{ "func_start",	1,	0 },

	{ "func_end",	0,	0 },

	// points to address
	{ "call",	1,	777 },

	// function argument points to expression
	{ "arg",	1,	0 },
	
	{ "ret",	1,	0 },

	{ "data",	0,	1 },

	{ "ERROR",	0,	9999 },
};

int IRCode::GetCost( ICode_t  opcode ) {
	return _irdata[ opcode ].cost;
}

const char * IRCode::GetOpcodeName( ICode_t  opcode ) {
	return _irdata[ opcode ].name;
}

void IRCode::DumpTree( IRCode * tree ) {
	if ( ! tree ) {
		return;
	}
	Assert( tree->GetOpCode() != OPCODE_ERROR );
	c4::common->Print( "%s(", GetOpcodeName( tree->GetOpCode() ) );

	switch ( GetOperatorArity( tree->GetOpCode() ) )
	{
	case 0:
		c4::common->Print( "%s", tree->Name() );	
		break;

	case 1:
		DumpTree( tree->GetOp1() );
		break;

	case 2:
		DumpTree( tree->GetOp1() );
		c4::common->Print( ", " );
		DumpTree( tree->GetOp2() );
		break;

	default:	Unreachable();
	}
	c4::common->Print( ")" );
}

bool IRCode::IsBranchInsn( ICode_t  opcode )
{
	return opcode == jmp
		|| opcode == br_true || opcode == br_false
		|| opcode == je || opcode == jne
		|| opcode == jge || opcode == jg
		|| opcode == jle || opcode == jl;
}

bool IRCode::IsCommutative( ICode_t  opcode ) {
	return opcode == add || opcode == mul
		|| opcode == and || opcode == or || opcode == xor
		|| opcode == eq || opcode == neq;
}

int IRCode::GetOperatorArity( ICode_t  opcode ) {
	return _irdata[ opcode ].arity;
}

/*
=========================
	IRGenerator::IRGenerator()
=========================
*/

IRGenerator::IRGenerator()
	: m_tail( NULL )
	, m_bOnRHSofAssignment( false )
{
	InitConversionTable();

	//----------------
	//m_iNumLabels = 0;
}

IRGenerator::~IRGenerator() {
	Cleanup();
}

IRList_t & IRGenerator::Generate( CompilationUnit * program ) {
	Visit( program );

	// optional pass
	IRList_t::Iterator  iter( m_insnList.Begin() );
	while ( iter != m_insnList.End() )
	{
		Canonicalize( *iter );
		++iter;
	}

	//***
	InsertLabels();

	return m_insnList;
}

void IRGenerator::Cleanup() {
	m_insnList.Clear();
	m_tail = NULL;
}

void IRGenerator::DumpInsnList() const {
	IRList_t::Iterator   iter( m_insnList.Begin() );
	while ( iter != m_insnList.End() )
	{
		IRCode * pInsn = *iter;
		IRCode::DumpTree( pInsn );
		c4::common->Print( "\n" );
		++iter;
	}
}
// canonicalize ( put the most complex expression first )
void IRGenerator::Canonicalize( IRCode * node ) {
	if ( node != NULL )
	{
		Canonicalize( node->GetOp1() );
		Canonicalize( node->GetOp2() );

		if( node->GetNumOperands() == 2
			&& node->IsCommutative() )
		{
			if ( node->GetOp2()->complexity > node->GetOp1()->complexity )
			{
				c4::common->Print( "swapping '%s' and '%s'\n", node->op1->Name(), node->op2->Name() );
				idSwap< IRCode* >( node->op2, node->op1 );
			}
			// place div, rem, mul before add, sub
			//if ( node->op1->theMostExpensiveOp < node->op2->theMostExpensiveOp )
			//{
			//	idSwap< IRCode* >( node->op2, node->op1 );
			//}
		}
	}//if ( node != NULL )
}


//---------------------------------------------------------------------------

void IRGenerator::Visit( CompilationUnit * compUnit ) {
	idList< Declaration* > & decls = compUnit->GetActualList();

	for ( int i = 0; i < decls.Num(); i++ )	{
		decls[ i ]->Accept( this );
	}
}

void IRGenerator::Visit( Declaration * decl ) {
	Unreachable();
}

void IRGenerator::Visit( DeclarationList * declList )
{ 
	idList< Declaration* > & decls = declList->GetActualList();

	for ( int i = 0; i < decls.Num(); i++ )	{
		decls[ i ]->Accept( this );
	}
}

void IRGenerator::Visit( VarDecl * decl ) {
	Expression * initExp = decl->GetInitializer();
	if ( initExp )
	{
		// transform initialization into a temporary assignment
		// example: int a = 7 -> int a; a = 7;
		VarSymbol * varSymbol = decl->GetSymbol();	
		VarExp * varExp = new VarExp( varSymbol );

		AssignmentExp * tempAssignment = new AssignmentExp( varExp, initExp );
/*
		//TODO:
		Expression * e = tempAssignment->ResolveInto( NULL );
		Visit( e );
		e->ToString( m_tail->comment );		
		delete e;
*/
		GenerateExpression( tempAssignment );
		tempAssignment->ToString( m_tail->comment );		
	}
}

void IRGenerator::Visit( Argument * ) { }
void IRGenerator::Visit( ArgumentList * ) { }

void IRGenerator::Visit( FunctionDecl * functionDecl )
{ 
	IRCode * funcEnd = new IRCode( func_end, NULL, NULL );
	funcEnd->SetText( functionDecl->GetName()->Text() );

		// the first child of IRCode for 'func_start' points to 'func_end'
	IRCode * funcStart = new IRCode( func_start, funcEnd, NULL );
	funcStart->SetText( functionDecl->GetName()->Text() );
	funcStart->SetIntegerValue( functionDecl->GetSymbol()->GetSizeOfLocals() );

	m_insnList.Append( funcStart );

	//*****
	m_funcStack.Push( funcStart );


	m_iNestingLevel = 0;
	GenerateStatement( functionDecl->GetBody() );


	m_insnList.Append( funcEnd );
	m_tail = funcEnd;

	//*****
	m_funcStack.Pop();
}

void IRGenerator::Visit( ClassDecl * ) { }
void IRGenerator::Visit( StructDecl * ) { }
void IRGenerator::Visit( EnumDecl * ) { }
void IRGenerator::Visit( NamespaceDecl * ) { }

void IRGenerator::Visit( Statement * stmt ) {
	c4::common->Print( "--- generating statement: %s.", stmt->ToChars() );
}

void IRGenerator::Visit( EmptyStatement * ) { }
void IRGenerator::Visit( ErrorStatement * ) { }

void IRGenerator::Visit( StatementList * stmtList ) 
{ 
	LinkedList< Statement* >::Iterator  iter = stmtList->GetActualList().Begin();
	
	while ( iter != stmtList->GetActualList().End() )
	{
		Statement * stmt = *iter;
		
		GenerateStatement( stmt );

		++iter;

		if ( m_tail && (m_tail->GetOpCode() == ICode_t::ret)
			// and if there are statements after 'return'
//			&& (iter != stmtList->GetActualList().End()) 
//			&& (m_iNestingLevel > 0) )
)		{
			// if not at the end of the curr. function then
			// generate unconditional 'jmp' to the current function's prologue
			IRCode * pCurrFunc = GetStartOfCurrentFunction();
			IRCode * pFuncEnd = pCurrFunc->GetOp1();

			IRCode * labelRet = NULL;
			
			if ( pFuncEnd->m_pLabel ) {
				labelRet = pFuncEnd->m_pLabel;
			} else {
				labelRet = DefineLabel();
				pFuncEnd->m_pLabel = labelRet;
			}

			Emit( ICode_t::jmp, labelRet );
		}
	}
}

void IRGenerator::Visit( ExpressionStatement * expStmt ) {
	expStmt->GetExpression()->Accept( this );
	m_insnList.Append( m_tail );
}

void IRGenerator::Visit( LabeledStatement * ) { }

void IRGenerator::Visit( DeclStatement * declStmt ) {
	declStmt->GetDeclaration()->Accept( this );
}

void IRGenerator::Visit( Block * block ) {
	++m_iNestingLevel;
	
	StatementList * stmtList = block->GetStatements();
	Visit( stmtList );
	
	--m_iNestingLevel;
}

void IRGenerator::Visit( AutoDeclStatement * ) { }

void IRGenerator::Visit( IfStatement * ifStmt ) {
/*
===========================================
  generic if-then code:
 
 if:
	<condition ot evaluate>
	jxx else	; jump ot the else part if the condition is false
	<code to implement the 'then' statements>
jmp end			; jump to the end
else:
	<code to implement the 'else' statements>
end:


if ( TestExp ) ThenStmt
		[TestExp]
		brfalse l1;
		[ThenStmt]
	l1:

if ( TestExp ) ThenStmt else ElseStmt
		[TestExp]
		brfalse l1;
		[ThenStmt]
		jmp l2;
	l1:
		[ElseStmt]
	l2:
===========================================
*/
#pragma region Comment
	IRCode * pComment = new IRCode( ICode_t::nop );
	pComment->comment = "if ";
	ifStmt->GetCondition()->ToString( pComment->comment );
	m_insnList.Append( pComment );
#pragma endregion

	IRCode * testExp = GenerateExpression( ifStmt->GetCondition() );
	//m_insnList.Append( testExp );
	
	IRCode * l1 = DefineLabel();
	
	EmitBranchIfFalse( testExp, l1 );
	
	GenerateStatement( ifStmt->GetThenStatement() );

	// case when we do have an 'else'
	if ( Statement * elseStmt = ifStmt->GetElseStatement() )
	{
		IRCode * l2 = DefineLabel();
		Emit( ICode_t::jmp, l2 );
		MarkLabel( l1 );

		GenerateStatement( elseStmt );
		MarkLabel( l2 );
	}
	else // case for no 'else'
	{
		MarkLabel( l1 );
	}
}

void IRGenerator::Visit( LoopStatement * ) {
	Unreachable();
}

void IRGenerator::Visit( WhileStatement * whileStmt ) { 
/*
===========================================
while ( TestExp )  BodyStmt;

	lContinue:
		[TestExp]
		brfalse lBreak;
		[BodyStmt]
		goto lContinue;
	lBreak:
===========================================
*/
	IRCode * lContinue = DefineLabel();
	IRCode * lBreak = DefineLabel();

	MarkLabel( lContinue );
	
	IRCode * testExp = GenerateExpression( whileStmt->GetCondition() );


#pragma region Comment
	IRCode * pComment = new IRCode( ICode_t::nop );
	pComment->comment = "while ";
	whileStmt->GetCondition()->ToString( pComment->comment );
	m_insnList.Append( pComment );
#pragma endregion

	EmitBranchIfFalse( testExp, lBreak );

	//LoopFrame f = new LoopFrame( lBreak, lContinue )
	//PushLoopFrame( f )

	GenerateStatement( whileStmt->GetBody() );
	Emit( jmp, lContinue );

	//PopLoopFrame( f );

	MarkLabel( lBreak );
}

void IRGenerator::Visit( DoStatement * doStmt ) { 
/*
===========================================
do  BodyStmt  while( TestExp );

	l1:
		[BodyStmt]
	lContinue:
		[TestExp]
		brtrue l1;
	lBreak:
===========================================
*/
	IRCode * l1 = DefineLabel();
	IRCode * lContinue = DefineLabel();
	IRCode * lBreak = DefineLabel();

	//LoopFrame f = new LoopFrame( lBreak, lContinue )
	//PushLoopFrame( f )

	MarkLabel( l1 );
	//comment
	m_tail->comment = "do";

	GenerateStatement( doStmt->GetBody() );	

	//PopLoopFrame( f );

	MarkLabel( lContinue );

	IRCode * testExp = GenerateExpression( doStmt->GetCondition() );
	//comment
	testExp->comment = "while ";
	doStmt->GetCondition()->ToString( testExp->comment );

	EmitBranchIfTrue( testExp, l1 );

	MarkLabel( lBreak );
}

void IRGenerator::Visit( WithStatement * ) { }

void IRGenerator::Visit( ReturnStatement * returnStmt )
{
	if ( Expression * e = returnStmt->GetExpression() )
	{
		GenerateExpression( e );
	}
	else
	{
		//Unimplemented();
		m_tail = new IRCode( ICode_t::nop );
	}

	IRCode * retInsn = new IRCode( ICode_t::ret, m_tail, NULL );
	retInsn->SetText( "return" );
	m_tail = retInsn;
	m_insnList.Append( m_tail );
}

void IRGenerator::Visit( ForLoopStatement * forStmt ) { 
/*
===========================================
for:
	<condition to evaluate for loop counter variable>
	jxx forcode		; jump to the code if the condition is true
	jmp end			; jump to the end if the condition is false
forcode:
	< for loop code to execute >
	<increment for loop counter>
	jmp for		; go back to the start of the For statement
end;


for ( InitExp; TestExp ; NextExp )
		[InitExp]
	l1:
		[TestExp]
		brfalse lBreak;
		[BodyStmt]
	lContinue:
		[NextExp]
		jmp l1;
	lBreak:
===========================================
*/
	IRCode * l1 = DefineLabel();
	IRCode * lContinue = DefineLabel();
	IRCode * lBreak = DefineLabel();

	//LoopFrame f = new LoopFrame( lBreak, lContinue )
	//PushLoopFrame( f )

	
	
	GenerateStatement( forStmt->GetInitStatement() );
	
	//comment
	//m_tail->comment = "for ( ";
	
	
	MarkLabel( l1 );
	
	IRCode * testExp = GenerateExpression( forStmt->GetCondition() );
	m_insnList.Append( testExp );

	EmitBranchIfFalse( testExp, lBreak );
	
	GenerateStatement( forStmt->GetBody() );	
	
	MarkLabel( lContinue );


	IRCode * nextExp = GenerateExpression( forStmt->GetUpdate() );
	m_insnList.Append( nextExp );

	Emit( jmp, l1 );

	MarkLabel( lBreak );

	//PopLoopFrame( f );	
}

void IRGenerator::Visit( CaseStatement * ) { }
void IRGenerator::Visit( DefaultStatement * ) { }
void IRGenerator::Visit( ContinueStatement * ) { }
void IRGenerator::Visit( BreakStatement * ) { }

/*
================================================
	Expressions
================================================
*/
void IRGenerator::Visit( Expression * expr ) {
	Unreachable();
}

void IRGenerator::Visit( ExpressionList * expList ) {
	Unimplemented();
}

void IRGenerator::Visit( BooleanConstant * b ) {}

void IRGenerator::Visit( IntegerConstant * i ) {
	IRCode * insn = new IRCode( (int)i->Value() );
	m_tail = insn;
}

void IRGenerator::Visit( FloatingConstant * f ) {
	EmitData( (float)f->Value() );
}

void IRGenerator::Visit( CharConstant * c ) {
	IRCode * insn = new IRCode( (int)c->Value() ); // ??? is this right ???
	m_tail = insn;
}

void IRGenerator::Visit( StringLiteral * s ) {
	EmitData( s->Value().c_str() );
}

void IRGenerator::Visit( IdExp * e ) {
	c4::common->Error( "IRGenerator::Visit( IdExp* ) : unresolved identifier '%s'!", e->GetIdentifier()->Text() );
}

void IRGenerator::Visit( VarExp * varExp )
{
	Type * type = varExp->GetType();
	Assert( type->IsPrimitive() && "complex types are not implemented" );

	ICode_t   addrType;
	switch ( type->GetTypeKind() )
	{
	case BOOL:
		addrType = address_8;
		break;

	case CHAR :
	case TYPE_INT8 :
		addrType = address_8;
		break;

	case TYPE_INT16 :
		addrType = address_16;
		break;

	case TYPE_INT32 :
		addrType = address_32;
		break;

	case TYPE_FLOAT32 :
		addrType = address_f32;
		break;

	case TYPE_FLOAT64 :
		addrType = address_f64;
		break;

	default:
		Unreachable();
	}

	IRCode * insnVar = new IRCode( addrType, NULL, NULL );

	int  stackOffset = varExp->GetSymbol()->StackOffset();

	if ( ::c4::compiler->IsOptionSet( USE_SYMBOL_NAMES ) ) {
		insnVar->SetText( varExp->GetSymbol()->Name() );
	} else {
		insnVar->SetText( "ebp - " + String( stackOffset ) );
	}

	insnVar->SetIntegerValue( stackOffset );

	
	if ( m_bOnRHSofAssignment )
	{
		ICode_t  indirectionType;

		switch ( type->GetTypeKind() )
		{
		case BOOL :
			indirectionType = indir_u8;
			break;

		case CHAR :
		case TYPE_INT8:
			indirectionType = type->IsSigned() ? indir_i8 : indir_u8;
			break;

		case WCHAR:
		case TYPE_INT16:
			indirectionType = type->IsSigned() ? indir_i16 : indir_u16;
			break;

		case TYPE_INT32:
			indirectionType = indir_i32;
			break;

		case TYPE_INT64:
			indirectionType = indir_i64;
			break;

		case TYPE_FLOAT32:
			indirectionType = indir_f32;
			break;

		case TYPE_FLOAT64:
			indirectionType = indir_f64;
			break;

		default:
			Unreachable();
		}

		IRCode * insn = new IRCode( indirectionType, insnVar, NULL );

		if ( ::c4::compiler->IsOptionSet( USE_SYMBOL_NAMES ) ) {
			insn->SetText( varExp->GetSymbol()->Name() );
		} else {
			insn->SetText( "ebp - " + String( stackOffset ) );
		}

		insn->complexity = 1;
		//insn->theMostExpensiveOp = address_32;

		m_tail = insn;
	} 
	else // if we're on lhs of assignment
	{
		m_tail = insnVar;
	}
}

void IRGenerator::Visit( Identifier * e ) {
	Unreachable();
}

void IRGenerator::Visit( UnaryExpression * e ) { 
	Unreachable();
}

void IRGenerator::Visit( BinaryExpression * e ) { 
	Unreachable();
}

void IRGenerator::Visit( AssertExpression * ) { }
void IRGenerator::Visit( AddrOfExp * ) { }
void IRGenerator::Visit( ArrayAccessExp * ) { }
void IRGenerator::Visit( PtrIndirectionExp * ) { }
void IRGenerator::Visit( DotExp * ) { }

/*
==================================
	IRGenerator::Visit( CastExp * exp )

 example:
	int i; char c;
	c = i;	// c = cast< char >( i );
			// char is a target type, int is a source type
	IR: assign( address_32, indir_i32( address_32 ) );
==================================
*/
void IRGenerator::Visit( CastExp * exp ) 
{
	exp->GetSourceExp()->Accept( this );

	//TODO: correct casting from one type to another !!!
	const Type * targetType = exp->GetTargetType();
	const Type * srcType = exp->GetSourceExp()->GetType();

	Assert2( targetType->IsPrimitive() && srcType->IsPrimitive(),
		"only primitive types are implemented!" );

	const ICode_t  conversionOp = m_convTable
								[ srcType->GetTypeKind() ]
								[ targetType->GetTypeKind() ];
	Assert( conversionOp != OPCODE_ERROR );

	if ( conversionOp == nop ) {
		return;
	}
	IRCode * insn = new IRCode( conversionOp, m_tail, NULL );
	insn->text = m_tail->text;
	
	insn->complexity = m_tail->complexity + IRCode::GetCost( conversionOp );
	//insn->theMostExpensiveOp = m_tail->theMostExpensiveOp;

	m_tail = insn;
}

void IRGenerator::Visit( NegationExp * e )
{
	if ( e->GetType()->IsIntegral() ) {
		GenerateUnaryExpression( e, neg );
	}
	else if ( e->GetType()->IsFloating() ) {
		GenerateUnaryExpression( e, fneg );
	}
	else {
		Unreachable();
	}
}

void IRGenerator::Visit( UnaryPlusExp * e ) {
	e->GetExp()->Accept( this );
}

void IRGenerator::Visit( PreIncExp * preIncExp ) {
	//GenerateUnaryExpression( e, ICode_t::preinc );
	// transform ++E -> (E = E + 1)
	AddExp * addExp = new AddExp( preIncExp->GetExp(), new IntegerConstant( 1 ) );
	AssignmentExp * e = new AssignmentExp( preIncExp->GetExp(), addExp );
	m_insnList.Append( GenerateExpression( e ) );

	GenerateExpression( preIncExp->GetExp() );
}

void IRGenerator::Visit( PreDecExp * e ) {
	//GenerateUnaryExpression( e, ICode_t::predec );
	Unimplemented();
}

void IRGenerator::Visit( PostIncExp * postIncExp ) {
	//GenerateUnaryExpression( e, ICode_t::postinc );
	Unimplemented();
/*	IRCode * result = GenerateExpression( postIncExp->GetExp() );

	AddExp * addExp = new AddExp( postIncExp->GetExp(), new IntegerConstant( 1 ) );
	AssignmentExp * e = new AssignmentExp( postIncExp->GetExp(), addExp );
	m_insnList.Append( GenerateExpression( e ) );

	m_tail = result;
*/
}

void IRGenerator::Visit( PostDecExp * e ) {
	//GenerateUnaryExpression( e, ICode_t::postdec );
	Unimplemented();
}

void IRGenerator::Visit( ComplExp * e ) {
	GenerateUnaryExpression( e, ICode_t::compl );
}

void IRGenerator::Visit( NotExp * e ) {
	//TODO:
	//GenerateUnaryExpression( e, not );

	// transform ( ! exp ) into ( exp == 0 )
	e->GetExp()->Accept( this );
	
	IRCode * zero =	( e->GetType()->IsIntegral() ) 
		? new IRCode( (int)0 ) : new IRCode( 0.0f );

	IRCode * node = new IRCode( eq, m_tail, zero );
	node->complexity = IRCode::GetCost( eq ) + m_tail->complexity;

	m_tail = node;
}

void IRGenerator::Visit( CommaExpression * e ) 
{
	IRCode * left = GenerateExpression( e->Left() );
	e->Left()->ToString( left->comment );
	m_insnList.Append( left );
	
	GenerateExpression( e->Right() );
}

void IRGenerator::Visit( AssignmentExp * e ) 
{
	IRCode * insn = new IRCode( ICode_t::assign );

	m_bOnRHSofAssignment = true;
	e->Right()->Accept( this );
	insn->op2 = m_tail;
	insn->complexity = insn->op2->complexity;

	m_bOnRHSofAssignment = false;
	e->Left()->Accept( this );
	insn->op1 = m_tail;
	insn->complexity += insn->op1->complexity + 2;

	//insn->theMostExpensiveOp = insn->op2->theMostExpensiveOp;

	m_tail = insn;

	m_bOnRHSofAssignment = true;
}

void IRGenerator::Visit( AddExp * e ) {
	GenerateBinaryExpression( e, ICode_t::add );
}

void IRGenerator::Visit( SubExp * e ) {
	GenerateBinaryExpression( e, ICode_t::sub );
}

void IRGenerator::Visit( MulExp * e ) {
	GenerateBinaryExpression( e, ICode_t::mul );

	//---------------------------------------
	if ( ! ::c4::compiler->IsOptionSet( TRANSFORM_MUL_INTO_SHL ) ) {
		return;
	}
	// transform 'mul' by a power of two into 'shl'
	//	
	IRCode * left = m_tail->op1;
	IRCode * right = m_tail->op2;

	switch ( right->GetOpCode() )
	{
	//case iconst_0 :
	//	m_insnList.Erase( m_insnList.GetLast() );
	//	break;
/* these will be considered by the code generator:
	case iconst_2 :
		m_tail->opcode = shl;
		right->SetText( "1" );
		break;

	case iconst_4 :
		m_tail->opcode = shl;
		right->SetText( "2" );
		break;

	case iconst_8 :
		m_tail->opcode = shl;
		right->SetText( "3" );
		break;
*/
	case iconst :
		{
			int  num = atoi( right->GetText() );
			int  powOf2 = -1;

			// TODO: remove this crap!!!
			switch ( num ) {
			case 16:			powOf2 = 4;		break;
			case 32:			powOf2 = 5;		break;
			case 64:			powOf2 = 6;		break;
			case 128:			powOf2 = 7;		break;
			case 256:			powOf2 = 8;		break;
			case 512:			powOf2 = 9;		break;
			case 1024:			powOf2 = 10;	break;
			case 2048:			powOf2 = 11;	break;
			case 4096:			powOf2 = 12;	break;
			case 8192:			powOf2 = 13;	break;
			case 16384:			powOf2 = 14;	break;
			case 32768:			powOf2 = 15;	break;
			case 65536:			powOf2 = 16;	break;
			case 131072:		powOf2 = 17;	break;
			case 262144:		powOf2 = 18;	break;
			case 524288:		powOf2 = 19;	break;
			case 1048576:		powOf2 = 20;	break;
			case 2097152:		powOf2 = 21;	break;
			case 4194304:		powOf2 = 22;	break;
			case 8388608:		powOf2 = 23;	break;
			case 16777216:		powOf2 = 24;	break;
			case 33554432:		powOf2 = 25;	break;
			case 67108864:		powOf2 = 26;	break;
			case 134217728:		powOf2 = 27;	break;
			case 268435456:		powOf2 = 28;	break;
			case 536870912:		powOf2 = 29;	break;
			case 1073741824:	powOf2 = 30;	break;
			case 2147483648:	powOf2 = 31;	break;
			
			case 4294967296:  // ??? MVC++ calls a library function ( which is located in 'llmul.asm' )
				powOf2 = 32;
				break;
			}

			if ( powOf2 != -1 ) {
				m_tail->opcode = shl;
				right->SetText( String( powOf2 ) );
			}
		}
		break;

	default: /* nothing */ ;
	}
}

void IRGenerator::Visit( DivExp * e )
{
	Type * pType = e->GetType();
	if ( pType->IsIntegral() )
	{
		if ( pType->IsSigned() ) {
			GenerateBinaryExpression( e, ICode_t::div );
		}
		if ( pType->IsUnsigned() ) {
			GenerateBinaryExpression( e, ICode_t::udiv );
		}
	}
	else if ( pType->IsFloating() ) {
		GenerateBinaryExpression( e, ICode_t::fdiv );
	}
	else {
		Unreachable();
	}

	//---------------------------------------
	if ( ! ::c4::compiler->IsOptionSet( TRANSFORM_DIV_INTO_SHR ) ) {
		return;
	}

	IRCode * left = m_tail->op1;
	IRCode * right = m_tail->op2;

	switch ( right->GetOpCode() )
	{
	case iconst :
		{
			int  num = atoi( right->GetText() );
			int  powOf2 = -1;

			// TODO: remove this crap!!!
			switch ( num ) {
			case 16:			powOf2 = 4;		break;
			case 32:			powOf2 = 5;		break;
			case 64:			powOf2 = 6;		break;
			case 128:			powOf2 = 7;		break;
			case 256:			powOf2 = 8;		break;
			case 512:			powOf2 = 9;		break;
			case 1024:			powOf2 = 10;	break;
			case 2048:			powOf2 = 11;	break;
			case 4096:			powOf2 = 12;	break;
			case 8192:			powOf2 = 13;	break;
			case 16384:			powOf2 = 14;	break;
			case 32768:			powOf2 = 15;	break;
			case 65536:			powOf2 = 16;	break;
			case 131072:		powOf2 = 17;	break;
			case 262144:		powOf2 = 18;	break;
			case 524288:		powOf2 = 19;	break;
			case 1048576:		powOf2 = 20;	break;
			case 2097152:		powOf2 = 21;	break;
			case 4194304:		powOf2 = 22;	break;
			case 8388608:		powOf2 = 23;	break;
			case 16777216:		powOf2 = 24;	break;
			case 33554432:		powOf2 = 25;	break;
			case 67108864:		powOf2 = 26;	break;
			case 134217728:		powOf2 = 27;	break;
			case 268435456:		powOf2 = 28;	break;
			case 536870912:		powOf2 = 29;	break;
			case 1073741824:	powOf2 = 30;	break;
			case 2147483648:	powOf2 = 31;	break;
			
			case 4294967296:  // ??? MVC++ calls a library function ( which is located in 'llmul.asm' )
				powOf2 = 32;
				break;
			}

			if ( powOf2 != -1 ) {
				m_tail->opcode = shr;
				right->SetText( String( powOf2 ) );
			}
		}
		break;

	default: /* nothing */ ;
	}
}

void IRGenerator::Visit( ModExp * e ) {
	GenerateBinaryExpression( e, ICode_t::rem );
}

void IRGenerator::Visit( ShlExp * e ) {
	GenerateBinaryExpression( e, ICode_t::shl );
}

void IRGenerator::Visit( ShrExp * e ) {
	if ( e->GetType()->IsSigned() ) {
		GenerateBinaryExpression( e, ICode_t::shr );
	} else {
		GenerateBinaryExpression( e, ICode_t::ushr );
	}
}

void IRGenerator::Visit( UShrExp * e ) {
	GenerateBinaryExpression( e, ICode_t::ushr );
}

void IRGenerator::Visit( XorExp * e ) {
	GenerateBinaryExpression( e, ICode_t::xor );
}

void IRGenerator::Visit( OrOrExp * e ) {
	Unimplemented();
}

void IRGenerator::Visit( InExp * ) { 
	Unimplemented();
}

void IRGenerator::Visit( CondExp * e ) {
	Unimplemented();
}

void IRGenerator::Visit( EquExp * e ) {
	switch ( e->GetKind() ) {
		case EquExp::Equal :
			GenerateBinaryExpression( e, ICode_t::eq );
			break;

		case EquExp::NotEqual :
			GenerateBinaryExpression( e, ICode_t::neq );
			break;

		default: Unreachable();
	}
}

void IRGenerator::Visit( RelExp * e ) {
	switch ( e->GetKind() ) {
		case RelExp::LT :
			GenerateBinaryExpression( e, ICode_t::lt );
			break;

		case RelExp::GT :
			GenerateBinaryExpression( e, ICode_t::gt );
			break;

		case RelExp::LE :
			GenerateBinaryExpression( e, ICode_t::le );
			break;

		case RelExp::GE :
			GenerateBinaryExpression( e, ICode_t::ge );
			break;

		default: Unreachable();
	}
}

void IRGenerator::Visit( BitAndExp * e ) {
	GenerateBinaryExpression( e, ICode_t::and );
}

void IRGenerator::Visit( BitXorExp * e ) { 
	GenerateBinaryExpression( e, ICode_t::xor );
}

void IRGenerator::Visit( BitOrExp * e ) {
	GenerateBinaryExpression( e, ICode_t::or );
}

void IRGenerator::Visit( AddAssignExp * ) { }
void IRGenerator::Visit( SubAssignExp * ) { }
void IRGenerator::Visit( MulAssignExp * ) { }
void IRGenerator::Visit( DivAssignExp * ) { }
void IRGenerator::Visit( ModAssignExp * ) { }
void IRGenerator::Visit( BitAndAssignExp * ) { }
void IRGenerator::Visit( BitXorAssignExp * ) { }
void IRGenerator::Visit( BitOrAssignExp * ) { }
void IRGenerator::Visit( ShlAssignExp * ) { }
void IRGenerator::Visit( ShrAssignExp * ) { }
void IRGenerator::Visit( UShrAssignExp * ) { }

void IRGenerator::Visit( CallExp * callExp )
{
	//if ( ! callExp->CheckSideEffects() ) {
	//	return;
	//}

	FunctionSymbol * pSymbol = callExp->GetSymbol();

	// pass parameters
	LinkedList< Expression* > &	rList = callExp->GetArguments()->GetList();

	int  sizeOfParams = 0;

/* if parameters are passed from right to left
	LinkedList< Expression* >::Iterator  iter( rList.Begin() );

	for ( ; iter != rList.End(); iter++ ) 
	{
		Expression * e = *iter;

		IRCode * exp = GenerateExpression( e );
		IRCode * param = new IRCode( ICode_t::arg, exp, NULL );
		e->ToString( param->comment );
		m_insnList.Append( param );
	}
*/

	// C-style calling convention, parameters are passed from left to right
	LinkedList< Expression* >::Iterator  iter( rList.GetLast() );
	while ( iter != rList.Begin()-1 )
	{
		Expression * e = *iter;
		IRCode * exp = GenerateExpression( e );
		IRCode * param = new IRCode( ICode_t::arg, exp, NULL );
		e->ToString( param->comment );
		m_insnList.Append( param );

		//TODO:
		sizeOfParams += 4; //e->GetType()->Size();

		--iter;
	}


	IRCode * addrj = new IRCode( ICode_t::address_32 );
	addrj->SetText( pSymbol->GetAsmName() );

	// call
	IRCode * insnCall = new IRCode( ICode_t::call, addrj, NULL );
	insnCall->m_clobberedRegs = pSymbol->GetClobberedRegs();
	insnCall->SetIntegerValue( sizeOfParams );

	m_tail = insnCall;
}

void IRGenerator::Visit( ErroneousExpression * ) { }
void IRGenerator::Visit( ThisExpression * ) { }
void IRGenerator::Visit( AST_Node * ) { }
//----------------------------------------------------------------------
IRCode * IRGenerator::GenerateExpression( Expression * e ) { 
	//HACK:
	m_bOnRHSofAssignment = true;
	
	e->Accept( this );
	return m_tail;
}

IRCode * IRGenerator::GenerateStatement( Statement * s ) {
	//HACK:
	m_bOnRHSofAssignment = true;

	s->Accept( this );
	s->ToString( m_tail->comment );
	return m_tail;
}

void IRGenerator::GenerateUnaryExpression( UnaryExpression* exp, ICode_t  opcode )
{
	IRCode * insn = new IRCode( opcode );

	exp->GetExp()->Accept( this );
	insn->op1 = m_tail;

	insn->complexity = insn->op1->complexity;
	insn->complexity += IRCode::GetCost( opcode );

	m_tail = insn;
}

void IRGenerator::GenerateBinaryExpression( BinaryExpression* exp, ICode_t  opcode )
{
	IRCode * insn = new IRCode( opcode );

	exp->Left()->Accept( this );
	insn->op1 = m_tail;

	insn->complexity = insn->op1->complexity + IRCode::GetCost( opcode );

	exp->Right()->Accept( this );
	insn->op2 = m_tail;

	insn->complexity += insn->op2->complexity;

//	insn->theMostExpensiveOp =
//		insn->op1->theMostExpensiveOp > insn->op2->theMostExpensiveOp
//		?
//		insn->op1->theMostExpensiveOp : insn->op2->theMostExpensiveOp;

	//if ( insn->opcode > insn->theMostExpensiveOp ) {
	//	insn->theMostExpensiveOp = insn->opcode;
	//}

	Canonicalize( insn );

	m_tail = insn;
}


IRCode * IRGenerator::DefineLabel() {
	static int numLabels;

	IRCode * pLabel = new IRCode( ICode_t::label );
	
	// generate a unique name for this label
	pLabel->SetText( "L" + String( numLabels ) );
	++numLabels;
	
	return pLabel;
}

void IRGenerator::Emit( ICode_t  opcode, IRCode * pLabel )
{
	assert( opcode == ICode_t::jmp 
		|| opcode == ICode_t::br_false 
		|| opcode == ICode_t::br_true );

	Assert( pLabel );

	IRCode * pNewNode = new IRCode( opcode, pLabel, NULL );
	pNewNode->SetText( pLabel->GetText() );

	m_tail = pNewNode;
	m_insnList.Append( m_tail );
}

void IRGenerator::EmitBranchIfFalse( IRCode * testExp, IRCode * pLabel )
{
	Assert( testExp && pLabel );
	
	IRCode * pNewNode = new IRCode( ICode_t::br_false, testExp, pLabel );
	m_tail = pNewNode;
	m_insnList.Append( m_tail );
}

void IRGenerator::EmitBranchIfTrue( IRCode * testExp, IRCode * pLabel )
{
	Assert( testExp && pLabel );
	
	IRCode * pNewNode = new IRCode( ICode_t::br_true, testExp, pLabel );
	m_tail = pNewNode;
	m_insnList.Append( m_tail );
}

void IRGenerator::MarkLabel( IRCode * pLabel ) {
	Assert( pLabel );
	m_tail = pLabel;
	m_insnList.Append( m_tail );
}

void IRGenerator::InsertLabels()
{
	IRList_t::Iterator  iter( m_insnList.Begin() );
	while ( iter != m_insnList.End() )
	{
		IRCode * pInsn = *iter;		

		IRCode * pLabel = pInsn->m_pLabel;
		if ( pLabel ) {
			m_insnList.InsertBefore( iter, pLabel );
		}
		++iter;
	}

	//----- minor optimizations ----------------------------------------------
	if ( ::c4::compiler->IsOptionSet( DO_NOT_PERFORM_ANY_OPTIMIZATIONS ) ) {
		return;
	}
	// remove unnecessary jumps and labels
	for ( iter = m_insnList.Begin();
		iter.HasNext(); )
	{
		IRCode * pInsn = *iter;
		IRCode * pNext = *( iter + 1 );
		
		//-------------------
		// remove such things
		// jmp Label
		// Label:
		if ( IRCode::IsBranchInsn( pInsn->GetOpCode() )
			&& pNext->GetOpCode() == ICode_t::label
			&& pInsn->GetOp1() == pNext )
		{
			delete pInsn;
			iter = m_insnList.Remove( iter );
		}

		// remove unreachable code
		// jmp L1
		// jmp L2
		if ( pInsn->GetOpCode() == ICode_t::jmp
			&& pNext->GetOpCode() == ICode_t::jmp )
		{
			delete pNext;
			iter = m_insnList.Remove( iter + 1 );
		}

		//TODO: merge duplicate labels
		// L1:
		// L2:
		// L3:

		//-------------------
		++iter;
	}
}

void IRGenerator::InitConversionTable() 
{
	for ( int i = 0; i < NUM_OF_TYPES; i++ )
	{
		for ( int j = 0; j < NUM_OF_TYPES; j++ )
		{
			m_convTable[i][j] = OPCODE_ERROR;
		}
	}

	// These are machine-specific !
	// ex.: float32 == float64 == float80 in FPU of Machine_x86
	// and so they needn't to be converted

	// TODO: _boolean

	m_convTable[ CHAR ][ CHAR ]			= nop;
	m_convTable[ CHAR ][ BOOL ]			= i2b;
	m_convTable[ CHAR ][ TYPE_INT8 ]	= nop;
	m_convTable[ CHAR ][ TYPE_INT16 ]	= nop;
	m_convTable[ CHAR ][ TYPE_INT32 ]	= nop;
	//m_convTable[ CHAR ][ TYPE_INT64 ]	= TYPE_INT64;
	m_convTable[ CHAR ][ TYPE_FLOAT32 ]	= i2f;
	m_convTable[ CHAR ][ TYPE_FLOAT64 ]	= i2f;
	//m_convTable[ CHAR ][ FLOAT80 ]		= FLOAT80;
	//m_convTable[ CHAR ][ STRING ]		= STRING;

	m_convTable[ BOOL ][ CHAR ]			= nop;
	m_convTable[ BOOL ][ BOOL ]			= nop;
	m_convTable[ BOOL ][ TYPE_INT8 ]	= nop;
	m_convTable[ BOOL ][ TYPE_INT32 ]	= nop;
	//m_convTable[ BOOL ][ TYPE_INT64 ]	= nop;
	m_convTable[ BOOL ][ TYPE_FLOAT32 ]	= i2f;
	m_convTable[ BOOL ][ TYPE_FLOAT64 ]	= i2f;
	//m_convTable[ BOOL ][ FLOAT80 ]		= i2f;
	//m_convTable[ BOOL ][ STRING ]		= STRING;

	m_convTable[ TYPE_INT8 ][ CHAR ]		= nop;
	m_convTable[ TYPE_INT8 ][ BOOL ]		= i2b;
	m_convTable[ TYPE_INT8 ][ TYPE_INT8 ]	= nop;
	m_convTable[ TYPE_INT8 ][ TYPE_INT16 ]	= nop;
	m_convTable[ TYPE_INT8 ][ TYPE_INT32 ]	= nop;
	//m_convTable[ TYPE_INT8 ][ TYPE_INT64 ]	= TYPE_INT64;
	m_convTable[ TYPE_INT8 ][ TYPE_FLOAT32 ]= i2f;
	m_convTable[ TYPE_INT8 ][ TYPE_FLOAT64 ]= i2f;
	//m_convTable[ TYPE_INT8 ][ FLOAT80 ]		= FLOAT80;
	//m_convTable[ TYPE_INT8 ][ STRING ]		= STRING;

	m_convTable[ TYPE_INT16 ][ CHAR ]		= nop;
	m_convTable[ TYPE_INT16 ][ BOOL ]		= i2b;
	m_convTable[ TYPE_INT16 ][ TYPE_INT8 ]	= nop;
	m_convTable[ TYPE_INT16 ][ TYPE_INT16 ]	= nop;
	m_convTable[ TYPE_INT16 ][ TYPE_INT32 ]	= nop;
	//m_convTable[ TYPE_INT16 ][ TYPE_INT64 ]	= TYPE_INT64;
	m_convTable[ TYPE_INT16 ][ TYPE_FLOAT32 ]= i2f;
	m_convTable[ TYPE_INT16 ][ TYPE_FLOAT64 ]= i2f;

	m_convTable[ TYPE_INT32 ][ CHAR ]		= nop;
	m_convTable[ TYPE_INT32 ][ BOOL ]		= i2b;
	m_convTable[ TYPE_INT32 ][ TYPE_INT8 ]	= nop;
	m_convTable[ TYPE_INT32 ][ TYPE_INT16 ]	= nop;
	m_convTable[ TYPE_INT32 ][ TYPE_INT32 ]	= nop;
//	m_convTable[ TYPE_INT32 ][ TYPE_INT64 ]	= TYPE_INT64;
	m_convTable[ TYPE_INT32 ][ TYPE_FLOAT32 ]= i2f;
	m_convTable[ TYPE_INT32 ][ TYPE_FLOAT64 ]= i2f;
	//m_convTable[ TYPE_INT32 ][ FLOAT80 ]	= FLOAT80;
	//m_convTable[ TYPE_INT32 ][ STRING ]		= STRING;

	// TODO:

	m_convTable[ TYPE_FLOAT32 ][ CHAR ]			= f2i;
	m_convTable[ TYPE_FLOAT32 ][ BOOL ]			= f2b;
	m_convTable[ TYPE_FLOAT32 ][ TYPE_INT8 ]	= f2i;
	m_convTable[ TYPE_FLOAT32 ][ TYPE_INT16 ]	= f2i;
	m_convTable[ TYPE_FLOAT32 ][ TYPE_INT32 ]	= f2i;
//	m_convTable[ TYPE_FLOAT32 ][ TYPE_INT64 ]	= f2l;
	m_convTable[ TYPE_FLOAT32 ][ TYPE_FLOAT32 ]	= nop;
	m_convTable[ TYPE_FLOAT32 ][ TYPE_FLOAT64 ]	= nop;
//	m_convTable[ TYPE_FLOAT32 ][ FLOAT80 ]		= FLOAT80;
//	m_convTable[ TYPE_FLOAT32 ][ STRING ]		= STRING;


	m_convTable[ TYPE_FLOAT64 ][ CHAR ]			= f2i;
	m_convTable[ TYPE_FLOAT64 ][ BOOL ]			= f2b;
	m_convTable[ TYPE_FLOAT64 ][ TYPE_INT8 ]	= f2i;
	m_convTable[ TYPE_FLOAT64 ][ TYPE_INT32 ]	= f2i;
	m_convTable[ TYPE_FLOAT64 ][ TYPE_INT16 ]	= f2i;
//	m_convTable[ TYPE_FLOAT64 ][ TYPE_INT64 ]	= f2l;
	m_convTable[ TYPE_FLOAT64 ][ TYPE_FLOAT32 ]	= nop;
	m_convTable[ TYPE_FLOAT64 ][ TYPE_FLOAT64 ]	= nop;
//	m_convTable[ TYPE_FLOAT64 ][ FLOAT80 ]		= FLOAT80;
//	m_convTable[ TYPE_FLOAT64 ][ STRING ]		= STRING;
}

IRCode * IRGenerator::GetLastGeneratedInstruction() {
	return m_tail;
}

IRCode * IRGenerator::EmitData( const char * s ) {
	IRCode * pLabel = DefineLabel();
	pLabel->m_iSize = 1;

	pLabel->m_data = s;
	pLabel->comment = s;

	m_data_in_CONST_segment.Append( pLabel );
	
	//m_tail = new IRCode( ICode_t::address_32 );
	//m_tail = new IRCode( ICode_t::indir_i32, m_tail, NULL );
	//m_tail->SetText( pLabel->GetText() );
	m_tail = pLabel;

	return m_tail;
}

IRCode * IRGenerator::EmitData( int i ) {
	Unimplemented();
	return NULL;
}

IRCode * IRGenerator::EmitData( float f ) {
	IRCode * pLabel = DefineLabel();
	pLabel->m_iSize = 4;

	{
		float value = f;

		char  temp[4];
		memcpy( temp, &value, sizeof( value ) );
/*		//TODO: remove this crap
		pLabel->data = "abcd";
		pLabel->data[0] = temp[0];
		pLabel->data[1] = temp[1];
		pLabel->data[2] = temp[2];
		pLabel->data[3] = temp[3];
		pLabel->data[4] = '\0';
*/
		pLabel->m_data = temp;
		pLabel->comment = String( f );
	}

	m_data_in_CONST_segment.Append( pLabel );

	m_tail = new IRCode( ICode_t::fconst );
	m_tail->SetText( pLabel->GetText() );

	return m_tail;
}

}//end namespace c4

