#ifndef __ICOMPILER_H__
#define __ICOMPILER_H__

namespace c4 {

// for displaying parse trees in a tree view
class UINode;

class CompilationUnit;

//=====================================

enum TARGET_TYPE 
{
	NO_TARGET,		// perform semantic analysis only

	C4_BYTECODE,
	JVM_BYTECODE,
	MSIL_BYTECODE,

	LLVM_ASSEMBLY,

	C_SOURCE,
	C_PLUS_PLUS_SOURCE,
	C4_BEAUTIFIED_SOURCE,	// re-formatting
	C4_CODE_COVERAGE,		// profiling
	
	ASSEMBLY_LISTING,
	NATIVE_EXE
};

//-------------------------------------------------

enum COMPILER_SETTINGS {

	// check syntax and semantics only, don't generate code
	DO_NOT_GENERATE_CODE	= BIT( 0 ),

	// useful for tracking down compiler bugs
	EMIT_DEBUG_INFO			= BIT( 1 ),

	//
	// assembler output
	//

	// use variable names instead of stack offsets
	USE_SYMBOL_NAMES		= BIT( 2 ),

	// ex.: use 'xor\teax, eax' instead of 'xor eax, eax'
	USE_TABS_FOR_SPACING	= BIT( 3 ),

	// ex.: use 'EAX' instead of 'eax'
	REGISTER_NAMES_IN_UPPERCASE = BIT( 4 ),

	//
	// code generation
	//
	
	// don't do even minor optimizations, generate terrible code
	DO_NOT_PERFORM_ANY_OPTIMIZATIONS = BIT( 5 ),

	TRANSFORM_MUL_INTO_SHL	= BIT( 6 ),
	TRANSFORM_DIV_INTO_SHR	= BIT( 7 ),
	FOLD_CONSTANTS			= BIT( 8 )
};

/*
==========================================
	class ICompiler
==========================================
*/

class ICompiler {
public:
	virtual					~ICompiler() {}

	// Initialize the compiler for the first time.
	virtual bool			Init() = 0;

	// Shutdown the compiler, free resources, ... .
	virtual void			Shutdown() = 0;

	virtual void			SetInputFile( const char * srcFileName ) = 0;

	// Prepare the compiler for processing another source file.
	virtual bool			Reset( const char * pSource ) = 0;	// source is a pointer to memory
	
	virtual bool			GenerateCode( TARGET_TYPE ) = 0;

	virtual void			SetMaxErrorCount( int maxNumErrors ) = 0;
	virtual void			SetMaxNumWarnings( int maxNumWarnings ) = 0;

							// example: "test"
	virtual void			SetOutputFileName( const char * filename ) = 0;

	virtual const char *	GetOutputFileName() const = 0;

	// returns false if 'option' is an unsupported or invalid option
	virtual bool		SetOption( COMPILER_SETTINGS  option, int data = 0 ) = 0;
	virtual bool		IsOptionSet( COMPILER_SETTINGS  option ) const = 0;
	//-- error handling -------------------------

						// prints a warning message
	virtual void		Warning( const char * format, ... ) = 0;

						// prints an error message and exits the program
	virtual void		Error( const char * format, ... ) = 0;

						// fatal error - print error message and exit.
	virtual void		Fatal( const char * format, ... ) = 0;
};

extern ICompiler *		compiler;

}//end namespace c4

#endif // !__ICOMPILER_H__