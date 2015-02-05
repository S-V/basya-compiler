#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

int Math::IsPow2( unsigned long u ) {
	if ( u > 1 && (u & (u - 1)) == 0 )
	{
		for ( int n = 0; u; u >>= 1, n++ )
		{
			if ( u & 1 ) {
				return n;
			}
		}
	}
	return 0;
}

}//end namespace c4
