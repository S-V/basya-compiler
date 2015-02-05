#ifndef __LEXER_H__
#define __LEXER_H__

namespace c4 {

/*
==========================================================================
	class Lexer - scanner ( or tokenizer ).

	It builds a list of tokens; scans and appends new tokens as needed.
==========================================================================
*/

class Lexer {
public:
					Lexer();
					Lexer( const char * source );	// 'source' is a pointer to the script
					~Lexer();

						// must be called only once to fill the table of keywords, etc..
	static void			StaticInit();

						// load a script from the given memory
	void				Reset( const char * source );

						// reset to the beginning of the script if loaded
	void				Reset();

						// load a script from the given path
	bool				LoadFile( const char *filename );

						// free the script
	void				FreeSource();

						// returns true if a script is loaded
	bool				IsLoaded() const;

						// read a token from the source and return its type
	int					ReadToken();
						// read a token from the source and return a pointer to it
	Token *				ReadNextToken();
						// unread one token, move back to the previous token
	void				UnreadToken();

						// returns the next character of the script without reading it
	char				PeekChar() const;
	char				PreviousChar() const;

	int					PeekTokenType();
	Token *				PeekToken();
						// useful for doing lookaheads, for example, PeekNextToken( PeekNextToken() )->Type() ...
	Token *				PeekNextToken( Token* prevToken = 0 );
	const char *		PeekTokenString();

						// read a token and return true if its type equals the given token type
	bool				ExpectToken( int expectedType );
	bool				ExpectTokenString( const char * expectedText );
	bool				ExpectAnyToken();

						// returns true when the token is available
	bool				CheckTokenType( int	token );
	bool				CheckTokenString( const char * str );

						// returns the current line number
	int					GetLineNum() const;
	int					CurrentColumn() const;
	const Token &		CurrentToken() const;
						// returns the current token type
	int					TokenType() const;
	char				CurrentChar() const;
//	const char *		CurrentTokenString() const;
	char				CurrentTokenChar() const;

						// get the current location in a source file
	const Location &	 GetLocation() const;

						// skip the rest of the current line
	bool				SkipRestOfLine();

						// skip tokens until the given token string is read
	bool				SkipUntilString( const char * str );

						// skip the braced section
	bool				SkipBracedSection( bool parseFirstBrace = true );

						// parse a braced section into a string
	const char *		ParseBracedSection();

						// returns true if at the end of the file
	bool				EndOfFile() const;

						// returns the current filename
	const char *		GetFileName() const;

						// get current offset in current script
	uint				GetFileOffset() const;

	bool				HadError() const;

						// returns a pointer to the string buffer
	char *				GetBufferPtr();

						// returns the text of the current token
	const char *		Text() const;

						// reads all tokens from the source and constructs a token list
	TokenList &			BuildTokenList();
	TokenList &			GetTokenList();

						// needed to keep track of columns
	static void			SetTabWidth( int newTabWidth );
	static int			GetTabWidth();

public:
	static bool		IsWhitespace( char c );
	static bool		IsDigit( char c );
	static bool		IsHexDigit( char c );
	static bool		IsBinDigit( char c );
	static bool		IsOctDigit( char c );
	static bool		IsFirstIdChar( char c );
	static bool		IsIdChar( char c );
	static int		AsHexDigit( char c );

	static  bool	IsKeyword( const char * text );


	enum DEFAULT_SETTINGS {
		ID_LENGTH				= 32,		// number of significant letters in identifiers
		MAX_TOKEN_LENGTH		= 256,
		MAX_PARENTHESES_LEVEL   = 64,	// maximum number of syntactical nested parentheses
		MAX_PARSE_LEVEL			= 128	// maximum number of syntactical nested non-terminal
	};

	enum LEXER_FLAGS {
		DEFAULT_SETTINGS		= 0,
		HAD_ERRORS				= BIT( 0 ),
		COLLECT_DOC_INFORMATION = BIT( 1 )		// collect doc comment information
	};

	void		Print( const char * message, ... ) const;

public: // for saving current lexer state, doing infinite number of lookaheads and returning back.
	class LexerState {
		friend Lexer;

		const char *	p;
		const char *	prev;
		Token *			token;
	};

	void	SaveState( LexerState & state ) const;
	void	RestoreState( const LexerState & savedState );

private:
	const char *		m_source;		// ptr to the buffer containing the script
	int					m_size;			// size of the loaded script in bytes; != if some memory was alloced and script was copied
	bool				m_isLoaded;		// set when a script file is loaded from file or memory

	String				m_filename;

private:
	const char *		m_p;		// current position in the source file
	const char *		m_prev;		// previous position in the source file

	char		ReadChar();		// reads the next character, appends it to the string buffer
	char		SkipChar();		// reads the next char, doesn't keep it in the string buffer

	int					m_line;			// current line number in script
	int					m_lastLine;		// line before reading token
	int					m_column;		// current column
	int					m_lastColumn;	// column before reading token

	StringBuffer		m_buffer;

	Token *				m_token;	// current token in the token list
	TokenList			m_tokenList;

				// the main worker function which actually reads a token
	void		Scan( Token * token );
				// update the token's linenumber info
	void		Update( Token * token );

	u32					m_flags;
	bool				m_eof;	// is true when end of file has been reached

private:
	void	UpdateLexerState();		// called before reading next token to update previous lexer values
	void	ResetBuffer();

private:
	static unsigned char	charMap[ 256 ];
	static void			InitCharMap();

	static const int		cmOctal;
	static const int		cmHex;
	static const int		cmIdChar;

	c4::Hash< int > &		hashTable;
	static c4::Hash< int >	keywords;
	static void			InitKeywords();
public:
	void	SetHashTable( c4::Hash< int > & rHash ) { hashTable = rHash; }

private:

	static char			__time[ 10 ];	// "HH:MM:SS" for __TIME__ macro
	static char			__date[ 12 ];	// "MMM DD YYYY" for __DATE__ macro

	//static int		__version;		// compiler version
	//static char		__vendor[ 32 ];	// compiler vendor

	static int			tabWidth;

public:
	static unsigned char	IsOctal( unsigned char c );
	static unsigned char	IsHex( unsigned char c );
	static unsigned char	IsIdChar( unsigned char c );

private:
	void		Init();
					
				// reads a keyword or an identifier into the internal buffer
	bool		ReadName();

				// reads a numerical literal and returns its type
	int			ReadNumber();

				// reads a character constant ( ex.: 'A' ).
	int			ReadCharConstant();

				// reads '\n', '\t', etc.
	int			ReadEscapeSequence();

				// reads "string literals", "etc", ...
	int			ReadStringConstant();

				// reads a floating-point number
	int			ReadReal();

				// skip comments, whitespaces, etc..
	bool		ReadWhiteSpace();
	void		SkipComments();

private:
				// prints a warning message
	void		Warning( const char *message, ... ) const;

				// prints an error message
	void		Error( const char *message, ... );

				// prints an error message and stops the lexer
	void		FatalError( const char *message, ... );
};

INLINE bool Lexer::IsLoaded() const {
	return m_isLoaded;
}

INLINE bool Lexer::IsWhitespace( char c ) {
	return 0x20 == c || '\t' == c || '\n' == c || '\r' == c;
}

INLINE bool Lexer::IsDigit( char c ) {
	return c >= '0' && c <= '9';
}

INLINE bool Lexer::IsHexDigit( char c ) {
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

INLINE bool	Lexer::IsBinDigit( char c ) {
	return c == '0' || c == '1';
}

INLINE bool	Lexer::IsOctDigit( char c ) {
	return c >= '0' && c <= '7';
}

INLINE bool Lexer::IsFirstIdChar( char c ) {
	return ( c == '_' ) || ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' );
}

INLINE bool Lexer::IsIdChar( char c ) {
	return IsFirstIdChar( c ) || IsDigit( c );	
}

INLINE int Lexer::AsHexDigit( char c ) {
	if (c >= '0' && c <= '9') {
		return  c - '0';
	}
	if (c >= 'A' && c <= 'F') {
		return  c - 'A' + 10;
	}
	if (c >= 'a' && c <= 'f') {
		return  c - 'a' + 10;   
	}
	return  -1;
}

FORCEINLINE char Lexer::PeekChar() const {
	return *( m_p + 1 );
}

FORCEINLINE char Lexer::ReadChar() {
	++m_column;
	m_buffer += *m_p;
	return *( ++m_p );
}

FORCEINLINE char Lexer::PreviousChar() const {
	return *( m_p - 1 );
}

FORCEINLINE char Lexer::SkipChar() {
	++m_column;

	return *( ++m_p );
}

INLINE bool Lexer::EndOfFile() const {
	return !( *m_p );
}

INLINE bool Lexer::HadError() const {
	return HAD_ERRORS & m_flags;
}

INLINE int Lexer::TokenType() const {
	return m_token->m_type;
}

INLINE char Lexer::CurrentChar() const {
	return *m_p;
}

INLINE uint Lexer::GetFileOffset() const {
	return ( m_p - m_source );
}

INLINE const char * Lexer::GetFileName() const {
	return m_filename.c_str();
}

INLINE char * Lexer::GetBufferPtr() {
	return const_cast< char* >( m_buffer.ToChar() );
}

INLINE const Token & Lexer::CurrentToken() const {
	return *m_token;
}

INLINE TokenList & Lexer::GetTokenList() {
	return m_tokenList;
}

INLINE void Lexer::SetTabWidth( int newTabWidth ) {
	tabWidth = newTabWidth;
}

INLINE int Lexer::GetTabWidth() {
	return tabWidth;
}

}//end namespace c4

#endif // !__LEXER_H__