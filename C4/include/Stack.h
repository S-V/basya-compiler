#ifndef __SIMPLE_STACK_H__
#define __SIMPLE_STACK_H__

namespace c4 {

//----------------------------------------
//  A very simple array-based stack.
//----------------------------------------
template< typename type, int maxItems >
class Stack {

	type	items[ maxItems ];
	int		top;

public:
					Stack( void );
					~Stack( void );

	type &			operator [] ( int index );
	const type &    operator [] ( int index ) const;

	type &			Top();
	const type &	Top() const;

	bool			IsEmpty() const;
	bool			IsFull() const;

					// add item to stack
	bool			Push( const type & item );

					// pop top into item
	bool			Pop( type & item );

	type 			Pop( void );

					// reset the top to a new value
	void			Reset( const int new_index = 0 );

					// get the number of stored elements
	int				Num() const ;
};

template< typename type, int maxItems >
INLINE Stack< type, maxItems >::Stack( void ) : top( 0 ) {
}

template< typename type, int maxItems >
INLINE Stack< type, maxItems >::~Stack( void ) {
}

template< typename type, int maxItems >
INLINE bool Stack< type, maxItems >::IsEmpty() const {
	return  ( top == 0 );
}

template< typename type, int maxItems >
INLINE bool Stack< type, maxItems >::IsFull() const {
	return  ( top == maxItems );
}

template< typename type, int maxItems >
INLINE type & Stack< type, maxItems >::operator [] ( const int index ) {
	BREAK_IF( index < 0 && index >= maxItems );
	return  items[ index ];
}

template< typename type, int maxItems >
INLINE const type & Stack< type, maxItems >::operator [] ( const int index ) const {
	BREAK_IF( index < 0 && index >= maxItems );
	return  items[ index ];
}

template< typename type, int maxItems >
INLINE bool Stack< type, maxItems >::Push( const type & item ) {  
	BREAK_IF( top >= maxItems );
	if ( top < maxItems ) {
		items[ top++ ] = item;
		return	true;
	}
	return false;
}

template< typename type, int maxItems >
INLINE bool Stack< type, maxItems >::Pop( type & item ) {
	BREAK_IF( top >= maxItems );
	if ( top > 0 ) {
		item = items[ --top ];
		return  true;
	}
	return false;
}

template< typename type, int maxItems >
INLINE void Stack< type, maxItems >::Reset( const int new_index /* =0 */ ) {
	BREAK_IF( new_index < 0 || new_index >= maxItems );
	top = new_index;
}

template< typename type, int maxItems >
INLINE int Stack< type, maxItems >::Num() const {
	return top;
}

template< typename type, int maxItems >
INLINE type & Stack< type, maxItems >::Top() {
	return items[ top - 1 ];
}

template< typename type, int maxItems >
INLINE const type &	Stack< type, maxItems >::Top() const {
	return items[ top - 1 ];
}

template< typename type, int maxItems >
INLINE type Stack< type, maxItems >::Pop( void ) {
	BREAK_IF( top < 0 );
	if ( top > 0 ) {
		return items[ --top ];
	}
	return type(); // return default-constructed value
}

}//end namespace c4

#endif // !__SIMPLE_STACK_H__