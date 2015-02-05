/*
=================================================
	Some macros that appear throughout the system.
=================================================
*/
#ifndef __DEFINES_H__
#define __DEFINES_H__

#define __C4_DEBUG__

//#define IMPLEMENTATION_WARNINGS

//==================== defines ======================================================

#define INLINE			inline
#define FORCEINLINE		__forceinline

#define	 override	//virtual

//===========================================================
#define _CRT_SECURE_NO_WARNINGS

// unknown pragma
#pragma warning( disable : 4068 )

//===================================================================================

#ifndef BIT
#define BIT(n)		(1 << (n))
#endif

#define SAFE_DELETE( p )         { if(p) { delete (p);     (p) = NULL; } }
#define SAFE_DELETE_ARRAY( p )   { if(p) { delete[] (p);   (p) = NULL; } }
#define SAFE_RELEASE( p )        { if(p) { (p)->Release(); (p) = NULL; } }

#define GRAB( p )				 { if(p) { (p)->Grab(); } }
#define DROP( p )				 { if(p) { (p)->Drop(); } }

//===================================================================================

#define ARRAY_SIZE( ptr )		( sizeof( ptr ) / sizeof( ptr[ 0 ] ) )

#endif // !__PRECOMPILED_H__