#ifndef __ICOMMON_H__
#define __ICOMMON_H__

namespace c4 {

/*
===========================
	class ICommon

	used for displaying messages and reporting errors;
	the pointer c4::common must be initialized first.
===========================
*/

class ICommon {
public:
	virtual			~ICommon() {}

					// prints a message
	virtual void	Print( const char * format, ... ) = 0;

					// prints a warning message
	virtual void	Warning( const char * format, ... ) = 0;

					// prints an error message and exits the program
	virtual void	Error( const char * format, ... ) = 0;

					// fatal error - terminate execution immediately; does not return;
					// may write something into the error log
	virtual void	Fatal( const char * format, ... ) = 0;

					// removes all messages and clears the console
	virtual void	Clear() = 0;

	virtual void	DebugPrint( const char * format, ... ) = 0;
};

extern ICommon *	common;

}//end namespace c4

#endif // !__ICOMMON_H__