#ifndef __FILE_H__
#define __FILE_H__

namespace c4 {

// TODO: this code is a mess. Clean it up later.

class IFile {
public:
	virtual			~IFile() {}

	virtual void	Open( const char* filename = 0 ) = 0;
	virtual void	Close() = 0;

	virtual void	Write( char c ) = 0;
	virtual void	Write( const char* format, ... ) = 0;
};

class MemoryFile : public IFile {
public:
				MemoryFile();
				~MemoryFile();

	// overrides

			// the given file name doesn't matter
	void	Open( const char* filename = 0 );
	void	Close();

	void	Write( char c );
	void	Write( const char* format, ... );

	void	FlushToDisk( const char* path );
};

//-------------------------------------------------------
//	class File
//		for writing into files
//-------------------------------------------------------
class File {
public:
					File();
					~File();

	bool		Open( const char* path );
	void		Close();

	void		Write( char c );
	void		Write( const void * data, uint bytes );
	void		Write( const char * str );
	void		Printf( const char * format, ... );
	void		WriteNewLine();	// '\n'
	void		WriteByte( byte value );
	void		WriteWord( word	value );
	void		WriteDWord( dword value );
	void		WriteQWord( qword value );

	void		Print( const char * format, ... );
//	void		Align( uint size );

protected:
	FILE *		m_pFile;

	enum { BUFFER_SIZE = 1024 };
	char	m_buffer[ BUFFER_SIZE ];
};

}//end namespace c4 

#endif // !__FILE_H__