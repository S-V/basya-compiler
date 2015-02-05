// common stuff shared by the code generator generator ( iburg )
// and the code generator

#ifndef __X86_CODEGEN_H__
#define __X86_CODEGEN_H__

// max. number of operands in tree patterns ( instructions )
enum { MAX_NUM_OPERANDS = 5 };

//----------------------------------------------------
typedef enum REGISTER_CODES {
	rEAX,	rAX,	rAL,	rAH,	
	rECX,	rCX,	rCL,	rCH,
	rEDX,	rDX,	rDL,	rDH,

	rESP,	rSP,
	rEBP,	rBP,
	rEBX,	rBX,	rBL,	rBH,

	rESI,	rSI,
	rEDI,	rDI,

	rES,	rCS,	rSS,	rDS,	rFS,	rGS,

	rRRR,	// for debugging

	NUM_OF_REGS
} RegCode_t;

//enum {
//	NUM_OF_GENERAL_32_BIT_REGS	= 5,
//	NUM_OF_ALL_32_BIT_REGS		= 8,
//};

typedef int32  RegMask_t;

//----------------------------------------------------

inline bool IsReg32( RegCode_t  regCode ) {
	return regCode == rEAX || regCode == rECX || regCode == rEBX || regCode == rEDX 
		|| regCode == rESI || regCode == rEDI || regCode == rEBP || regCode == rESP;
}

//----------------------------------------------------
typedef enum PROCESSOR_FLAGS {
	CF	= BIT( 0 ),
	
	PF	= BIT( 2 ),
	
	AF	= BIT( 4 ),

	ZF	= BIT( 6 ),
	SF	= BIT( 7 ),

	TF	= BIT( 8 ),		// Trap flag
	IF	= BIT( 9 ),		// Interrupt enable flag

	DF	= BIT( 10 ),
	OF	= BIT( 11 ),

	// Bits 12 and 13 - I/O privilege level flag
	IOPL_1 = BIT( 12 ),
	IOPL_2 = BIT( 13 ),

	NT	= BIT( 14 ),	// Nested task flag
	RF	= BIT( 16 ),	// Resume flag
	VM	= BIT( 17 ),	// Virtual-8086 mode flag
	AC	= BIT( 18 ),	// Alignment check flag
	VIF	= BIT( 19 ),	// Virtual interrupt flag
	VIP	= BIT( 20 ),	// Virtual interrupt pending flag
	ID	= BIT( 21 ),	// Identification flag

} EFLAGS_t;

typedef int32	Flags_t;

/*
===========================
			Bits	Bytes
---------------------------
byte		8		1
word		16		2
dword		32		4
fword		48		6
pword		48		6
qword		64		8
tbyte		80		10
tword		80		10
dqword		128		16
===========================
*/
typedef enum {
	SIZE_ERROR = 0,

	BYTE_PTR	= 1,
	WORD_PTR	= 2,
	DWORD_PTR	= 4,
	QWORD_PTR	= 8,

	// immediate values can be addressed in many correct ways
	// ( as 'byte ptr', 'word ptr', etc.. )
	VARIABLE_SIZE

} EOperandSize;

//----------------------------------------------------------------------

inline EOperandSize  GetSizeOfRegister( RegCode_t  reg ) {
	// TODO: use some sort of a lookup table
	switch ( reg ) {
		case rEAX:  case rECX:	case rEDX:	case rEBX:
		case rESI:	case rEDI:
			return DWORD_PTR;

		case rAX:	case rBX:	case rCX:	case rDX:
			return WORD_PTR;

		case rAL:	case rAH:
		case rBL:	case rBH:
		case rCL:	case rCH:
		case rDL:	case rDH:
			return BYTE_PTR;

		// TODO: other cases
		default: Unreachable();
	}
}

//----------------------------------------------------------------------
typedef enum {
	eDefault,			// the result is rhs nonterminal

	eImmediate,
	eMemory,

	eReg_8,
	eReg_16,
	eReg_32,			// a general-purpose 32-bit register

	eRegister,			// the result (operand) must be in the specified register

	// not actually an operand type,
	// used for codegen
	eResultInOperand,	// the result is stored in some operand ( '%0', '%1', ... )	


	eDebug,	// for debugging

} EOperandType;

typedef struct operand_info_s {
	int32		type; // EOperandType
	int32		data;
//	RegMask_t		usedRegs;
//	Flags_t			usedFlags;
} operand_info_t;

typedef struct side_effects_s {
	RegMask_t	clobberedRegs;
	Flags_t		modifiedFlags;
} side_effects_t;

//----------------------------------------------------
enum INSTRUCTION_FLAGS
{
	// != 0 => this can be emitted as an assembler statement
	IS_INSTRUCTION		= BIT( 0 ),

	HAS_CONSTRUCTOR		= BIT( 1 ),
	HAS_SEMANTIC_ACTION	= BIT( 2 ),
	HAS_DESTRUCTOR		= BIT( 3 ),

	// for debugging; != 0 => assembler template contains "%c"
	// ( where the result name will be inserted )
	//
	HAS_RESULT_NAME_IN_TEMPLATE = BIT( 4 )
};

enum INSTRUCTION_ATTRIBUTES 
{
	// != 0 => children will not be emitted by the code generator itself
	// ( instead they can be emitted in the semantic action )
	//
	DO_NOT_EMIT_CHILDREN	= BIT( 0 ),
	
	// emit children in order of complexity, if DO_NOT_EMIT_CHILDREN == 0
	// ( the most complex children are emitted first,
	// which usually results in better code )
	//
	EMIT_COMPLEX_CHILDREN_FIRST	= BIT( 1 ),

	// example: if we need to move the value of EAX into "byte ptr [address]",
	// then we'll take the lower byte of EAX:  "mov  byte ptr [address], AL";
	// movzx and movsx ( and other conversion operators ) will have this flag set to zero
	OPERANDS_MUST_HAVE_EQUAL_SIZE = BIT( 2 )
};

typedef struct attributes_s {
	int32	flags;	// INSTRUCTION_ATTRIBUTES
	int32	data;
} attributes_t;

//----------------------------------------------------

typedef struct instruction_data_s
{
	const char *	name;

	int32		flags;	// bit flags containing INSTRUCTION_FLAGS

	// where the result is stored
	operand_info_t		result;

	// where the operands must be located
	operand_info_t		operands[ MAX_NUM_OPERANDS ];
	
	// which registers are clobbered, processor flags changed, etc.
	side_effects_t		side_effects;

	attributes_t	attributes;	// additional information about instruction

	int		numChildNodes;
	int		numOperandsInTemplate; // for error-checking

} InstructionData;

#endif // !__X86_CODEGEN_H__