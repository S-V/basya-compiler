/*
=============================================
	Intermediate representation
=============================================
*/
#ifndef __IR_H__
#define __IR_H__

namespace c4 {

enum INTERMEDIATE_CODES
{
	nop,	// denotes "don't generate this instruction"

	// Primitive ( leaf ) codes
	iconst,			// integer constant
	iconst_0,
	iconst_1,
	iconst_2,
	iconst_4,
	iconst_8,

	fconst,

	// & mem

	//TODO:  remove these and leave only 'address'
	address_8,
	address_16,
	address_32,
	address_64,

	address_f32,
	address_f64,

	// indirection operator, pointer dereferencing
	indir_i8,
	indir_u8,	// dereferencing an unsigned byte
	indir_i16,
	indir_u16,
	indir_i32,
	indir_i64,
	indir_f32,
	indir_f64,

	index,			// array[ index ]

	assign,		// lvalue = rvalue

	// Arithmetic instructions
	//

	// both arguments must have identical types!
	add,
	sub,
	mul,
	
	div,	// signed integer division
	udiv,	// unsigned integer division
	fdiv,	// floating point division

	rem,	// signed integer remainder, %
	urem,	// unsigned integer remainder from the unsigned division of two integer arguments
	
	neg,	// negation, unary -
	fneg,

	//TODO: remove these opcodes and transform E++ into E = E+1 and so on
	preinc,
	postinc,
	predec,
	postdec,

	and,	// &
	or,		// |
	xor,	// ^
	not,	// !
	compl,	// ~

	shl,
	shr,
	ushr,	// >>>

	// Logical operators
	//
	eq,		// ==
	neq,	// !=
	lt,		// <
	gt,		// >
	le,		// <=
	ge,		// >=
	
	// Primitive type conversions
	//
	f2i,	// float to int
	i2f,	// int to float

	//b2i,	// bool to int
	//b2f,	// bool to float
	i2b,	// int to bool
	f2b,	// float to bool

/*	b2i,	// byte to integer
	b2l,
	b2f,
	b2d,

	i2b,
	i2l,
	i2f,
	i2d,

	l2b,
	l2i,
	l2f,
	l2d,

	f2b,
	f2i,
	f2l,
	f2d,

	d2b,
	d2i,
	d2l,
	d2f,
*/

	// Control flow instructions
	//
	jmp,	// unconditionally transfers control to a target instruction
	br_true,
	br_false,

	je,		// transfers control to a target instruction if two values are equal
	jne,
	jge,	// transfers control to a target instruction if the first value is greater or equal to the second value
	jg,		// transfers control to a target instruction if the first value is greater than the second value
	jle,	// transfers control to a target instruction if the first value is less than or equal to the second value
	jl,		// transfers control to a target instruction if the first value is less than the second value

	label,

	loop_start,	// start of a loop
	loop_end,

	func_start,	// start of a function
	func_end,

	// Procedures
	//	
	call,
	arg,	// function argument
	ret,	// return from a function

	data,

	OPCODE_ERROR,	// invalid instruction

	NUM_OPCODES
};

typedef INTERMEDIATE_CODES	ICode_t;

//---------------------------------------------

// generated from machine description
struct state_s;

/*
======================================
	Intermediate representation code
======================================
*/
class IRCode 
{
	friend class IRGenerator;

	// TODO: refactor these:
	ICode_t			opcode;

	// these are used for IR tree construction
	IRCode *		op1;
	IRCode *		op2;

	//std::string			text;
	String		text;	// text data for this instruction ( a name of a variable, function, ... )
	String		m_data;

public:
					explicit IRCode( ICode_t  opcode );
					IRCode( const IRCode & other );
					explicit IRCode( bool b );
					explicit IRCode( int i );
					explicit IRCode( float f );
					//explicit IRCode( const String & s );
					IRCode( ICode_t  opcode, IRCode* op1, IRCode* op2 );
					~IRCode() {}

					// if this is a variable, then return its name;
					// if this is a constant, then return a corresponding string;
					// if this is an operation, then return the name of my opcode
	const char *	Name() const;

	ICode_t			GetOpCode() const { return opcode; }
	IRCode *		GetOp1() { return op1; }
	IRCode *		GetOp2() { return op2; }

					// returns the size of this thing, in bytes
	int				GetSize() const { return m_iSize; }

	bool			IsLeaf() const;
	int				GetNumOperands() const;	// returns 0 or 1 or 2

	void			SetText( const char *str )	{ text = str; }
	const char *	GetText() const				{ return text.c_str(); }

	const char *	GetData() const { return m_data.c_str(); }
	void	SetIntegerValue( int value )	{ m_int_value = value; }
	int		GetIntegerValue() const			{ return m_int_value; }

	void	SetFloatValue( double value )	{ m_double_value = value; }
	double	GetFloatValue() const			{ return m_double_value; }

	IRCode *	GetLabel() { return m_pLabel; }

	bool		IsCommutative() const { return IsCommutative( opcode ); }

	int32	GetClobberedRegs() const { Assert( opcode == ICode_t::call ); return m_clobberedRegs; }

	bool		IsUnsigned() const { return m_bUnsigned; }
public:
	String			comment;	// for placing comments in assembly listings; TODO: remove this in the future

private:
	union {
		int		m_int_value;	// for storing integer value, or stack offset if it's a variable access or memory
		double	m_double_value;
	};
	int			m_iSize;

	// valid only if this insn is 'call'
	int32		m_clobberedRegs;

	// points to the label before this node
	IRCode *		m_pLabel;

	bool		m_bUnsigned;
public:
	int		complexity;	// leafs have minimum complexity

public:
	// for pattern matching and labelling IR tree; 
	// this pointer is managed by Machine_x86
	state_s *	state;

private:
	void	Init();

private:
	struct ircode_data_s {
		const char *	name;
		int				arity;
		int				cost;
	};
	static const ircode_data_s	_irdata[ NUM_OPCODES ];

public:
	// returns the cost for the given opcode
	static int		GetCost( ICode_t  opcode );

	// returns the name for the given opcode
	static const char *	GetOpcodeName( ICode_t  opcode );

	// for debugging; writes out the given intermediate tree
	static void		DumpTree( IRCode * root );

	static bool		IsBranchInsn( ICode_t  opcode );
	static bool		IsCommutative( ICode_t  opcode );
	static int		GetOperatorArity( ICode_t  opcode );
};

typedef c4::LinkedList< IRCode* >	IRList_t;

/*
=============================================
	class IRGenerator
=============================================
*/
class IRGenerator : public ITreeVisitor {
public:
					IRGenerator();
					~IRGenerator();

	IRList_t &		Generate( CompilationUnit * program );
	IRList_t &		GetInsnList() { return m_insnList; }

					// delete generated intermediate instructions, free allocated resources
	void			Cleanup();

	// for debugging
	void	DumpInsnList() const;

#pragma region Visitor

public:
	virtual void	Visit( CompilationUnit *		compUnit 		);

	virtual void	Visit( Declaration *			decl 			);
	virtual void	Visit( DeclarationList *		declList 		);
	virtual void	Visit( VarDecl * 				varDecl 		);
	virtual void	Visit( Argument *				arg 			);
	virtual void	Visit( ArgumentList * 			argList 		);
	virtual void	Visit( FunctionDecl * 			functionDecl 	);
	virtual void	Visit( ClassDecl * 				classDecl 		);
	virtual void	Visit( StructDecl *				structDecl 		);
	virtual void	Visit( EnumDecl *				enumDecl 		);
	virtual void	Visit( NamespaceDecl * 			namespaceDecl 	);

	virtual void	Visit( Statement *				stmt 			);
	virtual void	Visit( EmptyStatement *			emptyStmt 		);
	virtual void	Visit( ErrorStatement *			errorStmt 		);
	virtual void	Visit( StatementList *			stmtList 		);
	virtual void	Visit( ExpressionStatement *	expStmt 		);
	virtual void	Visit( LabeledStatement *		labelStmt 		);
	virtual void	Visit( DeclStatement *			declStmt 		);
	virtual void	Visit( Block *					block 			);
	virtual void	Visit( AutoDeclStatement *		autoDeclStmt 	);
	virtual void	Visit( IfStatement *			ifStmt 			);
	virtual void	Visit( LoopStatement *			loopStmt 		);
	virtual void	Visit( WhileStatement *			whileStmt 		);
	virtual void	Visit( DoStatement *			doStmt 			);
	virtual void	Visit( WithStatement *			withStmt 		);
	virtual void	Visit( ReturnStatement *		returnStmt 		);
	virtual void	Visit( ForLoopStatement *		forStmt			);
	virtual void	Visit( CaseStatement *			caseStmt 		);
	virtual void	Visit( DefaultStatement *		defaultStmt 	);
	virtual void	Visit( ContinueStatement *		continueStmt 	);
	virtual void	Visit( BreakStatement *			breakStmt 		);
	
	virtual void	Visit( Expression *				expr 			);
	virtual void	Visit( ExpressionList *			exprList 		);
	virtual void	Visit( BooleanConstant * 		boolConst		);
	virtual void	Visit( IntegerConstant * 		intConst		);
	virtual void	Visit( FloatingConstant *		floatConst		);
	virtual void	Visit( CharConstant *			charConst 		);
	virtual void	Visit( StringLiteral *			strConst 		);	
	virtual void	Visit( IdExp *					idExp			);
	virtual void	Visit( VarExp *					varExp			);
	virtual void	Visit( Identifier * 			id				);
	virtual void	Visit( UnaryExpression *		unaryExp		);
	virtual void	Visit( BinaryExpression * 		binaryExp		);
	virtual void	Visit( AssertExpression * 		assertExp 		);
	virtual void	Visit( AddrOfExp *				addrOfExpr		);
	virtual void	Visit( ArrayAccessExp *			arrayExp		);
	virtual void	Visit( PtrIndirectionExp *		ptrExp			);
	virtual void	Visit( DotExp *					dotExp			);
	virtual void	Visit( CastExp *				castExp			);
	virtual void	Visit( NegationExp * 			negExp			);
	virtual void	Visit( UnaryPlusExp *			uplusExp		);
	virtual void	Visit( PreIncExp * 				preIncExp		);
	virtual void	Visit( PreDecExp * 				preDecExp		);
	virtual void	Visit( PostIncExp * 			postIncExp		);
	virtual void	Visit( PostDecExp * 			postDecExp		);
	virtual void	Visit( ComplExp *				complExp		);
	virtual void	Visit( NotExp *					notExp			);
	virtual void	Visit( CommaExpression *		commaExp		);
	virtual void	Visit( AssignmentExp *			assignExp		);
	virtual void	Visit( AddExp * 				addExp			);
	virtual void	Visit( SubExp * 				subExp			);
	virtual void	Visit( MulExp * 				mulExp			);
	virtual void	Visit( DivExp * 				divExp			);
	virtual void	Visit( ModExp * 				modExp			);
	virtual void	Visit( ShlExp * 				shlExp			);
	virtual void	Visit( ShrExp * 				shrExp			);
	virtual void	Visit( UShrExp *				ushrExp			);
	virtual void	Visit( XorExp *					xorExp			);
	virtual void	Visit( OrOrExp *				orExp			);
	virtual void	Visit( InExp * 					inExp			);
	virtual void	Visit( CondExp * 				condExp			);
	virtual void	Visit( EquExp * 				equExp			);
	virtual void	Visit( RelExp * 				relExp			);
	virtual void	Visit( BitAndExp * 				bitAndExp		);
	virtual void	Visit( BitXorExp * 				bitXorExp		);
	virtual void	Visit( BitOrExp * 				bitOrExp		);
	virtual void	Visit( AddAssignExp * 			addAssignExp	);
	virtual void	Visit( SubAssignExp * 			subAssignExp	);
	virtual void	Visit( MulAssignExp * 			mulAssignExp	);
	virtual void	Visit( DivAssignExp * 			divAssign		);
	virtual void	Visit( ModAssignExp * 			modAssign		);
	virtual void	Visit( BitAndAssignExp * 		bitAndAssignExp	);
	virtual void	Visit( BitXorAssignExp * 		bitXorAssignExp	);
	virtual void	Visit( BitOrAssignExp * 		bitOrAssignExp	);
	virtual void	Visit( ShlAssignExp * 			shlAssignExp	);
	virtual void	Visit( ShrAssignExp * 			shrAssignExp	);
	virtual void	Visit( UShrAssignExp * 			ushrAssignExp	);
	virtual void	Visit( CallExp *				callExp			);
	virtual void	Visit( ErroneousExpression *	errorExp		);
	virtual void	Visit( ThisExpression *			thisExp			);

	virtual void	Visit( AST_Node *				rootNode		);
#pragma endregion

public:
	IRCode *		GetLastGeneratedInstruction();

	// for canonicalization ( more complex expressions should be on the left side )
	void	Canonicalize( IRCode * node );

	// examples: (x+5+a)+8 => (x + a) + (5 + 8),
	// (c * 9) + (a + b) -> ((c * 9) + a) + b
	void	Reassociate( IRCode * node );

	// identify common subexpressions and build a diadic acyclic graph
	void	BuildDAG( IRCode * node );

private:
	IRList_t		m_insnList;		// list of intermediate instructions
	IRCode *		m_tail;		// the last generated instruction

	IRList_t		m_data_in_CONST_segment;

	bool	m_bOnRHSofAssignment;	// true if we're on rhs of an assignment

	// lookup table for selecting the right opcode for primitive type conversion
	// TODO: this is a sparse table; reduce memory usage
	ICode_t		m_convTable[ NUM_OF_TYPES ][ NUM_OF_TYPES ];
	void	InitConversionTable();

	//-----------------------------------------------
	// a wrapper to generate the given expression and insert an IR node in the list
	IRCode * GenerateExpression( Expression * e );
	
	IRCode * GenerateStatement( Statement * s );

	// a helper function for generating intermediate instructions for the given unary expression
	void	GenerateUnaryExpression( UnaryExpression* exp, ICode_t  opcode );

	// a helper function for generating intermediate instructions for the given binary expression
	void	GenerateBinaryExpression( BinaryExpression* exp, ICode_t  opcode );

	//int		m_iNumLabels;

	IRCode *	DefineLabel();
	void		Emit( ICode_t  opcode, IRCode * pLabel );

	//TODO: remove these
	void		EmitBranchIfFalse( IRCode * testExp, IRCode * pLabel );
	void		EmitBranchIfTrue( IRCode * testExp, IRCode * pLabel );
	
	void		MarkLabel( IRCode * pLabel );

	// this pass backpatches and fixes any labels
	// and removes unnecessary jumps
	void	InsertLabels();
	//-----------------------------------------------
	enum { MAX_NUM_NESTED_FUNCTIONS = 32 };

	// top of this stack always points to the beginning of the current function
	::c4::Stack< IRCode*, MAX_NUM_NESTED_FUNCTIONS >	m_funcStack;
	IRCode *	GetStartOfCurrentFunction() { return m_funcStack.Top(); }

	// how many '{' ... '}' are we in ?
	// will be 1, if we're in a function scope;
	int			m_iNestingLevel;

	//----------------------------------------
	IRCode *	EmitData( const char * s );
	IRCode *	EmitData( int i );
	IRCode *	EmitData( float f );

public:
	IRList_t &	Get_CONST_Data() { return m_data_in_CONST_segment; }
};

}//end namespace c4

#endif // !__IR_H__