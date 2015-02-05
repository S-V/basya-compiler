#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

namespace c4 {

//------------------------------------------------------------------------------
// typedefs for built-in types to ensure portability
//------------------------------------------------------------------------------
//

#ifdef _MSC_VER

// 8 bits
//
typedef unsigned __int8		 byte;		
typedef	 signed char		i8;
typedef	 unsigned char		u8;
typedef unsigned char		uchar;

// 16 bits
//
typedef unsigned __int16	word;		
typedef  signed short		i16;
typedef  unsigned short		u16;

// 32 bits
//
typedef unsigned __int32	 dword;
typedef unsigned __int32	 uint;
typedef			 __int32     int32;
typedef unsigned __int32	 uint32;
typedef	  signed __int32	 i32;
typedef unsigned __int32	 u32;

typedef  float		f32;

// 64 bits
//
typedef			 __int64     qword;		
typedef unsigned __int64     ulong;		
typedef			 __int64     int64;
typedef unsigned __int64	 uint64;
typedef  signed __int64		 i64;
typedef  unsigned __int64	 u64;

typedef  double		f64;

//-------------------------------------------------

typedef	long double	float80;

// This should be the widest efficient host integer type.
// It can be 32 or 64 bits, except that if we are
// targeting a machine with 64-bit size_t then it has to be 64 bits.
typedef int64		wideInt;

typedef	int		FASTBOOL;

// Bitfield type
typedef unsigned long	BITFIELD;
//-------------------------------------------------

#else // !_MSC_VER

	int g_static_assert[0];

#endif // !_MSC_VER


// compile-time assertions

STATIC_CHECK( sizeof( c4::i8 ) == 1,   i8_should_be_1_byte );
STATIC_CHECK( sizeof( c4::i16 ) == 2, i16_should_be_2_bytes );
STATIC_CHECK( sizeof( c4::i32 ) == 4, i32_should_be_4_bytes );
STATIC_CHECK( sizeof( c4::i64 ) == 8, i64_should_be_8_bytes );

STATIC_CHECK( sizeof( c4::u8 ) == 1,   u8_should_be_1_byte );
STATIC_CHECK( sizeof( c4::u16 ) == 2, u16_should_be_2_bytes );
STATIC_CHECK( sizeof( c4::u32 ) == 4, u32_should_be_4_bytes );
STATIC_CHECK( sizeof( c4::u64 ) == 8, u64_should_be_8_bytes );

STATIC_CHECK( sizeof( c4::f32 ) == 4, f32_should_be_4_bytes );
STATIC_CHECK( sizeof( c4::f64 ) == 8, f64_should_be_8_bytes );

}//end namespace c4

#endif // !__TYPEDEFS_H__