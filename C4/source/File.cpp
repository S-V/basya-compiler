#include "stdafx.h"
#pragma hdrstop
#include <include/c4.h>

namespace c4 {

File::File()
	: m_pFile( NULL )
{
	m_buffer[ 0 ] = '\n';
}

File::~File()
{
}

// opens a file for writing
//
bool File::Open( const char* path ) {
	m_pFile = fopen( path, "w" );
	if ( ! m_pFile ) {
		c4::common->Error( "cannot open file '%s'.", path );
		return false;
	}
	return true;
}

void File::Close() {
	if ( m_pFile ) {
		fclose( m_pFile );
	}
}

void File::Write( char c ) {
	char  tmp = c;
	fwrite( & tmp, sizeof( char ), 1, m_pFile );
}

void File::Write( const void * data, uint bytes ) {
	fwrite( data, sizeof( byte ), bytes, m_pFile );
}

void File::Write( const char * str ) {
	fwrite( str, sizeof( char ), strlen( str ), m_pFile );
}

void File::Printf( const char * format, ... ) {
	va_list   argptr;

	va_start( argptr, format );
	vsprintf_s( m_buffer, BUFFER_SIZE, format, argptr );
	va_end( argptr );

	Write( m_buffer );
}

void File::WriteNewLine() {
	char c = '\n';
	fwrite( &c, sizeof( char ), 1, m_pFile );
}

void File::WriteByte( byte value ) {
	byte  tmp = value;
	fwrite( & tmp, sizeof( byte ), 1, m_pFile );
}

void File::WriteWord( word	value ) {
}

void File::WriteDWord( dword value ) {
}

void File::WriteQWord( qword value ) {
}

void File::Print( const char * format, ... ) {
	va_list   argptr;

	va_start( argptr, format );
	int length = vsprintf_s( m_buffer, BUFFER_SIZE, format, argptr );
	va_end( argptr );

	Write( m_buffer, length );
}

//void File::Align( uint size ) {
//}

}//end namespace c4