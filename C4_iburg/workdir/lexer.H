%{
	#include "parser.h"
	#include "../iburg.h"

	unsigned	g_lineNo = 1;
	
	//#define	YYLMAX	8192
	#define	YYLMAX		98192
%}

%array

ID		        [_a-zA-Z][_a-zA-Z0-9]*
NUMBER			[0-9]+

%%

{ID}			{ return IDENTIFIER; }
{NUMBER}		{ return INTEGER; }

%start			{ return START; }
%operator		{ return TERMINAL; }

%ctor			{ return CONSTRUCTOR; }
%dtor			{ return DESTRUCTOR; }

"("				{ return '('; }
","				{ return ','; }
")"				{ return ')'; }
"["				{ return '['; }
"]"				{ return ']'; }
":"				{ return ':'; }
";"				{ return ';'; }
"%"				{ return '%'; }
"%%"			{ return PPERCENT; }

"#"				{ return '#'; }
"?"				{ return '?'; }
"!"				{ return '!'; }
[\n]			{ ++g_lineNo; return '\n'; }

[ \t\f]+		{ /* eat up whitespace */ }

"//"			{ /* printf( "Skipping a one-line comment!\n" ); */
					char  c;
					
					while ( 1 )
					{
						c = input();
						
						if ( EOF == c ) {
							//printf( "EOF in one-line comment!\n" );
							return 0;
						}
						if ( '\n' == c ) {
							++g_lineNo;
							return '\n';
						}
					}
				}

"/*"			{
					char  c; // temp. char
					
					//printf( "Skipping a multi-line comment!\n" );

					// multi-line comments /* ... */
					// these can span multiple lines, but do not nest
					while ( 1 )
					{
						c = input();
						
						if ( EOF == c ) {
							printf( "unterminated comment, EOF reached!\n" );
							return 0;
						}
						if ( '\n' == c ) {
							++g_lineNo;
						}
						else if ( '*' == c ) {
							c = input();
							if ( '/' == c ) {
								break;
							}
							if ( EOF == c ) {
								printf( "unterminated comment, EOF reached!\n" );
								return 0;
							}
							if ( '\n' == c ) {
								++g_lineNo;
							}
							continue;
						}
					
					}//while ( true )
				}

"{"		{	
			int  c;  // tmp. char
			int	 brace_count = 1;
			char *	p = yytext;
			*p++ = '{';
			while ( 1 )
			{
				c = input();
				*p++ = c;
				if ( c == EOF ) {
					Error( "EOF in '{'...'}'!\n" );
					return 0;
				}
				if ( '\n' == c ) {
					++g_lineNo;
				}
				if ( c == '{' ) {
					++brace_count;
				}
				if ( c == '}' ) {
					--brace_count;
				}
				if ( brace_count == 0 ) {
					break;
				}
			}//while ( true )		
			*p = '\0';
			return CODE;
		}

"<"		{
			int  c;
			char *	p = yytext;
			while ( 1 ) 
			{
				c = input();	
				if ( c == EOF ) {
					Error( "EOF in cost < ...>!\n" );
				}
				if ( '\n' == c ) {
					++g_lineNo;
				}
				if ( c == '>' ) {
					break;
				}
				*p = c;
				++p;
			}//while ( true )		
			*p = '\0';
			return COST;
		}
		
"\""	{
			int  c;
			char *	p = yytext;
			*p++ = '\"';
			while ( 1 ) 
			{
				c = input();
				if ( c == EOF ) {
					Error( "EOF in assembler template \"...\"!\n" );
				}
				if ( '\n' == c ) {
					++g_lineNo;
				}
				if ( c == '\"' ) {
					*p++ = c;
					break;
				}
				*p = c;
				++p;
			}//while ( true )		
			*p = '\0';
			return TEMPLATE;
		}
		