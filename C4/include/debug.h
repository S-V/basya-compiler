#ifndef __DEBUG_H__
#define __DEBUG_H__

namespace c4 {

#if 0 //24.07.2008 - use c4::common instead
//----------------------------------------------------------------------------
// Fatal error - terminate execution immediately. Does not return.
//----------------------------------------------------------------------------
extern void Fatal			( const char * format, ... );

//----------------------------------------------------------------------------
// Similar, but say we got an internal error.
//----------------------------------------------------------------------------
extern void InternalError	( const char* file, int line, const char* where );

//----------------------------------------------------------------------------
// Redefine abort to report an internal error w/o coredump,
// and reporting the location of the error in the source file.
//----------------------------------------------------------------------------
extern void	Abort( const char* file, int line, const char* where );
#endif

//----------------------------------------------------------------------------

#ifdef __C4_DEBUG__

	//----------------------------------------------------------------------------
	//	Use this to mark unreachable locations 
	//	( like an unreachable default case of a switch )
	//----------------------------------------------------------------------------
	#define  Unreachable()	\
		c4::common->Error( "Unreachable code in %s, at '%s', line %d", __FUNCTION__, __FILE__, __LINE__ ); 
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	Examples of usage:
	//	Assert( a > b );
	//	Assert2( x == y, "X must be equal to Y!" );
	//----------------------------------------------------------------------------
	#define Assert( expr )	\
		(void)( (expr) ? 1 : c4::common->Error( "Assertion failed: '%s' in %s, at '%s', line %d", #expr, __FUNCTION__, __FILE__, __LINE__ ) )

	#define Assert2( expr, message )	\
		(void)( (expr) ? 1 : c4::common->Error( "Assertion failed: '%s': '%s' in %s, at '%s', line %d", message, #expr, __FUNCTION__, __FILE__, __LINE__ ) )

	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	Use this to mark unimplemented features
	//	which are not supported yet and will cause a crash
	//----------------------------------------------------------------------------
	#define  Unimplemented()	\
		c4::common->Error( "Unimplemented feature in %s, at '%s', line %d", __FUNCTION__, __FILE__, __LINE__ ); 

#elif !defined( __C4_DEBUG__ )

	#define Unreachable()

	#define Assert( expr )
	#define Assert( expr, message )

#endif // !__C4_DEBUG__

//----------------------------------------------------------------------------

namespace Debug {

//-------------------------------------------------------
//static const bool	DebugEnabled = true;
//-------------------------------------------------------

/*
=================================================================
	1) StaticAssert< bool >();
	2) STATIC_ASSERT( expression, message )

	Exampes of usage:
		1) Debug::StaticAssert< x>0 >();
		2) Debug::StaticAssert( p != NULL, POINTER_MUST_NOT_BE_NULL );

=================================================================
*/
template< bool > void	StaticAssert();
template<> void	StaticAssert< true >();

#define		STATIC_ASSERT( expression, message )		\
			{																					\
				struct ERROR_#message() { ERROR_#message() { StaticAssert< expression >(); } };	\
			}


//-------------------------------------------------------

//-------------------------------------------------------
//	struct CompileTimeChecker
//-------------------------------------------------------
namespace StaticChecks {

	template< bool > struct CompileTimeChecker;
	template<> struct CompileTimeChecker< true > {
		CompileTimeChecker( ... );
	};

}//end namespace StaticChecks

//-------------------------------------------------------
//	can be very useful for compile-time assersions
//-------------------------------------------------------
#define STATIC_CHECK( expr, msg )	\
	namespace Debug {										\
		namespace StaticChecks {							\
			class ERROR_##msg {								\
				CompileTimeChecker< (expr) != 0 > error;	\
			};												\
		}													\
	}//namespace Debug

//-------------------------------------------------------
#define compile_time_assert( x )				{ typedef int compile_time_assert_failed[ (x) ? 1 : -1 ]; }
#define file_scoped_compile_time_assert( x )	extern int compile_time_assert_failed[ (x) ? 1 : -1 ]
#define assert_sizeof( type, size )				file_scoped_compile_time_assert( sizeof( type ) == size )
#define assert_offsetof( type, field, offset )	file_scoped_compile_time_assert( offsetof( type, field ) == offset )
#define assert_sizeof_8_byte_multiple( type )	file_scoped_compile_time_assert( ( sizeof( type ) & 8 ) == 0 )
#define assert_sizeof_16_byte_multiple( type )	file_scoped_compile_time_assert( ( sizeof( type ) & 15 ) == 0 )
#define assert_sizeof_32_byte_multiple( type )	file_scoped_compile_time_assert( ( sizeof( type ) & 31 ) == 0 )

#define BREAK_IF( _CONDITION_ ) if (_CONDITION_) { _asm int 3 }

}//end namespace Debug
}//end namespace c4

#endif // !__DEBUG_H__