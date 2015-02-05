#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__

namespace c4 {
	
// identifiers store fileinfo + text

class Identifier : public AST_Node {
public:
				Identifier( const char * text );
				~Identifier();

					// raw text of this identifier
	const char *	Text() const;
 
	// overrides
	bool			Equals( const Object * other ) const;
	hash_t			GetHash() const;
	int				Compare( const Object * other ) const;
	int				GetId() const;

	const char *	ToChars() const;

	const char *	uiToString() const;
	int				uiNumChildren() const;
	UINode *		uiChildAt( int index );
	int				uiIndexOf( const UINode *child ) const;

protected:
	String			m_text;
};

INLINE const char *	Identifier::uiToString() const {
	return ToChars();
}

}//end namespace c4 

#endif // !__IDENTIFIER_H__