#ifndef __UI_H__
#define __UI_H__

namespace c4 {

//======================================
//	class UINode
//		is needed to display parse trees in QTreeView
//======================================
//
class UINode {
public:
					UINode( UINode* parent = 0 );
	virtual			~UINode() {}
	
	virtual const char *	uiToString() const = 0;

							// for displaying object's detailed info
	virtual const char *	uiGetInfo() const;

							// returns -1 if the specified node was not found
	virtual int				uiIndexOf( const UINode * child ) const = 0;

							// returns the child of this node at the specified index
	virtual UINode *		uiChildAt( int index ) = 0;

	virtual int				uiNumChildren() const = 0;

	UINode *				uiGetParent();
	void					uiSetParent( UINode * node );

private:
	UINode *			ui_parent; // needed for tree view model

public:
	void *		uiPointer;	// user-manageable data
};

INLINE UINode::UINode( UINode* parent /* =0 */ )
	: ui_parent( parent )
{
}

INLINE UINode * UINode::uiGetParent() {
	return ui_parent;
}

INLINE void UINode::uiSetParent( UINode * node ) {
	ui_parent = node;
}

INLINE const char * UINode::uiGetInfo() const {
	return "This is a tree node for displaying ASTs in a tree view";
}

}//end namespace c4

#endif // !__UI_H__