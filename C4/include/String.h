#ifndef __STRING_H__
#define __STRING_H__

namespace c4 {
	
// swiped from Id Software ( idStr )

class String {
public:
						String( void );
						String( const String & other );
						String( const String & other, int start, int end );
						String( const char *text );
						String( const char *text, int start, int end );
						explicit String( const bool b );
						explicit String( const char c );
						explicit String( const int i );
						explicit String( const unsigned u );
						explicit String( const float f );
						~String( void );

	size_t				Size( void ) const;
	const char *		c_str( void ) const;
	operator 			const char * ( void ) const;
	operator 			const char * ( void );

	char				operator [] ( int index ) const;
	char &				operator [] ( int index );

	void				operator = ( const String & other );
	void				operator = ( const char * text );

	friend String		operator + ( const String &a, const String &b );
	friend String		operator + ( const String &a, const char *b );
	friend String		operator + ( const char *a, const String &b );

	friend String		operator + ( const String &a, const float b );
	friend String		operator + ( const String &a, const int b );
	friend String		operator + ( const String &a, const unsigned b );
	friend String		operator + ( const String &a, const bool b );
	friend String		operator + ( const String &a, const char b );

	String &			operator += ( const String &a );
	String &			operator += ( const char *a );
	String &			operator += ( const float a );
	String &			operator += ( const char a );
	String &			operator += ( const int a );
	String &			operator += ( const unsigned a );
	String &			operator += ( const bool a );

						// case sensitive compare
	friend bool			operator == ( const String &a, const String &b );
	friend bool			operator == ( const String &a, const char *b );
	friend bool			operator == ( const char *a, const String &b );

						// case sensitive compare
	friend bool			operator != ( const String &a, const String &b );
	friend bool			operator != ( const String &a, const char *b );
	friend bool			operator != ( const char *a, const String &b );

						// case sensitive compare
	int					Cmp( const char *text ) const;
	int					Cmpn( const char *text, int n ) const;
	int					CmpPrefix( const char *text ) const;

						// case insensitive compare
	int					Icmp( const char *text ) const;
	int					Icmpn( const char *text, int n ) const;
	int					IcmpPrefix( const char *text ) const;

						// compares paths and makes sure folders come first
	int					IcmpPath( const char *text ) const;
	int					IcmpnPath( const char *text, int n ) const;
	int					IcmpPrefixPath( const char *text ) const;

	int					Length( void ) const;		
	int					Allocated( void ) const;
	void				Empty( void );
	bool				IsEmpty( void ) const;
	void				Clear( void );
	void				Append( const char a );
	void				Append( const String &text );
	void				Append( const char *text );
	void				Append( const char *text, int len );
	void				Insert( const char a, int index );
	void				Insert( const char *text, int index );
	void				ToLower( void );
	void				ToUpper( void );
	bool				IsNumeric( void ) const;

	bool				HasLower( void ) const;
	bool				HasUpper( void ) const;
	int					LengthWithoutColors( void ) const;
	void				CapLength( int );
	void				Fill( const char ch, int newlen );

	int					Find( const char c, int start = 0, int end = -1 ) const;
	int					Find( const char *text, bool casesensitive = true, int start = 0, int end = -1 ) const;
	bool				Filter( const char *filter, bool casesensitive ) const;
	int					Last( const char c ) const;						// return the index to the last occurance of 'c', returns -1 if not found
	const char *		Left( int len, String &result ) const;			// store the leftmost 'len' characters in the result
	const char *		Right( int len, String &result ) const;			// store the rightmost 'len' characters in the result
	const char *		Mid( int start, int len, String &result ) const;	// store 'len' characters starting at 'start' in result
	String				Left( int len ) const;							// return the leftmost 'len' characters
	String				Right( int len ) const;							// return the rightmost 'len' characters
	String				Mid( int start, int len ) const;				// return 'len' characters starting at 'start'
	void				StripLeading( const char c );					// strip char from front as many times as the char occurs
	void				StripLeading( const char *str );				// strip String from front as many times as the String occurs
	bool				StripLeadingOnce( const char *str );			// strip String from front just once if it occurs
	void				StripTrailing( const char c );					// strip char from end as many times as the char occurs
	void				StripTrailing( const char *str );				// strip String from end as many times as the String occurs
	bool				StripTrailingOnce( const char *str );			// strip String from end just once if it occurs
	void				Strip( const char c );							// strip char from front and end as many times as the char occurs
	void				Strip( const char *str );						// strip String from front and end as many times as the String occurs
	void				StripTrailingWhitespace( void );				// strip trailing white space characters
	String &			StripQuotes( void );							// strip quotes around String
	void				Replace( const char *old, const char *nw );
	const char *		ToChar() const;

	enum { FILE_HASH_SIZE =	1024 };

	// file name methods
	int					FileNameHash( void ) const;						// hash key for the filename (skips extension)
	String &				BackSlashesToSlashes( void );					// convert slashes
	String &				SetFileExtension( const char *extension );		// set the given file extension
	String &				StripFileExtension( void );						// remove any file extension
	String &				StripAbsoluteFileExtension( void );				// remove any file extension looking from front (useful if there are multiple .'s)
	String &				DefaultFileExtension( const char *extension );	// if there's no file extension use the default
	String &				DefaultPath( const char *basepath );			// if there's no path use the default
	void				AppendPath( const char *text );					// append a partial path
	String &				StripFilename( void );							// remove the filename from a path
	String &				StripPath( void );								// remove the path from the filename
	void				ExtractFilePath( String &dest ) const;			// copy the file path to another String
	void				ExtractFileName( String &dest ) const;			// copy the filename to another String
	void				ExtractFileBase( String &dest ) const;			// copy the filename minus the extension to another String
	void				ExtractFileExtension( String &dest ) const;		// copy the file extension to another String
	bool				CheckExtension( const char *ext );

	// char * methods to replace library functions
	static int			Length( const char *s );
	static char *		ToLower( char *s );
	static char *		ToUpper( char *s );
	static bool			IsNumeric( const char *s );
	static bool			HasLower( const char *s );
	static bool			HasUpper( const char *s );
	static int			Cmp( const char *s1, const char *s2 );
	static int			Cmpn( const char *s1, const char *s2, int n );
	static int			Icmp( const char *s1, const char *s2 );
	static int			Icmpn( const char *s1, const char *s2, int n );
	static int			IcmpNoColor( const char *s1, const char *s2 );
	static int			IcmpPath( const char *s1, const char *s2 );			// compares paths and makes sure folders come first
	static int			IcmpnPath( const char *s1, const char *s2, int n );	// compares paths and makes sure folders come first
	static void			Append( char *dest, int size, const char *src );
	static void			Copynz( char *dest, const char *src, int destsize );
	static int			snPrintf( char *dest, int size, const char *fmt, ... );
	static int			vsnPrintf( char *dest, int size, const char *fmt, va_list argptr );
	static int			FindChar( const char *str, const char c, int start = 0, int end = -1 );
	static int			FindText( const char *str, const char *text, bool casesensitive = true, int start = 0, int end = -1 );
	static bool			Filter( const char *filter, const char *name, bool casesensitive );
	static void			StripMediaName( const char *name, String &mediaName );
	static bool			CheckExtension( const char *name, const char *ext );
	static const char *	FloatArrayToString( const float *array, const int length, const int precision );

	// hash keys
	static int			Hash( const char *str );
	static int			Hash( const char *str, int length );
	static int			IHash( const char *str );					// case insensitive
	static int			IHash( const char *str, int length );		// case insensitive

	// character methods
	static char			ToLower( char c );
	static char			ToUpper( char c );
	static bool			CharIsPrintable( int c );
	static bool			CharIsLower( int c );
	static bool			CharIsUpper( int c );
	static bool			CharIsAlpha( int c );
	static bool			CharIsNumeric( int c );
	static bool			CharIsNewLine( char c );
	static bool			CharIsTab( char c );
	static int			ColorIndex( int c );

	friend int			_sprintf( String &dest, const char *fmt, ... );
	friend int			_vsprintf( String &dest, const char *fmt, va_list ap );

	void				ReAllocate( int amount, bool keepold );				// reallocate String data buffer
	void				FreeData( void );									// free allocated String memory

	static void			InitMemory( void );
	static void			ShutdownMemory( void );
	static void			PurgeMemory( void );

	int					DynamicMemoryUsed() const;

	char *				GetBufferPtr();

protected:
	enum { STR_ALLOC_GRANULARITY = 32, STR_ALLOC_BASE = 20 };

	int					len;
	char *				data;
	int					alloced;
	char				baseBuffer[ STR_ALLOC_BASE ];

	void				Init( void );										// initialize String using base buffer
	void				EnsureAlloced( int amount, bool keepold = true );	// ensure String data buffer is large anough
};

INLINE void String::EnsureAlloced( int amount, bool keepold ) {
	if ( amount > alloced ) {
		ReAllocate( amount, keepold );
	}
}

INLINE void String::Init( void ) {
	len = 0;
	alloced = STR_ALLOC_BASE;
	data = baseBuffer;
	data[ 0 ] = '\0';
}

INLINE String::String( void ) {
	Init();
}

INLINE String::String( const String &text ) {
	int l;

	Init();
	l = text.Length();
	EnsureAlloced( l + 1 );
	strcpy( data, text.data );
	len = l;
}

INLINE String::String( const String &text, int start, int end ) {
	int i;
	int l;

	Init();
	if ( end > text.Length() ) {
		end = text.Length();
	}
	if ( start > text.Length() ) {
		start = text.Length();
	} else if ( start < 0 ) {
		start = 0;
	}

	l = end - start;
	if ( l < 0 ) {
		l = 0;
	}

	EnsureAlloced( l + 1 );

	for ( i = 0; i < l; i++ ) {
		data[ i ] = text[ start + i ];
	}

	data[ l ] = '\0';
	len = l;
}

INLINE String::String( const char *text ) {
	int l;

	Init();
	if ( text ) {
		l = strlen( text );
		EnsureAlloced( l + 1 );
		strcpy( data, text );
		len = l;
	}
}

INLINE String::String( const char *text, int start, int end ) {
	int i;
	int l = strlen( text );

	Init();
	if ( end > l ) {
		end = l;
	}
	if ( start > l ) {
		start = l;
	} else if ( start < 0 ) {
		start = 0;
	}

	l = end - start;
	if ( l < 0 ) {
		l = 0;
	}

	EnsureAlloced( l + 1 );

	for ( i = 0; i < l; i++ ) {
		data[ i ] = text[ start + i ];
	}

	data[ l ] = '\0';
	len = l;
}

INLINE String::String( const bool b ) {
	Init();
	EnsureAlloced( 2 );
	data[ 0 ] = b ? '1' : '0';
	data[ 1 ] = '\0';
	len = 1;
}

INLINE String::String( const char c ) {
	Init();
	EnsureAlloced( 2 );
	data[ 0 ] = c;
	data[ 1 ] = '\0';
	len = 1;
}

INLINE String::String( const int i ) {
	char text[ 64 ];
	int l;

	Init();
	l = sprintf( text, "%d", i );
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

INLINE String::String( const unsigned u ) {
	char text[ 64 ];
	int l;

	Init();
	l = sprintf( text, "%u", u );
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

INLINE String::String( const float f ) {
	char text[ 64 ];
	int l;

	Init();
	l = String::snPrintf( text, sizeof( text ), "%f", f );
	while( l > 0 && text[l-1] == '0' ) text[--l] = '\0';
	while( l > 0 && text[l-1] == '.' ) text[--l] = '\0';
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

INLINE String::~String( void ) {
	FreeData();
}

INLINE size_t String::Size( void ) const {
	return sizeof( *this ) + Allocated();
}

INLINE const char *String::c_str( void ) const {
	return data;
}

INLINE String::operator  const char * ( void ) {
	return c_str();
}

INLINE String::operator  const char * ( void ) const {
	return c_str();
}

INLINE char String::operator [] ( int index ) const {
	assert( ( index >= 0 ) && ( index <= len ) );
	return data[ index ];
}

INLINE char &String::operator [] ( int index ) {
	assert( ( index >= 0 ) && ( index <= len ) );
	return data[ index ];
}

INLINE void String::operator = ( const String &text ) {
	int l;

	l = text.Length();
	EnsureAlloced( l + 1, false );
	memcpy( data, text.data, l );
	data[l] = '\0';
	len = l;
}

INLINE String operator + ( const String &a, const String &b ) {
	String result( a );
	result.Append( b );
	return result;
}

INLINE String operator + ( const String &a, const char *b ) {
	String result( a );
	result.Append( b );
	return result;
}

INLINE String operator + ( const char *a, const String &b ) {
	String result( a );
	result.Append( b );
	return result;
}

INLINE String operator + ( const String &a, const bool b ) {
	String result( a );
	result.Append( b ? "true" : "false" );
	return result;
}

INLINE String operator + ( const String &a, const char b ) {
	String result( a );
	result.Append( b );
	return result;
}

INLINE String operator + ( const String &a, const float b ) {
	char	text[ 64 ];
	String	result( a );

	sprintf( text, "%f", b );
	result.Append( text );

	return result;
}

INLINE String operator + ( const String &a, const int b ) {
	char	text[ 64 ];
	String	result( a );

	sprintf( text, "%d", b );
	result.Append( text );

	return result;
}

INLINE String operator + ( const String &a, const unsigned b ) {
	char	text[ 64 ];
	String	result( a );

	sprintf( text, "%u", b );
	result.Append( text );

	return result;
}

INLINE String &String::operator += ( const float a ) {
	char text[ 64 ];

	sprintf( text, "%f", a );
	Append( text );

	return *this;
}

INLINE String &String::operator += ( const int a ) {
	char text[ 64 ];

	sprintf( text, "%d", a );
	Append( text );

	return *this;
}

INLINE String &String::operator += ( const unsigned a ) {
	char text[ 64 ];

	sprintf_s( text, 64, "%u", a );
	Append( text );

	return *this;
}

INLINE String &String::operator += ( const String &a ) {
	Append( a );
	return *this;
}

INLINE String &String::operator += ( const char *a ) {
	Append( a );
	return *this;
}

INLINE String &String::operator += ( const char a ) {
	Append( a );
	return *this;
}

INLINE String &String::operator += ( const bool a ) {
	Append( a ? "true" : "false" );
	return *this;
}

INLINE bool operator == ( const String &a, const String &b ) {
	return ( !String::Cmp( a.data, b.data ) );
}

INLINE bool operator == ( const String &a, const char *b ) {
	assert( b );
	return ( !String::Cmp( a.data, b ) );
}

INLINE bool operator == ( const char *a, const String &b ) {
	assert( a );
	return ( !String::Cmp( a, b.data ) );
}

INLINE bool operator != ( const String &a, const String &b ) {
	return !( a == b );
}

INLINE bool operator != ( const String &a, const char *b ) {
	return !( a == b );
}

INLINE bool operator != ( const char *a, const String &b ) {
	return !( a == b );
}

INLINE int String::Cmp( const char *text ) const {
	assert( text );
	return String::Cmp( data, text );
}

INLINE int String::Cmpn( const char *text, int n ) const {
	assert( text );
	return String::Cmpn( data, text, n );
}

INLINE int String::CmpPrefix( const char *text ) const {
	assert( text );
	return String::Cmpn( data, text, strlen( text ) );
}

INLINE int String::Icmp( const char *text ) const {
	assert( text );
	return String::Icmp( data, text );
}

INLINE int String::Icmpn( const char *text, int n ) const {
	assert( text );
	return String::Icmpn( data, text, n );
}

INLINE int String::IcmpPrefix( const char *text ) const {
	assert( text );
	return String::Icmpn( data, text, strlen( text ) );
}

INLINE int String::Length( void ) const {
	return len;
}

INLINE int String::Allocated( void ) const {
	if ( data != baseBuffer ) {
		return alloced;
	} else {
		return 0;
	}
}

INLINE void String::Empty( void ) {
	EnsureAlloced( 1 );
	data[ 0 ] = '\0';
	len = 0;
}

INLINE bool String::IsEmpty( void ) const {
	return ( String::Cmp( data, "" ) == 0 );
}

INLINE void String::Clear( void ) {
	FreeData();
	Init();
}

INLINE void String::Append( const char a ) {
	EnsureAlloced( len + 2 );
	data[ len ] = a;
	len++;
	data[ len ] = '\0';
}

INLINE void String::Append( const String &text ) {
	int newLen;
	int i;

	newLen = len + text.Length();
	EnsureAlloced( newLen + 1 );
	for ( i = 0; i < text.len; i++ ) {
		data[ len + i ] = text[ i ];
	}
	len = newLen;
	data[ len ] = '\0';
}

INLINE void String::Append( const char *text ) {
	int newLen;
	int i;

	if ( text ) {
		newLen = len + strlen( text );
		EnsureAlloced( newLen + 1 );
		for ( i = 0; text[ i ]; i++ ) {
			data[ len + i ] = text[ i ];
		}
		len = newLen;
		data[ len ] = '\0';
	}
}

INLINE void String::Append( const char *text, int l ) {
	int newLen;
	int i;

	if ( text && l ) {
		newLen = len + l;
		EnsureAlloced( newLen + 1 );
		for ( i = 0; text[ i ] && i < l; i++ ) {
			data[ len + i ] = text[ i ];
		}
		len = newLen;
		data[ len ] = '\0';
	}
}

INLINE void String::Insert( const char a, int index ) {
	int i, l;

	if ( index < 0 ) {
		index = 0;
	} else if ( index > len ) {
		index = len;
	}

	l = 1;
	EnsureAlloced( len + l + 1 );
	for ( i = len; i >= index; i-- ) {
		data[i+l] = data[i];
	}
	data[index] = a;
	len++;
}

INLINE void String::Insert( const char *text, int index ) {
	int i, l;

	if ( index < 0 ) {
		index = 0;
	} else if ( index > len ) {
		index = len;
	}

	l = strlen( text );
	EnsureAlloced( len + l + 1 );
	for ( i = len; i >= index; i-- ) {
		data[i+l] = data[i];
	}
	for ( i = 0; i < l; i++ ) {
		data[index+i] = text[i];
	}
	len += l;
}

INLINE void String::ToLower( void ) {
	for (int i = 0; data[i]; i++ ) {
		// HUMANHEAD pdm: cast to unsigned for the sake of western european characters, which use the sign bit
		if ( CharIsUpper( (unsigned char)data[i] ) ) {
			data[i] += ( 'a' - 'A' );
		}
	}
}

INLINE void String::ToUpper( void ) {
	for (int i = 0; data[i]; i++ ) {
		// HUMANHEAD pdm: cast to unsigned for the sake of western european characters, which use the sign bit
		if ( CharIsLower( (unsigned char)data[i] ) ) {
			data[i] -= ( 'a' - 'A' );
		}
	}
}

INLINE bool String::IsNumeric( void ) const {
	return String::IsNumeric( data );
}

INLINE bool String::HasLower( void ) const {
	return String::HasLower( data );
}

INLINE bool String::HasUpper( void ) const {
	return String::HasUpper( data );
}

INLINE void String::CapLength( int newlen ) {
	if ( len <= newlen ) {
		return;
	}
	data[ newlen ] = 0;
	len = newlen;
}

INLINE void String::Fill( const char ch, int newlen ) {
	EnsureAlloced( newlen + 1 );
	len = newlen;
	memset( data, ch, len );
	data[ len ] = 0;
}

INLINE int String::Find( const char c, int start, int end ) const {
	if ( end == -1 ) {
		end = len;
	}
	return String::FindChar( data, c, start, end );
}

INLINE int String::Find( const char *text, bool casesensitive, int start, int end ) const {
	if ( end == -1 ) {
		end = len;
	}
	return String::FindText( data, text, casesensitive, start, end );
}

INLINE bool String::Filter( const char *filter, bool casesensitive ) const {
	return String::Filter( filter, data, casesensitive );
}

INLINE const char *String::Left( int len, String &result ) const {
	return Mid( 0, len, result );
}

INLINE const char *String::Right( int len, String &result ) const {
	if ( len >= Length() ) {
		result = *this;
		return result;
	}
	return Mid( Length() - len, len, result );
}

INLINE String String::Left( int len ) const {
	return Mid( 0, len );
}

INLINE String String::Right( int len ) const {
	if ( len >= Length() ) {
		return *this;
	}
	return Mid( Length() - len, len );
}

INLINE void String::Strip( const char c ) {
	StripLeading( c );
	StripTrailing( c );
}

INLINE void String::Strip( const char *str ) {
	StripLeading( str );
	StripTrailing( str );
}

INLINE bool String::CheckExtension( const char *ext ) {
	return String::CheckExtension( data, ext );
}

INLINE int String::Length( const char *s ) {
	int i;
	for ( i = 0; s[i]; i++ ) {}
	return i;
}

INLINE char *String::ToLower( char *s ) {
	for ( int i = 0; s[i]; i++ ) {
		// HUMANHEAD pdm: cast to unsigned for the sake of western european characters, which use the sign bit
		if ( CharIsUpper( (unsigned char)s[i] ) ) {
			s[i] += ( 'a' - 'A' );
		}
	}
	return s;
}

INLINE char *String::ToUpper( char *s ) {
	for ( int i = 0; s[i]; i++ ) {
		// HUMANHEAD pdm: cast to unsigned for the sake of western european characters, which use the sign bit
		if ( CharIsLower( (unsigned char)s[i] ) ) {
			s[i] -= ( 'a' - 'A' );
		}
	}
	return s;
}

INLINE int String::Hash( const char *str ) {
	int i, hash = 0;
	for ( i = 0; *str != '\0'; i++ ) {
		hash += ( *str++ ) * ( i + 119 );
	}
	return hash;
}

INLINE int String::Hash( const char *str, int length ) {
	int i, hash = 0;
	for ( i = 0; i < length; i++ ) {
		hash += ( *str++ ) * ( i + 119 );
	}
	return hash;
}

INLINE int String::IHash( const char *str ) {
	int i, hash = 0;
	for( i = 0; *str != '\0'; i++ ) {
		hash += ToLower( *str++ ) * ( i + 119 );
	}
	return hash;
}

INLINE int String::IHash( const char *str, int length ) {
	int i, hash = 0;
	for ( i = 0; i < length; i++ ) {
		hash += ToLower( *str++ ) * ( i + 119 );
	}
	return hash;
}

INLINE char String::ToLower( char c ) {
	if ( c <= 'Z' && c >= 'A' ) {
		return ( c + ( 'a' - 'A' ) );
	}
	return c;
}

INLINE char String::ToUpper( char c ) {
	if ( c >= 'a' && c <= 'z' ) {
		return ( c - ( 'a' - 'A' ) );
	}
	return c;
}

INLINE bool String::CharIsPrintable( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c >= 0x20 && c <= 0x7E ) || ( c >= 0xA1 && c <= 0xFF );
}

INLINE bool String::CharIsLower( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c >= 'a' && c <= 'z' ) || ( c >= 0xE0 && c <= 0xFF );
}

INLINE bool String::CharIsUpper( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c <= 'Z' && c >= 'A' ) || ( c >= 0xC0 && c <= 0xDF );
}

INLINE bool String::CharIsAlpha( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ||
			 ( c >= 0xC0 && c <= 0xFF ) );
}

INLINE bool String::CharIsNumeric( int c ) {
	return ( c <= '9' && c >= '0' );
}

INLINE bool String::CharIsNewLine( char c ) {
	return ( c == '\n' || c == '\r' || c == '\v' );
}

INLINE bool String::CharIsTab( char c ) {
	return ( c == '\t' );
}

INLINE int String::ColorIndex( int c ) {
	return ( c & 15 );
}

INLINE int String::DynamicMemoryUsed() const {
	return ( data == baseBuffer ) ? 0 : alloced;
}

INLINE const char* String::ToChar() const {
	return data;
}

INLINE char * String::GetBufferPtr() {
	return data;
}

}// end namespace c4

#endif // !__STRING_H__