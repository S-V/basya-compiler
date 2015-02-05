%operator	nop
%operator	iconst	iconst_0	iconst_1	iconst_2	iconst_4	iconst_8
%operator	fconst

%operator	address_8	address_16	address_32	address_64	address_f32	address_f64
%operator	indir_i8	indir_u8
%operator	indir_i16	indir_u16
%operator	indir_i32
%operator	indir_i64
%operator	indir_f32	indir_f64
%operator	assign
%operator	add		sub		mul
%operator	div		udiv	fdiv
%operator	rem		urem

%operator	neg		fneg

%operator	preinc	predec
%operator	postinc	postdec

%operator	and
%operator	or
%operator	xor
%operator	not
%operator	compl

%operator	shl		shr		ushr

%operator	eq		neq
%operator	lt		gt
%operator	le		ge

// type conversion operators
%operator	i2f		f2i
%operator	i2b		f2b

%operator	jmp
%operator	br_false	br_true
%operator	je  jge  jg  jle  jl
%operator	label

%operator	func_start	func_end
%operator	call		arg		ret
%%

// TODO: clean up this mess with address:

address:	address_8	"byte ptr [%0]"		%ctor{ $$ = new MemoryOperand( insn, rulenum ); }
address:	address_16	"word ptr [%0]"		%ctor{ $$ = new MemoryOperand( insn, rulenum ); }
address:	address_32	"dword ptr [%0]"	%ctor{ $$ = new MemoryOperand( insn, rulenum ); }

mem_s8:		indir_i8( address_8 )	"byte ptr [%0]"		%ctor{ $$ = new MemoryOperand( insn, rulenum ); }
mem_u8:		indir_u8( address_8 )	"byte ptr [%0]"		%ctor{ $$ = new MemoryOperand( insn, rulenum ); }

mem_s16:	indir_i16( address_16 ) "word ptr [%0]"		%ctor{ $$ = new MemoryOperand( insn, rulenum ); }
mem_u16:	indir_u16( address_16 ) "word ptr [%0]"		%ctor{ $$ = new MemoryOperand( insn, rulenum ); }

mem:	indir_i32( address_32 ) "dword ptr [%0]"	%ctor{ $$ = new MemoryOperand( insn, rulenum ); }

imm:	iconst		"%0"	%ctor{ $$ = new ImmediateOperand( insn, rulenum ); }
imm:	iconst_0	"%0"	%ctor{ $$ = new ImmediateOperand( insn, rulenum ); }
imm:	iconst_1	"%0"	%ctor{ $$ = new ImmediateOperand( insn, rulenum ); }
imm:	iconst_2	"%0"	%ctor{ $$ = new ImmediateOperand( insn, rulenum ); }
imm:	iconst_4	"%0"	%ctor{ $$ = new ImmediateOperand( insn, rulenum ); }
imm:	iconst_8	"%0"	%ctor{ $$ = new ImmediateOperand( insn, rulenum ); }

iconst_2_4_8: iconst_2 "2"	%ctor{ $$ = new ImmediateOperand( insn, rulenum ); } 
iconst_2_4_8: iconst_4 "4"	%ctor{ $$ = new ImmediateOperand( insn, rulenum ); }
iconst_2_4_8: iconst_8 "8"	%ctor{ $$ = new ImmediateOperand( insn, rulenum ); }

mc:		mem		"%0"
mc:		imm		"%0"

mr:		mem		"%0"
mr:		reg		"%0"

rc:		reg		"%0"
rc:		imm		"%0"

// memory or register or constant
mrc:	rc		"%0"
mrc:	mem		"%0"

/*
==================================
	Type conversion instructions
==================================
	cwd and cdq double the size fo value ax or eax register respectively
and store the extra bits into the dx of edx register.
The conversion is done using the sign extension. These instructions have no m_operands.
	cbw extends the sign of the byte in al throughout ax, and cwde extends the sign of the word in ax throughout eax.
These instructions also have no m_operands.
	movsx converts a byte to word or double word and a word to double word using the sign extension.
movzx does the same, but it uses the zero extension. The source operand can be general register
or memory, while the destination operand must a general register.
* /
/*
The MOVZX instuction has a prefix and takes 3 cycles
to execute ( a total of 4 cycles ). As with the Intel486 CPU, the following code sequence is
recommended instead of using the MOVZX instruction with a prefix:
	xor eax, eax
	mov al, mem
*/

// TODO: should be 'indir_i8( address )'

[ r32: mem_s8
	"movsx %c, %0"
	<4>
]

[ r32: mem_u8
	"movzx %c, %0"
	<4>
]

[ r32: mem_s16
	"movsx %c, %0"
	<3>
]

[ r32: mem_u16
	"movzx %c, %0"
	<3>
]

/*
==================================
	Data movement instructions
==================================
*/
[ reg: imm
	[{ "ensure_operands_have_equal_size" }]
	"mov %c, %0"
	<3>
]

[ reg: iconst_0
	"xor %c, %c"
	<1>
]

[ reg: mem
	[{ "ensure_operands_have_equal_size" }]
	"mov %c, %0"
	<3>
]

[ %1: assign( address, rc )
	[{ "ensure_operands_have_equal_size" }]
	"mov %0, %1"	
	<2>
]
stmt: rc

[ %0: assign( reg, mrc )
	[{ "ensure_operands_have_equal_size" }]	
	"mov %0, %1"
	<2>
]

/*
==================================
	Binary arithmetic instructions
==================================
*/

//------- Addition ------------------------------
[ %0: add( reg, mrc )
	!{ set: CF }
	[{ "emit_the_most_complex_nodes_first" }]
	"add %0, %1"	
	<6>
]

[ %0: add( mr, rc )
	!{ set: CF }
	[{ "emit_the_most_complex_nodes_first" }]
	"add %0, %1"
	<7>
]

// inc adds one to the operand, it doesn't affect CF
[ %0: add( reg, iconst_1 )
	"inc %0"
	<5>
]//inc

// TODO: lea 		"result_can_be: %0" */
[ r32: add( mul( r32, iconst_2_4_8 ), rc )
	[{ "emit_the_most_complex_nodes_first" }]
	"lea %c, [%0*%1 + %2]"	
	<3>
]

//TODO: lea optimization:
// lea eax,[2*edx] -> lea eax,[edx+edx]
// lea eax,[1+5*(edx+1)] -> lea eax,[6+4*edx+edx]
// 2*j + 9
// mov eax, [j]
// lea eax, [eax + eax + 9]

// TODO: "result_can_be: %0, %2"
[ r32: add( add( mul( r32, iconst_2_4_8 ), r32 ), imm )
	[{ "emit_the_most_complex_nodes_first" }]
	"lea %c, [%2 + %0*%1 + %3]"
	<3>
]

[ r32: add( add( r32, imm ), r32 )
	[{ "emit_the_most_complex_nodes_first" }]
	"lea %c, [%0 + %2 + %1]"
	<3>
]
[ r32: add( add( r32, r32 ), imm )
	[{ "emit_the_most_complex_nodes_first" }]
	"lea %c, [%0 + %1 + %2]"
	<3>
]

//------- Subtraction ------------------------------
[ %0: sub( reg, mrc )
	!{ set: CF }
	[{ "emit_the_most_complex_nodes_first" }]
	"sub %0, %1"
	<6>
]

[ %0: sub( mr, rc )
	!{ set: CF }
	[{ "emit_the_most_complex_nodes_first" }]
	"sub %0, %1"
	<6>
]

[ %0: sub( reg, iconst_1 )
	"dec %0"
	<5>
]//dec

//----------------------------------------------------
// ex.: b * 8 + c - 1
// mov	eax, [b]
// mov	ecx, [c]
// lea	eax, [ecx + eax*8 - 1]
[ r32: add( mul( r32, iconst_2_4_8 ), sub( r32, imm ) )
	[{ "emit_the_most_complex_nodes_first" }]
	"lea %c, [%2 + %0*%1 - %3]"
	<3>
]

// ex.: b*8 + c + 1
[ r32: add( mul( r32, iconst_2_4_8 ), add( r32, imm ) )
	[{ "emit_the_most_complex_nodes_first" }]
	"lea %c, [%2 + %0*%1 + %3]"
	<3>
]

//------- Multiplication ------------------------------
/* TODO:
[ AX : mul( AL, byteOperand )
	?{ "unsigned" }
	!{ set: CF, OF }
	"mul %1"
	<11>
]//mul byte_operand
*/

//TODO: r32: mul( reg, imm )  "imul %c, %0, %1"
[ reg: mul( reg, imm )
	?{ "signed" }
	!{ set: CF, OF }
	[{ "result_can_be_%0" }]
	"imul %c, %0, %1"
	<13>
]//imul reg, reg, imm

[ %0: mul( reg, imm )
	?{ "signed" }
	!{ set: CF, OF }
	"imul %0, %0, %1"
	<13>
]//imul reg, reg, imm

[ %0: mul( reg, mrc )
	?{ "signed" }
	!{ set: CF, OF }
	[{ "emit_the_most_complex_nodes_first" }]
	"imul %0, %1"
	<14>
]//imul reg, mrc

[ r32: mul( mem, imm )
	?{ "signed" }
	!{ set: CF, OF }
	"imul %c, %0, %1"
	<14>
]//imul reg, mem, imm

//------- Division ------------------------------
[ EAX: div( EAX, mr )
	?{ "signed" }
	!{ clobber: EDX }
	[{ "emit_the_most_complex_nodes_first" }]
	"cdq\nidiv %1"
	<14>
]

[ EAX: udiv( EAX, mr )
	?{ "unsigned" }
	!{ clobber: EDX }
	[{ "emit_the_most_complex_nodes_first" }]
	"xor edx, edx\ndiv %1"
	<14>
]

//------- Remainder ------------------------------
[ EDX: rem( EAX, mr )
	!{ clobber: EDX }
	[{ "emit_the_most_complex_nodes_first" }]
	"cdq\nidiv %1"
	<14>
]

//------- Shifts ---------------------------------
[ %0: shl( r32, imm )
	!{ set: CF }
	"shl %0, %1"	// or 'sal'
	<6>
]
[ %0: shl( r32, ECX )
	!{ set: CF }
	[{ "emit_the_most_complex_nodes_first" }]
	"shl %0, cl"	// or 'sal'
	<7>
]

[ %0: shr( r32, imm )
	"sar %0, %1"
	<6>
]
[ %0: shr( r32, ECX )
	[{ "emit_the_most_complex_nodes_first" }]
	"sar %0, cl"
	<7>
]

[ %0: ushr( r32, imm )
	"shr %0, %1"
	<6>
]
[ %0: ushr( r32, ECX )
	[{ "emit_the_most_complex_nodes_first" }]
	"shr %0, cl"
	<7>
]

//==--------------------------------------==//

[ %0: mul( r32, iconst_2 )
	"shl %0, 1"
	<7>
]
[ %0: mul( r32, iconst_4 )
	"shl %0, 2"
	<7>
]
[ %0: mul( r32, iconst_8 )
	"shl %0, 3"
	<7>
]
[ %0: div( r32, iconst_2 )
	"shr %0, 1"
	<7>
]
[ %0: div( r32, iconst_4 )
	"shr %0, 2"
	<7>
]
[ %0: div( r32, iconst_8 )
	"shr %0, 3"
	<7>
]
[ %0: udiv( r32, iconst_2 )
	"shr %0, 1"
	<7>
]
[ %0: udiv( r32, iconst_4 )
	"shr %0, 2"
	<7>
]
[ %0: udiv( r32, iconst_8 )
	"shr %0, 3"
	<7>
]

//---------------------- Negation -----------------------------------------------
[ %0: neg( reg )
	"neg %0"
	<5>
]

// TODO: negate memory : a = -a;  ->  neg dword ptr [a]

//--------------------------------------------------------------------------------
// Bitwise operations

[ %0: and( reg, mrc )
	!{ set: SF, ZF, PF }
	[{ "emit_the_most_complex_nodes_first" }]
	"and %0, %1"
	<5>
]

[ %0: or( reg, mrc )
	!{ set: SF, ZF, PF }
	[{ "emit_the_most_complex_nodes_first" }]
	"or %0, %1"
	<5>
]

[ %0: xor( reg, mrc )
	!{ set: SF, ZF, PF }
	[{ "emit_the_most_complex_nodes_first" }]
	"xor %0, %1"
	<5>
]

//TODO:
[ %0: not( reg )
	""
	<5>
]

[ %0: compl( reg )
	"not %0"
	<5>
]

//==----- Boolean operations -----------------------------------------------------

//i2b		f2b

/*
[ ECX: i2b( mr )
	!{ set: FLAGS }
	"cmp %0, 0\nsetne cl"
	<10>
]
*/
[ r32: i2b( mr )
	""
	<7>
	%ctor{ Register & reg = GetFreeGeneralReg32();
		String  s;	EmitOperandIntoString( $0, &s );
		Emit( "cmp%s%s, 0\n", m_spacing, s.c_str() );
		Emit( "setne%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		++m_numInstructions; // m_numInstructions will be incremented because it's an instruction
		$$ = new RegisterOperand( reg, rulenum );
	}
]

[ r32: i2b( reg )
	""
	<6>
	%ctor{ Register & reg = GetFreeGeneralReg32();
		String  s;	EmitOperandIntoString( $0, &s );
		Emit( "test%s%s, %s\n", m_spacing, s.c_str(), s.c_str() );
		Emit( "setne%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		++m_numInstructions; // m_numInstructions will be incremented because it's an instruction
		$$ = new RegisterOperand( reg, rulenum );
	}
]

//--------------------------------------------------------------------------------
// Comparisons

/*HACK: */
[ r32: gt( reg, mrc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );

		String  temp;
		EmitOperandIntoString( $1, &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, $0->Name(), temp.c_str() );

		Emit( "setg%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );

		m_numInstructions += 2; // m_numInstructions will be incremented because it's an instruction

		$$ = new RegisterOperand( reg, rulenum );
	}
]
[ r32: gt( mem, rc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );

		String  s0;	EmitOperandIntoString( $0, &s0 );
		String  s1;	EmitOperandIntoString( $1, &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );

		Emit( "setg%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2; // m_numInstructions will be incremented because it's an instruction

		$$ = new RegisterOperand( reg, rulenum );
	}
]
/* */
//--------------------------------------------------------------------------------------
[ r32: eq( reg, mrc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  temp;		EmitOperandIntoString( $1, &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, $0->Name(), temp.c_str() );
		Emit( "sete%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		$$ = new RegisterOperand( reg, rulenum ); }
]
[ r32: eq( mem, rc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		
		String  s0;	EmitOperandIntoString( $0, &s0 );
		String  s1;	EmitOperandIntoString( $1, &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );

		Emit( "sete%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		$$ = new RegisterOperand( reg, rulenum ); }
]
//--------------------------------------------------------------------------------------
[ r32: neq( reg, mrc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  temp;		EmitOperandIntoString( $1, &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, $0->Name(), temp.c_str() );
		Emit( "setne%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		$$ = new RegisterOperand( reg, rulenum ); }
]
[ r32: neq( mem, rc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  s0;	EmitOperandIntoString( $0, &s0 );
		String  s1;	EmitOperandIntoString( $1, &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );
		Emit( "setne%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		$$ = new RegisterOperand( reg, rulenum ); }
]
//--------------------------------------------------------------------------------------
[ r32: lt( reg, mrc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  temp;		EmitOperandIntoString( $1, &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, $0->Name(), temp.c_str() );
		Emit( "setl%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		$$ = new RegisterOperand( reg, rulenum ); }
]
[ r32: lt( mem, rc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  s0;	EmitOperandIntoString( $0, &s0 );
		String  s1;	EmitOperandIntoString( $1, &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );
		Emit( "setl%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		$$ = new RegisterOperand( reg, rulenum ); }
]
//--------------------------------------------------------------------------------------
[ r32: le( reg, mrc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  temp;		EmitOperandIntoString( $1, &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, $0->Name(), temp.c_str() );
		Emit( "setle%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		$$ = new RegisterOperand( reg, rulenum ); }
]
[ r32: le( mem, rc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  s0;	EmitOperandIntoString( $0, &s0 );
		String  s1;	EmitOperandIntoString( $1, &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );
		Emit( "setle%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		$$ = new RegisterOperand( reg, rulenum ); }
]
//--------------------------------------------------------------------------------------
[ r32: ge( reg, mrc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  temp;		EmitOperandIntoString( $1, &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, $0->Name(), temp.c_str() );
		Emit( "setge%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		$$ = new RegisterOperand( reg, rulenum ); }
]
[ r32: ge( mem, rc )
	[{ "emit_the_most_complex_nodes_first" }]
	""
	<7>
	%ctor{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  s0;	EmitOperandIntoString( $0, &s0 );
		String  s1;	EmitOperandIntoString( $1, &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );
		Emit( "setge%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		$$ = new RegisterOperand( reg, rulenum ); }
]
/* */
//--------------------------------------------------------------------------------------
/*
//TODO: use any general-purpose register, not only ECX
[ ECX: eq( reg, mrc )
	!{ clobber: ECX }
	"xor ecx, ecx\ncmp %0, %1\nsete cl"
	<10>
]
[ ECX: neq( reg, mrc )
	!{ clobber: ECX }
	"xor ecx, ecx\ncmp %0, %1\nsetne cl"
	<10>
]
[ ECX: lt( reg, mrc )
	!{ clobber: ECX }
	"xor ecx, ecx\ncmp %0, %1\nsetl cl"
	<10>
]
[ ECX: gt( reg, mrc )
	!{ clobber: ECX }
	"xor ecx, ecx\ncmp %0, %1\nsetg cl"
	<10>
]
[ ECX: le( reg, mrc )
	!{ clobber: ECX }
	"xor ecx, ecx\ncmp %0, %1\nsetle cl"
	<10>
]
[ ECX: ge( reg, mrc )
	!{ clobber: ECX }
	"xor ecx, ecx\ncmp %0, %1\nsetge cl"
	<10>
]
*/
//-----------------------------------------------------------------------------

/*
==================================
	FPU instructions
==================================
*/

memf32:	indir_f32( address_f32 )	"dword ptr [%0]"	%ctor{ $$ = new MemoryOperand( insn, rulenum ); }
memf64:	indir_f64( address_f64 )	"qword ptr [%0]"	%ctor{ $$ = new MemoryOperand( insn, rulenum ); }

addr_f32: address_f32	"dword ptr [%0]"	%ctor{ $$ = new MemoryOperand( insn, rulenum ); }
addr_f64: address_f64	"qword ptr [%0]"	%ctor{ $$ = new MemoryOperand( insn, rulenum ); }

float_address: addr_f32 "%0"
float_address: addr_f64 "%0"

float_const: fconst "dword ptr %0"	%ctor{ $$ = new ImmediateOperand( insn, rulenum ); }
			 
/*			 %ctor{ float value = insn->GetFloatValue();
										MemoryLocation * loc = DefineConstant_Float32( value, rulenum );
										loc->size = DWORD_PTR;

										byte  temp[4];
										memcpy( temp, &value, sizeof( value ) );
										//TODO: remove this crap
										loc->data = "abcd";
										loc->data[0] = temp[0];
										loc->data[1] = temp[1];
										loc->data[2] = temp[2];
										loc->data[3] = temp[3];
										loc->data[4] = '\0';
			
										$$ = loc; }
*/

float_value: float_const	"%0"
float_value: memf32			"%0"
float_value: memf64			"%0"

[ fpu_reg: float_value
	"fld %0"
	<10>
]

[ fpu_reg: float_address
	"fld %0"
	<10>
]

[ EAX: f2i( fpu_reg )
	"call __ftol2_sse"
	<10>
]

//==--------------------------------------------------------
[ fpu_reg: i2f( mem )
	"fild %0"
	<7>
]
[ fpu_reg: i2f( mr )
	"push %0\nfild dword ptr [esp]\nadd esp, 4"
	<7>
]
//==--------------------------------------------------------

[ %0: assign( float_address, fpu_reg )
	"fstp %0"
	<10>
]
stmt: float_address

//-------------------------------------------------------------

[ %0: add( fpu_reg, float_value )
	"fadd %1"
	<10>
]
[ %0: add( fpu_reg, fpu_reg )
	"faddp st(1), st"
	<10>
]

[ %0: sub( fpu_reg, float_value )
	"fsub %1"
	<10>
]
[ %0: sub( fpu_reg, fpu_reg )
	"fsubp st(1), st"
	<10>
]

[ %0: mul( fpu_reg, float_value )
	"fmul %1"
	<10>
]
[ %0: mul( fpu_reg, fpu_reg )
	"fmulp st(1), st"
	<10>
]

[ %0: fdiv( fpu_reg, float_value )
	"fdiv %1"
	<10>
]
[ %0: fdiv( fpu_reg, fpu_reg )
	"fdivp st(1), st"
	<10>
]

[ %0: fneg( fpu_reg )
	"fchs"
	<10>
]

//TODO:
//[ EAX: eq( fpu_reg, float_value )
//	"fcomp %1\nfstsw ax\nsahf"
//	<12>
//]
//[ stmt: assign( address, f2i( eq( fpu_reg, float_value )) )
//	"fcomp %1\nfstsw ax\nsahf"
//	<12>
//]
//-----------------------------------------------------------------------------------------------------------------------------
[ r32: f2b( eq( fpu_reg, fpu_reg ) )
	!{ clobber: EAX }
	""
	<12>
	%ctor {	
		Emit( "fxch%sst(1)\t;exchange st(0) with st(1)\n", m_spacing );
		Emit( "fucompp\t;compare st(0) with st(1), set flags in the FPU status word and pop the stack twice\n" );
		Emit( "fstsw%sax\t;store the value of FPU status word into ax, don't check for pending unmasked FPU exceptions\n", m_spacing );
		Emit( "test%sah, 44h\n", m_spacing );
		
		Register & reg = GetFreeGeneralReg32();
		Emit( "setnp%s%s\n", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		Emit( "and%s%s, 1", m_spacing, reg.Name() );
		
		m_numInstructions += 5;
		$$ = new RegisterOperand( reg, rulenum ); 
	}
]
[ r32: f2i( eq( fpu_reg, fpu_reg ) )
	!{ clobber: EAX }
	""
	<12>
	%ctor {	
		Emit( "fxch%sst(1)\t;exchange st(0) with st(1)\n", m_spacing );
		Emit( "fucompp\t;compare st(0) with st(1), set flags in the FPU status word and pop the stack twice\n" );
		Emit( "fstsw%sax\t;store the value of FPU status word into ax, don't check for pending unmasked FPU exceptions\n", m_spacing );
		Emit( "test%sah, 44h\n", m_spacing );
		
		Register & reg = GetFreeGeneralReg32();
		Emit( "setnp%s%s\n", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		Emit( "and%s%s, 1", m_spacing, reg.Name() );
		
		m_numInstructions += 5;
		$$ = new RegisterOperand( reg, rulenum ); 
	}
]
//-----------------------------------------------------------------------------------------------------------------------------


/*
==================================
	Constant folding
	TODO: make a tool, 'Optimizer generator'
	( similar to iburg, will traverse trees / graphs
	and execute semantic actions )
==================================
*/
/*imm: mul( imm, imm )	"%0"	%ctor{ ImmediateOperand * newNode = new ImmediateOperand( insn, i1 = $0->GetInsn()->GetIntegerValue();
//									$$ = newNode; }
*/


/*
==================================
	Control flow
==================================
*/

// address of jump
addrj: address_32	"dword ptr %0"
addrj: reg			"%0"			<2>
addrj: mem			"%0"			<2>
addrj: label		"%0"

[ stmt: jmp( addrj )
	"jmp %c"
	<3>
]

[ stmt: label
	"%c:"
	<3>
]

[ boolean_value: eq( reg, mrc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
[ boolean_value: eq( mr, rc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
//--------------------------------------------
[boolean_value: neq( reg, mrc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
[ boolean_value: neq( mr, rc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
//--------------------------------------------
[ boolean_value: lt( reg, mrc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
[ boolean_value: lt( mr, rc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
//--------------------------------------------
[ boolean_value: gt( reg, mrc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
[ boolean_value: gt( mr, rc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
//--------------------------------------------
[ boolean_value: le( reg, mrc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
[ boolean_value: le( mr, rc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
//--------------------------------------------
[ boolean_value: ge( reg, mrc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
[ boolean_value: ge( mr, rc )
	!{ set: CF }
	"cmp %0, %1"
	<5>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
//--------------------------------------------
[ boolean_value: reg
	"test %0, %0"
	<3>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
]
//--------------------------------------------
stmt: boolean_value
//[ stmt: gt( reg, mrc )
//	"cmp %0, %1\njg %c"
//	<5>
//]

[ boolean_value: assign( address, imm )
	"mov %0, %1"
	<4>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); 
	}%dtor{
		Register & reg = GetFreeReg32();
		Emit( "mov%s%s, %s\n", m_spacing, reg.Name(), $1->Name() );
		Emit( "test%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		$$->DropChildren();
	}
]
[ boolean_value: assign( address, reg )
	"mov %0, %1"
	<4>
	{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); 
	}%dtor{
		Emit( "test%s%s, %s\n", m_spacing, $1->Name(), $1->Name() );
		$$->DropChildren();
	}
]

_label: label "%0"

[ stmt: br_false( boolean_value, _label )
	""
	<5>
	{
		const char * op;
		switch ( m_last_cond_opcode ) {
			case assign:
			case eq:
				if ( m_is_signed ) {
					op = "jne";
				} else {
					op = "jnz";
				}
				break;

			case neq:
				if ( m_is_signed ) {
					op = "je";
				} else {
					op = "jz";
				}
				break;

			case le:
				if ( m_is_signed ) {
					op = "jg";
				} else {
					op = "ja";
				}
				break;

			case ge:
				if ( m_is_signed ) {
					op = "jl";
				} else {
					op = "jb";
				}
				break;

			case gt:
				if ( m_is_signed ) {
					op = "jle";
				} else {
					op = "jbe";
				}
				break;

			case lt:
				if ( m_is_signed ) {
					op = "jge";
				} else {
					op = "jae";
				}
				break;

			default:
				if ( m_is_signed ) {
					op = "jne";
				} else {
					op = "jnz";
				}
		}
		Emit( "%s%s%s\n", op, m_spacing, $1->Name() );
	}
]
[ stmt: br_true( boolean_value, _label )
	""
	<5>
	{
		const char * op;
		switch ( m_last_cond_opcode ) {
			case assign:
			case eq:
				if ( m_is_signed ) {
					op = "je";
				} else {
					op = "jz";
				}
				break;

			case neq:
				if ( m_is_signed ) {
					op = "jne";
				} else {
					op = "jnz";
				}
				break;

			case le:
				if ( m_is_signed ) {
					op = "jle";
				} else {
					op = "jbe";
				}
				break;

			case ge:
				if ( m_is_signed ) {
					op = "jge";
				} else {
					op = "jae";
				}
				break;

			case gt:
				if ( m_is_signed ) {
					op = "jg";
				} else {
					op = "ja";
				}
				break;

			case lt:
				if ( m_is_signed ) {
					op = "jle";
				} else {
					op = "jbe";
				}
				break;

			default:
				if ( m_is_signed ) {
					op = "je";
				} else {
					op = "jz";
				}
		}
		Emit( "%s%s%s\n", op, m_spacing, $1->Name() );
	}
]

/*
stmt: nop {
	if ( ! insn->comment.IsEmpty() ) {
		Emit( "\n; %s\n", insn->comment.c_str() );
	}
}
*/
stmt: nop

/*
==================================
	Functions ( procedures, subroutines )
==================================
*/
[ stmt: func_start
	"_%0 proc"
	<15>
	{
		Emit( "\n" );
		//TODO:
		//Emit( "i = ebp - 32\t\t; size = 4\n" );
		// var   equ   stackOffset

	}%dtor {
		Emit( "push%sebp\nmov%sebp, esp\nsub%sesp, %d\npush%sebx\npush%sesi\npush%sedi\n",
			m_spacing, m_spacing, m_spacing, insn->GetIntegerValue(), m_spacing, m_spacing, m_spacing, m_spacing );
	}
]

[ stmt: func_end
	"_%0 endp"
	<15>
	{//TODO: ret %s
	Emit( "pop%sedi\npop%sesi\npop%sebx\nmov%sesp, ebp\npop%sebp\nret%s0\n",
		m_spacing, m_spacing, m_spacing, m_spacing, m_spacing, m_spacing, m_spacing );
	}
]

stmt: ret( nop )
stmt: ret( EAX )

[ stmt: arg( mrc )
	"push %0"
	<5>
]

[ stmt: arg( _label )
	"push offset %0"
	<3>
]

//[ stmt: arg( float )
//	"push %0 add esp, .."
//	<2>
//]
/*
[ reg: call
	"call %0"
	<6>
	{
		//{ ClobberRegisters( insn->GetClobberedRegs() );
		Emit( "call%s%s\n", m_spacing, insn->Name() );
		//TODO:
		// add esp, ...
		++m_numInstructions;
	}
]
*/


//TODO: ctor / dtor to alloc register for result
[ EAX: call( addrj )
	!{ clobber: EAX }
	"call %0"
	<3>
	%dtor {
		int  sizeOfParams = insn->GetIntegerValue();
		if ( sizeOfParams > 0 ) {
			Emit( "add%sesp, %d\n", m_spacing, sizeOfParams );
			++m_numInstructions;
		}
	}
]

//----------------------------------------------

//mem: address_32 "%0"

// ++E
[ stmt : preinc( mem )
	"inc %0"
	<5>
]
[ %0 : preinc( reg )
	"inc %0"
	<5>
]

// --E
[ stmt : predec( mem )
	"dec %0"
	<5>
]
[ %0 : predec( reg )
	"dec %0"
	<5>
]

// E++
[ stmt : postinc( mem )
	"inc %0"
	<5>
]

/*
[ %0 : postinc( reg )
	""
	<5>
	%ctor{		
		RegisterOperand * pRegOp = static_cast<RegisterOperand*>( $0 );
		Register & rPrevReg = pRegOp->GetRegister();

		// save the previous value
		Register & rFreeReg = GetFreeReg32();
		EmitMove( rFreeReg, rPrevReg );

		Emit( "inc%s%s", m_spacing, rPrevReg.Name() );
		rPrevReg.SetUsed();
		
		m_numInstructions += 5;
		// return the prev. value
		$$ = new RegisterOperand( rFreeReg, rulenum ); 
	}
]
*/

// E--
[ stmt : postdec( mem )
	"dec %0"
	<5>
]
//[ %0 : postdec( reg )
//	"dec %0"
//	<5>
//]
!