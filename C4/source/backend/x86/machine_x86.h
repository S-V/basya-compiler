/*
=============================================
	A very simple on-the-fly code generator for x86 processors.
	It's pretty fast, but generates truly terrible code
	without any optimizations.

	1) Instruction selection
	2) Register allocation
	3) Code emission
=============================================
*/
#ifndef __MACHINE_INTEL_386_H__
#define __MACHINE_INTEL_386_H__

#define DEBUG_I386

namespace c4 {

#include "common/x86_codegen.h"

/*
=============================
	class OperandLocation
	TODO: rename this 'Location'
=============================
*/
class OperandLocation {
public:
				OperandLocation( RegCode_t  reg );
				OperandLocation( RegMask_t  regs );
				OperandLocation( const operand_info_t & info );

	bool	IsReg8() const;
	bool	IsReg16() const;
	bool	IsReg32() const;

	bool	IsRegister( RegCode_t  reg ) const;
	bool	IsRegisters( RegMask_t  regs ) const;

	EOperandType	GetType() const { return ( EOperandType )m_info->type; }
	int				GetData() const { return m_info->data; }

private:
	const operand_info_t *	m_info;
};

// forward declarations
class Machine_x86;

/*
====================================================
	class Operand represents a generic operand

	a tree of such nodes is constructed
	from IR tree and then used for code generation
====================================================
*/
class Operand {
protected:

	friend class Machine_x86;

				Operand( int ruleNumber );
	virtual		~Operand();

public:
	virtual EOperandType	GetType() const = 0;
	virtual EOperandSize	GetSize() const = 0;

	int			GetRuleNumber() const { return m_rule_number; }
	//void		SetRuleNumber( int rule ) { m_rule_number = rule; }

	int			GetNumOperands() const;

	Operand *&	GetOperand( int num );
	void		SetOperand( int num, Operand *value );

	// Has this operand been emitted as assembler?
	bool		IsEmitted() const { return m_bEmitted; }
	void		SetEmitted() { m_bEmitted = true; }

	bool		IsLeaf() const;

	Operand **	GetPtr() { return m_operands; }

protected:
	int		m_rule_number;	// rule number, associated with this node

	Operand *	m_operands[ MAX_NUM_OPERANDS ];	// pointers to child nodes
	//int			m_numOperands;

	bool		m_bEmitted; // true, if this node has already been emitted ( as assembly language construct )
	
private: //---- reference counting -------------------
	mutable uint	m_numReferences;

public:
	uint	GetReferenceCount() const;
	void	Grab();
	bool	Drop();

public:
	virtual const char *	Name() const = 0;	// for inserting names into assembly listings
	virtual void			Dispose() {} // call this if this operand is no longer needed

	// call Drop() on my children;
	// those children that are not referenced, will be deleted;
	// used after emitting assembler for this operand;
	// leaves this node without any child nodes ( operands )
	//
	void		DropChildren();
};
//------------------------------------------------------------------------
// operand with access to an intermediate instruction
//
class LeafOperand : public Operand {
protected:
		LeafOperand( IRCode *insn, int ruleNumber );

protected://private:
	IRCode *	m_insn;
public:
	IRCode *	GetInsn() { return m_insn; }
	// overriden
	const char *	Name() const;
};
//------------------------------------------------------------------------
class MemoryOperand : public LeafOperand {
public:
	MemoryOperand( IRCode *insn, int ruleNumber );

	virtual EOperandType  GetType() const { return eMemory; }
	virtual EOperandSize  GetSize() const;
};

//------------------------------------------------------------------------
class ImmediateOperand : public LeafOperand {
public:
	ImmediateOperand( IRCode *insn, int ruleNumber );

	virtual EOperandType  GetType() const { return eImmediate; }
	virtual EOperandSize  GetSize() const;
};

//------------------------------------------------------------------------
// operand, representing transition from one rule to another;
// it always has only one children ( representing rhs nonterminal )
//
class InsnOperand : public LeafOperand {
public:
		InsnOperand( IRCode *insn, int ruleNumber );

	virtual EOperandType  GetType() const { return eDefault; }
	virtual EOperandSize  GetSize() const;
};

//------------------------------------------------------------------------
// operand with a given name
class NamedOperand : public Operand {
protected:
	NamedOperand( const char *name, int rule )
		: Operand( rule ), m_name( name )
	{}

	String		m_name;
public:
	virtual const char * Name() const { return m_name.c_str(); }
};
//------------------------------------------------------------------------
class DebugOperand : public NamedOperand {
public:
	DebugOperand( int rule, const char *debugName )
		: NamedOperand( debugName, rule )
	{}

	virtual EOperandType  GetType() const { return eDebug; }
	virtual EOperandSize  GetSize() const { return SIZE_ERROR; }
};
//------------------------------------------------------------------------

/*
=============================
	class MemoryLocation

	represents labels;
	cannot have children nodes;

	TODO: conflicts with the classes MemoryOperand and OperandLocation,
		rename this into 'class Label'
=============================
*/
class MemoryLocation : public NamedOperand {
public:
	MemoryLocation( const char *nameOfLabel, int rule )
		: NamedOperand( nameOfLabel, rule )
	{
		size = BYTE_PTR;
	}

	virtual EOperandType  GetType() const { return eMemory; }
	virtual EOperandSize  GetSize() const { return VARIABLE_SIZE; }

public:
	// what shall we use: 'db', 'dw', 'dd', ... ?
	// ??? should GetSize() return this 'size' ???
	EOperandSize	size;

	// contains bytes of data to be emitted in DATA segment;
	// TODO: make specialized classes for storing data
	// ( with arrays of bytes ( or ints, floats, doubles, etc. ) instead of strings )
	String		data;
};

//------------------------------------------------------------------------

class RegisterOperand;

/*
=============================
	class Register
	represents a hardware register
=============================
*/
class Register
{
	friend class Machine_x86;
	
	void	Init( RegCode_t code, char* name );

private:
	mutable RegCode_t	m_code;
	mutable char *		m_name;

	bool	m_bFree;		// true => this register is free

	RegisterOperand *	m_operand; // ptr to the referring operand	

private:
			Register();
			Register( RegCode_t code, char* name );
public:
	RegCode_t		GetCode() const { return m_code; }
	const char *	Name() const { return m_name; }

	// if this register is a subregister then return that subregister, otherwise, return NULL
	Register *		IsSubRegister();

	bool	IsFree() const;
	bool	IsUsed() const;
	void	SetUsed();
	void	SetFree();

	void	SetOperand( RegisterOperand *operand );

	// may return NULL
	RegisterOperand *	GetOperand();
};

//------------------------------------------------------------------------
// operand, located in a single register ( TODO: operands in several registers ( "EDX:EDX", etc. ) )
//
class RegisterOperand : public Operand {
public:
		RegisterOperand( Register & reg, int ruleNumber );

		// frees the occupied register
		~RegisterOperand();

	Register *	GetRegister();
	RegCode_t	GetRegisterCode() const;
	void		SetRegister( Register & reg );

private:
	Register *		m_register;

public:
	// overrides
	const char *	Name() const;
	void			Dispose();

	EOperandType	GetType() const;
	EOperandSize	GetSize() const;
};

/*
=======================================================
	class Machine_x86

	TODO: make this a sort of processor emulator
	( for code selection based on context information
	and other types of optimizations, profiling,
	opcode interpretation, etc. )
=======================================================
*/
class Machine_x86 {
public:
					Machine_x86();
					~Machine_x86();

	bool		GenerateCode( IRList_t & list );	
		
	void		EmitStandardHeader();
	void		EmitStandardEnding();

	void		Emit_TEXT_begin();
	void		Emit_TEXT_end();

private:
	virtual bool	GenerateCode( IRCode * node );
private:
	//--- tree pattern matching ---------------------------------------------------

	// current pattern matching algorithm is static by nature;
	// only constant integer costs are allowed;
	// TODO: dynamic code generation, context-based instruction selection

					// try to cover the intermediate tree with minimum cost
	state_s *		Label( IRCode * irTree );

	enum { START_NONTERMINAL = 1 };

					// returns true if the given tree is labelled
	bool			CheckCover( IRCode * irTree, int start_nonterminal = START_NONTERMINAL );

	//--- generated from machine description --------------------------------------
	
					// records minimum cost matches
	state_s *		_GetState( IRCode * insn, state_s* left, state_s* right );
	
					// stores the kids of the given tree into the array kids[];
					// returns 'kids' and 'numKids'
	void			_GetKids( IRCode * irTree, int ruleNumber, IRCode * kids[], int *numKids );

	//--- debugging -------------------------------------

					// writes out the optimal cover for the given labeled tree
	void			_DumpCover( IRCode * irTree, int nonterm, int indent = 0 );

					// writes out assembler templates selected for the given labeled tree;
					// returns the last rule number
	int				_DumpTemplates( IRCode * irTree, int nonterm );

	//--- code generation -------------------------------
						// emits the given node and creates a new operand
	Operand *		EmitNode( IRCode * node, int start_nonterminal, OperandLocation *resultLocation = 0 );
	Operand *	EmitInstruction( IRCode *insn, int ruleNumber, Operand **operands, int numOperands, const OperandLocation *pLocation );
	Operand *		CreateOperand( IRCode *insn, int ruleNumber, Operand ** pOperands, const OperandLocation *pLocation );

					// executes the semantic action associated with the given node
					// prior to emitting assembly;
					// 'pOperands' is a pointer to an array [0..MAX_NUM_OPERANDS] of ( Operand* )
	void			_ExecuteConstructor( IRCode *insn, int ruleNumber, Operand *& node, Operand ** pOperands );
		
	void			_ExecuteAction( IRCode *insn, int ruleNumber, Operand *& node, Operand ** operands );

	void			_ExecuteDestructor( IRCode *insn, int ruleNumber, Operand *& node );

	void			EnsureOperandLocation( Operand *& operand, const OperandLocation & location );
	void			EnsureOperandsAreInRightPlaces( const InstructionData & insnData, Operand ** pOperands );
	OperandLocation *SelectBestLocationForNewOperand( OperandLocation *pLoc1, OperandLocation *pLoc2 );

			// emits assembly language code for the given node into the staging buffer
	void	EmitAssembler( Operand * operand );

	// a small patch to improve codegen

	// If the result of this instruction must be in a certain register
	// and one of the operands is in this register, and the operands satisfy
	// all constraints, then emit 'xchg' and return a new operand, otherwise, don't do anything and return NULL.
	Operand *	EmitXchg( RegCode_t  regForResult, Operand ** operands, int numOperands );

	void	EmitOperandNameIntoBuffer( const Operand * operand );

public:
	const InstructionData &	GetInsnData( int ruleNumber ) const;	

	bool	IsInstruction( int ruleNumber ) const;

private:
	//---------------------------------------------------------

	// to determine which flags have been changed by the last instruction, etc.
	side_effects_t		m_current_side_effects;


	// side effects of the next instruction
	// ( one instruction lookahead for improving register allocation and assignment )
	side_effects_t		m_future_side_effects;

	void	SetFutureSideEffects( const side_effects_t & rSideEffects );
	void	ResetFutureSideEffects();

private: //--- code generation ----------------------------------

	Register &	GetRegisterByCode( RegCode_t  regCode );

	// example: if 'regCode' == rEAX then return AL;
	// if 'regCode' == rESI then return NULL
	const Register *	GetLowerByteRegister( RegCode_t  regCode );
	const Register *	GetLowerWordRegister( RegCode_t  regCode );

	// returns a free 32-bit register
	Register &	GetFreeReg32();
	
	// returns a free 32-bit general-purpose register
	Register &	GetFreeGeneralReg32();	

	// frees the specified register
	Register &	GetFreeRegister( RegCode_t  regCode );

	void	EnsureFree( RegCode_t  regCode );
	void	EnsureFree( Register & reg );

	void	Clobber( RegCode_t  regCode );
	void	Clobber( Register & reg );

	// moves operand from 'src' to 'dest', frees the source register;
	// issues an error if 'dest' == 'src'
	void	EmitMove( Register & dest, Register & src );

	// if the given operand is already in 'dest', then return without any errors
	void	MoveOperandIntoRegister( Register & dest, Operand *& operand );

	void	PerformSideEffects( const side_effects_t & rSideEffects );
	void	UndoSideEffects( const side_effects_t & rSideEffects );

	void	ClobberRegisters( RegMask_t  regs );
	void	UnclobberRegisters( RegMask_t  regs );

private:
	Register	m_registers[ NUM_OF_REGS ];

	// registers that are going to be clobbered
	//RegMask_t	m_clobberedRegs;

	// for selecting correct opcodes ( 'jle', 'ge', etc. )
	ICode_t		m_last_cond_opcode;
	bool		m_is_signed;

private:
	//--- initialization ----------------------------------------
					// initialize everything; called in ctor
	void			Init();


	

	// for measuring the count of generated instructions
	int		m_numInstructions;

	// if true, then all the operands of the current instruction
	// must have the same size as the first operand;
	// ( for example, if 'operand[0]' is 'byte ptr [c]' and 'operand[1]' is EDX,
	// then 'operand[0]' will become 'DL'.
	bool			b_operands_must_be_of_given_size;
	EOperandSize	m_operand_size;

public:
	// resets the counter of emitted instructions, list with constants, etc.;
	// usually called before generating a user-defined portion of code
	void	Reset();

	int		GetNumberOfInstructions() const { return m_numInstructions; }

private:	//--- emitting assembly listings --------------------

					// writes the given string into the output file
	void			Emit( const char* format, ... );

	void			Print( const char* format, ... );	// diagnostics

	// for inserting tabs and spaces ( writing "mov \t eax, ecx" instead of "mov eax, ecx" )
	char *	m_spacing;
	char *	m_tab;
	
	File	m_file;	// for writing into the output file
	enum { BUFFER_SIZE = 256 };
	char	m_temp_buf[ BUFFER_SIZE ];	// for temporary string storage

	//---------------------------------------------------
	// instructions are first written into this buffer
	char	m_instruction_buffer[ BUFFER_SIZE ];

	void	EmitIntoBuffer( const char* format, ... );
	void	EmitInstructionInBuffer();
	void	ClearInstructionBuffer();

	//---------------------------------------------------
	//TODO: remove this kludge

	// emits assembly into the given string
	void	EmitOperandIntoString( Operand *operand, String *pStr );
	//---------------------------------------------------

	//MemoryLocation *	DefineConstant( const char *string );
/*
						// create an operand standing for labels
	MemoryLocation *	DefineConstant_Float32( float value, int ruleNumber );
	MemoryLocation *	DefineConstant_Float64( double value, int ruleNumber );
*/
public:
	// TODO: remove this in the future:
	void	Emit_CONST_Segment( IRList_t & rList );
};

// TODO: get rid of this global var!!!
extern Machine_x86 *  g_Machine;

}//end namespace c4

#endif // !__MACHINE_INTEL_386_H__