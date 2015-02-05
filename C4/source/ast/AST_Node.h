/*
======================================
	AST_Node, the base class for all ast nodes.
======================================
*/
#ifndef __AST_NODE_H__
#define __AST_NODE_H__

namespace c4 {

class CodeGenerator;

//----------------------------------------------------------------------
// Common interface for all nodes in an abstract syntax tree.
// AST_Node is the base class for all ast nodes.
//----------------------------------------------------------------------
//
class AST_Node : public Object, public UINode {
public:
			DECLARE_TYPE( AST_Node );

	virtual				~AST_Node();

	virtual int			GetId() const;

	//-- code generation ---------------------------
	virtual void		Generate( CodeGenerator * cg );

	//-- Visitor -----------------------------------
	virtual void		Accept( ITreeVisitor * visitor );

	//-- error handling ----------------------------
	void			Print( const char * format, ... );
	void			Error( const char * format, ... );
	void			Warning( const char * format, ... );

						// for tracking position in a source file
	void				SetLocation( const Location & loc );
	const Location &	GetLocation() const;

	//-- ui ---------------------------------------------------------------

	override const char *	uiToString() const;
	virtual void			UpdateUIinfo();
	override const char *	uiGetInfo() const;	// for displaying object's detailed info

protected:
	String		m_detailedInfo;

public:
	Location		location;

	// debugging
	static ulong	GetNumNodes();
	static void		PrintStatistics();

protected:
					AST_Node( UINode* parent = 0 );
					AST_Node( const Location &location, UINode* parent = 0 );
private:
	static ulong	numASTnodes;
};

INLINE void AST_Node::SetLocation( const Location & loc ) {
	location = loc;
}

INLINE const Location & AST_Node::GetLocation() const {
	return location;
}

//======================================================================
#define CLASS_PROTOTYPE( nameOfClass )		friend class FlowchartBuilder;	\
											friend class Machine;			\
											DECLARE_TYPE( nameOfClass )
					
}//end namespace c4

#endif // !__AST_NODE_H__