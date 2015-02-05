#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

#if 0 //24.07.2008 - use c4::common instead
//----------------------------------------------------------------------------
void Fatal( const char * format, ... )
{
	va_list   ap;

	va_start( ap, format );
	fprintf( stderr, format, ap );
	va_end( ap );

	fputc( '\n', stderr );
	exit( EXIT_FAILURE );
}
//----------------------------------------------------------------------------
void InternalError( const char * format, ... )
{
	va_list   ap;

	va_start( ap, format );
	fprintf( stderr, "%s: Internal error: ", "C4 Compiler" );
	vfprintf( stderr, format, ap );
	va_end( ap );

	fputc( '\n', stderr );
	exit( EXIT_FAILURE );
}
//-------------------------------------------------------
void Abort( const char* file, int line, const char* where )
{
	c4::common->Error( "abort in %s, at '%s', line %d", where, file, line );
}
#endif

//-------------------------------------------------------

namespace Debug {

template<> void	StaticAssert< true >() {}

}//end namespace Debug
}//end namespace c4