/*
===============================================
This file is generated from C:/Documents and Settings/admin/Рабочий стол/C4/source/backend/x86/x86.h.h
===============================================
*/
#include <include/c4.h>

#include "../IR.h"
#include "Machine_x86.h"

namespace c4 {

enum {	MAX_COST = 9999	};

// nonterminals
//
const int	stmt_NT = 1;
const int	address_NT = 2;
const int	mem_s8_NT = 3;
const int	mem_u8_NT = 4;
const int	mem_s16_NT = 5;
const int	mem_u16_NT = 6;
const int	mem_NT = 7;
const int	imm_NT = 8;
const int	iconst_2_4_8_NT = 9;
const int	mc_NT = 10;
const int	mr_NT = 11;
const int	r32_NT = 12;
const int	rc_NT = 13;
const int	mrc_NT = 14;
const int	memf32_NT = 15;
const int	memf64_NT = 16;
const int	addr_f32_NT = 17;
const int	addr_f64_NT = 18;
const int	float_address_NT = 19;
const int	float_const_NT = 20;
const int	float_value_NT = 21;
const int	fpu_reg_NT = 22;
const int	addrj_NT = 23;
const int	boolean_value_NT = 24;
const int	_label_NT = 25;

const int	NUM_NONTERMS = 25; // number of nonterminals
const int	NUM_RULES = 156; // number of rules

struct state_s 
{
	state_s()
		: op( 0 )
		, left( NULL )
		, right( NULL )
	{
		Init();
	}

	state_s( int opcode, state_s* leftChild, state_s* rightChild ) 
		: op( opcode )
		, left( leftChild )
		, right( rightChild )
	{
		Init();
	}

	// for tree pattern matching
	int			op;
	state_s *	left;
	state_s *	right;

	int			cost[ NUM_NONTERMS + 1 ];	// for recording costs of matches
	int			rule[ NUM_NONTERMS + 1 ];	// for recording matches

public:
	// record a match only if its cost is less than that of previous matches
	__forceinline void Record( int nonterm, int _cost, int _rulenum ) {
		if ( _cost < cost[ nonterm ] ) {
			cost[ nonterm ] = _cost;
			rule[ nonterm ] = _rulenum;
		}
	}
private:
	// called in the constructor to initialize the data for the first time
	void  Init() {
		memset( cost, MAX_COST, (NUM_NONTERMS + 1) * sizeof( cost[ 0 ] ) );
		memset( rule, 0, (NUM_NONTERMS + 1) * sizeof( rule[ 0 ] ) );
	}
};

state_s *  Machine_x86::_GetState( IRCode * insn, state_s* left, state_s* right ) {
	int  c = 0;  // cost

	state_s * p = new state_s( insn->GetOpCode(), left, right );

	switch ( insn->GetOpCode() ) {
	case nop:
		p->Record( stmt_NT, c + 0, 143 );	/* stmt : nop, "" */
		break;

	case iconst:
		p->Record( imm_NT, c + 0, 9 );	/* imm : iconst, "%0" */
		p->Record( r32_NT, c + 0 + 3, 30 );	/* r32 : imm, "mov %c, %0", <3> */
		p->Record( boolean_value_NT, c + 0 + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
		p->Record( stmt_NT, c + 0 + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
		p->Record( addrj_NT, c + 0 + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
		p->Record( rc_NT, c + 0 + 3 + 0, 22 );	/* rc : r32, "%0" */
		p->Record( stmt_NT, c + 0 + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mr_NT, c + 0 + 3 + 0, 21 );	/* mr : r32, "%0" */
		p->Record( rc_NT, c + 0 + 0, 23 );	/* rc : imm, "%0" */
		p->Record( stmt_NT, c + 0 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mc_NT, c + 0 + 0, 19 );	/* mc : imm, "%0" */
		break;

	case iconst_0:
		p->Record( imm_NT, c + 0, 10 );	/* imm : iconst_0, "%0" */
		p->Record( r32_NT, c + 0 + 3, 30 );	/* r32 : imm, "mov %c, %0", <3> */
		p->Record( boolean_value_NT, c + 0 + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
		p->Record( stmt_NT, c + 0 + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
		p->Record( addrj_NT, c + 0 + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
		p->Record( rc_NT, c + 0 + 3 + 0, 22 );	/* rc : r32, "%0" */
		p->Record( stmt_NT, c + 0 + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mr_NT, c + 0 + 3 + 0, 21 );	/* mr : r32, "%0" */
		p->Record( rc_NT, c + 0 + 0, 23 );	/* rc : imm, "%0" */
		p->Record( stmt_NT, c + 0 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mc_NT, c + 0 + 0, 19 );	/* mc : imm, "%0" */
		p->Record( r32_NT, c + 1, 31 );	/* r32 : iconst_0, "xor %c, %c", <1> */
		p->Record( boolean_value_NT, c + 1 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
		p->Record( stmt_NT, c + 1 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
		p->Record( addrj_NT, c + 1 + 2, 119 );	/* addrj : r32, "%0", <2> */
		p->Record( rc_NT, c + 1 + 0, 22 );	/* rc : r32, "%0" */
		p->Record( stmt_NT, c + 1 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 1 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mr_NT, c + 1 + 0, 21 );	/* mr : r32, "%0" */
		break;

	case iconst_1:
		p->Record( imm_NT, c + 0, 11 );	/* imm : iconst_1, "%0" */
		p->Record( r32_NT, c + 0 + 3, 30 );	/* r32 : imm, "mov %c, %0", <3> */
		p->Record( boolean_value_NT, c + 0 + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
		p->Record( stmt_NT, c + 0 + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
		p->Record( addrj_NT, c + 0 + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
		p->Record( rc_NT, c + 0 + 3 + 0, 22 );	/* rc : r32, "%0" */
		p->Record( stmt_NT, c + 0 + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mr_NT, c + 0 + 3 + 0, 21 );	/* mr : r32, "%0" */
		p->Record( rc_NT, c + 0 + 0, 23 );	/* rc : imm, "%0" */
		p->Record( stmt_NT, c + 0 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mc_NT, c + 0 + 0, 19 );	/* mc : imm, "%0" */
		break;

	case iconst_2:
		p->Record( imm_NT, c + 0, 12 );	/* imm : iconst_2, "%0" */
		p->Record( r32_NT, c + 0 + 3, 30 );	/* r32 : imm, "mov %c, %0", <3> */
		p->Record( boolean_value_NT, c + 0 + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
		p->Record( stmt_NT, c + 0 + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
		p->Record( addrj_NT, c + 0 + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
		p->Record( rc_NT, c + 0 + 3 + 0, 22 );	/* rc : r32, "%0" */
		p->Record( stmt_NT, c + 0 + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mr_NT, c + 0 + 3 + 0, 21 );	/* mr : r32, "%0" */
		p->Record( rc_NT, c + 0 + 0, 23 );	/* rc : imm, "%0" */
		p->Record( stmt_NT, c + 0 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mc_NT, c + 0 + 0, 19 );	/* mc : imm, "%0" */
		p->Record( iconst_2_4_8_NT, c + 0, 15 );	/* iconst_2_4_8 : iconst_2, "2" */
		break;

	case iconst_4:
		p->Record( imm_NT, c + 0, 13 );	/* imm : iconst_4, "%0" */
		p->Record( r32_NT, c + 0 + 3, 30 );	/* r32 : imm, "mov %c, %0", <3> */
		p->Record( boolean_value_NT, c + 0 + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
		p->Record( stmt_NT, c + 0 + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
		p->Record( addrj_NT, c + 0 + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
		p->Record( rc_NT, c + 0 + 3 + 0, 22 );	/* rc : r32, "%0" */
		p->Record( stmt_NT, c + 0 + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mr_NT, c + 0 + 3 + 0, 21 );	/* mr : r32, "%0" */
		p->Record( rc_NT, c + 0 + 0, 23 );	/* rc : imm, "%0" */
		p->Record( stmt_NT, c + 0 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mc_NT, c + 0 + 0, 19 );	/* mc : imm, "%0" */
		p->Record( iconst_2_4_8_NT, c + 0, 16 );	/* iconst_2_4_8 : iconst_4, "4" */
		break;

	case iconst_8:
		p->Record( imm_NT, c + 0, 14 );	/* imm : iconst_8, "%0" */
		p->Record( r32_NT, c + 0 + 3, 30 );	/* r32 : imm, "mov %c, %0", <3> */
		p->Record( boolean_value_NT, c + 0 + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
		p->Record( stmt_NT, c + 0 + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
		p->Record( addrj_NT, c + 0 + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
		p->Record( rc_NT, c + 0 + 3 + 0, 22 );	/* rc : r32, "%0" */
		p->Record( stmt_NT, c + 0 + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mr_NT, c + 0 + 3 + 0, 21 );	/* mr : r32, "%0" */
		p->Record( rc_NT, c + 0 + 0, 23 );	/* rc : imm, "%0" */
		p->Record( stmt_NT, c + 0 + 0 + 0, 34 );	/* stmt : rc, "" */
		p->Record( mrc_NT, c + 0 + 0 + 0, 24 );	/* mrc : rc, "%0" */
		p->Record( mc_NT, c + 0 + 0, 19 );	/* mc : imm, "%0" */
		p->Record( iconst_2_4_8_NT, c + 0, 17 );	/* iconst_2_4_8 : iconst_8, "8" */
		break;

	case fconst:
		p->Record( float_const_NT, c + 0, 96 );	/* float_const : fconst, "dword ptr %0" */
		p->Record( float_value_NT, c + 0 + 0, 97 );	/* float_value : float_const, "%0" */
		p->Record( fpu_reg_NT, c + 0 + 0 + 10, 100 );	/* fpu_reg : float_value, "fld %0", <10> */
		break;

	case address_8:
		p->Record( address_NT, c + 0, 1 );	/* address : address_8, "byte ptr [%0]" */
		break;

	case address_16:
		p->Record( address_NT, c + 0, 2 );	/* address : address_16, "word ptr [%0]" */
		break;

	case address_32:
		p->Record( address_NT, c + 0, 3 );	/* address : address_32, "dword ptr [%0]" */
		p->Record( addrj_NT, c + 0, 118 );	/* addrj : address_32, "dword ptr %0" */
		break;

	case address_64:
		break;

	case address_f32:
		p->Record( addr_f32_NT, c + 0, 92 );	/* addr_f32 : address_f32, "dword ptr [%0]" */
		p->Record( float_address_NT, c + 0 + 0, 94 );	/* float_address : addr_f32, "%0" */
		p->Record( stmt_NT, c + 0 + 0 + 0, 106 );	/* stmt : float_address, "" */
		p->Record( fpu_reg_NT, c + 0 + 0 + 10, 101 );	/* fpu_reg : float_address, "fld %0", <10> */
		break;

	case address_f64:
		p->Record( addr_f64_NT, c + 0, 93 );	/* addr_f64 : address_f64, "qword ptr [%0]" */
		p->Record( float_address_NT, c + 0 + 0, 95 );	/* float_address : addr_f64, "%0" */
		p->Record( stmt_NT, c + 0 + 0 + 0, 106 );	/* stmt : float_address, "" */
		p->Record( fpu_reg_NT, c + 0 + 0 + 10, 101 );	/* fpu_reg : float_address, "fld %0", <10> */
		break;

	case indir_i8:
		c = 0;
		if ( left->op == address_8 ) {
			c = 0+ 0;
			p->Record( mem_s8_NT, c + 0, 4 );	/* mem_s8 : indir_i8( address_8 ), "byte ptr [%0]" */
			p->Record( r32_NT, c + 0 + 4, 26 );	/* r32 : mem_s8, "movsx %c, %0", <4> */
			p->Record( boolean_value_NT, c + 0 + 4 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 0 + 4 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 0 + 4 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 0 + 4 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 0 + 4 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 0 + 4 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 0 + 4 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case indir_u8:
		c = 0;
		if ( left->op == address_8 ) {
			c = 0+ 0;
			p->Record( mem_u8_NT, c + 0, 5 );	/* mem_u8 : indir_u8( address_8 ), "byte ptr [%0]" */
			p->Record( r32_NT, c + 0 + 4, 27 );	/* r32 : mem_u8, "movzx %c, %0", <4> */
			p->Record( boolean_value_NT, c + 0 + 4 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 0 + 4 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 0 + 4 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 0 + 4 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 0 + 4 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 0 + 4 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 0 + 4 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case indir_i16:
		c = 0;
		if ( left->op == address_16 ) {
			c = 0+ 0;
			p->Record( mem_s16_NT, c + 0, 6 );	/* mem_s16 : indir_i16( address_16 ), "word ptr [%0]" */
			p->Record( r32_NT, c + 0 + 3, 28 );	/* r32 : mem_s16, "movsx %c, %0", <3> */
			p->Record( boolean_value_NT, c + 0 + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 0 + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 0 + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 0 + 3 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 0 + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 0 + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 0 + 3 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case indir_u16:
		c = 0;
		if ( left->op == address_16 ) {
			c = 0+ 0;
			p->Record( mem_u16_NT, c + 0, 7 );	/* mem_u16 : indir_u16( address_16 ), "word ptr [%0]" */
			p->Record( r32_NT, c + 0 + 3, 29 );	/* r32 : mem_u16, "movzx %c, %0", <3> */
			p->Record( boolean_value_NT, c + 0 + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 0 + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 0 + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 0 + 3 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 0 + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 0 + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 0 + 3 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case indir_i32:
		c = 0;
		if ( left->op == address_32 ) {
			c = 0+ 0;
			p->Record( mem_NT, c + 0, 8 );	/* mem : indir_i32( address_32 ), "dword ptr [%0]" */
			p->Record( addrj_NT, c + 0 + 2, 120 );	/* addrj : mem, "%0", <2> */
			p->Record( r32_NT, c + 0 + 3, 32 );	/* r32 : mem, "mov %c, %0", <3> */
			p->Record( boolean_value_NT, c + 0 + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 0 + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 0 + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 0 + 3 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 0 + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 0 + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 0 + 3 + 0, 21 );	/* mr : r32, "%0" */
			p->Record( mrc_NT, c + 0 + 0, 25 );	/* mrc : mem, "%0" */
			p->Record( mr_NT, c + 0 + 0, 20 );	/* mr : mem, "%0" */
			p->Record( mc_NT, c + 0 + 0, 18 );	/* mc : mem, "%0" */
		}
		break;

	case indir_i64:
		break;

	case indir_f32:
		c = 0;
		if ( left->op == address_f32 ) {
			c = 0+ 0;
			p->Record( memf32_NT, c + 0, 90 );	/* memf32 : indir_f32( address_f32 ), "dword ptr [%0]" */
			p->Record( float_value_NT, c + 0 + 0, 98 );	/* float_value : memf32, "%0" */
			p->Record( fpu_reg_NT, c + 0 + 0 + 10, 100 );	/* fpu_reg : float_value, "fld %0", <10> */
		}
		break;

	case indir_f64:
		c = 0;
		if ( left->op == address_f64 ) {
			c = 0+ 0;
			p->Record( memf64_NT, c + 0, 91 );	/* memf64 : indir_f64( address_f64 ), "qword ptr [%0]" */
			p->Record( float_value_NT, c + 0 + 0, 99 );	/* float_value : memf64, "%0" */
			p->Record( fpu_reg_NT, c + 0 + 0 + 10, 100 );	/* fpu_reg : float_value, "fld %0", <10> */
		}
		break;

	case assign:
		if ( left->rule[address_NT] && right->rule[rc_NT] ) {
			c = left->cost[address_NT] + right->cost[rc_NT];
			p->Record( rc_NT, c + 2, 33 );	/* rc : assign( address, rc ), "mov %0, %1", <2> */
			p->Record( stmt_NT, c + 2 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 2 + 0, 24 );	/* mrc : rc, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 2, 35 );	/* r32 : assign( r32, mrc ), "mov %0, %1", <2> */
			p->Record( boolean_value_NT, c + 2 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 2 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 2 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 2 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 2 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 2 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 2 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[float_address_NT] && right->rule[fpu_reg_NT] ) {
			c = left->cost[float_address_NT] + right->cost[fpu_reg_NT];
			p->Record( float_address_NT, c + 10, 105 );	/* float_address : assign( float_address, fpu_reg ), "fstp %0", <10> */
			p->Record( stmt_NT, c + 10 + 0, 106 );	/* stmt : float_address, "" */
			p->Record( fpu_reg_NT, c + 10 + 10, 101 );	/* fpu_reg : float_address, "fld %0", <10> */
		}
		if ( left->rule[address_NT] && right->rule[imm_NT] ) {
			c = left->cost[address_NT] + right->cost[imm_NT];
			p->Record( boolean_value_NT, c + 4, 138 );	/* boolean_value : assign( address, imm ), "mov %0, %1", <4> */
			p->Record( stmt_NT, c + 4 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		if ( left->rule[address_NT] && right->rule[r32_NT] ) {
			c = left->cost[address_NT] + right->cost[r32_NT];
			p->Record( boolean_value_NT, c + 4, 139 );	/* boolean_value : assign( address, r32 ), "mov %0, %1", <4> */
			p->Record( stmt_NT, c + 4 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		break;

	case add:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 6, 36 );	/* r32 : add( r32, mrc ), "add %0, %1", <6> */
			p->Record( boolean_value_NT, c + 6 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 6 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 6 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 6 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 6 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 6 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 6 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[mr_NT] && right->rule[rc_NT] ) {
			c = left->cost[mr_NT] + right->cost[rc_NT];
			p->Record( mr_NT, c + 7, 37 );	/* mr : add( mr, rc ), "add %0, %1", <7> */
		}
		if ( left->rule[r32_NT] && right->op == iconst_1 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 5, 38 );	/* r32 : add( r32, iconst_1 ), "inc %0", <5> */
			p->Record( boolean_value_NT, c + 5 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 5 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 5 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 5 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 5 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 5 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 5 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->op == mul && left->left->rule[r32_NT] && left->right->rule[iconst_2_4_8_NT] && right->rule[rc_NT] ) {
			c = left->left->cost[r32_NT] + left->right->cost[iconst_2_4_8_NT] + right->cost[rc_NT];
			p->Record( r32_NT, c + 3, 39 );	/* r32 : add( mul, rc ), "lea %c, [%0*%1 + %2]", <3> */
			p->Record( boolean_value_NT, c + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 3 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 3 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->op == add && left->left->op == mul && left->left->left->rule[r32_NT] && left->left->right->rule[iconst_2_4_8_NT] && left->right->rule[r32_NT] && right->rule[imm_NT] ) {
			c = left->left->left->cost[r32_NT] + left->left->right->cost[iconst_2_4_8_NT] + left->right->cost[r32_NT] + right->cost[imm_NT];
			p->Record( r32_NT, c + 3, 40 );	/* r32 : add( add, imm ), "lea %c, [%2 + %0*%1 + %3]", <3> */
			p->Record( boolean_value_NT, c + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 3 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 3 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->op == add && left->left->rule[r32_NT] && left->right->rule[imm_NT] && right->rule[r32_NT] ) {
			c = left->left->cost[r32_NT] + left->right->cost[imm_NT] + right->cost[r32_NT];
			p->Record( r32_NT, c + 3, 41 );	/* r32 : add( add, r32 ), "lea %c, [%0 + %2 + %1]", <3> */
			p->Record( boolean_value_NT, c + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 3 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 3 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->op == add && left->left->rule[r32_NT] && left->right->rule[r32_NT] && right->rule[imm_NT] ) {
			c = left->left->cost[r32_NT] + left->right->cost[r32_NT] + right->cost[imm_NT];
			p->Record( r32_NT, c + 3, 42 );	/* r32 : add( add, imm ), "lea %c, [%0 + %1 + %2]", <3> */
			p->Record( boolean_value_NT, c + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 3 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 3 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->op == mul && left->left->rule[r32_NT] && left->right->rule[iconst_2_4_8_NT] && right->op == sub && right->left->rule[r32_NT] && right->right->rule[imm_NT] ) {
			c = left->left->cost[r32_NT] + left->right->cost[iconst_2_4_8_NT] + right->left->cost[r32_NT] + right->right->cost[imm_NT];
			p->Record( r32_NT, c + 3, 46 );	/* r32 : add( mul, sub ), "lea %c, [%2 + %0*%1 - %3]", <3> */
			p->Record( boolean_value_NT, c + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 3 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 3 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->op == mul && left->left->rule[r32_NT] && left->right->rule[iconst_2_4_8_NT] && right->op == add && right->left->rule[r32_NT] && right->right->rule[imm_NT] ) {
			c = left->left->cost[r32_NT] + left->right->cost[iconst_2_4_8_NT] + right->left->cost[r32_NT] + right->right->cost[imm_NT];
			p->Record( r32_NT, c + 3, 47 );	/* r32 : add( mul, add ), "lea %c, [%2 + %0*%1 + %3]", <3> */
			p->Record( boolean_value_NT, c + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 3 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 3 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[fpu_reg_NT] && right->rule[float_value_NT] ) {
			c = left->cost[fpu_reg_NT] + right->cost[float_value_NT];
			p->Record( fpu_reg_NT, c + 10, 107 );	/* fpu_reg : add( fpu_reg, float_value ), "fadd %1", <10> */
		}
		if ( left->rule[fpu_reg_NT] && right->rule[fpu_reg_NT] ) {
			c = left->cost[fpu_reg_NT] + right->cost[fpu_reg_NT];
			p->Record( fpu_reg_NT, c + 10, 108 );	/* fpu_reg : add( fpu_reg, fpu_reg ), "faddp st(1), st", <10> */
		}
		break;

	case sub:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 6, 43 );	/* r32 : sub( r32, mrc ), "sub %0, %1", <6> */
			p->Record( boolean_value_NT, c + 6 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 6 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 6 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 6 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 6 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 6 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 6 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[mr_NT] && right->rule[rc_NT] ) {
			c = left->cost[mr_NT] + right->cost[rc_NT];
			p->Record( mr_NT, c + 6, 44 );	/* mr : sub( mr, rc ), "sub %0, %1", <6> */
		}
		if ( left->rule[r32_NT] && right->op == iconst_1 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 5, 45 );	/* r32 : sub( r32, iconst_1 ), "dec %0", <5> */
			p->Record( boolean_value_NT, c + 5 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 5 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 5 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 5 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 5 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 5 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 5 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[fpu_reg_NT] && right->rule[float_value_NT] ) {
			c = left->cost[fpu_reg_NT] + right->cost[float_value_NT];
			p->Record( fpu_reg_NT, c + 10, 109 );	/* fpu_reg : sub( fpu_reg, float_value ), "fsub %1", <10> */
		}
		if ( left->rule[fpu_reg_NT] && right->rule[fpu_reg_NT] ) {
			c = left->cost[fpu_reg_NT] + right->cost[fpu_reg_NT];
			p->Record( fpu_reg_NT, c + 10, 110 );	/* fpu_reg : sub( fpu_reg, fpu_reg ), "fsubp st(1), st", <10> */
		}
		break;

	case mul:
		if ( left->rule[r32_NT] && right->rule[imm_NT] ) {
			c = left->cost[r32_NT] + right->cost[imm_NT];
			p->Record( r32_NT, c + 13, 48 );	/* r32 : mul( r32, imm ), "imul %c, %0, %1", <13> */
			p->Record( boolean_value_NT, c + 13 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 13 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 13 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 13 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 13 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 13 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 13 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[imm_NT] ) {
			c = left->cost[r32_NT] + right->cost[imm_NT];
			p->Record( r32_NT, c + 13, 49 );	/* r32 : mul( r32, imm ), "imul %0, %0, %1", <13> */
			p->Record( boolean_value_NT, c + 13 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 13 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 13 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 13 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 13 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 13 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 13 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 14, 50 );	/* r32 : mul( r32, mrc ), "imul %0, %1", <14> */
			p->Record( boolean_value_NT, c + 14 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 14 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 14 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 14 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 14 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 14 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 14 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[mem_NT] && right->rule[imm_NT] ) {
			c = left->cost[mem_NT] + right->cost[imm_NT];
			p->Record( r32_NT, c + 14, 51 );	/* r32 : mul( mem, imm ), "imul %c, %0, %1", <14> */
			p->Record( boolean_value_NT, c + 14 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 14 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 14 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 14 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 14 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 14 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 14 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->op == iconst_2 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 7, 61 );	/* r32 : mul( r32, iconst_2 ), "shl %0, 1", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->op == iconst_4 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 7, 62 );	/* r32 : mul( r32, iconst_4 ), "shl %0, 2", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->op == iconst_8 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 7, 63 );	/* r32 : mul( r32, iconst_8 ), "shl %0, 3", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[fpu_reg_NT] && right->rule[float_value_NT] ) {
			c = left->cost[fpu_reg_NT] + right->cost[float_value_NT];
			p->Record( fpu_reg_NT, c + 10, 111 );	/* fpu_reg : mul( fpu_reg, float_value ), "fmul %1", <10> */
		}
		if ( left->rule[fpu_reg_NT] && right->rule[fpu_reg_NT] ) {
			c = left->cost[fpu_reg_NT] + right->cost[fpu_reg_NT];
			p->Record( fpu_reg_NT, c + 10, 112 );	/* fpu_reg : mul( fpu_reg, fpu_reg ), "fmulp st(1), st", <10> */
		}
		break;

	case div:
		if ( left->rule[r32_NT] && right->rule[mr_NT] ) {
			c = left->cost[r32_NT] + right->cost[mr_NT];
			p->Record( r32_NT, c + 14, 52 );	/* r32 : div( r32, mr ), "cdq\nidiv %1", <14> */
			p->Record( boolean_value_NT, c + 14 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 14 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 14 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 14 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 14 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 14 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 14 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->op == iconst_2 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 7, 64 );	/* r32 : div( r32, iconst_2 ), "shr %0, 1", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->op == iconst_4 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 7, 65 );	/* r32 : div( r32, iconst_4 ), "shr %0, 2", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->op == iconst_8 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 7, 66 );	/* r32 : div( r32, iconst_8 ), "shr %0, 3", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case udiv:
		if ( left->rule[r32_NT] && right->rule[mr_NT] ) {
			c = left->cost[r32_NT] + right->cost[mr_NT];
			p->Record( r32_NT, c + 14, 53 );	/* r32 : udiv( r32, mr ), "xor edx, edx\ndiv %1", <14> */
			p->Record( boolean_value_NT, c + 14 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 14 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 14 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 14 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 14 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 14 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 14 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->op == iconst_2 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 7, 67 );	/* r32 : udiv( r32, iconst_2 ), "shr %0, 1", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->op == iconst_4 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 7, 68 );	/* r32 : udiv( r32, iconst_4 ), "shr %0, 2", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->op == iconst_8 ) {
			c = left->cost[r32_NT] + 0;
			p->Record( r32_NT, c + 7, 69 );	/* r32 : udiv( r32, iconst_8 ), "shr %0, 3", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case fdiv:
		if ( left->rule[fpu_reg_NT] && right->rule[float_value_NT] ) {
			c = left->cost[fpu_reg_NT] + right->cost[float_value_NT];
			p->Record( fpu_reg_NT, c + 10, 113 );	/* fpu_reg : fdiv( fpu_reg, float_value ), "fdiv %1", <10> */
		}
		if ( left->rule[fpu_reg_NT] && right->rule[fpu_reg_NT] ) {
			c = left->cost[fpu_reg_NT] + right->cost[fpu_reg_NT];
			p->Record( fpu_reg_NT, c + 10, 114 );	/* fpu_reg : fdiv( fpu_reg, fpu_reg ), "fdivp st(1), st", <10> */
		}
		break;

	case rem:
		if ( left->rule[r32_NT] && right->rule[mr_NT] ) {
			c = left->cost[r32_NT] + right->cost[mr_NT];
			p->Record( r32_NT, c + 14, 54 );	/* r32 : rem( r32, mr ), "cdq\nidiv %1", <14> */
			p->Record( boolean_value_NT, c + 14 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 14 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 14 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 14 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 14 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 14 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 14 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case urem:
		break;

	case neg:
		c = 0;
		if ( left->rule[r32_NT] ) {
			c = left->cost[r32_NT]+ 5;
			p->Record( r32_NT, c + 5, 70 );	/* r32 : neg( r32 ), "neg %0", <5> */
			p->Record( boolean_value_NT, c + 5 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 5 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 5 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 5 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 5 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 5 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 5 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case fneg:
		c = 0;
		if ( left->rule[fpu_reg_NT] ) {
			c = left->cost[fpu_reg_NT]+ 10;
			p->Record( fpu_reg_NT, c + 10, 115 );	/* fpu_reg : fneg( fpu_reg ), "fchs", <10> */
		}
		break;

	case preinc:
		c = 0;
		if ( left->rule[mem_NT] ) {
			c = left->cost[mem_NT]+ 5;
			p->Record( stmt_NT, c + 5, 151 );	/* stmt : preinc( mem ), "inc %0", <5> */
		}
		c = 0;
		if ( left->rule[r32_NT] ) {
			c = left->cost[r32_NT]+ 5;
			p->Record( r32_NT, c + 5, 152 );	/* r32 : preinc( r32 ), "inc %0", <5> */
			p->Record( boolean_value_NT, c + 5 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 5 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 5 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 5 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 5 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 5 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 5 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case predec:
		c = 0;
		if ( left->rule[mem_NT] ) {
			c = left->cost[mem_NT]+ 5;
			p->Record( stmt_NT, c + 5, 153 );	/* stmt : predec( mem ), "dec %0", <5> */
		}
		c = 0;
		if ( left->rule[r32_NT] ) {
			c = left->cost[r32_NT]+ 5;
			p->Record( r32_NT, c + 5, 154 );	/* r32 : predec( r32 ), "dec %0", <5> */
			p->Record( boolean_value_NT, c + 5 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 5 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 5 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 5 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 5 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 5 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 5 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case postinc:
		c = 0;
		if ( left->rule[mem_NT] ) {
			c = left->cost[mem_NT]+ 5;
			p->Record( stmt_NT, c + 5, 155 );	/* stmt : postinc( mem ), "inc %0", <5> */
		}
		break;

	case postdec:
		c = 0;
		if ( left->rule[mem_NT] ) {
			c = left->cost[mem_NT]+ 5;
			p->Record( stmt_NT, c + 5, 156 );	/* stmt : postdec( mem ), "dec %0", <5> */
		}
		break;

	case and:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 5, 71 );	/* r32 : and( r32, mrc ), "and %0, %1", <5> */
			p->Record( boolean_value_NT, c + 5 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 5 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 5 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 5 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 5 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 5 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 5 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case or:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 5, 72 );	/* r32 : or( r32, mrc ), "or %0, %1", <5> */
			p->Record( boolean_value_NT, c + 5 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 5 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 5 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 5 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 5 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 5 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 5 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case xor:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 5, 73 );	/* r32 : xor( r32, mrc ), "xor %0, %1", <5> */
			p->Record( boolean_value_NT, c + 5 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 5 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 5 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 5 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 5 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 5 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 5 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case not:
		c = 0;
		if ( left->rule[r32_NT] ) {
			c = left->cost[r32_NT]+ 5;
			p->Record( r32_NT, c + 5, 74 );	/* r32 : not( r32 ), "", <5> */
			p->Record( boolean_value_NT, c + 5 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 5 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 5 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 5 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 5 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 5 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 5 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case compl:
		c = 0;
		if ( left->rule[r32_NT] ) {
			c = left->cost[r32_NT]+ 5;
			p->Record( r32_NT, c + 5, 75 );	/* r32 : compl( r32 ), "not %0", <5> */
			p->Record( boolean_value_NT, c + 5 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 5 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 5 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 5 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 5 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 5 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 5 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case shl:
		if ( left->rule[r32_NT] && right->rule[imm_NT] ) {
			c = left->cost[r32_NT] + right->cost[imm_NT];
			p->Record( r32_NT, c + 6, 55 );	/* r32 : shl( r32, imm ), "shl %0, %1", <6> */
			p->Record( boolean_value_NT, c + 6 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 6 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 6 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 6 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 6 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 6 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 6 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[r32_NT] ) {
			c = left->cost[r32_NT] + right->cost[r32_NT];
			p->Record( r32_NT, c + 7, 56 );	/* r32 : shl( r32, r32 ), "shl %0, cl", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case shr:
		if ( left->rule[r32_NT] && right->rule[imm_NT] ) {
			c = left->cost[r32_NT] + right->cost[imm_NT];
			p->Record( r32_NT, c + 6, 57 );	/* r32 : shr( r32, imm ), "sar %0, %1", <6> */
			p->Record( boolean_value_NT, c + 6 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 6 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 6 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 6 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 6 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 6 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 6 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[r32_NT] ) {
			c = left->cost[r32_NT] + right->cost[r32_NT];
			p->Record( r32_NT, c + 7, 58 );	/* r32 : shr( r32, r32 ), "sar %0, cl", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case ushr:
		if ( left->rule[r32_NT] && right->rule[imm_NT] ) {
			c = left->cost[r32_NT] + right->cost[imm_NT];
			p->Record( r32_NT, c + 6, 59 );	/* r32 : ushr( r32, imm ), "shr %0, %1", <6> */
			p->Record( boolean_value_NT, c + 6 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 6 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 6 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 6 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 6 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 6 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 6 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[r32_NT] ) {
			c = left->cost[r32_NT] + right->cost[r32_NT];
			p->Record( r32_NT, c + 7, 60 );	/* r32 : ushr( r32, r32 ), "shr %0, cl", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case eq:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 7, 80 );	/* r32 : eq( r32, mrc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[mem_NT] && right->rule[rc_NT] ) {
			c = left->cost[mem_NT] + right->cost[rc_NT];
			p->Record( r32_NT, c + 7, 81 );	/* r32 : eq( mem, rc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( boolean_value_NT, c + 5, 124 );	/* boolean_value : eq( r32, mrc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		if ( left->rule[mr_NT] && right->rule[rc_NT] ) {
			c = left->cost[mr_NT] + right->cost[rc_NT];
			p->Record( boolean_value_NT, c + 5, 125 );	/* boolean_value : eq( mr, rc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		break;

	case neq:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 7, 82 );	/* r32 : neq( r32, mrc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[mem_NT] && right->rule[rc_NT] ) {
			c = left->cost[mem_NT] + right->cost[rc_NT];
			p->Record( r32_NT, c + 7, 83 );	/* r32 : neq( mem, rc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( boolean_value_NT, c + 5, 126 );	/* boolean_value : neq( r32, mrc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		if ( left->rule[mr_NT] && right->rule[rc_NT] ) {
			c = left->cost[mr_NT] + right->cost[rc_NT];
			p->Record( boolean_value_NT, c + 5, 127 );	/* boolean_value : neq( mr, rc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		break;

	case lt:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 7, 84 );	/* r32 : lt( r32, mrc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[mem_NT] && right->rule[rc_NT] ) {
			c = left->cost[mem_NT] + right->cost[rc_NT];
			p->Record( r32_NT, c + 7, 85 );	/* r32 : lt( mem, rc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( boolean_value_NT, c + 5, 128 );	/* boolean_value : lt( r32, mrc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		if ( left->rule[mr_NT] && right->rule[rc_NT] ) {
			c = left->cost[mr_NT] + right->cost[rc_NT];
			p->Record( boolean_value_NT, c + 5, 129 );	/* boolean_value : lt( mr, rc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		break;

	case gt:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 7, 78 );	/* r32 : gt( r32, mrc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[mem_NT] && right->rule[rc_NT] ) {
			c = left->cost[mem_NT] + right->cost[rc_NT];
			p->Record( r32_NT, c + 7, 79 );	/* r32 : gt( mem, rc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( boolean_value_NT, c + 5, 130 );	/* boolean_value : gt( r32, mrc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		if ( left->rule[mr_NT] && right->rule[rc_NT] ) {
			c = left->cost[mr_NT] + right->cost[rc_NT];
			p->Record( boolean_value_NT, c + 5, 131 );	/* boolean_value : gt( mr, rc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		break;

	case le:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 7, 86 );	/* r32 : le( r32, mrc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[mem_NT] && right->rule[rc_NT] ) {
			c = left->cost[mem_NT] + right->cost[rc_NT];
			p->Record( r32_NT, c + 7, 87 );	/* r32 : le( mem, rc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( boolean_value_NT, c + 5, 132 );	/* boolean_value : le( r32, mrc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		if ( left->rule[mr_NT] && right->rule[rc_NT] ) {
			c = left->cost[mr_NT] + right->cost[rc_NT];
			p->Record( boolean_value_NT, c + 5, 133 );	/* boolean_value : le( mr, rc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		break;

	case ge:
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( r32_NT, c + 7, 88 );	/* r32 : ge( r32, mrc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[mem_NT] && right->rule[rc_NT] ) {
			c = left->cost[mem_NT] + right->cost[rc_NT];
			p->Record( r32_NT, c + 7, 89 );	/* r32 : ge( mem, rc ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		if ( left->rule[r32_NT] && right->rule[mrc_NT] ) {
			c = left->cost[r32_NT] + right->cost[mrc_NT];
			p->Record( boolean_value_NT, c + 5, 134 );	/* boolean_value : ge( r32, mrc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		if ( left->rule[mr_NT] && right->rule[rc_NT] ) {
			c = left->cost[mr_NT] + right->cost[rc_NT];
			p->Record( boolean_value_NT, c + 5, 135 );	/* boolean_value : ge( mr, rc ), "cmp %0, %1", <5> */
			p->Record( stmt_NT, c + 5 + 0, 137 );	/* stmt : boolean_value, "" */
		}
		break;

	case i2f:
		c = 0;
		if ( left->rule[mem_NT] ) {
			c = left->cost[mem_NT]+ 7;
			p->Record( fpu_reg_NT, c + 7, 103 );	/* fpu_reg : i2f( mem ), "fild %0", <7> */
		}
		c = 0;
		if ( left->rule[mr_NT] ) {
			c = left->cost[mr_NT]+ 7;
			p->Record( fpu_reg_NT, c + 7, 104 );	/* fpu_reg : i2f( mr ), "push %0\nfild dword ptr [esp]\nadd esp, 4", <7> */
		}
		break;

	case f2i:
		c = 0;
		if ( left->rule[fpu_reg_NT] ) {
			c = left->cost[fpu_reg_NT]+ 10;
			p->Record( r32_NT, c + 10, 102 );	/* r32 : f2i( fpu_reg ), "call __ftol2_sse", <10> */
			p->Record( boolean_value_NT, c + 10 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 10 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 10 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 10 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 10 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 10 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 10 + 0, 21 );	/* mr : r32, "%0" */
		}
		c = 0;
		if ( left->op == eq && left->left->rule[fpu_reg_NT] && left->right->rule[fpu_reg_NT] ) {
			c = left->left->cost[fpu_reg_NT] + left->right->cost[fpu_reg_NT]+ 12;
			p->Record( r32_NT, c + 12, 117 );	/* r32 : f2i( eq ), "", <12> */
			p->Record( boolean_value_NT, c + 12 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 12 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 12 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 12 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 12 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 12 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 12 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case i2b:
		c = 0;
		if ( left->rule[mr_NT] ) {
			c = left->cost[mr_NT]+ 7;
			p->Record( r32_NT, c + 7, 76 );	/* r32 : i2b( mr ), "", <7> */
			p->Record( boolean_value_NT, c + 7 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 7 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 7 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 7 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 7 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 7 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 7 + 0, 21 );	/* mr : r32, "%0" */
		}
		c = 0;
		if ( left->rule[r32_NT] ) {
			c = left->cost[r32_NT]+ 6;
			p->Record( r32_NT, c + 6, 77 );	/* r32 : i2b( r32 ), "", <6> */
			p->Record( boolean_value_NT, c + 6 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 6 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 6 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 6 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 6 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 6 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 6 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case f2b:
		c = 0;
		if ( left->op == eq && left->left->rule[fpu_reg_NT] && left->right->rule[fpu_reg_NT] ) {
			c = left->left->cost[fpu_reg_NT] + left->right->cost[fpu_reg_NT]+ 12;
			p->Record( r32_NT, c + 12, 116 );	/* r32 : f2b( eq ), "", <12> */
			p->Record( boolean_value_NT, c + 12 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 12 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 12 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 12 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 12 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 12 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 12 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case jmp:
		c = 0;
		if ( left->rule[addrj_NT] ) {
			c = left->cost[addrj_NT]+ 3;
			p->Record( stmt_NT, c + 3, 122 );	/* stmt : jmp( addrj ), "jmp %c", <3> */
		}
		break;

	case br_false:
		if ( left->rule[boolean_value_NT] && right->rule[_label_NT] ) {
			c = left->cost[boolean_value_NT] + right->cost[_label_NT];
			p->Record( stmt_NT, c + 5, 141 );	/* stmt : br_false( boolean_value, _label ), "", <5> */
		}
		break;

	case br_true:
		if ( left->rule[boolean_value_NT] && right->rule[_label_NT] ) {
			c = left->cost[boolean_value_NT] + right->cost[_label_NT];
			p->Record( stmt_NT, c + 5, 142 );	/* stmt : br_true( boolean_value, _label ), "", <5> */
		}
		break;

	case je:
		break;

	case jge:
		break;

	case jg:
		break;

	case jle:
		break;

	case jl:
		break;

	case label:
		p->Record( addrj_NT, c + 0, 121 );	/* addrj : label, "%0" */
		p->Record( stmt_NT, c + 3, 123 );	/* stmt : label, "%c:", <3> */
		p->Record( _label_NT, c + 0, 140 );	/* _label : label, "%0" */
		break;

	case func_start:
		p->Record( stmt_NT, c + 15, 144 );	/* stmt : func_start, "_%0 proc", <15> */
		break;

	case func_end:
		p->Record( stmt_NT, c + 15, 145 );	/* stmt : func_end, "_%0 endp", <15> */
		break;

	case call:
		c = 0;
		if ( left->rule[addrj_NT] ) {
			c = left->cost[addrj_NT]+ 3;
			p->Record( r32_NT, c + 3, 150 );	/* r32 : call( addrj ), "call %0", <3> */
			p->Record( boolean_value_NT, c + 3 + 3, 136 );	/* boolean_value : r32, "test %0, %0", <3> */
			p->Record( stmt_NT, c + 3 + 3 + 0, 137 );	/* stmt : boolean_value, "" */
			p->Record( addrj_NT, c + 3 + 2, 119 );	/* addrj : r32, "%0", <2> */
			p->Record( rc_NT, c + 3 + 0, 22 );	/* rc : r32, "%0" */
			p->Record( stmt_NT, c + 3 + 0 + 0, 34 );	/* stmt : rc, "" */
			p->Record( mrc_NT, c + 3 + 0 + 0, 24 );	/* mrc : rc, "%0" */
			p->Record( mr_NT, c + 3 + 0, 21 );	/* mr : r32, "%0" */
		}
		break;

	case arg:
		c = 0;
		if ( left->rule[mrc_NT] ) {
			c = left->cost[mrc_NT]+ 5;
			p->Record( stmt_NT, c + 5, 148 );	/* stmt : arg( mrc ), "push %0", <5> */
		}
		c = 0;
		if ( left->rule[_label_NT] ) {
			c = left->cost[_label_NT]+ 3;
			p->Record( stmt_NT, c + 3, 149 );	/* stmt : arg( _label ), "push offset %0", <3> */
		}
		break;

	case ret:
		c = 0;
		if ( left->op == nop ) {
			c = 0+ 0;
			p->Record( stmt_NT, c + 0, 146 );	/* stmt : ret( nop ), "" */
		}
		c = 0;
		if ( left->rule[r32_NT] ) {
			c = left->cost[r32_NT]+ 0;
			p->Record( stmt_NT, c + 0, 147 );	/* stmt : ret( r32 ), "" */
		}
		break;

	default:
		c4::common->Error( "Machine_x86::GetState() : unknown opcode: '%s'.\n", IRCode::GetOpcodeName( insn->GetOpCode() ) );
	}
	return p;
}


// assembler templates ( they are indexed by rule numbers )
//
const char * g_templates[ NUM_RULES + 1 ] = {
/* 0 */		NULL,
/* 1 */ "byte ptr [%0]",	/* address : address_8, "byte ptr [%0]" */
/* 2 */ "word ptr [%0]",	/* address : address_16, "word ptr [%0]" */
/* 3 */ "dword ptr [%0]",	/* address : address_32, "dword ptr [%0]" */
/* 4 */ "byte ptr [%0]",	/* mem_s8 : indir_i8( address_8 ), "byte ptr [%0]" */
/* 5 */ "byte ptr [%0]",	/* mem_u8 : indir_u8( address_8 ), "byte ptr [%0]" */
/* 6 */ "word ptr [%0]",	/* mem_s16 : indir_i16( address_16 ), "word ptr [%0]" */
/* 7 */ "word ptr [%0]",	/* mem_u16 : indir_u16( address_16 ), "word ptr [%0]" */
/* 8 */ "dword ptr [%0]",	/* mem : indir_i32( address_32 ), "dword ptr [%0]" */
/* 9 */ "%0",	/* imm : iconst, "%0" */
/* 10 */ "%0",	/* imm : iconst_0, "%0" */
/* 11 */ "%0",	/* imm : iconst_1, "%0" */
/* 12 */ "%0",	/* imm : iconst_2, "%0" */
/* 13 */ "%0",	/* imm : iconst_4, "%0" */
/* 14 */ "%0",	/* imm : iconst_8, "%0" */
/* 15 */ "2",	/* iconst_2_4_8 : iconst_2, "2" */
/* 16 */ "4",	/* iconst_2_4_8 : iconst_4, "4" */
/* 17 */ "8",	/* iconst_2_4_8 : iconst_8, "8" */
/* 18 */ "%0",	/* mc : mem, "%0" */
/* 19 */ "%0",	/* mc : imm, "%0" */
/* 20 */ "%0",	/* mr : mem, "%0" */
/* 21 */ "%0",	/* mr : r32, "%0" */
/* 22 */ "%0",	/* rc : r32, "%0" */
/* 23 */ "%0",	/* rc : imm, "%0" */
/* 24 */ "%0",	/* mrc : rc, "%0" */
/* 25 */ "%0",	/* mrc : mem, "%0" */
/* 26 */ "movsx %c, %0",	/* r32 : mem_s8, "movsx %c, %0", <4> */
/* 27 */ "movzx %c, %0",	/* r32 : mem_u8, "movzx %c, %0", <4> */
/* 28 */ "movsx %c, %0",	/* r32 : mem_s16, "movsx %c, %0", <3> */
/* 29 */ "movzx %c, %0",	/* r32 : mem_u16, "movzx %c, %0", <3> */
/* 30 */ "mov %c, %0",	/* r32 : imm, "mov %c, %0", <3> */
/* 31 */ "xor %c, %c",	/* r32 : iconst_0, "xor %c, %c", <1> */
/* 32 */ "mov %c, %0",	/* r32 : mem, "mov %c, %0", <3> */
/* 33 */ "mov %0, %1",	/* rc : assign( address, rc ), "mov %0, %1", <2> */
/* 34 */ NULL,	/* stmt : rc, "" */
/* 35 */ "mov %0, %1",	/* r32 : assign( r32, mrc ), "mov %0, %1", <2> */
/* 36 */ "add %0, %1",	/* r32 : add( r32, mrc ), "add %0, %1", <6> */
/* 37 */ "add %0, %1",	/* mr : add( mr, rc ), "add %0, %1", <7> */
/* 38 */ "inc %0",	/* r32 : add( r32, iconst_1 ), "inc %0", <5> */
/* 39 */ "lea %c, [%0*%1 + %2]",	/* r32 : add( mul, rc ), "lea %c, [%0*%1 + %2]", <3> */
/* 40 */ "lea %c, [%2 + %0*%1 + %3]",	/* r32 : add( add, imm ), "lea %c, [%2 + %0*%1 + %3]", <3> */
/* 41 */ "lea %c, [%0 + %2 + %1]",	/* r32 : add( add, r32 ), "lea %c, [%0 + %2 + %1]", <3> */
/* 42 */ "lea %c, [%0 + %1 + %2]",	/* r32 : add( add, imm ), "lea %c, [%0 + %1 + %2]", <3> */
/* 43 */ "sub %0, %1",	/* r32 : sub( r32, mrc ), "sub %0, %1", <6> */
/* 44 */ "sub %0, %1",	/* mr : sub( mr, rc ), "sub %0, %1", <6> */
/* 45 */ "dec %0",	/* r32 : sub( r32, iconst_1 ), "dec %0", <5> */
/* 46 */ "lea %c, [%2 + %0*%1 - %3]",	/* r32 : add( mul, sub ), "lea %c, [%2 + %0*%1 - %3]", <3> */
/* 47 */ "lea %c, [%2 + %0*%1 + %3]",	/* r32 : add( mul, add ), "lea %c, [%2 + %0*%1 + %3]", <3> */
/* 48 */ "imul %c, %0, %1",	/* r32 : mul( r32, imm ), "imul %c, %0, %1", <13> */
/* 49 */ "imul %0, %0, %1",	/* r32 : mul( r32, imm ), "imul %0, %0, %1", <13> */
/* 50 */ "imul %0, %1",	/* r32 : mul( r32, mrc ), "imul %0, %1", <14> */
/* 51 */ "imul %c, %0, %1",	/* r32 : mul( mem, imm ), "imul %c, %0, %1", <14> */
/* 52 */ "cdq\nidiv %1",	/* r32 : div( r32, mr ), "cdq\nidiv %1", <14> */
/* 53 */ "xor edx, edx\ndiv %1",	/* r32 : udiv( r32, mr ), "xor edx, edx\ndiv %1", <14> */
/* 54 */ "cdq\nidiv %1",	/* r32 : rem( r32, mr ), "cdq\nidiv %1", <14> */
/* 55 */ "shl %0, %1",	/* r32 : shl( r32, imm ), "shl %0, %1", <6> */
/* 56 */ "shl %0, cl",	/* r32 : shl( r32, r32 ), "shl %0, cl", <7> */
/* 57 */ "sar %0, %1",	/* r32 : shr( r32, imm ), "sar %0, %1", <6> */
/* 58 */ "sar %0, cl",	/* r32 : shr( r32, r32 ), "sar %0, cl", <7> */
/* 59 */ "shr %0, %1",	/* r32 : ushr( r32, imm ), "shr %0, %1", <6> */
/* 60 */ "shr %0, cl",	/* r32 : ushr( r32, r32 ), "shr %0, cl", <7> */
/* 61 */ "shl %0, 1",	/* r32 : mul( r32, iconst_2 ), "shl %0, 1", <7> */
/* 62 */ "shl %0, 2",	/* r32 : mul( r32, iconst_4 ), "shl %0, 2", <7> */
/* 63 */ "shl %0, 3",	/* r32 : mul( r32, iconst_8 ), "shl %0, 3", <7> */
/* 64 */ "shr %0, 1",	/* r32 : div( r32, iconst_2 ), "shr %0, 1", <7> */
/* 65 */ "shr %0, 2",	/* r32 : div( r32, iconst_4 ), "shr %0, 2", <7> */
/* 66 */ "shr %0, 3",	/* r32 : div( r32, iconst_8 ), "shr %0, 3", <7> */
/* 67 */ "shr %0, 1",	/* r32 : udiv( r32, iconst_2 ), "shr %0, 1", <7> */
/* 68 */ "shr %0, 2",	/* r32 : udiv( r32, iconst_4 ), "shr %0, 2", <7> */
/* 69 */ "shr %0, 3",	/* r32 : udiv( r32, iconst_8 ), "shr %0, 3", <7> */
/* 70 */ "neg %0",	/* r32 : neg( r32 ), "neg %0", <5> */
/* 71 */ "and %0, %1",	/* r32 : and( r32, mrc ), "and %0, %1", <5> */
/* 72 */ "or %0, %1",	/* r32 : or( r32, mrc ), "or %0, %1", <5> */
/* 73 */ "xor %0, %1",	/* r32 : xor( r32, mrc ), "xor %0, %1", <5> */
/* 74 */ "",	/* r32 : not( r32 ), "", <5> */
/* 75 */ "not %0",	/* r32 : compl( r32 ), "not %0", <5> */
/* 76 */ "",	/* r32 : i2b( mr ), "", <7> */
/* 77 */ "",	/* r32 : i2b( r32 ), "", <6> */
/* 78 */ "",	/* r32 : gt( r32, mrc ), "", <7> */
/* 79 */ "",	/* r32 : gt( mem, rc ), "", <7> */
/* 80 */ "",	/* r32 : eq( r32, mrc ), "", <7> */
/* 81 */ "",	/* r32 : eq( mem, rc ), "", <7> */
/* 82 */ "",	/* r32 : neq( r32, mrc ), "", <7> */
/* 83 */ "",	/* r32 : neq( mem, rc ), "", <7> */
/* 84 */ "",	/* r32 : lt( r32, mrc ), "", <7> */
/* 85 */ "",	/* r32 : lt( mem, rc ), "", <7> */
/* 86 */ "",	/* r32 : le( r32, mrc ), "", <7> */
/* 87 */ "",	/* r32 : le( mem, rc ), "", <7> */
/* 88 */ "",	/* r32 : ge( r32, mrc ), "", <7> */
/* 89 */ "",	/* r32 : ge( mem, rc ), "", <7> */
/* 90 */ "dword ptr [%0]",	/* memf32 : indir_f32( address_f32 ), "dword ptr [%0]" */
/* 91 */ "qword ptr [%0]",	/* memf64 : indir_f64( address_f64 ), "qword ptr [%0]" */
/* 92 */ "dword ptr [%0]",	/* addr_f32 : address_f32, "dword ptr [%0]" */
/* 93 */ "qword ptr [%0]",	/* addr_f64 : address_f64, "qword ptr [%0]" */
/* 94 */ "%0",	/* float_address : addr_f32, "%0" */
/* 95 */ "%0",	/* float_address : addr_f64, "%0" */
/* 96 */ "dword ptr %0",	/* float_const : fconst, "dword ptr %0" */
/* 97 */ "%0",	/* float_value : float_const, "%0" */
/* 98 */ "%0",	/* float_value : memf32, "%0" */
/* 99 */ "%0",	/* float_value : memf64, "%0" */
/* 100 */ "fld %0",	/* fpu_reg : float_value, "fld %0", <10> */
/* 101 */ "fld %0",	/* fpu_reg : float_address, "fld %0", <10> */
/* 102 */ "call __ftol2_sse",	/* r32 : f2i( fpu_reg ), "call __ftol2_sse", <10> */
/* 103 */ "fild %0",	/* fpu_reg : i2f( mem ), "fild %0", <7> */
/* 104 */ "push %0\nfild dword ptr [esp]\nadd esp, 4",	/* fpu_reg : i2f( mr ), "push %0\nfild dword ptr [esp]\nadd esp, 4", <7> */
/* 105 */ "fstp %0",	/* float_address : assign( float_address, fpu_reg ), "fstp %0", <10> */
/* 106 */ NULL,	/* stmt : float_address, "" */
/* 107 */ "fadd %1",	/* fpu_reg : add( fpu_reg, float_value ), "fadd %1", <10> */
/* 108 */ "faddp st(1), st",	/* fpu_reg : add( fpu_reg, fpu_reg ), "faddp st(1), st", <10> */
/* 109 */ "fsub %1",	/* fpu_reg : sub( fpu_reg, float_value ), "fsub %1", <10> */
/* 110 */ "fsubp st(1), st",	/* fpu_reg : sub( fpu_reg, fpu_reg ), "fsubp st(1), st", <10> */
/* 111 */ "fmul %1",	/* fpu_reg : mul( fpu_reg, float_value ), "fmul %1", <10> */
/* 112 */ "fmulp st(1), st",	/* fpu_reg : mul( fpu_reg, fpu_reg ), "fmulp st(1), st", <10> */
/* 113 */ "fdiv %1",	/* fpu_reg : fdiv( fpu_reg, float_value ), "fdiv %1", <10> */
/* 114 */ "fdivp st(1), st",	/* fpu_reg : fdiv( fpu_reg, fpu_reg ), "fdivp st(1), st", <10> */
/* 115 */ "fchs",	/* fpu_reg : fneg( fpu_reg ), "fchs", <10> */
/* 116 */ "",	/* r32 : f2b( eq ), "", <12> */
/* 117 */ "",	/* r32 : f2i( eq ), "", <12> */
/* 118 */ "dword ptr %0",	/* addrj : address_32, "dword ptr %0" */
/* 119 */ "%0",	/* addrj : r32, "%0", <2> */
/* 120 */ "%0",	/* addrj : mem, "%0", <2> */
/* 121 */ "%0",	/* addrj : label, "%0" */
/* 122 */ "jmp %c",	/* stmt : jmp( addrj ), "jmp %c", <3> */
/* 123 */ "%c:",	/* stmt : label, "%c:", <3> */
/* 124 */ "cmp %0, %1",	/* boolean_value : eq( r32, mrc ), "cmp %0, %1", <5> */
/* 125 */ "cmp %0, %1",	/* boolean_value : eq( mr, rc ), "cmp %0, %1", <5> */
/* 126 */ "cmp %0, %1",	/* boolean_value : neq( r32, mrc ), "cmp %0, %1", <5> */
/* 127 */ "cmp %0, %1",	/* boolean_value : neq( mr, rc ), "cmp %0, %1", <5> */
/* 128 */ "cmp %0, %1",	/* boolean_value : lt( r32, mrc ), "cmp %0, %1", <5> */
/* 129 */ "cmp %0, %1",	/* boolean_value : lt( mr, rc ), "cmp %0, %1", <5> */
/* 130 */ "cmp %0, %1",	/* boolean_value : gt( r32, mrc ), "cmp %0, %1", <5> */
/* 131 */ "cmp %0, %1",	/* boolean_value : gt( mr, rc ), "cmp %0, %1", <5> */
/* 132 */ "cmp %0, %1",	/* boolean_value : le( r32, mrc ), "cmp %0, %1", <5> */
/* 133 */ "cmp %0, %1",	/* boolean_value : le( mr, rc ), "cmp %0, %1", <5> */
/* 134 */ "cmp %0, %1",	/* boolean_value : ge( r32, mrc ), "cmp %0, %1", <5> */
/* 135 */ "cmp %0, %1",	/* boolean_value : ge( mr, rc ), "cmp %0, %1", <5> */
/* 136 */ "test %0, %0",	/* boolean_value : r32, "test %0, %0", <3> */
/* 137 */ NULL,	/* stmt : boolean_value, "" */
/* 138 */ "mov %0, %1",	/* boolean_value : assign( address, imm ), "mov %0, %1", <4> */
/* 139 */ "mov %0, %1",	/* boolean_value : assign( address, r32 ), "mov %0, %1", <4> */
/* 140 */ "%0",	/* _label : label, "%0" */
/* 141 */ "",	/* stmt : br_false( boolean_value, _label ), "", <5> */
/* 142 */ "",	/* stmt : br_true( boolean_value, _label ), "", <5> */
/* 143 */ NULL,	/* stmt : nop, "" */
/* 144 */ "_%0 proc",	/* stmt : func_start, "_%0 proc", <15> */
/* 145 */ "_%0 endp",	/* stmt : func_end, "_%0 endp", <15> */
/* 146 */ NULL,	/* stmt : ret( nop ), "" */
/* 147 */ NULL,	/* stmt : ret( r32 ), "" */
/* 148 */ "push %0",	/* stmt : arg( mrc ), "push %0", <5> */
/* 149 */ "push offset %0",	/* stmt : arg( _label ), "push offset %0", <3> */
/* 150 */ "call %0",	/* r32 : call( addrj ), "call %0", <3> */
/* 151 */ "inc %0",	/* stmt : preinc( mem ), "inc %0", <5> */
/* 152 */ "inc %0",	/* r32 : preinc( r32 ), "inc %0", <5> */
/* 153 */ "dec %0",	/* stmt : predec( mem ), "dec %0", <5> */
/* 154 */ "dec %0",	/* r32 : predec( r32 ), "dec %0", <5> */
/* 155 */ "inc %0",	/* stmt : postinc( mem ), "inc %0", <5> */
/* 156 */ "dec %0",	/* stmt : postdec( mem ), "dec %0", <5> */
};


// rules ( for debugging )
//
const char * g_rules[ NUM_RULES + 1 ] = {
/* 0 */		NULL,
/* 1 */ "address : address_8",
/* 2 */ "address : address_16",
/* 3 */ "address : address_32",
/* 4 */ "mem_s8 : indir_i8( address_8 )",
/* 5 */ "mem_u8 : indir_u8( address_8 )",
/* 6 */ "mem_s16 : indir_i16( address_16 )",
/* 7 */ "mem_u16 : indir_u16( address_16 )",
/* 8 */ "mem : indir_i32( address_32 )",
/* 9 */ "imm : iconst",
/* 10 */ "imm : iconst_0",
/* 11 */ "imm : iconst_1",
/* 12 */ "imm : iconst_2",
/* 13 */ "imm : iconst_4",
/* 14 */ "imm : iconst_8",
/* 15 */ "iconst_2_4_8 : iconst_2",
/* 16 */ "iconst_2_4_8 : iconst_4",
/* 17 */ "iconst_2_4_8 : iconst_8",
/* 18 */ "mc : mem",
/* 19 */ "mc : imm",
/* 20 */ "mr : mem",
/* 21 */ "mr : r32",
/* 22 */ "rc : r32",
/* 23 */ "rc : imm",
/* 24 */ "mrc : rc",
/* 25 */ "mrc : mem",
/* 26 */ "r32 : mem_s8",
/* 27 */ "r32 : mem_u8",
/* 28 */ "r32 : mem_s16",
/* 29 */ "r32 : mem_u16",
/* 30 */ "r32 : imm",
/* 31 */ "r32 : iconst_0",
/* 32 */ "r32 : mem",
/* 33 */ "rc : assign( address, rc )",
/* 34 */ "stmt : rc",
/* 35 */ "r32 : assign( r32, mrc )",
/* 36 */ "r32 : add( r32, mrc )",
/* 37 */ "mr : add( mr, rc )",
/* 38 */ "r32 : add( r32, iconst_1 )",
/* 39 */ "r32 : add( mul, rc )",
/* 40 */ "r32 : add( add, imm )",
/* 41 */ "r32 : add( add, r32 )",
/* 42 */ "r32 : add( add, imm )",
/* 43 */ "r32 : sub( r32, mrc )",
/* 44 */ "mr : sub( mr, rc )",
/* 45 */ "r32 : sub( r32, iconst_1 )",
/* 46 */ "r32 : add( mul, sub )",
/* 47 */ "r32 : add( mul, add )",
/* 48 */ "r32 : mul( r32, imm )",
/* 49 */ "r32 : mul( r32, imm )",
/* 50 */ "r32 : mul( r32, mrc )",
/* 51 */ "r32 : mul( mem, imm )",
/* 52 */ "r32 : div( r32, mr )",
/* 53 */ "r32 : udiv( r32, mr )",
/* 54 */ "r32 : rem( r32, mr )",
/* 55 */ "r32 : shl( r32, imm )",
/* 56 */ "r32 : shl( r32, r32 )",
/* 57 */ "r32 : shr( r32, imm )",
/* 58 */ "r32 : shr( r32, r32 )",
/* 59 */ "r32 : ushr( r32, imm )",
/* 60 */ "r32 : ushr( r32, r32 )",
/* 61 */ "r32 : mul( r32, iconst_2 )",
/* 62 */ "r32 : mul( r32, iconst_4 )",
/* 63 */ "r32 : mul( r32, iconst_8 )",
/* 64 */ "r32 : div( r32, iconst_2 )",
/* 65 */ "r32 : div( r32, iconst_4 )",
/* 66 */ "r32 : div( r32, iconst_8 )",
/* 67 */ "r32 : udiv( r32, iconst_2 )",
/* 68 */ "r32 : udiv( r32, iconst_4 )",
/* 69 */ "r32 : udiv( r32, iconst_8 )",
/* 70 */ "r32 : neg( r32 )",
/* 71 */ "r32 : and( r32, mrc )",
/* 72 */ "r32 : or( r32, mrc )",
/* 73 */ "r32 : xor( r32, mrc )",
/* 74 */ "r32 : not( r32 )",
/* 75 */ "r32 : compl( r32 )",
/* 76 */ "r32 : i2b( mr )",
/* 77 */ "r32 : i2b( r32 )",
/* 78 */ "r32 : gt( r32, mrc )",
/* 79 */ "r32 : gt( mem, rc )",
/* 80 */ "r32 : eq( r32, mrc )",
/* 81 */ "r32 : eq( mem, rc )",
/* 82 */ "r32 : neq( r32, mrc )",
/* 83 */ "r32 : neq( mem, rc )",
/* 84 */ "r32 : lt( r32, mrc )",
/* 85 */ "r32 : lt( mem, rc )",
/* 86 */ "r32 : le( r32, mrc )",
/* 87 */ "r32 : le( mem, rc )",
/* 88 */ "r32 : ge( r32, mrc )",
/* 89 */ "r32 : ge( mem, rc )",
/* 90 */ "memf32 : indir_f32( address_f32 )",
/* 91 */ "memf64 : indir_f64( address_f64 )",
/* 92 */ "addr_f32 : address_f32",
/* 93 */ "addr_f64 : address_f64",
/* 94 */ "float_address : addr_f32",
/* 95 */ "float_address : addr_f64",
/* 96 */ "float_const : fconst",
/* 97 */ "float_value : float_const",
/* 98 */ "float_value : memf32",
/* 99 */ "float_value : memf64",
/* 100 */ "fpu_reg : float_value",
/* 101 */ "fpu_reg : float_address",
/* 102 */ "r32 : f2i( fpu_reg )",
/* 103 */ "fpu_reg : i2f( mem )",
/* 104 */ "fpu_reg : i2f( mr )",
/* 105 */ "float_address : assign( float_address, fpu_reg )",
/* 106 */ "stmt : float_address",
/* 107 */ "fpu_reg : add( fpu_reg, float_value )",
/* 108 */ "fpu_reg : add( fpu_reg, fpu_reg )",
/* 109 */ "fpu_reg : sub( fpu_reg, float_value )",
/* 110 */ "fpu_reg : sub( fpu_reg, fpu_reg )",
/* 111 */ "fpu_reg : mul( fpu_reg, float_value )",
/* 112 */ "fpu_reg : mul( fpu_reg, fpu_reg )",
/* 113 */ "fpu_reg : fdiv( fpu_reg, float_value )",
/* 114 */ "fpu_reg : fdiv( fpu_reg, fpu_reg )",
/* 115 */ "fpu_reg : fneg( fpu_reg )",
/* 116 */ "r32 : f2b( eq )",
/* 117 */ "r32 : f2i( eq )",
/* 118 */ "addrj : address_32",
/* 119 */ "addrj : r32",
/* 120 */ "addrj : mem",
/* 121 */ "addrj : label",
/* 122 */ "stmt : jmp( addrj )",
/* 123 */ "stmt : label",
/* 124 */ "boolean_value : eq( r32, mrc )",
/* 125 */ "boolean_value : eq( mr, rc )",
/* 126 */ "boolean_value : neq( r32, mrc )",
/* 127 */ "boolean_value : neq( mr, rc )",
/* 128 */ "boolean_value : lt( r32, mrc )",
/* 129 */ "boolean_value : lt( mr, rc )",
/* 130 */ "boolean_value : gt( r32, mrc )",
/* 131 */ "boolean_value : gt( mr, rc )",
/* 132 */ "boolean_value : le( r32, mrc )",
/* 133 */ "boolean_value : le( mr, rc )",
/* 134 */ "boolean_value : ge( r32, mrc )",
/* 135 */ "boolean_value : ge( mr, rc )",
/* 136 */ "boolean_value : r32",
/* 137 */ "stmt : boolean_value",
/* 138 */ "boolean_value : assign( address, imm )",
/* 139 */ "boolean_value : assign( address, r32 )",
/* 140 */ "_label : label",
/* 141 */ "stmt : br_false( boolean_value, _label )",
/* 142 */ "stmt : br_true( boolean_value, _label )",
/* 143 */ "stmt : nop",
/* 144 */ "stmt : func_start",
/* 145 */ "stmt : func_end",
/* 146 */ "stmt : ret( nop )",
/* 147 */ "stmt : ret( r32 )",
/* 148 */ "stmt : arg( mrc )",
/* 149 */ "stmt : arg( _label )",
/* 150 */ "r32 : call( addrj )",
/* 151 */ "stmt : preinc( mem )",
/* 152 */ "r32 : preinc( r32 )",
/* 153 */ "stmt : predec( mem )",
/* 154 */ "r32 : predec( r32 )",
/* 155 */ "stmt : postinc( mem )",
/* 156 */ "stmt : postdec( mem )",
};

// nonterminals on rhs of rules
//
const int _nonterms_1[] = { 0, 0 };
const int _nonterms_2[] = { 0, 0 };
const int _nonterms_3[] = { 0, 0 };
const int _nonterms_4[] = { 0, 0 };
const int _nonterms_5[] = { 0, 0 };
const int _nonterms_6[] = { 0, 0 };
const int _nonterms_7[] = { 0, 0 };
const int _nonterms_8[] = { 0, 0 };
const int _nonterms_9[] = { 0, 0 };
const int _nonterms_10[] = { 0, 0 };
const int _nonterms_11[] = { 0, 0 };
const int _nonterms_12[] = { 0, 0 };
const int _nonterms_13[] = { 0, 0 };
const int _nonterms_14[] = { 0, 0 };
const int _nonterms_15[] = { 0, 0 };
const int _nonterms_16[] = { 0, 0 };
const int _nonterms_17[] = { 0, 0 };
const int _nonterms_18[] = { mem_NT, 0 };
const int _nonterms_19[] = { imm_NT, 0 };
const int _nonterms_20[] = { mem_NT, 0 };
const int _nonterms_21[] = { r32_NT, 0 };
const int _nonterms_22[] = { r32_NT, 0 };
const int _nonterms_23[] = { imm_NT, 0 };
const int _nonterms_24[] = { rc_NT, 0 };
const int _nonterms_25[] = { mem_NT, 0 };
const int _nonterms_26[] = { mem_s8_NT, 0 };
const int _nonterms_27[] = { mem_u8_NT, 0 };
const int _nonterms_28[] = { mem_s16_NT, 0 };
const int _nonterms_29[] = { mem_u16_NT, 0 };
const int _nonterms_30[] = { imm_NT, 0 };
const int _nonterms_31[] = { 0, 0 };
const int _nonterms_32[] = { mem_NT, 0 };
const int _nonterms_33[] = { address_NT, rc_NT, 0 };
const int _nonterms_34[] = { rc_NT, 0 };
const int _nonterms_35[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_36[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_37[] = { mr_NT, rc_NT, 0 };
const int _nonterms_38[] = { r32_NT, 0, 0 };
const int _nonterms_39[] = { r32_NT, iconst_2_4_8_NT, rc_NT, 0 };
const int _nonterms_40[] = { r32_NT, iconst_2_4_8_NT, r32_NT, imm_NT, 0 };
const int _nonterms_41[] = { r32_NT, imm_NT, r32_NT, 0 };
const int _nonterms_42[] = { r32_NT, r32_NT, imm_NT, 0 };
const int _nonterms_43[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_44[] = { mr_NT, rc_NT, 0 };
const int _nonterms_45[] = { r32_NT, 0, 0 };
const int _nonterms_46[] = { r32_NT, iconst_2_4_8_NT, r32_NT, imm_NT, 0 };
const int _nonterms_47[] = { r32_NT, iconst_2_4_8_NT, r32_NT, imm_NT, 0 };
const int _nonterms_48[] = { r32_NT, imm_NT, 0 };
const int _nonterms_49[] = { r32_NT, imm_NT, 0 };
const int _nonterms_50[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_51[] = { mem_NT, imm_NT, 0 };
const int _nonterms_52[] = { r32_NT, mr_NT, 0 };
const int _nonterms_53[] = { r32_NT, mr_NT, 0 };
const int _nonterms_54[] = { r32_NT, mr_NT, 0 };
const int _nonterms_55[] = { r32_NT, imm_NT, 0 };
const int _nonterms_56[] = { r32_NT, r32_NT, 0 };
const int _nonterms_57[] = { r32_NT, imm_NT, 0 };
const int _nonterms_58[] = { r32_NT, r32_NT, 0 };
const int _nonterms_59[] = { r32_NT, imm_NT, 0 };
const int _nonterms_60[] = { r32_NT, r32_NT, 0 };
const int _nonterms_61[] = { r32_NT, 0, 0 };
const int _nonterms_62[] = { r32_NT, 0, 0 };
const int _nonterms_63[] = { r32_NT, 0, 0 };
const int _nonterms_64[] = { r32_NT, 0, 0 };
const int _nonterms_65[] = { r32_NT, 0, 0 };
const int _nonterms_66[] = { r32_NT, 0, 0 };
const int _nonterms_67[] = { r32_NT, 0, 0 };
const int _nonterms_68[] = { r32_NT, 0, 0 };
const int _nonterms_69[] = { r32_NT, 0, 0 };
const int _nonterms_70[] = { r32_NT, 0 };
const int _nonterms_71[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_72[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_73[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_74[] = { r32_NT, 0 };
const int _nonterms_75[] = { r32_NT, 0 };
const int _nonterms_76[] = { mr_NT, 0 };
const int _nonterms_77[] = { r32_NT, 0 };
const int _nonterms_78[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_79[] = { mem_NT, rc_NT, 0 };
const int _nonterms_80[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_81[] = { mem_NT, rc_NT, 0 };
const int _nonterms_82[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_83[] = { mem_NT, rc_NT, 0 };
const int _nonterms_84[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_85[] = { mem_NT, rc_NT, 0 };
const int _nonterms_86[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_87[] = { mem_NT, rc_NT, 0 };
const int _nonterms_88[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_89[] = { mem_NT, rc_NT, 0 };
const int _nonterms_90[] = { 0, 0 };
const int _nonterms_91[] = { 0, 0 };
const int _nonterms_92[] = { 0, 0 };
const int _nonterms_93[] = { 0, 0 };
const int _nonterms_94[] = { addr_f32_NT, 0 };
const int _nonterms_95[] = { addr_f64_NT, 0 };
const int _nonterms_96[] = { 0, 0 };
const int _nonterms_97[] = { float_const_NT, 0 };
const int _nonterms_98[] = { memf32_NT, 0 };
const int _nonterms_99[] = { memf64_NT, 0 };
const int _nonterms_100[] = { float_value_NT, 0 };
const int _nonterms_101[] = { float_address_NT, 0 };
const int _nonterms_102[] = { fpu_reg_NT, 0 };
const int _nonterms_103[] = { mem_NT, 0 };
const int _nonterms_104[] = { mr_NT, 0 };
const int _nonterms_105[] = { float_address_NT, fpu_reg_NT, 0 };
const int _nonterms_106[] = { float_address_NT, 0 };
const int _nonterms_107[] = { fpu_reg_NT, float_value_NT, 0 };
const int _nonterms_108[] = { fpu_reg_NT, fpu_reg_NT, 0 };
const int _nonterms_109[] = { fpu_reg_NT, float_value_NT, 0 };
const int _nonterms_110[] = { fpu_reg_NT, fpu_reg_NT, 0 };
const int _nonterms_111[] = { fpu_reg_NT, float_value_NT, 0 };
const int _nonterms_112[] = { fpu_reg_NT, fpu_reg_NT, 0 };
const int _nonterms_113[] = { fpu_reg_NT, float_value_NT, 0 };
const int _nonterms_114[] = { fpu_reg_NT, fpu_reg_NT, 0 };
const int _nonterms_115[] = { fpu_reg_NT, 0 };
const int _nonterms_116[] = { fpu_reg_NT, fpu_reg_NT, 0 };
const int _nonterms_117[] = { fpu_reg_NT, fpu_reg_NT, 0 };
const int _nonterms_118[] = { 0, 0 };
const int _nonterms_119[] = { r32_NT, 0 };
const int _nonterms_120[] = { mem_NT, 0 };
const int _nonterms_121[] = { 0, 0 };
const int _nonterms_122[] = { addrj_NT, 0 };
const int _nonterms_123[] = { 0, 0 };
const int _nonterms_124[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_125[] = { mr_NT, rc_NT, 0 };
const int _nonterms_126[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_127[] = { mr_NT, rc_NT, 0 };
const int _nonterms_128[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_129[] = { mr_NT, rc_NT, 0 };
const int _nonterms_130[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_131[] = { mr_NT, rc_NT, 0 };
const int _nonterms_132[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_133[] = { mr_NT, rc_NT, 0 };
const int _nonterms_134[] = { r32_NT, mrc_NT, 0 };
const int _nonterms_135[] = { mr_NT, rc_NT, 0 };
const int _nonterms_136[] = { r32_NT, 0 };
const int _nonterms_137[] = { boolean_value_NT, 0 };
const int _nonterms_138[] = { address_NT, imm_NT, 0 };
const int _nonterms_139[] = { address_NT, r32_NT, 0 };
const int _nonterms_140[] = { 0, 0 };
const int _nonterms_141[] = { boolean_value_NT, _label_NT, 0 };
const int _nonterms_142[] = { boolean_value_NT, _label_NT, 0 };
const int _nonterms_143[] = { 0, 0 };
const int _nonterms_144[] = { 0, 0 };
const int _nonterms_145[] = { 0, 0 };
const int _nonterms_146[] = { 0, 0 };
const int _nonterms_147[] = { r32_NT, 0 };
const int _nonterms_148[] = { mrc_NT, 0 };
const int _nonterms_149[] = { _label_NT, 0 };
const int _nonterms_150[] = { addrj_NT, 0 };
const int _nonterms_151[] = { mem_NT, 0 };
const int _nonterms_152[] = { r32_NT, 0 };
const int _nonterms_153[] = { mem_NT, 0 };
const int _nonterms_154[] = { r32_NT, 0 };
const int _nonterms_155[] = { mem_NT, 0 };
const int _nonterms_156[] = { mem_NT, 0 };

// indices into the jagged array above
//
const int * g_nonterms[ NUM_RULES + 1 ] = {
/* 0 */		{ 0 },
/* 1 */ _nonterms_1,		/* address : address_8, "byte ptr [%0]" */
/* 2 */ _nonterms_2,		/* address : address_16, "word ptr [%0]" */
/* 3 */ _nonterms_3,		/* address : address_32, "dword ptr [%0]" */
/* 4 */ _nonterms_4,		/* mem_s8 : indir_i8( address_8 ), "byte ptr [%0]" */
/* 5 */ _nonterms_5,		/* mem_u8 : indir_u8( address_8 ), "byte ptr [%0]" */
/* 6 */ _nonterms_6,		/* mem_s16 : indir_i16( address_16 ), "word ptr [%0]" */
/* 7 */ _nonterms_7,		/* mem_u16 : indir_u16( address_16 ), "word ptr [%0]" */
/* 8 */ _nonterms_8,		/* mem : indir_i32( address_32 ), "dword ptr [%0]" */
/* 9 */ _nonterms_9,		/* imm : iconst, "%0" */
/* 10 */ _nonterms_10,		/* imm : iconst_0, "%0" */
/* 11 */ _nonterms_11,		/* imm : iconst_1, "%0" */
/* 12 */ _nonterms_12,		/* imm : iconst_2, "%0" */
/* 13 */ _nonterms_13,		/* imm : iconst_4, "%0" */
/* 14 */ _nonterms_14,		/* imm : iconst_8, "%0" */
/* 15 */ _nonterms_15,		/* iconst_2_4_8 : iconst_2, "2" */
/* 16 */ _nonterms_16,		/* iconst_2_4_8 : iconst_4, "4" */
/* 17 */ _nonterms_17,		/* iconst_2_4_8 : iconst_8, "8" */
/* 18 */ _nonterms_18,		/* mc : mem, "%0" */
/* 19 */ _nonterms_19,		/* mc : imm, "%0" */
/* 20 */ _nonterms_20,		/* mr : mem, "%0" */
/* 21 */ _nonterms_21,		/* mr : r32, "%0" */
/* 22 */ _nonterms_22,		/* rc : r32, "%0" */
/* 23 */ _nonterms_23,		/* rc : imm, "%0" */
/* 24 */ _nonterms_24,		/* mrc : rc, "%0" */
/* 25 */ _nonterms_25,		/* mrc : mem, "%0" */
/* 26 */ _nonterms_26,		/* r32 : mem_s8, "movsx %c, %0", <4> */
/* 27 */ _nonterms_27,		/* r32 : mem_u8, "movzx %c, %0", <4> */
/* 28 */ _nonterms_28,		/* r32 : mem_s16, "movsx %c, %0", <3> */
/* 29 */ _nonterms_29,		/* r32 : mem_u16, "movzx %c, %0", <3> */
/* 30 */ _nonterms_30,		/* r32 : imm, "mov %c, %0", <3> */
/* 31 */ _nonterms_31,		/* r32 : iconst_0, "xor %c, %c", <1> */
/* 32 */ _nonterms_32,		/* r32 : mem, "mov %c, %0", <3> */
/* 33 */ _nonterms_33,		/* rc : assign( address, rc ), "mov %0, %1", <2> */
/* 34 */ _nonterms_34,		/* stmt : rc, "" */
/* 35 */ _nonterms_35,		/* r32 : assign( r32, mrc ), "mov %0, %1", <2> */
/* 36 */ _nonterms_36,		/* r32 : add( r32, mrc ), "add %0, %1", <6> */
/* 37 */ _nonterms_37,		/* mr : add( mr, rc ), "add %0, %1", <7> */
/* 38 */ _nonterms_38,		/* r32 : add( r32, iconst_1 ), "inc %0", <5> */
/* 39 */ _nonterms_39,		/* r32 : add( mul, rc ), "lea %c, [%0*%1 + %2]", <3> */
/* 40 */ _nonterms_40,		/* r32 : add( add, imm ), "lea %c, [%2 + %0*%1 + %3]", <3> */
/* 41 */ _nonterms_41,		/* r32 : add( add, r32 ), "lea %c, [%0 + %2 + %1]", <3> */
/* 42 */ _nonterms_42,		/* r32 : add( add, imm ), "lea %c, [%0 + %1 + %2]", <3> */
/* 43 */ _nonterms_43,		/* r32 : sub( r32, mrc ), "sub %0, %1", <6> */
/* 44 */ _nonterms_44,		/* mr : sub( mr, rc ), "sub %0, %1", <6> */
/* 45 */ _nonterms_45,		/* r32 : sub( r32, iconst_1 ), "dec %0", <5> */
/* 46 */ _nonterms_46,		/* r32 : add( mul, sub ), "lea %c, [%2 + %0*%1 - %3]", <3> */
/* 47 */ _nonterms_47,		/* r32 : add( mul, add ), "lea %c, [%2 + %0*%1 + %3]", <3> */
/* 48 */ _nonterms_48,		/* r32 : mul( r32, imm ), "imul %c, %0, %1", <13> */
/* 49 */ _nonterms_49,		/* r32 : mul( r32, imm ), "imul %0, %0, %1", <13> */
/* 50 */ _nonterms_50,		/* r32 : mul( r32, mrc ), "imul %0, %1", <14> */
/* 51 */ _nonterms_51,		/* r32 : mul( mem, imm ), "imul %c, %0, %1", <14> */
/* 52 */ _nonterms_52,		/* r32 : div( r32, mr ), "cdq\nidiv %1", <14> */
/* 53 */ _nonterms_53,		/* r32 : udiv( r32, mr ), "xor edx, edx\ndiv %1", <14> */
/* 54 */ _nonterms_54,		/* r32 : rem( r32, mr ), "cdq\nidiv %1", <14> */
/* 55 */ _nonterms_55,		/* r32 : shl( r32, imm ), "shl %0, %1", <6> */
/* 56 */ _nonterms_56,		/* r32 : shl( r32, r32 ), "shl %0, cl", <7> */
/* 57 */ _nonterms_57,		/* r32 : shr( r32, imm ), "sar %0, %1", <6> */
/* 58 */ _nonterms_58,		/* r32 : shr( r32, r32 ), "sar %0, cl", <7> */
/* 59 */ _nonterms_59,		/* r32 : ushr( r32, imm ), "shr %0, %1", <6> */
/* 60 */ _nonterms_60,		/* r32 : ushr( r32, r32 ), "shr %0, cl", <7> */
/* 61 */ _nonterms_61,		/* r32 : mul( r32, iconst_2 ), "shl %0, 1", <7> */
/* 62 */ _nonterms_62,		/* r32 : mul( r32, iconst_4 ), "shl %0, 2", <7> */
/* 63 */ _nonterms_63,		/* r32 : mul( r32, iconst_8 ), "shl %0, 3", <7> */
/* 64 */ _nonterms_64,		/* r32 : div( r32, iconst_2 ), "shr %0, 1", <7> */
/* 65 */ _nonterms_65,		/* r32 : div( r32, iconst_4 ), "shr %0, 2", <7> */
/* 66 */ _nonterms_66,		/* r32 : div( r32, iconst_8 ), "shr %0, 3", <7> */
/* 67 */ _nonterms_67,		/* r32 : udiv( r32, iconst_2 ), "shr %0, 1", <7> */
/* 68 */ _nonterms_68,		/* r32 : udiv( r32, iconst_4 ), "shr %0, 2", <7> */
/* 69 */ _nonterms_69,		/* r32 : udiv( r32, iconst_8 ), "shr %0, 3", <7> */
/* 70 */ _nonterms_70,		/* r32 : neg( r32 ), "neg %0", <5> */
/* 71 */ _nonterms_71,		/* r32 : and( r32, mrc ), "and %0, %1", <5> */
/* 72 */ _nonterms_72,		/* r32 : or( r32, mrc ), "or %0, %1", <5> */
/* 73 */ _nonterms_73,		/* r32 : xor( r32, mrc ), "xor %0, %1", <5> */
/* 74 */ _nonterms_74,		/* r32 : not( r32 ), "", <5> */
/* 75 */ _nonterms_75,		/* r32 : compl( r32 ), "not %0", <5> */
/* 76 */ _nonterms_76,		/* r32 : i2b( mr ), "", <7> */
/* 77 */ _nonterms_77,		/* r32 : i2b( r32 ), "", <6> */
/* 78 */ _nonterms_78,		/* r32 : gt( r32, mrc ), "", <7> */
/* 79 */ _nonterms_79,		/* r32 : gt( mem, rc ), "", <7> */
/* 80 */ _nonterms_80,		/* r32 : eq( r32, mrc ), "", <7> */
/* 81 */ _nonterms_81,		/* r32 : eq( mem, rc ), "", <7> */
/* 82 */ _nonterms_82,		/* r32 : neq( r32, mrc ), "", <7> */
/* 83 */ _nonterms_83,		/* r32 : neq( mem, rc ), "", <7> */
/* 84 */ _nonterms_84,		/* r32 : lt( r32, mrc ), "", <7> */
/* 85 */ _nonterms_85,		/* r32 : lt( mem, rc ), "", <7> */
/* 86 */ _nonterms_86,		/* r32 : le( r32, mrc ), "", <7> */
/* 87 */ _nonterms_87,		/* r32 : le( mem, rc ), "", <7> */
/* 88 */ _nonterms_88,		/* r32 : ge( r32, mrc ), "", <7> */
/* 89 */ _nonterms_89,		/* r32 : ge( mem, rc ), "", <7> */
/* 90 */ _nonterms_90,		/* memf32 : indir_f32( address_f32 ), "dword ptr [%0]" */
/* 91 */ _nonterms_91,		/* memf64 : indir_f64( address_f64 ), "qword ptr [%0]" */
/* 92 */ _nonterms_92,		/* addr_f32 : address_f32, "dword ptr [%0]" */
/* 93 */ _nonterms_93,		/* addr_f64 : address_f64, "qword ptr [%0]" */
/* 94 */ _nonterms_94,		/* float_address : addr_f32, "%0" */
/* 95 */ _nonterms_95,		/* float_address : addr_f64, "%0" */
/* 96 */ _nonterms_96,		/* float_const : fconst, "dword ptr %0" */
/* 97 */ _nonterms_97,		/* float_value : float_const, "%0" */
/* 98 */ _nonterms_98,		/* float_value : memf32, "%0" */
/* 99 */ _nonterms_99,		/* float_value : memf64, "%0" */
/* 100 */ _nonterms_100,		/* fpu_reg : float_value, "fld %0", <10> */
/* 101 */ _nonterms_101,		/* fpu_reg : float_address, "fld %0", <10> */
/* 102 */ _nonterms_102,		/* r32 : f2i( fpu_reg ), "call __ftol2_sse", <10> */
/* 103 */ _nonterms_103,		/* fpu_reg : i2f( mem ), "fild %0", <7> */
/* 104 */ _nonterms_104,		/* fpu_reg : i2f( mr ), "push %0\nfild dword ptr [esp]\nadd esp, 4", <7> */
/* 105 */ _nonterms_105,		/* float_address : assign( float_address, fpu_reg ), "fstp %0", <10> */
/* 106 */ _nonterms_106,		/* stmt : float_address, "" */
/* 107 */ _nonterms_107,		/* fpu_reg : add( fpu_reg, float_value ), "fadd %1", <10> */
/* 108 */ _nonterms_108,		/* fpu_reg : add( fpu_reg, fpu_reg ), "faddp st(1), st", <10> */
/* 109 */ _nonterms_109,		/* fpu_reg : sub( fpu_reg, float_value ), "fsub %1", <10> */
/* 110 */ _nonterms_110,		/* fpu_reg : sub( fpu_reg, fpu_reg ), "fsubp st(1), st", <10> */
/* 111 */ _nonterms_111,		/* fpu_reg : mul( fpu_reg, float_value ), "fmul %1", <10> */
/* 112 */ _nonterms_112,		/* fpu_reg : mul( fpu_reg, fpu_reg ), "fmulp st(1), st", <10> */
/* 113 */ _nonterms_113,		/* fpu_reg : fdiv( fpu_reg, float_value ), "fdiv %1", <10> */
/* 114 */ _nonterms_114,		/* fpu_reg : fdiv( fpu_reg, fpu_reg ), "fdivp st(1), st", <10> */
/* 115 */ _nonterms_115,		/* fpu_reg : fneg( fpu_reg ), "fchs", <10> */
/* 116 */ _nonterms_116,		/* r32 : f2b( eq ), "", <12> */
/* 117 */ _nonterms_117,		/* r32 : f2i( eq ), "", <12> */
/* 118 */ _nonterms_118,		/* addrj : address_32, "dword ptr %0" */
/* 119 */ _nonterms_119,		/* addrj : r32, "%0", <2> */
/* 120 */ _nonterms_120,		/* addrj : mem, "%0", <2> */
/* 121 */ _nonterms_121,		/* addrj : label, "%0" */
/* 122 */ _nonterms_122,		/* stmt : jmp( addrj ), "jmp %c", <3> */
/* 123 */ _nonterms_123,		/* stmt : label, "%c:", <3> */
/* 124 */ _nonterms_124,		/* boolean_value : eq( r32, mrc ), "cmp %0, %1", <5> */
/* 125 */ _nonterms_125,		/* boolean_value : eq( mr, rc ), "cmp %0, %1", <5> */
/* 126 */ _nonterms_126,		/* boolean_value : neq( r32, mrc ), "cmp %0, %1", <5> */
/* 127 */ _nonterms_127,		/* boolean_value : neq( mr, rc ), "cmp %0, %1", <5> */
/* 128 */ _nonterms_128,		/* boolean_value : lt( r32, mrc ), "cmp %0, %1", <5> */
/* 129 */ _nonterms_129,		/* boolean_value : lt( mr, rc ), "cmp %0, %1", <5> */
/* 130 */ _nonterms_130,		/* boolean_value : gt( r32, mrc ), "cmp %0, %1", <5> */
/* 131 */ _nonterms_131,		/* boolean_value : gt( mr, rc ), "cmp %0, %1", <5> */
/* 132 */ _nonterms_132,		/* boolean_value : le( r32, mrc ), "cmp %0, %1", <5> */
/* 133 */ _nonterms_133,		/* boolean_value : le( mr, rc ), "cmp %0, %1", <5> */
/* 134 */ _nonterms_134,		/* boolean_value : ge( r32, mrc ), "cmp %0, %1", <5> */
/* 135 */ _nonterms_135,		/* boolean_value : ge( mr, rc ), "cmp %0, %1", <5> */
/* 136 */ _nonterms_136,		/* boolean_value : r32, "test %0, %0", <3> */
/* 137 */ _nonterms_137,		/* stmt : boolean_value, "" */
/* 138 */ _nonterms_138,		/* boolean_value : assign( address, imm ), "mov %0, %1", <4> */
/* 139 */ _nonterms_139,		/* boolean_value : assign( address, r32 ), "mov %0, %1", <4> */
/* 140 */ _nonterms_140,		/* _label : label, "%0" */
/* 141 */ _nonterms_141,		/* stmt : br_false( boolean_value, _label ), "", <5> */
/* 142 */ _nonterms_142,		/* stmt : br_true( boolean_value, _label ), "", <5> */
/* 143 */ _nonterms_143,		/* stmt : nop, "" */
/* 144 */ _nonterms_144,		/* stmt : func_start, "_%0 proc", <15> */
/* 145 */ _nonterms_145,		/* stmt : func_end, "_%0 endp", <15> */
/* 146 */ _nonterms_146,		/* stmt : ret( nop ), "" */
/* 147 */ _nonterms_147,		/* stmt : ret( r32 ), "" */
/* 148 */ _nonterms_148,		/* stmt : arg( mrc ), "push %0", <5> */
/* 149 */ _nonterms_149,		/* stmt : arg( _label ), "push offset %0", <3> */
/* 150 */ _nonterms_150,		/* r32 : call( addrj ), "call %0", <3> */
/* 151 */ _nonterms_151,		/* stmt : preinc( mem ), "inc %0", <5> */
/* 152 */ _nonterms_152,		/* r32 : preinc( r32 ), "inc %0", <5> */
/* 153 */ _nonterms_153,		/* stmt : predec( mem ), "dec %0", <5> */
/* 154 */ _nonterms_154,		/* r32 : predec( r32 ), "dec %0", <5> */
/* 155 */ _nonterms_155,		/* stmt : postinc( mem ), "inc %0", <5> */
/* 156 */ _nonterms_156,		/* stmt : postdec( mem ), "dec %0", <5> */
};

// stores the kids of the given tree into the array kids[]
void Machine_x86::_GetKids( IRCode * irTree, int rulenum, IRCode * kids[], int *numKids )
{
	if ( ! irTree ) {
		c4::common->Error( "Machine_x86::_GetKids() : null tree!\n" );
	}
	if ( ! kids ) {
		c4::common->Error( "Machine_x86::_GetKids() : null kids!\n" );
	}
	switch ( rulenum ) {
	case 0: return;
	case 1:	/* address : address_8 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 2:	/* address : address_16 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 3:	/* address : address_32 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 4:	/* mem_s8 : indir_i8( address_8 ) */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 5:	/* mem_u8 : indir_u8( address_8 ) */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 6:	/* mem_s16 : indir_i16( address_16 ) */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 7:	/* mem_u16 : indir_u16( address_16 ) */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 8:	/* mem : indir_i32( address_32 ) */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 9:	/* imm : iconst */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 10:	/* imm : iconst_0 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 11:	/* imm : iconst_1 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 12:	/* imm : iconst_2 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 13:	/* imm : iconst_4 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 14:	/* imm : iconst_8 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 15:	/* iconst_2_4_8 : iconst_2 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 16:	/* iconst_2_4_8 : iconst_4 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 17:	/* iconst_2_4_8 : iconst_8 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 18:	/* mc : mem */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 19:	/* mc : imm */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 20:	/* mr : mem */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 21:	/* mr : r32 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 22:	/* rc : r32 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 23:	/* rc : imm */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 24:	/* mrc : rc */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 25:	/* mrc : mem */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 26:	/* r32 : mem_s8 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 27:	/* r32 : mem_u8 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 28:	/* r32 : mem_s16 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 29:	/* r32 : mem_u16 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 30:	/* r32 : imm */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 31:	/* r32 : iconst_0 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 32:	/* r32 : mem */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 33:	/* rc : assign( address, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 34:	/* stmt : rc */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 35:	/* r32 : assign( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 36:	/* r32 : add( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 37:	/* mr : add( mr, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 38:	/* r32 : add( r32, iconst_1 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 39:	/* r32 : add( mul, rc ) */
		kids[0] = irTree->GetOp1()->GetOp1();
		kids[1] = irTree->GetOp1()->GetOp2();
		kids[2] = irTree->GetOp2();
		kids[3] = NULL;
		*numKids = 3;
		break;

	case 40:	/* r32 : add( add, imm ) */
		kids[0] = irTree->GetOp1()->GetOp1()->GetOp1();
		kids[1] = irTree->GetOp1()->GetOp1()->GetOp2();
		kids[2] = irTree->GetOp1()->GetOp2();
		kids[3] = irTree->GetOp2();
		kids[4] = NULL;
		*numKids = 4;
		break;

	case 41:	/* r32 : add( add, r32 ) */
		kids[0] = irTree->GetOp1()->GetOp1();
		kids[1] = irTree->GetOp1()->GetOp2();
		kids[2] = irTree->GetOp2();
		kids[3] = NULL;
		*numKids = 3;
		break;

	case 42:	/* r32 : add( add, imm ) */
		kids[0] = irTree->GetOp1()->GetOp1();
		kids[1] = irTree->GetOp1()->GetOp2();
		kids[2] = irTree->GetOp2();
		kids[3] = NULL;
		*numKids = 3;
		break;

	case 43:	/* r32 : sub( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 44:	/* mr : sub( mr, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 45:	/* r32 : sub( r32, iconst_1 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 46:	/* r32 : add( mul, sub ) */
		kids[0] = irTree->GetOp1()->GetOp1();
		kids[1] = irTree->GetOp1()->GetOp2();
		kids[2] = irTree->GetOp2()->GetOp1();
		kids[3] = irTree->GetOp2()->GetOp2();
		kids[4] = NULL;
		*numKids = 4;
		break;

	case 47:	/* r32 : add( mul, add ) */
		kids[0] = irTree->GetOp1()->GetOp1();
		kids[1] = irTree->GetOp1()->GetOp2();
		kids[2] = irTree->GetOp2()->GetOp1();
		kids[3] = irTree->GetOp2()->GetOp2();
		kids[4] = NULL;
		*numKids = 4;
		break;

	case 48:	/* r32 : mul( r32, imm ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 49:	/* r32 : mul( r32, imm ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 50:	/* r32 : mul( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 51:	/* r32 : mul( mem, imm ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 52:	/* r32 : div( r32, mr ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 53:	/* r32 : udiv( r32, mr ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 54:	/* r32 : rem( r32, mr ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 55:	/* r32 : shl( r32, imm ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 56:	/* r32 : shl( r32, r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 57:	/* r32 : shr( r32, imm ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 58:	/* r32 : shr( r32, r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 59:	/* r32 : ushr( r32, imm ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 60:	/* r32 : ushr( r32, r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 61:	/* r32 : mul( r32, iconst_2 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 62:	/* r32 : mul( r32, iconst_4 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 63:	/* r32 : mul( r32, iconst_8 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 64:	/* r32 : div( r32, iconst_2 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 65:	/* r32 : div( r32, iconst_4 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 66:	/* r32 : div( r32, iconst_8 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 67:	/* r32 : udiv( r32, iconst_2 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 68:	/* r32 : udiv( r32, iconst_4 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 69:	/* r32 : udiv( r32, iconst_8 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 70:	/* r32 : neg( r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 71:	/* r32 : and( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 72:	/* r32 : or( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 73:	/* r32 : xor( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 74:	/* r32 : not( r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 75:	/* r32 : compl( r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 76:	/* r32 : i2b( mr ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 77:	/* r32 : i2b( r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 78:	/* r32 : gt( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 79:	/* r32 : gt( mem, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 80:	/* r32 : eq( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 81:	/* r32 : eq( mem, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 82:	/* r32 : neq( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 83:	/* r32 : neq( mem, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 84:	/* r32 : lt( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 85:	/* r32 : lt( mem, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 86:	/* r32 : le( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 87:	/* r32 : le( mem, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 88:	/* r32 : ge( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 89:	/* r32 : ge( mem, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 90:	/* memf32 : indir_f32( address_f32 ) */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 91:	/* memf64 : indir_f64( address_f64 ) */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 92:	/* addr_f32 : address_f32 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 93:	/* addr_f64 : address_f64 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 94:	/* float_address : addr_f32 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 95:	/* float_address : addr_f64 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 96:	/* float_const : fconst */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 97:	/* float_value : float_const */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 98:	/* float_value : memf32 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 99:	/* float_value : memf64 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 100:	/* fpu_reg : float_value */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 101:	/* fpu_reg : float_address */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 102:	/* r32 : f2i( fpu_reg ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 103:	/* fpu_reg : i2f( mem ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 104:	/* fpu_reg : i2f( mr ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 105:	/* float_address : assign( float_address, fpu_reg ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 106:	/* stmt : float_address */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 107:	/* fpu_reg : add( fpu_reg, float_value ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 108:	/* fpu_reg : add( fpu_reg, fpu_reg ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 109:	/* fpu_reg : sub( fpu_reg, float_value ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 110:	/* fpu_reg : sub( fpu_reg, fpu_reg ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 111:	/* fpu_reg : mul( fpu_reg, float_value ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 112:	/* fpu_reg : mul( fpu_reg, fpu_reg ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 113:	/* fpu_reg : fdiv( fpu_reg, float_value ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 114:	/* fpu_reg : fdiv( fpu_reg, fpu_reg ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 115:	/* fpu_reg : fneg( fpu_reg ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 116:	/* r32 : f2b( eq ) */
		kids[0] = irTree->GetOp1()->GetOp1();
		kids[1] = irTree->GetOp1()->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 117:	/* r32 : f2i( eq ) */
		kids[0] = irTree->GetOp1()->GetOp1();
		kids[1] = irTree->GetOp1()->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 118:	/* addrj : address_32 */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 119:	/* addrj : r32 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 120:	/* addrj : mem */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 121:	/* addrj : label */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 122:	/* stmt : jmp( addrj ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 123:	/* stmt : label */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 124:	/* boolean_value : eq( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 125:	/* boolean_value : eq( mr, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 126:	/* boolean_value : neq( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 127:	/* boolean_value : neq( mr, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 128:	/* boolean_value : lt( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 129:	/* boolean_value : lt( mr, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 130:	/* boolean_value : gt( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 131:	/* boolean_value : gt( mr, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 132:	/* boolean_value : le( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 133:	/* boolean_value : le( mr, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 134:	/* boolean_value : ge( r32, mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 135:	/* boolean_value : ge( mr, rc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 136:	/* boolean_value : r32 */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 137:	/* stmt : boolean_value */
		kids[0] = irTree;
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 138:	/* boolean_value : assign( address, imm ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 139:	/* boolean_value : assign( address, r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 140:	/* _label : label */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 141:	/* stmt : br_false( boolean_value, _label ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 142:	/* stmt : br_true( boolean_value, _label ) */
		kids[0] = irTree->GetOp1();
		kids[1] = irTree->GetOp2();
		kids[2] = NULL;
		*numKids = 2;
		break;

	case 143:	/* stmt : nop */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 144:	/* stmt : func_start */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 145:	/* stmt : func_end */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 146:	/* stmt : ret( nop ) */
		kids[0] = NULL;
		*numKids = 0;
		break;

	case 147:	/* stmt : ret( r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 148:	/* stmt : arg( mrc ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 149:	/* stmt : arg( _label ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 150:	/* r32 : call( addrj ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 151:	/* stmt : preinc( mem ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 152:	/* r32 : preinc( r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 153:	/* stmt : predec( mem ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 154:	/* r32 : predec( r32 ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 155:	/* stmt : postinc( mem ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	case 156:	/* stmt : postdec( mem ) */
		kids[0] = irTree->GetOp1();
		kids[1] = NULL;
		*numKids = 1;
		break;

	default: printf( "Invalid rule number: %d\n", rulenum );
	}
}

void Machine_x86::_ExecuteConstructor( IRCode * insn, int rulenum, Operand *& node, Operand ** pOperands ) {
	switch ( rulenum ) {
	case 0:  // no action
		break;

	case 1:	/* address : address_8, "byte ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 2:	/* address : address_16, "word ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 3:	/* address : address_32, "dword ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 4:	/* mem_s8 : indir_i8( address_8 ), "byte ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 5:	/* mem_u8 : indir_u8( address_8 ), "byte ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 6:	/* mem_s16 : indir_i16( address_16 ), "word ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 7:	/* mem_u16 : indir_u16( address_16 ), "word ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 8:	/* mem : indir_i32( address_32 ), "dword ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 9:	/* imm : iconst, "%0" */
		{ node = new ImmediateOperand( insn, rulenum ); }
		break;
	case 10:	/* imm : iconst_0, "%0" */
		{ node = new ImmediateOperand( insn, rulenum ); }
		break;
	case 11:	/* imm : iconst_1, "%0" */
		{ node = new ImmediateOperand( insn, rulenum ); }
		break;
	case 12:	/* imm : iconst_2, "%0" */
		{ node = new ImmediateOperand( insn, rulenum ); }
		break;
	case 13:	/* imm : iconst_4, "%0" */
		{ node = new ImmediateOperand( insn, rulenum ); }
		break;
	case 14:	/* imm : iconst_8, "%0" */
		{ node = new ImmediateOperand( insn, rulenum ); }
		break;
	case 15:	/* iconst_2_4_8 : iconst_2, "2" */
		{ node = new ImmediateOperand( insn, rulenum ); }
		break;
	case 16:	/* iconst_2_4_8 : iconst_4, "4" */
		{ node = new ImmediateOperand( insn, rulenum ); }
		break;
	case 17:	/* iconst_2_4_8 : iconst_8, "8" */
		{ node = new ImmediateOperand( insn, rulenum ); }
		break;
	case 76:	/* r32 : i2b( mr ), "", <7> */
		{ Register & reg = GetFreeGeneralReg32();
		String  s;	EmitOperandIntoString( pOperands[0], &s );
		Emit( "cmp%s%s, 0\n", m_spacing, s.c_str() );
		Emit( "setne%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		++m_numInstructions; // m_numInstructions will be incremented because it's an instruction
		node = new RegisterOperand( reg, rulenum );
	}
		break;
	case 77:	/* r32 : i2b( r32 ), "", <6> */
		{ Register & reg = GetFreeGeneralReg32();
		String  s;	EmitOperandIntoString( pOperands[0], &s );
		Emit( "test%s%s, %s\n", m_spacing, s.c_str(), s.c_str() );
		Emit( "setne%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		++m_numInstructions; // m_numInstructions will be incremented because it's an instruction
		node = new RegisterOperand( reg, rulenum );
	}
		break;
	case 78:	/* r32 : gt( r32, mrc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );

		String  temp;
		EmitOperandIntoString( pOperands[1], &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, pOperands[0]->Name(), temp.c_str() );

		Emit( "setg%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );

		m_numInstructions += 2; // m_numInstructions will be incremented because it's an instruction

		node = new RegisterOperand( reg, rulenum );
	}
		break;
	case 79:	/* r32 : gt( mem, rc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );

		String  s0;	EmitOperandIntoString( pOperands[0], &s0 );
		String  s1;	EmitOperandIntoString( pOperands[1], &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );

		Emit( "setg%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2; // m_numInstructions will be incremented because it's an instruction

		node = new RegisterOperand( reg, rulenum );
	}
		break;
	case 80:	/* r32 : eq( r32, mrc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  temp;		EmitOperandIntoString( pOperands[1], &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, pOperands[0]->Name(), temp.c_str() );
		Emit( "sete%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		node = new RegisterOperand( reg, rulenum ); }
		break;
	case 81:	/* r32 : eq( mem, rc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		
		String  s0;	EmitOperandIntoString( pOperands[0], &s0 );
		String  s1;	EmitOperandIntoString( pOperands[1], &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );

		Emit( "sete%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		node = new RegisterOperand( reg, rulenum ); }
		break;
	case 82:	/* r32 : neq( r32, mrc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  temp;		EmitOperandIntoString( pOperands[1], &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, pOperands[0]->Name(), temp.c_str() );
		Emit( "setne%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		node = new RegisterOperand( reg, rulenum ); }
		break;
	case 83:	/* r32 : neq( mem, rc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  s0;	EmitOperandIntoString( pOperands[0], &s0 );
		String  s1;	EmitOperandIntoString( pOperands[1], &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );
		Emit( "setne%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		node = new RegisterOperand( reg, rulenum ); }
		break;
	case 84:	/* r32 : lt( r32, mrc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  temp;		EmitOperandIntoString( pOperands[1], &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, pOperands[0]->Name(), temp.c_str() );
		Emit( "setl%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		node = new RegisterOperand( reg, rulenum ); }
		break;
	case 85:	/* r32 : lt( mem, rc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  s0;	EmitOperandIntoString( pOperands[0], &s0 );
		String  s1;	EmitOperandIntoString( pOperands[1], &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );
		Emit( "setl%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		node = new RegisterOperand( reg, rulenum ); }
		break;
	case 86:	/* r32 : le( r32, mrc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  temp;		EmitOperandIntoString( pOperands[1], &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, pOperands[0]->Name(), temp.c_str() );
		Emit( "setle%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		node = new RegisterOperand( reg, rulenum ); }
		break;
	case 87:	/* r32 : le( mem, rc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  s0;	EmitOperandIntoString( pOperands[0], &s0 );
		String  s1;	EmitOperandIntoString( pOperands[1], &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );
		Emit( "setle%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		node = new RegisterOperand( reg, rulenum ); }
		break;
	case 88:	/* r32 : ge( r32, mrc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  temp;		EmitOperandIntoString( pOperands[1], &temp );
		Emit( "cmp%s%s, %s\n", m_spacing, pOperands[0]->Name(), temp.c_str() );
		Emit( "setge%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		node = new RegisterOperand( reg, rulenum ); }
		break;
	case 89:	/* r32 : ge( mem, rc ), "", <7> */
		{
		Register & reg = GetFreeGeneralReg32();
		Emit( "xor%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		String  s0;	EmitOperandIntoString( pOperands[0], &s0 );
		String  s1;	EmitOperandIntoString( pOperands[1], &s1 );
		Emit( "cmp%s%s, %s\n", m_spacing, s0.c_str(), s1.c_str() );
		Emit( "setge%s%s", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		m_numInstructions += 2;
		node = new RegisterOperand( reg, rulenum ); }
		break;
	case 90:	/* memf32 : indir_f32( address_f32 ), "dword ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 91:	/* memf64 : indir_f64( address_f64 ), "qword ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 92:	/* addr_f32 : address_f32, "dword ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 93:	/* addr_f64 : address_f64, "qword ptr [%0]" */
		{ node = new MemoryOperand( insn, rulenum ); }
		break;
	case 96:	/* float_const : fconst, "dword ptr %0" */
		{ node = new ImmediateOperand( insn, rulenum ); }
		break;
	case 116:	/* r32 : f2b( eq ), "", <12> */
		{	
		Emit( "fxch%sst(1)\t;exchange st(0) with st(1)\n", m_spacing );
		Emit( "fucompp\t;compare st(0) with st(1), set flags in the FPU status word and pop the stack twice\n" );
		Emit( "fstsw%sax\t;store the value of FPU status word into ax, don't check for pending unmasked FPU exceptions\n", m_spacing );
		Emit( "test%sah, 44h\n", m_spacing );
		
		Register & reg = GetFreeGeneralReg32();
		Emit( "setnp%s%s\n", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		Emit( "and%s%s, 1", m_spacing, reg.Name() );
		
		m_numInstructions += 5;
		node = new RegisterOperand( reg, rulenum ); 
	}
		break;
	case 117:	/* r32 : f2i( eq ), "", <12> */
		{	
		Emit( "fxch%sst(1)\t;exchange st(0) with st(1)\n", m_spacing );
		Emit( "fucompp\t;compare st(0) with st(1), set flags in the FPU status word and pop the stack twice\n" );
		Emit( "fstsw%sax\t;store the value of FPU status word into ax, don't check for pending unmasked FPU exceptions\n", m_spacing );
		Emit( "test%sah, 44h\n", m_spacing );
		
		Register & reg = GetFreeGeneralReg32();
		Emit( "setnp%s%s\n", m_spacing, GetLowerByteRegister( reg.GetCode() )->Name() );
		Emit( "and%s%s, 1", m_spacing, reg.Name() );
		
		m_numInstructions += 5;
		node = new RegisterOperand( reg, rulenum ); 
	}
		break;
	}
}
void Machine_x86::_ExecuteAction( IRCode * insn, int rulenum, Operand *& node, Operand ** operands )
{
	switch ( rulenum ) {
	case 0:  // no action
		break;

	case 124:	/* boolean_value : eq( r32, mrc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 125:	/* boolean_value : eq( mr, rc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 126:	/* boolean_value : neq( r32, mrc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 127:	/* boolean_value : neq( mr, rc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 128:	/* boolean_value : lt( r32, mrc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 129:	/* boolean_value : lt( mr, rc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 130:	/* boolean_value : gt( r32, mrc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 131:	/* boolean_value : gt( mr, rc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 132:	/* boolean_value : le( r32, mrc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 133:	/* boolean_value : le( mr, rc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 134:	/* boolean_value : ge( r32, mrc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 135:	/* boolean_value : ge( mr, rc ), "cmp %0, %1", <5> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 136:	/* boolean_value : r32, "test %0, %0", <3> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); }
		break;

	case 138:	/* boolean_value : assign( address, imm ), "mov %0, %1", <4> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); 
	}
		break;

	case 139:	/* boolean_value : assign( address, r32 ), "mov %0, %1", <4> */
		{ m_last_cond_opcode = insn->GetOpCode(); m_is_signed = ! insn->IsUnsigned(); 
	}
		break;

	case 141:	/* stmt : br_false( boolean_value, _label ), "", <5> */
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
		Emit( "%s%s%s\n", op, m_spacing, operands[1]->Name() );
	}
		break;

	case 142:	/* stmt : br_true( boolean_value, _label ), "", <5> */
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
		Emit( "%s%s%s\n", op, m_spacing, operands[1]->Name() );
	}
		break;

	case 144:	/* stmt : func_start, "_%0 proc", <15> */
		{
		Emit( "\n" );
		//TODO:
		//Emit( "i = ebp - 32\t\t; size = 4\n" );
		// var   equ   stackOffset

	}
		break;

	case 145:	/* stmt : func_end, "_%0 endp", <15> */
		{//TODO: ret %s
	Emit( "pop%sedi\npop%sesi\npop%sebx\nmov%sesp, ebp\npop%sebp\nret%s0\n",
		m_spacing, m_spacing, m_spacing, m_spacing, m_spacing, m_spacing, m_spacing );
	}
		break;

	}
}
void Machine_x86::_ExecuteDestructor( IRCode * insn, int rulenum, Operand *& node ) {
	switch ( rulenum ) {
	case 0:  // no action
		break;

	case 138:	/* boolean_value : assign( address, imm ), "mov %0, %1", <4> */
		{
		Register & reg = GetFreeReg32();
		Emit( "mov%s%s, %s\n", m_spacing, reg.Name(), node->GetOperand(1)->Name() );
		Emit( "test%s%s, %s\n", m_spacing, reg.Name(), reg.Name() );
		node->DropChildren();
	}
		break;
	case 139:	/* boolean_value : assign( address, r32 ), "mov %0, %1", <4> */
		{
		Emit( "test%s%s, %s\n", m_spacing, node->GetOperand(1)->Name(), node->GetOperand(1)->Name() );
		node->DropChildren();
	}
		break;
	case 144:	/* stmt : func_start, "_%0 proc", <15> */
		{
		Emit( "push%sebp\nmov%sebp, esp\nsub%sesp, %d\npush%sebx\npush%sesi\npush%sedi\n",
			m_spacing, m_spacing, m_spacing, insn->GetIntegerValue(), m_spacing, m_spacing, m_spacing, m_spacing );
	}
		break;
	case 150:	/* r32 : call( addrj ), "call %0", <3> */
		{
		int  sizeOfParams = insn->GetIntegerValue();
		if ( sizeOfParams > 0 ) {
			Emit( "add%sesp, %d\n", m_spacing, sizeOfParams );
			++m_numInstructions;
		}
	}
		break;
	}
}
const InstructionData  g_instruction_data[ NUM_RULES + 1 ] = {
/* 0 */  { 0 },
/* 1 */  { "address_8_1", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 2 */  { "address_16_2", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 3 */  { "address_32_3", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 4 */  { "indir_i8_address_8_4", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 5 */  { "indir_u8_address_8_5", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 6 */  { "indir_i16_address_16_6", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 7 */  { "indir_u16_address_16_7", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 8 */  { "indir_i32_address_32_8", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 9 */  { "iconst_9", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 10 */  { "iconst_0_10", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 11 */  { "iconst_1_11", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 12 */  { "iconst_2_12", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 13 */  { "iconst_4_13", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 14 */  { "iconst_8_14", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 15 */  { "iconst_2_15", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 16 */  { "iconst_4_16", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 17 */  { "iconst_8_17", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 18 */  { "mem_18", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 19 */  { "imm_19", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 20 */  { "mem_20", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 21 */  { "r32_21", 0, { 0, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 22 */  { "r32_22", 0, { 0, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 23 */  { "imm_23", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 24 */  { "rc_24", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 25 */  { "mem_25", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 26 */  { "mem_s8_26", 17, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 27 */  { "mem_u8_27", 17, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 28 */  { "mem_s16_28", 17, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 29 */  { "mem_u16_29", 17, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 30 */  { "imm_30", 17, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 4, 0 }, 1 },
/* 31 */  { "iconst_0_31", 17, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 32 */  { "mem_32", 17, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 4, 0 }, 1 },
/* 33 */  { "assign_address_rc_33", 1, { 7, 1 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 4, 0 }, 2 },
/* 34 */  { "rc_34", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 35 */  { "assign_r32_mrc_35", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 4, 0 }, 2 },
/* 36 */  { "add_r32_mrc_36", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 2, 0 }, 2 },
/* 37 */  { "add_mr_rc_37", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 2, 0 }, 2 },
/* 38 */  { "add_r32_iconst_1_38", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 39 */  { "add_mul_rc_39", 17, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 3 },
/* 40 */  { "add_add_imm_40", 17, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 5, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 4 },
/* 41 */  { "add_add_r32_41", 17, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 5, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 3 },
/* 42 */  { "add_add_imm_42", 17, { 5, 0 }, { { 5, 0 }, { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 3 },
/* 43 */  { "sub_r32_mrc_43", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 2, 0 }, 2 },
/* 44 */  { "sub_mr_rc_44", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 2, 0 }, 2 },
/* 45 */  { "sub_r32_iconst_1_45", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 46 */  { "add_mul_sub_46", 17, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 5, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 4 },
/* 47 */  { "add_mul_add_47", 17, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 5, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 4 },
/* 48 */  { "mul_r32_imm_48", 17, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 2049 }, { 0, 0 }, 2 },
/* 49 */  { "mul_r32_imm_49", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 2049 }, { 0, 0 }, 3 },
/* 50 */  { "mul_r32_mrc_50", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 2049 }, { 2, 0 }, 2 },
/* 51 */  { "mul_mem_imm_51", 17, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 2049 }, { 0, 0 }, 2 },
/* 52 */  { "div_r32_mr_52", 1, { 6, 0 }, { { 6, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 256, 0 }, { 2, 0 }, 1 },
/* 53 */  { "udiv_r32_mr_53", 1, { 6, 0 }, { { 6, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 256, 0 }, { 2, 0 }, 1 },
/* 54 */  { "rem_r32_mr_54", 1, { 6, 8 }, { { 6, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 256, 0 }, { 2, 0 }, 1 },
/* 55 */  { "shl_r32_imm_55", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 56 */  { "shl_r32_r32_56", 1, { 7, 0 }, { { 5, 0 }, { 6, 4 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 2, 0 }, 1 },
/* 57 */  { "shr_r32_imm_57", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 2 },
/* 58 */  { "shr_r32_r32_58", 1, { 7, 0 }, { { 5, 0 }, { 6, 4 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 1 },
/* 59 */  { "ushr_r32_imm_59", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 2 },
/* 60 */  { "ushr_r32_r32_60", 1, { 7, 0 }, { { 5, 0 }, { 6, 4 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 1 },
/* 61 */  { "mul_r32_iconst_2_61", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 62 */  { "mul_r32_iconst_4_62", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 63 */  { "mul_r32_iconst_8_63", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 64 */  { "div_r32_iconst_2_64", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 65 */  { "div_r32_iconst_4_65", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 66 */  { "div_r32_iconst_8_66", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 67 */  { "udiv_r32_iconst_2_67", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 68 */  { "udiv_r32_iconst_4_68", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 69 */  { "udiv_r32_iconst_8_69", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 70 */  { "neg_r32_70", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 71 */  { "and_r32_mrc_71", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 196 }, { 2, 0 }, 2 },
/* 72 */  { "or_r32_mrc_72", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 196 }, { 2, 0 }, 2 },
/* 73 */  { "xor_r32_mrc_73", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 196 }, { 2, 0 }, 2 },
/* 74 */  { "not_r32_74", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 75 */  { "compl_r32_75", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 76 */  { "i2b_mr_76", 3, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 77 */  { "i2b_r32_77", 3, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 78 */  { "gt_r32_mrc_78", 3, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 79 */  { "gt_mem_rc_79", 3, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 80 */  { "eq_r32_mrc_80", 3, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 81 */  { "eq_mem_rc_81", 3, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 82 */  { "neq_r32_mrc_82", 3, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 83 */  { "neq_mem_rc_83", 3, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 84 */  { "lt_r32_mrc_84", 3, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 85 */  { "lt_mem_rc_85", 3, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 86 */  { "le_r32_mrc_86", 3, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 87 */  { "le_mem_rc_87", 3, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 88 */  { "ge_r32_mrc_88", 3, { 5, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 89 */  { "ge_mem_rc_89", 3, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 2, 0 }, 0 },
/* 90 */  { "indir_f32_address_f32_90", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 91 */  { "indir_f64_address_f64_91", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 92 */  { "address_f32_92", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 93 */  { "address_f64_93", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 94 */  { "addr_f32_94", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 95 */  { "addr_f64_95", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 96 */  { "fconst_96", 2, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 97 */  { "float_const_97", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 98 */  { "memf32_98", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 99 */  { "memf64_99", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 100 */  { "float_value_100", 1, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 101 */  { "float_address_101", 1, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 102 */  { "f2i_fpu_reg_102", 1, { 6, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 103 */  { "i2f_mem_103", 1, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 104 */  { "i2f_mr_104", 1, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 105 */  { "assign_float_address_fpu_reg_105", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 106 */  { "float_address_106", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 107 */  { "add_fpu_reg_float_value_107", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 108 */  { "add_fpu_reg_fpu_reg_108", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 109 */  { "sub_fpu_reg_float_value_109", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 110 */  { "sub_fpu_reg_fpu_reg_110", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 111 */  { "mul_fpu_reg_float_value_111", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 112 */  { "mul_fpu_reg_fpu_reg_112", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 113 */  { "fdiv_fpu_reg_float_value_113", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 114 */  { "fdiv_fpu_reg_fpu_reg_114", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 115 */  { "fneg_fpu_reg_115", 1, { 7, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 116 */  { "f2b_eq_116", 3, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 1, 0 }, { 0, 0 }, 0 },
/* 117 */  { "f2i_eq_117", 3, { 5, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 1, 0 }, { 0, 0 }, 0 },
/* 118 */  { "address_32_118", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 119 */  { "r32_119", 0, { 0, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 120 */  { "mem_120", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 121 */  { "label_121", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 122 */  { "jmp_addrj_122", 17, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 123 */  { "label_123", 17, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 124 */  { "eq_r32_mrc_124", 5, { 0, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 125 */  { "eq_mr_rc_125", 5, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 126 */  { "neq_r32_mrc_126", 5, { 0, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 127 */  { "neq_mr_rc_127", 5, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 128 */  { "lt_r32_mrc_128", 5, { 0, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 129 */  { "lt_mr_rc_129", 5, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 130 */  { "gt_r32_mrc_130", 5, { 0, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 131 */  { "gt_mr_rc_131", 5, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 132 */  { "le_r32_mrc_132", 5, { 0, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 133 */  { "le_mr_rc_133", 5, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 134 */  { "ge_r32_mrc_134", 5, { 0, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 135 */  { "ge_mr_rc_135", 5, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 1 }, { 0, 0 }, 2 },
/* 136 */  { "r32_136", 5, { 0, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 2 },
/* 137 */  { "boolean_value_137", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 138 */  { "assign_address_imm_138", 13, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 2 },
/* 139 */  { "assign_address_r32_139", 13, { 0, 0 }, { { 0, 0 }, { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 2 },
/* 140 */  { "label_140", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 141 */  { "br_false_boolean_value__label_141", 5, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 142 */  { "br_true_boolean_value__label_142", 5, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 143 */  { "nop_143", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 144 */  { "func_start_144", 13, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 145 */  { "func_end_145", 5, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 146 */  { "ret_nop_146", 0, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 147 */  { "ret_r32_147", 0, { 0, 0 }, { { 6, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 0 },
/* 148 */  { "arg_mrc_148", 1, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 149 */  { "arg__label_149", 1, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 150 */  { "call_addrj_150", 9, { 6, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 1, 0 }, { 0, 0 }, 1 },
/* 151 */  { "preinc_mem_151", 1, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 152 */  { "preinc_r32_152", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 153 */  { "predec_mem_153", 1, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 154 */  { "predec_r32_154", 1, { 7, 0 }, { { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 155 */  { "postinc_mem_155", 1, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
/* 156 */  { "postdec_mem_156", 1, { 0, 0 }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { 0, 0 }, { 0, 0 }, 1 },
};


#include "x86_cg_inc.h"

}//end namespace c4
