/*
===============================================
	A very primitive code generator
	which produces assembly listings.
===============================================
*/
#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

#include "../IR.h"
#include "Machine_x86.h"

namespace c4 {

/*
==============================
	class OperandLocation
==============================
*/
OperandLocation::OperandLocation( const operand_info_t & info )
	: m_info( &info )
{
}

bool OperandLocation::IsReg32() const {
	return m_info->type == eReg_32;
}

bool OperandLocation::IsRegister( RegCode_t  regCode ) const
{
	Unimplemented();
	return false;
}

/*
====================
	class Operand
====================
*/
Operand::Operand( int ruleNumber )
	: m_rule_number( ruleNumber )
	, m_bEmitted( false )
	, m_numReferences( 1 )
{
	m_operands[ 0 ] = NULL;
}

Operand::~Operand() {
	DropChildren();
	Dispose();
}

int Operand::GetNumOperands() const {
	//return m_numOperands;
	int i;
	for ( i = 0; m_operands[i] != NULL; i++ )
		;

	return i;
}

Operand *& Operand::GetOperand( int num ) {
	Operand *& op = m_operands[ num ]; 
	//Assert( op );
	BREAK_IF( op == NULL );
	return op;
}

void Operand::SetOperand( int num, Operand *value ) {
	m_operands[ num ] = value; 
}

bool Operand::IsLeaf() const {
	return m_operands[0] == NULL;
}

void Operand::DropChildren() {
	for ( int i = 0; m_operands[i] != NULL; i++ ) {
		m_operands[i]->Drop();
	}	
	// now I do not have any child nodes
	m_operands[ 0 ] = NULL;
}

uint Operand::GetReferenceCount() const {
	return m_numReferences;
}

void Operand::Grab() {
	++m_numReferences;
}

bool Operand::Drop() 
{
	if ( m_numReferences <= 0 ) {
		c4::common->Print( "\t(Ref.count<=0) for rule: %d\n", m_rule_number );
	}
	BREAK_IF( m_numReferences <= 0 )

	--m_numReferences;
	if ( m_numReferences == 0 ) {
		delete this;
		return true;
	}
	return false;
}

/*
========================
	class LeafOperand
========================
*/
LeafOperand::LeafOperand( IRCode *insn, int ruleNumber )
	: m_insn( insn )
	, Operand( ruleNumber )
{
}

const char * LeafOperand::Name() const {
	return m_insn->Name();
}

/*
========================
	class MemoryOperand
========================
*/
MemoryOperand::MemoryOperand( IRCode *insn, int ruleNumber )
	: LeafOperand( insn, ruleNumber )
{
}

EOperandSize MemoryOperand::GetSize() const {
	return ( EOperandSize ) m_insn->GetSize();
}

/*
============================
	class ImmediateOperand
============================
*/
ImmediateOperand::ImmediateOperand( IRCode *insn, int ruleNumber )
	: LeafOperand( insn, ruleNumber )
{
}

EOperandSize ImmediateOperand::GetSize() const {
	return VARIABLE_SIZE;
}

/*
========================
	class InsnOperand
========================
*/
InsnOperand::InsnOperand( IRCode *insn, int ruleNumber )
	: LeafOperand( insn, ruleNumber )
{
}

EOperandSize InsnOperand::GetSize() const {
	Assert( this->GetNumOperands() == 1 );
	return m_operands[ 0 ]->GetSize();
//	return SIZE_ERROR;
}

/*
==========================
	class RegisterOperand
==========================
*/
RegisterOperand::RegisterOperand( Register & reg, int ruleNumber )
	: Operand( ruleNumber )
	, m_register( &reg )
{
	reg.SetOperand( this );
}

RegisterOperand::~RegisterOperand() {
	//if ( m_register ) {
	//	m_register->SetFree();
	//}
	this->Dispose();
}

Register * RegisterOperand::GetRegister() {
	Assert( m_register );
	return m_register;
}

RegCode_t RegisterOperand::GetRegisterCode() const {
	Assert( m_register );
	return m_register->GetCode();
}

void RegisterOperand::SetRegister( Register & reg ) {
	m_register = & reg;
	reg.SetOperand( this );
	//reg.SetUsed();
}

const char * RegisterOperand::Name() const {
	Assert( m_register );
	return m_register->Name();
}

void RegisterOperand::Dispose() {
	if ( m_register ) {
		m_register->SetFree();
		m_register = NULL;
	}
}

EOperandType RegisterOperand::GetType() const {
	Assert( m_register );
	return eRegister;
}

EOperandSize RegisterOperand::GetSize() const {
	Assert( m_register );
	return GetSizeOfRegister( m_register->GetCode() );
}

/*
====================
	Register
====================
*/
Register::Register()
{
	Init( rRRR, "RRR" );
}

Register::Register( RegCode_t code, char* name )
{
	Init( code, name );
}

void Register::Init( RegCode_t  code, char* name ) {
	m_code = code;
	m_name = name;

	m_bFree = true;
	m_operand = NULL;
}

bool Register::IsFree() const { 
	return m_bFree; // && !m_bClobbered; 
}

bool Register::IsUsed() const { 
	return !m_bFree;
}

void Register::SetUsed() { 
	m_bFree = false; 
}

void Register::SetFree() {
	m_bFree = true;
	m_operand = NULL;
}

void Register::SetOperand( RegisterOperand *operand ) { 
	assert( operand != NULL );
	m_operand = operand;
	m_bFree = false;
}

RegisterOperand * Register::GetOperand() { 
	//Assert( m_operand != NULL );
	return m_operand;
}

/*
========================
	class Machine_x86
========================
*/

//TODO: remove this crap!
Machine_x86 *  g_Machine = NULL;

Machine_x86::Machine_x86()
{
/*
	String  dir( c4::compiler->GetOutputDirectory() );
	dir += '/';
	dir += c4::compiler->GetOutputFileName();	
	dir.StripFileExtension();
	dir += ".asm";		// make the name of the output file
	
	m_file.Open( dir );
*/
	String	filename( c4::compiler->GetOutputFileName() );
	//filename.StripFileExtension();
	//filename += ".asm";

	m_file.Open( filename );

	Init();

	g_Machine = this;

	//----------------------
}

Machine_x86::~Machine_x86() {
	m_file.Close();
}

void Machine_x86::Init() {
	m_spacing = "\t";

	//TODO:
	m_tab = "";//"\t";

	m_temp_buf[ 0 ] = '\0';
	m_instruction_buffer[ 0 ] = '\0';

	//---------------------------------------
	m_registers[ rEAX ].Init( rEAX, "eax" );
	m_registers[ rEBX ].Init( rEBX, "ebx" );
	m_registers[ rECX ].Init( rECX, "ecx" );
	m_registers[ rEDX ].Init( rEDX, "edx" );

	m_registers[ rESI ].Init( rESI, "esi" );
	m_registers[ rEDI ].Init( rEDI, "edi" );


	m_registers[ rAX ].Init( rAX, "ax" );
	m_registers[ rBX ].Init( rBX, "bx" );
	m_registers[ rCX ].Init( rCX, "cx" );
	m_registers[ rDX ].Init( rDX, "dx" );

	m_registers[ rSI ].Init( rSI, "si" );
	m_registers[ rDI ].Init( rDI, "di" );

	m_registers[ rAL ].Init( rAL, "al" );
	m_registers[ rBL ].Init( rBL, "bl" );
	m_registers[ rCL ].Init( rCL, "cl" );
	m_registers[ rDL ].Init( rDL, "dl" );

	//---------------------------------------
	m_future_side_effects.clobberedRegs = 0;

	//m_clobberedRegs = 0;

	//---------------------------------------
	Reset();
}

void Machine_x86::Reset() {
	m_numInstructions = 0;
}

bool Machine_x86::GenerateCode( IRList_t & list )
{
	IRList_t::Iterator   iter( list.Begin() );
	while ( iter != list.End() )
	{
		IRCode * pInsn = *iter;

		GenerateCode( pInsn );

		++iter;
	}
	return true;
}

bool Machine_x86::GenerateCode( IRCode * node ) {
	if ( ! node ) {
		return false;
	}
	Label( node );

#ifdef DEBUG_I386
	Print( "Intermediate tree:\n" );
	IRCode::DumpTree( node );
	Print( "\n" );

	Print( "Optimal cover for this tree:\n" );
	_DumpCover( node, START_NONTERMINAL );
	Print( "\n" );

	Print( "Assembler templates selected for this tree:\n" );
	int ruleNumber = _DumpTemplates( node, START_NONTERMINAL );
	Print( "\n" );
#else
	if ( ! CheckCover( node ) ) {
		c4::common->Error( "\tFailed to codegen!\n" );
		return false;
	}
#endif

	//--------------------------------------------------------

	if ( ! node->comment.IsEmpty() ) {
		Emit( "\n; %s\n", node->comment.c_str() );
	}

	//Print( "Assembler code:\n" );
	Operand * root = EmitNode( node, START_NONTERMINAL );
	delete root;

	return true;
}

void Machine_x86::EmitStandardHeader() {
	Emit( "\t.486\n\t.model\tflat\n" );
	Emit( "\nincludelib MSVCRTD\nincludelib OLDNAMES\n\n" );
	Emit( "public	_main\n" );

	Emit( "extrn	__imp__printf : PROC\n" );
	Emit( "extrn	__imp__getchar : PROC\n" );
	Emit( "extrn	__ftol2_sse : PROC\n" );
}

void Machine_x86::EmitStandardEnding() {
	Emit( "END" );
}

void Machine_x86::Emit_TEXT_begin() {
	Emit( "\n_TEXT segment\n" );
}

void Machine_x86::Emit_TEXT_end() {
	Emit( "\n_TEXT ends\n" );
}

state_s * Machine_x86::Label( IRCode * insn ) {
	if ( insn )	{
		state_s * left = Label( insn->GetOp1() );
		state_s * right = Label( insn->GetOp2() );
		return insn->state = _GetState( insn, left, right );
	} else {
		return NULL;
	}
}
//----------------------------------------------------------------------
void Machine_x86::Emit( const char* format, ... ) {
	va_list   argptr;
	va_start( argptr, format );
	//vsprintf_s( m_temp_buf, BUFFER_SIZE, format, argptr );
	vsprintf( m_temp_buf, format, argptr );
	va_end( argptr );

	m_file.Write( m_temp_buf );

	//c4::common->Print( m_temp_buf );
	printf( m_temp_buf );
}
//----------------------------------------------------------------------
void Machine_x86::Print( const char* format, ... ) {
	va_list   ap;
	va_start( ap, format );
	vsprintf_s( m_temp_buf, BUFFER_SIZE, format, ap );
	va_end( ap );
	printf( m_temp_buf );
	putchar( '\n' );
}
//----------------------------------------------------------------------
void Machine_x86::EmitIntoBuffer( const char* format, ... ) {
	va_list   argptr;
	va_start( argptr, format );
	
	//vsprintf_s( m_instruction_buffer, BUFFER_SIZE, format, argptr );
	vsprintf_s( m_temp_buf, BUFFER_SIZE, format, argptr );
	
	strcat( m_instruction_buffer, m_temp_buf );

	va_end( argptr );
}

void Machine_x86::EmitInstructionInBuffer() {
	Emit( m_instruction_buffer );
}

void Machine_x86::ClearInstructionBuffer() {
	m_instruction_buffer[ 0 ] = '\0';
}
//----------------------------------------------------------------------
/*
MemoryLocation * Machine_x86::DefineConstant_Float32( float value, int ruleNumber ) {
	String   nameOfLabel = "__real32_";

	nameOfLabel += String( m_constants.Count() );

	MemoryLocation * newConstant = new MemoryLocation( nameOfLabel.c_str(), ruleNumber );
	newConstant->Grab(); // prevent from deletion
	m_constants.Append( newConstant );

	return newConstant;
}

MemoryLocation * Machine_x86::DefineConstant_Float64( double value, int ruleNumber ) {
	String   nameOfLabel = "__real64_";

	nameOfLabel += String( m_constants.Count() );

	MemoryLocation * newConstant = new MemoryLocation( nameOfLabel.c_str(), ruleNumber );
	newConstant->Grab(); // prevent from deletion
	m_constants.Append( newConstant );

	return newConstant;
}
*/
void Machine_x86::Emit_CONST_Segment( IRList_t & rList ) {
	if ( rList.Count() == 0 ) {
		return;
	}
	
	Emit( "\nCONST segment\n" );

	IRList_t::Iterator  iter( rList.Begin() );

	while ( iter != rList.End() )
	{
		IRCode * pLabel = *iter;

		Assert( pLabel->GetOpCode() == ICode_t::label );

		// name of this label
		Emit( "%s%s", pLabel->GetText(), m_spacing );

		switch ( pLabel->GetSize() )
		{
		case BYTE_PTR :
			{
				Emit( "db " );
		
				/*
				const char * p = pLabel->GetData();
				while ( *p )
				{
					Emit( "'%c',", *p );
					++p;
				}
				*/
				Emit( "'%s', 0h\t; \"%s\"\n", pLabel->GetData(),
					pLabel->comment.c_str() ); // emit comment
			}
			break;

		//case WORD_PTR :
		case DWORD_PTR :
			{
				byte	temp[ 4 ];
				memcpy( temp, pLabel->GetData(), 4 );
				float * f = reinterpret_cast< float* >( temp );

				Emit( "dd 0%xh", *( unsigned* ) f );
				Emit( "\t; %f\n", *f ); // emit comment
			}
			break;

		//case QWORD_PTR :
		//	{
		//	}
		//	break;

		default: Unreachable();
		}
/*		byte	temp[ 4 ];
		memcpy( temp, pLabel->data.c_str(), 4 );
		float * f = reinterpret_cast< float* >( temp );

		// dd ...	
		Emit( "  dd 0%xh", *( unsigned* ) f );

		Emit( "\t; %f\n", *f ); // emit comment
*/
		//---------
		++iter;
	}

	Emit( "CONST ends\n" );
}
//----------------------------------------------------------------------

void Machine_x86::SetFutureSideEffects( const side_effects_t & rSideEffects )
{
	m_future_side_effects.clobberedRegs |= rSideEffects.clobberedRegs;

	//if ( m_future_side_effects.clobberedRegs != 0 ) {
	//	printf( "\t[clobber EDX]\n" );
	//}
}

void Machine_x86::ResetFutureSideEffects()
{
	m_future_side_effects.clobberedRegs = 0;
}

Register & Machine_x86::GetRegisterByCode( RegCode_t  regCode ) {
	return m_registers[ regCode ];
}

const Register * Machine_x86::GetLowerByteRegister( RegCode_t  regCode ) {
	switch ( regCode ) {
		case rEAX :
		case rAX :
			return & m_registers[ rAL ];

		case rEBX :
		case rBX :
			return & m_registers[ rBL ];

		case rECX :
		case rCX :
			return & m_registers[ rCL ];

		case rEDX :
		case rDX :
			return & m_registers[ rDL ];

		default : /* nothing */ ;
	}
	return NULL;
}

const Register * Machine_x86::GetLowerWordRegister( RegCode_t  regCode ) {
	switch ( regCode ) {
		case rEAX :
			return & m_registers[ rAX ];

		case rEBX :
			return & m_registers[ rBX ];

		case rECX :
			return & m_registers[ rCX ];

		case rEDX :
			return & m_registers[ rDX ];

		case rESI :
			return & m_registers[ rSI ];

		case rEDI :
			return & m_registers[ rDI ];

		case rESP :
			return & m_registers[ rSP ];

		case rEBP :
			return & m_registers[ rBP ];

		default : /* nothing */ ;
	}
	return NULL;
}

Register & Machine_x86::GetFreeReg32() {
	Register * pBestReg = & m_registers[ 0 ];

	enum { NUM_OF_REG32 = 6 };

	Register *  pRegs[ NUM_OF_REG32 ] = {
/*		& m_registers[ rEAX ],
		& m_registers[ rECX ],
		& m_registers[ rEDX ],

		& m_registers[ rESI ],
		& m_registers[ rEDI ],
*/
		& m_registers[ rEAX ],
		& m_registers[ rECX ],
		
		& m_registers[ rESI ],
		& m_registers[ rEDI ],

		& m_registers[ rEDX ],

		& m_registers[ rEBX ],
	};

	for ( int i = 0; i < NUM_OF_REG32; i++ )
	{
		if ( pRegs[i]->IsFree() ) {
			pBestReg = pRegs[ i ];
			break;
		}
	}

	Assert2( pBestReg, "failed to find a free 32-bit general-purpose register!" );
	return *pBestReg;
}

Register & Machine_x86::GetFreeGeneralReg32() {
	Register * pBestReg = & m_registers[ 0 ];

	enum { NUM_OF_REG32 = 4 };

	Register *  pRegs[ NUM_OF_REG32 ] = {
		& m_registers[ rEAX ],
		& m_registers[ rECX ],
		& m_registers[ rEBX ],
		& m_registers[ rEDX ],
	};

	for ( int i = 0; i < NUM_OF_REG32; i++ )
	{
		if ( pRegs[i]->IsFree() ) {
			pBestReg = pRegs[ i ];
			break;
		}
	}

	Assert2( pBestReg, "failed to find a free general-purpose 32-bit general-purpose register!" );
	return *pBestReg;
}

Register & Machine_x86::GetFreeRegister( RegCode_t  regCode )
{
	EnsureFree( regCode );
	return m_registers[ regCode ];
}

void Machine_x86::EnsureFree( RegCode_t  regCode ) {
	if ( m_registers[ regCode ].IsFree() ) {
		return;
	}
	// TODO: select reloading into another register or spilling to memory ( if all registers are used )
	Register & rFreeReg = GetFreeReg32();
	EmitMove( rFreeReg, m_registers[ regCode ] );
	//reg.SetFree();
}

void Machine_x86::EnsureFree( Register & reg ) {
	EnsureFree( reg.GetCode() );
}

void Machine_x86::Clobber( RegCode_t  regCode ) {
	Clobber( m_registers[ regCode ] );
}

void Machine_x86::Clobber( Register & reg ) {
	EnsureFree( reg );
	reg.SetUsed();
}

void Machine_x86::EmitMove( Register & dest, Register & src )
{
	Assert( dest.GetCode() != src.GetCode() );

	EnsureFree( dest );
	Emit( "%smov%s%s, %s\n", m_tab, m_spacing, dest.Name(), src.Name() );
	++m_numInstructions;

	if ( RegisterOperand * pSrcOperand = src.GetOperand() ) {
		pSrcOperand->SetRegister( dest );
	}
	dest.SetUsed();
	src.SetFree();
}

void Machine_x86::MoveOperandIntoRegister( Register & dest, Operand *& operand )
{
	if ( operand->GetType() == eRegister )
	{
		RegisterOperand * pRegOp = static_cast< RegisterOperand* >( operand );
		Register * pReg = pRegOp->GetRegister();
		
		if ( pReg->GetCode() == dest.GetCode() ) {
			return;
		}
		else {
			EmitMove( dest, *pReg );
		}
	}
	else
	{
Unreachable(); // ????
		EnsureFree( dest );
		Emit( "%smov%s%s, %s\n", m_tab, m_spacing, dest.Name(), operand->Name() );
		++m_numInstructions;

		RegisterOperand * newOperand = new RegisterOperand( dest, operand->GetRuleNumber() );
		memcpy( newOperand->GetPtr(), operand->GetPtr(), MAX_NUM_OPERANDS * sizeof( Operand* ) );

		const InstructionData & insnData = GetInsnData( operand->GetRuleNumber() );
		//for ( int i = 0; i < insnData.numChildNodes; i++ ) {
		for ( int i = 0; i < newOperand->GetNumOperands(); i++ ) {		
			newOperand->GetOperand( i )->Grab();
		}
		operand->Drop();
		operand = newOperand;
	}

	dest.SetUsed();
}

void Machine_x86::PerformSideEffects( const side_effects_t & rSideEffects )
{
	ClobberRegisters( rSideEffects.clobberedRegs );

	m_current_side_effects.clobberedRegs = rSideEffects.clobberedRegs;
	m_current_side_effects.modifiedFlags = rSideEffects.modifiedFlags;
}

void Machine_x86::UndoSideEffects( const side_effects_t & rSideEffects )
{
	UnclobberRegisters( rSideEffects.clobberedRegs );

	m_current_side_effects.clobberedRegs = 0;
	m_current_side_effects.modifiedFlags = 0;
}

//UNDONE:
void Machine_x86::ClobberRegisters( RegMask_t  regs ) {
	if ( regs == 0 ) {
		return;
	}

	//TODO: several simultaneously clobbered regs (
	//( currently, only one at time can be clobbered )
	if ( regs & BIT( rEAX ) ) {
		Clobber( rEAX );
	}
	else if ( regs & BIT( rEDX ) ) {
		//Assert2( (regs & ~BIT( rEDX )) == 0, "only EDX can be clobbered" );
		Clobber( rEDX );
	}
	else if ( regs & BIT( rECX ) ) {
		Clobber( rECX );
	}
	else {
		Unimplemented();
	}
}

//UNDONE:
void Machine_x86::UnclobberRegisters( RegMask_t  regs ) {
	if ( regs == 0 ) {
		return;
	}
// currently, only one register can be unclobbered
	if ( regs & BIT( rEAX ) ) {
		m_registers[ rEAX ].SetFree();
	}
	else if ( regs & BIT( rEDX ) ) {
		m_registers[ rEDX ].SetFree();
	}
	else if ( regs & BIT( rECX ) ) {
		m_registers[ rECX ].SetFree();
	}
	else {
		Unimplemented();
	}
}

void Machine_x86::EmitOperandNameIntoBuffer( const Operand * operand )
{
	if ( b_operands_must_be_of_given_size )
	{
		if ( m_operand_size == operand->GetSize() ) {
			EmitIntoBuffer( operand->Name() );
			return;
		}

		switch ( operand->GetType() ) {
		case eDefault :
			// nothing
			break;

		case eImmediate :
			//TODO: make sure that the value fits into the given size
			EmitIntoBuffer( operand->Name() );
			return;

		case eMemory :
			//TODO: check for errors
			EmitIntoBuffer( operand->Name() );
			return;

		case eRegister :
			{
				const RegisterOperand * pRegOp = static_cast< const RegisterOperand* >( operand );
				RegCode_t reg = pRegOp->GetRegisterCode();

				const EOperandSize	currSize = GetSizeOfRegister( reg );
				if ( m_operand_size == BYTE_PTR )
				{
					const Register * pByteReg = GetLowerByteRegister( reg );
					
					if ( ! pByteReg ) {
						Register & rFreeReg = GetFreeGeneralReg32();
						EmitMove( rFreeReg, GetRegisterByCode( reg ) );
						pByteReg = GetLowerByteRegister( rFreeReg.GetCode() );
					}
					
					EmitIntoBuffer( pByteReg->Name() );
				}
				else if ( m_operand_size == WORD_PTR )
				{
					const Register * pWordReg = GetLowerWordRegister( reg );
					
					if ( ! pWordReg ) {
						Register & rFreeReg = GetFreeReg32();
						EmitMove( rFreeReg, GetRegisterByCode( reg ) );
						pWordReg = GetLowerWordRegister( rFreeReg.GetCode() );
					}
					
					EmitIntoBuffer( pWordReg->Name() );
				}
				else if ( m_operand_size == DWORD_PTR ) {
					//UNDONE:
					EmitIntoBuffer( operand->Name() );
				}
				else {
					Unimplemented();
				}
			}
			break;

		default: Unreachable();
		}
	}
	else {
		EmitIntoBuffer( operand->Name() );
	}
}

}//end namespace c4