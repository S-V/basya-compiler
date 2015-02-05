const InstructionData & Machine_x86::GetInsnData( int ruleNumber ) const
{
	return g_instruction_data[ ruleNumber ];
}

bool Machine_x86::IsInstruction( int ruleNumber ) const
{
	return ( g_instruction_data[ ruleNumber ].flags & IS_INSTRUCTION );
}

namespace {

// these are used for sorting pointers to intermediate trees
//
struct kid_s {
	IRCode * pInsn;
	int		number;
};

int CompareKids( const void *p1, const void *p2 ) {
	if ( !p1 || !p2 ) {
		return 0;
	}
	IRCode * op1 = (( kid_s* )p1 )->pInsn;
	IRCode * op2 = (( kid_s* )p2 )->pInsn;

	if ( op2->complexity == op1->complexity ) {
		return 1;
	}
	return op2->complexity - op1->complexity;
}

}//end anonymous namespace

//----------------------------------------------------------------------------------------------------------------------------------------------------------
Operand * Machine_x86::CreateOperand( IRCode *insn, int ruleNumber, Operand ** pOperands, const OperandLocation *pLocation )
{
	Operand * newOperand = NULL;

	switch ( pLocation->GetType() ) {
	case eDefault :
		newOperand = new InsnOperand( insn, ruleNumber );
		break;

	case eReg_8 :
	case eReg_16 :
		Unimplemented();
		break;

	case eReg_32 :
		{
			Register & reg = GetFreeReg32();
			newOperand = new RegisterOperand( reg, ruleNumber );
		}
		break;

	case eRegister :
		{
			RegCode_t  regCode = ( RegCode_t )pLocation->GetData();
			Register & reg = GetFreeRegister( regCode );
			newOperand = new RegisterOperand( reg, ruleNumber );
		}
		break;

	case eResultInOperand :
		newOperand = new DebugOperand( ruleNumber, "resultInOp" );
		break;

	default:
		newOperand = new InsnOperand( insn, ruleNumber );
	}
	//------------------------------
	Assert( newOperand != NULL );
	return newOperand;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
Operand * Machine_x86::EmitNode( IRCode *insn, int nonterm, OperandLocation *resultLocation /* =0 */ )
{
	int  ruleNumber = insn->state->rule[ nonterm ];
	if ( ruleNumber == 0 ) {
		return NULL; //??? is this right ???
	}

	int  numKids = 0;
	IRCode *  kids[ MAX_NUM_OPERANDS ];	// temp vars to store pointers to the kids of insn
	_GetKids( insn, ruleNumber, kids, &numKids );

	const int * pNonterms = g_nonterms[ ruleNumber ];
	//Assert2( pNonterms, "Machine_x86::EmitNode() : pNonterms == NULL" );
	if ( ! pNonterms ) {
		c4::common->Print( "EmitNode() : pNonterms == NULL for rule %d ('%s', '%s')!\n", ruleNumber, g_rules[ ruleNumber ], g_templates[ ruleNumber ] );
		return 0;
	}

	Operand *	operands[ MAX_NUM_OPERANDS ];
	operands[ numKids ] = NULL;

//TODO: use information about this instruction ( clobbered registers, changed flags, etc. )
// to improve the quality of generated code
	const InstructionData & insnData = GetInsnData( ruleNumber );

	//SetFutureSideEffects( insnData.side_effects );

	//
	// first emit the children nodes
	//
#pragma region Emitting Kids
	if ( insnData.attributes.flags & DO_NOT_EMIT_CHILDREN ) 
	{
		// don't emit children

		// sanity check
		Assert( (insnData.attributes.flags & EMIT_COMPLEX_CHILDREN_FIRST) == 0 );
	}
	else
	{
		// emit the most complex subtrees in the first place
		//
/*		if ( insnData.attributes.flags & EMIT_COMPLEX_CHILDREN_FIRST )
*/		{
			// create a temp. array of pointers and sort it
			//
			kid_s	tempKids[ MAX_NUM_OPERANDS ];
			for ( int i = 0; i < numKids; i++ )
			{
				tempKids[i].pInsn = kids[i];
				tempKids[i].number = i;
			}

			qsort( tempKids, numKids, sizeof kid_s, CompareKids );
			
			for ( int i = 0; i < numKids; i++ ) 
			{
				kid_s & kid = tempKids[i];

				OperandLocation   loc( insnData.operands[ kid.number ] );
				
				operands[ kid.number ] = EmitNode( kid.pInsn, pNonterms[ kid.number ], &loc );
			}
		}
/*		else // emit children in the usual fashion, sequentially
		{
			for ( int i = 0; pNonterms[i] != NULL; i++ ) 
			{
				OperandLocation   loc( insnData.operands[i] );
				operands[i] = EmitNode( kids[i], pNonterms[i], &loc );
			}
		}
*/
	}
#pragma endregion

	//
	// create a new operand
	//

	// points to where a new operand must be located
	OperandLocation * pLocation = NULL;

	OperandLocation  loc( insnData.result );
	if ( resultLocation == NULL ) {
		resultLocation = & loc;
	}

	pLocation = SelectBestLocationForNewOperand( & loc, resultLocation );

	
	Operand * newOperand = NULL;


	if ( IsInstruction( ruleNumber ) )
	{
		// some child nodes may have been moved into some registers
		// and placed in wrong locations
		EnsureOperandsAreInRightPlaces( insnData, operands );
		
		newOperand = EmitInstruction( insn, ruleNumber, operands, numKids, pLocation );
	}
	else // if it's not an instruction
	{
		// if there's a constructor for this rule then call	it
		if ( insnData.flags & HAS_CONSTRUCTOR )
		{
			_ExecuteConstructor( insn, ruleNumber, newOperand, operands );
		}
		else // else invoke the default constructor
		{
			newOperand = CreateOperand( insn, ruleNumber, operands, pLocation );
			memcpy( newOperand->GetPtr(), operands, sizeof operands );
		}

		// execute the semantic action associated with this rule
		_ExecuteAction( insn, ruleNumber, newOperand, operands );

		if ( insnData.flags & HAS_DESTRUCTOR ) {
			_ExecuteDestructor( insn, ruleNumber, newOperand );
		}
	}

	//ResetFutureSideEffects();

	// ensure that the new operand is in the location pointed by parent
	//if ( newOperand->GetType() != eDebug ) {
		EnsureOperandLocation( newOperand, *resultLocation );
	//}

	return newOperand;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
Operand * Machine_x86::EmitInstruction( IRCode *insn, int ruleNumber, Operand **operands, int numOperands, const OperandLocation *pLocation )
{
	Operand * newOperand = NULL;

	const InstructionData & insnData = GetInsnData( ruleNumber );

	PerformSideEffects( insnData.side_effects );

	if ( insnData.attributes.flags & OPERANDS_MUST_HAVE_EQUAL_SIZE )
	{
		//c4::common->DebugPrint( "\t; operands must have equal size!\n" );
		b_operands_must_be_of_given_size = true;
		m_operand_size = operands[ 0 ]->GetSize();
	}
	else {
		b_operands_must_be_of_given_size = false;
	}

	if ( insnData.result.type == eRegister )
	{
		if ( insnData.flags & HAS_CONSTRUCTOR )
		{
			_ExecuteConstructor( insn, ruleNumber, newOperand, operands );
		}
		
		if ( newOperand == NULL )
		{
			newOperand = new DebugOperand( ruleNumber, 
				GetRegisterByCode( ( RegCode_t )insnData.result.data ).Name() );
		}

		memcpy( newOperand->GetPtr(), operands, sizeof( Operand* ) * MAX_NUM_OPERANDS );

		// execute the semantic action associated with this rule
		_ExecuteAction( insn, ruleNumber, newOperand, operands );

		ClearInstructionBuffer();
		EmitAssembler( newOperand );
		EmitInstructionInBuffer();

#ifdef DEBUG_I386
		printf( "\t; %s, (%d)", g_rules[ ruleNumber ], ruleNumber );
#endif
		Emit( "\n" );	// emit new line after each instruction

		UndoSideEffects( insnData.side_effects );

		//---------------------------------
		delete newOperand;

		//---------------------------------
		RegCode_t  regCode = ( RegCode_t )insnData.result.data;
//TODO: use 'xchg' when appropriate
		newOperand = EmitXchg( regCode, operands, insnData.numChildNodes );
		if ( newOperand == NULL )
		{
			Register & reg = GetFreeRegister( regCode );
			newOperand = new RegisterOperand( reg, ruleNumber );
			newOperand->SetEmitted();
		}

		if ( insnData.flags & HAS_DESTRUCTOR ) {
			_ExecuteDestructor( insn, ruleNumber, newOperand );
		}
	}
	else
	{
		if ( insnData.flags & HAS_CONSTRUCTOR )
		{
			_ExecuteConstructor( insn, ruleNumber, newOperand, operands );
		}

		if ( newOperand == NULL )
		{
			switch ( pLocation->GetType() )
			{
			case eDefault :
				newOperand = new InsnOperand( insn, ruleNumber );
				break;

			case eReg_8 :
			case eReg_16 :
				Unimplemented();
				break;

			case eReg_32 :
				{
					Register & reg = GetFreeReg32();
					newOperand = new RegisterOperand( reg, ruleNumber );
				}
				break;

			case eRegister :
				{
					RegCode_t  regCode = ( RegCode_t )pLocation->GetData();
					Register & reg = GetFreeRegister( regCode );
					newOperand = new RegisterOperand( reg, ruleNumber );
				}
				break;

			case eResultInOperand :
				newOperand = new DebugOperand( ruleNumber, "resultInOp" );
				break;

			default: Unreachable();
			}

		}//if this node hasn't a ctor

		memcpy( newOperand->GetPtr(), operands, sizeof( Operand* ) * MAX_NUM_OPERANDS );		

		// execute the semantic action associated with this rule
		_ExecuteAction( insn, ruleNumber, newOperand, operands );

		ClearInstructionBuffer();
		EmitAssembler( newOperand );
		EmitInstructionInBuffer();

		UndoSideEffects( insnData.side_effects );

#ifdef DEBUG_I386
		printf( "\t; %s, (%d)", g_rules[ ruleNumber ], ruleNumber );
#endif
		Emit( "\n" );	// emit new line after each instruction

		//--------------------------------------------------
		if ( insnData.flags & HAS_DESTRUCTOR ) {
			_ExecuteDestructor( insn, ruleNumber, newOperand );
		}
		//else
		//{
			if ( pLocation->GetType() == eResultInOperand )
			{
				int num = pLocation->GetData();
				Operand * pResultOp = operands[ num ];
				pResultOp->Grab();
				newOperand->Drop();
				newOperand = pResultOp;
			}
			else {
				newOperand->DropChildren();
			}
		//}//if no destructor
	}
	++m_numInstructions;

	Assert( newOperand );	
	return newOperand;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
OperandLocation * Machine_x86::SelectBestLocationForNewOperand( OperandLocation *pLoc1, OperandLocation *pLoc2 )
{
	OperandLocation * pBestLoc = NULL;

	const EOperandType	type1 = pLoc1->GetType();
	const int			data1 = pLoc1->GetData();

	const EOperandType	type2 = pLoc2->GetType();
	const int			data2 = pLoc2->GetData();

	// if both locations are equal
	if ( type1 == type2 && data1 == data2 ) {
		pBestLoc = pLoc1;
	}
	else if ( type1 == eDefault && type2 != eDefault ) {
		pBestLoc = pLoc2;
	}
	else if ( type2 == eDefault && type1 != eDefault ) {
		pBestLoc = pLoc1;
	}

	else if ( type1 == eRegister 
		&& (type2 == eReg_8 || type2 == eReg_16 || type2 == eReg_32) )
	{
		//Assert( data1 is reg8 or reg32 etc.
		pBestLoc = pLoc1;
	}
	else if ( type2 == eRegister
		&& (type1 == eReg_8 || type1 == eReg_16 || type1 == eReg_32) )
	{
		pBestLoc = pLoc2;
	}


	// ??? is this right ???
	//else if ( type1 == eRegister && type2 == eResultInOperand ) {
	//	pBestLoc = pLoc1;
	//}
	//else if ( type1 == eResultInOperand && type2 == eRegister ) {
	//	pBestLoc = pLoc2;
	//}


	else {
		pBestLoc = pLoc1;
	}

	return pBestLoc;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void Machine_x86::EnsureOperandLocation( Operand *& operand, const OperandLocation & location )
{
	const EOperandType	type = location.GetType();

	if ( type == eDefault ) {
		return;
	}

	const int  data = location.GetData();

	// if the operand must be in the specified register
	if ( type == eRegister )
	{
		Register & reg = GetRegisterByCode( ( RegCode_t )data );
		MoveOperandIntoRegister( reg, operand );
	}
	// if the operand must be in a general-purpose 32 bit register
	else if ( type == eReg_32 )
	{
		if ( operand->GetType() != eDebug ) {
			Assert( operand->GetType() == eRegister );
		}

		RegisterOperand * pRegOp = static_cast< RegisterOperand* >( operand );
		Register * pReg = pRegOp->GetRegister();
		Assert( IsReg32( pReg->GetCode() ) );
	}
	else
	{
		//TODO: other cases
		Assert( false );
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
Operand * Machine_x86::EmitXchg( RegCode_t  regForResult, Operand ** operands, int numOperands ) {
	Operand * newOperand = NULL;

/*	// 'xchg'
	if ( numOperands == 2 )
	{
		const EOperandType	type1 = operands[ 0 ]->GetType();
		const EOperandType	type2 = operands[ 1 ]->GetType();

		//if ( type1 )
	}
*/
	return newOperand;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
// ??? Should we pass 'numChildren' as a parameter?
void Machine_x86::EnsureOperandsAreInRightPlaces( const InstructionData & insnData, Operand ** pOperands )
{
	// TODO: use the 'xchg' instruction

	const int  numChildren = insnData.numChildNodes;
/** /
	
	if ( numChildren == 2 )
	{
		const EOperandType	type1 = ( EOperandType )insnData.operands[ 0 ].type;
		const EOperandType	type2 = ( EOperandType ) insnData.operands[ 1 ].type;

		if ( type1 == eRegister && type2 == eReg_32 )
		{
			const RegCode_t	 reg1 = ( RegCode_t ) insnData.operands[ 0 ].data;
			if ( IsReg32( reg1 ) )
			{
				const RegCode_t  reg2 = 
					static_cast< RegisterOperand* >( pOperands[ 1 ] )->GetRegister()->GetCode();

				// if the second operand is located in the register for the first operand,
				// then swap them
				if ( reg2 == reg1 )
				{
					Emit( "xchg%s%s, %s\n", m_spacing, 
						GetRegisterByCode( reg1 ).Name(), GetRegisterByCode( reg2 ).Name() );
				}

				//const int	data = insnData.operands[ i ].data;
			}
		}
	}
/**/

	//----------------------------------------------------------
	// TODO: choose whether to process the operands sequentially or in a specific order ( based on some heuristic )
	for ( int i = 0; i < numChildren; i++ )
	{
		const EOperandType	type = ( EOperandType )insnData.operands[ i ].type;

		if ( type == eDefault ) {
			continue;
		}

		const int	data = insnData.operands[ i ].data;

		if ( type == eRegister )
		{
			Register & reg = GetRegisterByCode( ( RegCode_t )data );

			//TODO: make a function "EnsureOperandIsInRegister"
			// which will be recursive
			// ( pOperands[i] may be "InsnOperand" and may have a child "RegisterOperand"
			MoveOperandIntoRegister( reg, pOperands[i] );
		}
	//TODO: other cases
	//	else
	//	{
	//	}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void Machine_x86::EmitAssembler( Operand * operand ) {
	int ruleNumber = operand->GetRuleNumber();

	if ( IsInstruction( ruleNumber )
		&& operand->IsEmitted() )
	{
		EmitOperandNameIntoBuffer( operand );
		return;
	}

	const char * format = g_templates[ ruleNumber ];
	if ( ! format ) { // nothing to write
		return;
	}
	
	bool  bSpaceInserted = false;

	const char * p = format;
	while ( *p )
	{
		if ( ! bSpaceInserted 
			&& IsInstruction( ruleNumber )
			&& *p == ' ' )
		{
			EmitIntoBuffer( m_spacing );
			bSpaceInserted = true;
			++p;
			continue;
		}

		if( *p == '%' ) {
			++p;  // skip '%'
			if ( *p >= '0' && *p <= '9' ) {
				int  num = *p - '0';
				if ( operand->IsLeaf() ) {
					BREAK_IF( num != 0 );
					EmitOperandNameIntoBuffer( operand );
				} else {
					EmitAssembler( operand->GetOperand( num ) );
				}
				++p;
				continue;
			}
			else if ( *p == 'c' ) {
				++p; // skip 'c'
				EmitIntoBuffer( operand->Name() );
				continue;
			}
			else {
				c4::common->Error( "Machine_x86::EmitAssembler() : invalid character '%c' after '%%'!", *p );
			}
			break;
		}//if( *p == '%' )

		if ( *p == '\n' ) {
			bSpaceInserted = false;
			++m_numInstructions;
		}

		EmitIntoBuffer( "%c", *p );
		++p;
	}//end while ( *p != NULL )

	operand->SetEmitted();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void Machine_x86::_DumpCover( IRCode * node, int nonterm, int indent /* =0 */ )
{
	if ( ! node ) {
		//c4::common->Error( "Machine_x86::_DumpCover() : node == NULL!\n" );
		return;
	}
	if ( ! node->state ) {
		c4::common->Error( "Machine_x86::_DumpCover() : node '%s' isn't labeled!\n", node->Name() );
		return;
	}

	int  ruleNumber = node->state->rule[ nonterm ];
	if ( ruleNumber == 0 ) {
		Emit( "\nPROGRAM ABORTED...\n" );
		c4::common->Error( "\tCouldn't tile the tree with the given patterns!\n" );
		return;
	}

	int i;
	for ( i = 0; i < indent; i++ ) {
		Emit( " " );
	}
	printf( "%s / '%s'\n", g_rules[ ruleNumber ], g_templates[ ruleNumber ] );

	int  numKids;
	IRCode *  kids[ 10 ];	// temp vars to store pointers to the kids of node
	memset( kids, 0, sizeof kids );

	_GetKids( node, ruleNumber, kids, &numKids );

	const int * pNonterms = g_nonterms[ ruleNumber ];
	if ( ! pNonterms ) {
		c4::common->Error( "Machine_x86::_DumpCover() : pNonterms == NULL for rule %d ('%s', '%s')!\n", ruleNumber, g_rules[ ruleNumber ], g_templates[ ruleNumber ] );
		return;
	}
	for ( i = 0; pNonterms[i] != NULL; i++ ) {
		_DumpCover( kids[i], pNonterms[i], indent + 1 );
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
int Machine_x86::_DumpTemplates( IRCode *insn, int nonterm )
{
	if ( ! insn ) {
		c4::common->DebugPrint( "Machine_x86::_DumpTemplates() : insn == NULL!\n" );
		return 0;
	}
	if ( ! insn->state ) {
		c4::common->DebugPrint( "Machine_x86::_DumpTemplates() : insn '%s' isn't labeled!\n", insn->Name() );
		return 0;
	}

	int  ruleNumber = insn->state->rule[ nonterm ];
	if ( ruleNumber == 0 ) {
		c4::common->Error("Machine_x86::_DumpTemplates() : ruleNumber == 0!\n" );
		return 0;
	}

	int  numKids;
	IRCode *  kids[ MAX_NUM_OPERANDS ];	// temp vars to store pointers to the kids of insn
	_GetKids( insn, ruleNumber, kids, &numKids );

	const int * pNonterms = g_nonterms[ ruleNumber ];
	if ( ! pNonterms ) {
		c4::common->DebugPrint( "Machine_x86::_DumpTemplates() : pNonterms == NULL for rule %d ('%s', '%s')!\n", ruleNumber, g_rules[ ruleNumber ], g_templates[ ruleNumber ] );
		return 0;
	}

	for ( int i = 0; pNonterms[i] != NULL; i++ ) {
		_DumpTemplates( kids[i], pNonterms[i] );
	}

	const InstructionData & insnData = g_instruction_data[ ruleNumber ];

	if ( insnData.flags & IS_INSTRUCTION ) {
		printf( "%s\t; %s (%d)\n", g_templates[ ruleNumber ], g_rules[ ruleNumber ], ruleNumber );
	}

	return ruleNumber;
}

bool Machine_x86::CheckCover( IRCode * irTree, int nonterm /* =START_NONTERMINAL */ )
{
	if ( (irTree == NULL) || (irTree->state == NULL) ) {
		return false;
	}
	int  ruleNumber = irTree->state->rule[ nonterm ];
	if ( ruleNumber == 0 ) {
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------------------
void Machine_x86::EmitOperandIntoString( Operand *operand, String *pStr ) {
	int ruleNumber = operand->GetRuleNumber();

	if ( IsInstruction( ruleNumber )
		&& operand->IsEmitted() )
	{
		*pStr += operand->Name();
		return;
	}

	Assert( ! IsInstruction( ruleNumber ) );

	const char * format = g_templates[ ruleNumber ];
	if ( ! format ) { // nothing to write
		return;
	}
	
	bool  bSpaceInserted = false;

	const char * p = format;
	while ( *p )
	{
		if ( ! bSpaceInserted 
			&& IsInstruction( ruleNumber )
			&& *p == ' ' )
		{
			*pStr += m_spacing;
			bSpaceInserted = true;
			++p;
			continue;
		}

		if( *p == '%' ) {
			++p;  // skip '%'
			if ( *p >= '0' && *p <= '9' ) {
				int  num = *p - '0';
				if ( operand->IsLeaf() ) {
					BREAK_IF( num != 0 );
					*pStr += operand->Name();
				} else {
					EmitOperandIntoString( operand->GetOperand( num ), pStr );
				}
				++p;
				continue;
			}
			else if ( *p == 'c' ) {
				++p; // skip 'c'
				*pStr += operand->Name();
				continue;
			}
			else {
				c4::common->Error( "Machine_x86::EmitAssembler() : invalid character '%c' after '%%'!", *p );
			}
			break;
		}//if( *p == '%' )

		if ( *p == '\n' ) {
			bSpaceInserted = false;
			++m_numInstructions;
		}

		*pStr += *p;
		++p;
	}//end while ( *p != NULL )

	operand->SetEmitted();
}