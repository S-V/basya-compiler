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

	while ( a > 0 ) {
		printf( "a = %d\n", a );
		a = a - 6;
		if ( a % 37 > 8 ) {
			printf( "Hee, hoo!\n" );
			return a - b;
		}
	}

	float f1, f2, f3, f4;

	f1 = (f2 = 67.34, f4 = 4343, f2 + 34 / f4 * f2 );

	unsigned int  ua, ub, uc;

	char  c1, c2, c3 = 19;
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


	getchar();


	a = ( i = i + 1, b );
	i = ++a / c;


	a = 1 / (b = i * i / c, a += 8, ++i);

	i = ++a - ub / ++i;
	i = ++a >>> ++b % c;

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

	{
		i = printf( i + b, c&7, 9, b ) % getchar();
	}

	// DONE:
	i = c2 = uc%a+13*f&1+~(a&b|c)<=b+13*8<<21>>>(298%c%1)/ua/ub;
	i = (a=b, i=i+1, a=8*b, m|b+3&32) % (c=a, i=43, a=~b|1, b<<c>=8);

	return 0;
}

/*

GOAL:

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