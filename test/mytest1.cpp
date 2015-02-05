float circumf( int r ) {
	return 2 * r * 3.14159265358;
}
float area( int r ) {
	return r * r * 3.14159265358;
}
float Mul( float a, float b ) {
	return a * b;
}
int Div( int a, int b, int c ) {
	return a / b;
}
int conditiontest( int test1, int test2 )
{
	int result;
	if ( test1 > test2 ) {
		result = test1;
	}
	else if ( test1 < test2 ) {
		result = test2;
	}
	else {
		result = 0;
	}
	return result;
}

int main()
{
	int a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q;
	a = 87;
	b = 343;
	c = 33;

	printf ("Starting the program..." );

	while ( a > 0 ) {
		printf( "a = %d\n", a );
		a = a - 6;
		if ( a % 37 > 8 ) {
			printf( "Hee, hoo!\n" );
			return a - b;
		}
	}

	float f1, f2, f3, f4;
	unsigned int  ua, ub, uc;
	char  c1, c2, c3 = 19;

	f1 = (f2 = 67.34, f4 = 4343, f2 + 34 / f4 * f2 );

	f1 = f2 + f3;
	f1 = i * f3;
	i = f1 / f3;
	c1 = f1 + i + ua / uc;
	f1 = c1 * c3;
	i = (f1 - f3)*67 + c2/f1;

	unsigned char c4;

	{
		char  bellChar = 7;
		printf( "Hello, world!%c, %d%c", 'a', i = f4*7, c3 );
		printf( "bell!!!%c", bellChar );
	}

	int x = a < b|c|d-32<<b+ua/uc;
	do {
		printf( "x = %d", x );
	} while ( x <= 33&a*b-c<<b);

	i = a / c % !(b % 3 );
/*	
	
	int i1, i2, i3;
	unsigned long int l1, l2, l3;
	long int ll; // long long int ll;
	
	
	short s1, s2, s3;
	unsigned short us1, us2, us3;
	bool b1, b2, b3;
*/
	// UNDONE:
	//char ^ pStr = "Hello, world";

	getchar();


	//i = (i=a) + (b=a);

	//i = (i=a) + (b=c);

	a = ( i = i + 1, b );

	//i = ++a / c;
/*
	a = i++;
				->
						t = i;
						i = i + 1;
						a = t;
*/

	//a = 1 / (i = i / c, 8, f1++);

//	i = ++a - ub / ++i;
//	i = ++a >>> ++b % c;

//	i = i++ + i--;

	if ( a + b ) {
		i = i + 1;
		return -1;
	}
	else if ( !(a + b) ) {
		a = b % i;
	}
	else if ( ~a ) {
		i = a << b;
	}

	if ( a > b ) {
		a = a - 1;
		return a - b;
	}

	//int x = (char) a;

	//printf( "%s, %d, %c", "TestMessage", 777, 7 );
	i = printf( i + b, c&7, 9, b ) % getchar();

/*	int Add( int a, int b ) {
		return a + b;
	}
*/

	// DONE:
	i = c2 = uc%a+13*f&1+~(a&b|c)<=b+13*8<<21>>>(298%c%1)/ua/ub;
	i = (a=b, i=i+1, a=8*b, m|b+3&32) % (c=a, i=43, a=~b|1, b<<c>=8);

	i = (54432 % ((a / b) * 34 * c / 1 + 32 % (2 * a) ) + 3258 / (483 % (a*b*7)) * 24 + 87323 / (a + 43 %c) % 54 - 43*a*b*c/17 ) * (a + b/4 + c * 343 % q * 232324 / ( 434 + (a*b) % 8 / 1 * c - 23 ));
	i = i / (b - 1 + b*8 + c * d) / (e + f);
	ua = (ub / uc + 43 * 8) + ua / uc;
	i = 3*a + b*c + (a >= b) + 34 % c;
	c3 = ((a == d)%3 == 1);
	i = ((a == d)*7 == 1);
	i = ((a < d)*7 > 1);
	i = (~a|b*4%d<<l%p+31*c*88-!a<0+11+k);
	return 0;
}

/*
long int g_i1, g_i2;

namespace c4 {

class Foo {
	public this() {
		m_fValue = 0;
		auto i = cast< int >( m_fValue );
	}
	
	private ~this() {
		assert( m_fValue != 0, "value should not be zero" );
	}

	public override int Add( int a, int b ) {
		return cast<int>(a + b);
	}

private:
	public abstract void Go() {}
	
	protected virtual int vGetNum( int ^^ p = 0 ) { return 0; }

	float m_fValue = 0;
}

}//end namespace c4

*/