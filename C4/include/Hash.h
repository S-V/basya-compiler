#ifndef __HASH_H__
#define	__HASH_H__

namespace c4 {

namespace Hashing {

#define IS_POWER2( x )     ( ((x) & ((x) - 1)) == 0 )

#define DEFINEHASHSIZE		2048
#define FILE_HASH_SIZE		1024

INLINE char ToLower( char c ) {
	if ( c <= 'Z' && c >= 'A' ) {
		return ( c + ( 'a' - 'A' ) );
	}
	return c;
}

INLINE char ToUpper( char c ) {
	if ( c >= 'a' && c <= 'z' ) {
		return ( c - ( 'a' - 'A' ) );
	}
	return c;
}

INLINE int GetHash( const char *str ) {
	int hash = 0;
	for ( int i = 0; *str != '\0'; i++ ) {
		hash += ( *str++ ) * ( i + 119 );
	}
	return hash;
}

INLINE uint GetHash( ulong key ) {
	if ( sizeof( ulong ) > sizeof( uint ) ) {
		return uint( ( key >> ( 8 * sizeof( uint ) - 1 ) ) ^ key );
	} else {
		return uint( key );
	}
}
/*
template< class T > 
INLINE uint GetHash( const T *key )
{
	if ( sizeof( const T* ) > sizeof( uint ) ) {
		return GetHash( reinterpret_cast< uint64 >( key ) );
	} else {
		return uint( reinterpret_cast< ulong >( key ) );
	}
}
*/
INLINE int NameHash( const char *name ) {
	int hash, i;

	hash = 0;
	for ( i = 0; name[i]; i++ ) {
		hash += name[i] * (119 + i);
	}
	hash = (hash ^ (hash >> 10) ^ (hash >> 20)) & (DEFINEHASHSIZE - 1);
	return hash;
}

INLINE int FileNameHash( const char *name ) {
	char letter;
	long hash = 0;
	int i = 0;
	while( name[i] != '\0' ) 
	{
		letter = tolower( name[i] );
		if ( letter == '.' ) {
			break;				// don't include extension
		}
		if ( letter =='\\' ) {
			letter = '/';
		}


		hash += (long)( letter ) * ( i + 119 );
		i++;
	}
	hash &= (FILE_HASH_SIZE-1);
	return hash;
}


// hashpjw
#define PRIME	211

inline int	hashpjw( const char *s )
{
	unsigned h = 0, g;
	for ( const char *p = s; *p; p++ ) {
		if ( g = (h = (h << 4) + *p) & 0xF0000000 ) {
			h ^= g >> 24 ^ g;
		}
	}
	return h % PRIME;
}

#undef PRIME

#undef DEFINEHASHSIZE
#undef FILE_HASH_SIZE

}//end namespace Hashing 

/*
=========================================================
	A simple hash table, suitable for storing symbols.
	Modelled on Id Software's container.
=========================================================
*/
template< class T >
class Hash {
public:
					Hash( int newtablesize = 256 );
					Hash( const Hash< T > &map );
					~Hash( void );

					// Returns a default-constructed value if the hash has no item with the given key.
    T &				operator [] ( const char *key );
	const T			operator [] ( const char *key ) const;

					// Returns the value associated with the key key.
					// If the hash contains no item with key key, the function returns a default-constructed value.
	T				Value( const char *key, T defaultValue );

					// Adds a ( key, value ) pair. Returns false, if such a pair already exists.
	bool			Add( const char *key, T &value );
	void			Insert( const char *key, T &value );
	
	bool			Contains( const char *key );

					// returns total size of allocated memory
	size_t			Allocated( void ) const;

					// returns total size of allocated memory including size of hash table T
	size_t			Size( void ) const;

	void			Set( const char *key, T &value );
	bool			Get( const char *key, T **value = NULL ) const;
	bool			Remove( const char *key );

	void			Clear( void );
	void			DeleteContents( void );

					// the entire contents can be iterated over, but note that the
					// exact index for a given element may change when new elements are added
	int				Num( void ) const;
	T *				GetIndex( int index, const c4::String **key = NULL ) const; //HUMANHEAD mdl:  Added ability to retrieve key for saving hashtables.

	int				GetSpread( void ) const;

private:
	struct hashnode_s {
		c4::String	key;
		T			value;
		hashnode_s *next;

		hashnode_s( const c4::String &k, T v, hashnode_s *n ) : key( k ), value( v ), next( n ) {};
		hashnode_s( const char *k, T v, hashnode_s *n ) : key( k ), value( v ), next( n ) {};
	};

	hashnode_s **	heads;

	int				tablesize;
	int				numentries;
	int				tablesizemask;

	int				GetHash( const char *key ) const;

	INLINE
	hashnode_s * CreateNode( const char *key, const T &value ) {
		hashnode_s *node, **nextPtr;
		int hash, s;

		hash = GetHash( key );
		for( nextPtr = &(heads[hash]), node = *nextPtr; node != NULL; nextPtr = &(node->next), node = *nextPtr ) {
			s = strcmp( node->key.c_str(), key );

			if ( s == 0 ) {
				node->value = value;
				return 0;
			}
			if ( s > 0 ) {
				break;
			}
		}

		numentries++;

		*nextPtr = new hashnode_s( key, value, heads[ hash ] );
		(*nextPtr)->next = node;

		return *nextPtr;
	}

	INLINE 
	hashnode_s ** FindNode( const char *key ) const {
		int hash = GetHash( key ), s;

		for( hashnode_s *node = heads[ hash ]; node; node = node->next ) {
			s = strcmp( node->key.c_str(), key );
			if ( s == 0 ) {
				return &node;
			}
			if ( s > 0 ) {
				break;
			}
		}
		return 0;
	}
};

/*
================
Hash< T >::Hash
================
*/
template< class T >
INLINE Hash< T >::Hash( int newtablesize ) {

	assert( IS_POWER2( newtablesize ) );

	tablesize = newtablesize;
	assert( tablesize > 0 );

	heads = new hashnode_s *[ tablesize ];
	memset( heads, 0, sizeof( *heads ) * tablesize );

	numentries		= 0;

	tablesizemask = tablesize - 1;
}

/*
================
Hash< T >::Hash
================
*/
template< class T >
INLINE Hash< T >::Hash( const Hash< T > &map ) {
	int			i;
	hashnode_s	*node;
	hashnode_s	**prev;

	assert( map.tablesize > 0 );

	tablesize		= map.tablesize;
	heads			= new hashnode_s *[ tablesize ];
	numentries		= map.numentries;
	tablesizemask	= map.tablesizemask;

	for( i = 0; i < tablesize; i++ ) {
		if ( !map.heads[ i ] ) {
			heads[ i ] = NULL;
			continue;
		}

		prev = &heads[ i ];
		for( node = map.heads[ i ]; node != NULL; node = node->next ) {
			*prev = new hashnode_s( node->key, node->value, NULL );
			prev = &( *prev )->next;
		}
	}
}

/*
================
Hash< T >::~Hash< T >
================
*/
template< class T >
INLINE Hash< T >::~Hash( void ) {
	Clear();
	delete[] heads;
}

/*
================
Hash< T >::Allocated
================
*/
template< class T >
INLINE size_t Hash< T >::Allocated( void ) const {
	return sizeof( heads ) * tablesize + sizeof( *heads ) * numentries;
}

/*
================
Hash< T >::Size
================
*/
template< class T >
INLINE size_t Hash< T >::Size( void ) const {
	return sizeof( Hash< T > ) + sizeof( heads ) * tablesize + sizeof( *heads ) * numentries;
}

/*
================
Hash< T >::GetHash
================
*/
template< class T >
INLINE int Hash< T >::GetHash( const char *key ) const {
	return ( String::Hash( key ) & tablesizemask );
}

/*
================
Hash< T >::Set
================
*/
template< class T >
INLINE void Hash< T >::Set( const char *key, T &value ) {
	hashnode_s *node, **nextPtr;
	int hash, s;

	hash = GetHash( key );
	for( nextPtr = &(heads[hash]), node = *nextPtr; node != NULL; nextPtr = &(node->next), node = *nextPtr ) {
		s = strcmp( node->key.c_str(), key );
		if ( s == 0 ) {
			node->value = value;
			return;
		}
		if ( s > 0 ) {
			break;
		}
	}

	numentries++;

	*nextPtr = new hashnode_s( key, value, heads[ hash ] );
	(*nextPtr)->next = node;
}

/*
================
Hash< T >::Get
================
*/
template< class T >
INLINE bool Hash< T >::Get( const char *key, T **value ) const {
	hashnode_s *node;
	int hash, s;

	hash = GetHash( key );
	for( node = heads[ hash ]; node != NULL; node = node->next ) {
		s = strcmp( node->key.c_str(), key );
		if ( s == 0 ) {
			if ( value ) {
				*value = &node->value;
			}
			return true;
		}
		if ( s > 0 ) {
			break;
		}
	}

	if ( value ) {
		*value = NULL;
	}

	return false;
}

/*
================
Hash< T >::GetIndex

the entire contents can be iterated over, but note that the
exact index for a given element may change when new elements are added
================
*/
template< class T >
INLINE T *Hash< T >::GetIndex( int index, const c4::String **key ) const {
	hashnode_s	*node;
	int			count;
	int			i;

	if ( ( index < 0 ) || ( index > numentries ) ) {
		assert( 0 );
		return NULL;
	}

	count = 0;
	for( i = 0; i < tablesize; i++ ) {
		for( node = heads[ i ]; node != NULL; node = node->next ) {
			if ( count == index ) {
				//HUMANHEAD mdl
				if ( key ) {
					*key = &node->key;
				}
				//HUMANHEAD END
				return &node->value;
			}
			count++;
		}
	}

	//HUMANHEAD mdl
	if ( key ) {
		*key = NULL;
	}
	//HUMANHEAD END
	return NULL;
}

/*
================
Hash< T >::Remove
================
*/
template< class T >
INLINE bool Hash< T >::Remove( const char *key ) {
	hashnode_s	**head;
	hashnode_s	*node;
	hashnode_s	*prev;
	int			hash;

	hash = GetHash( key );
	head = &heads[ hash ];
	if ( *head ) {
		for( prev = NULL, node = *head; node != NULL; prev = node, node = node->next ) {
			if ( node->key == key ) {
				if ( prev ) {
					prev->next = node->next;
				} else {
					*head = node->next;
				}

				delete node;
				numentries--;
				return true;
			}
		}
	}

	return false;
}

/*
================
Hash< T >::Clear
================
*/
template< class T >
INLINE void Hash< T >::Clear( void ) {
	int			i;
	hashnode_s	*node;
	hashnode_s	*next;

	for( i = 0; i < tablesize; i++ ) {
		next = heads[ i ];
		while( next != NULL ) {
			node = next;
			next = next->next;
			delete node;
		}

		heads[ i ] = NULL;
	}

	numentries = 0;
}

/*
================
Hash< T >::DeleteContents
================
*/
template< class T >
INLINE void Hash< T >::DeleteContents( void ) {
	int			i;
	hashnode_s	*node;
	hashnode_s	*next;

	for( i = 0; i < tablesize; i++ ) {
		next = heads[ i ];
		while( next != NULL ) {
			node = next;
			next = next->next;
			delete node->value;
			delete node;
		}

		heads[ i ] = NULL;
	}

	numentries = 0;
}

/*
================
Hash< T >::Num
================
*/
template< class T >
INLINE int Hash< T >::Num( void ) const {
	return numentries;
}

/*
================
Hash< T >::GetSpread
================
*/
template< class T >
INLINE int Hash< T >::GetSpread( void ) const {
	int i, average, error, e;
	hashnode_s	*node;

	// if no items in hash
	if ( !numentries ) {
		return 100;
	}
	average = numentries / tablesize;
	error = 0;
	for ( i = 0; i < tablesize; i++ ) {
		numItems = 0;
		for( node = heads[ i ]; node != NULL; node = node->next ) {
			numItems++;
		}
		e = abs( numItems - average );
		if ( e > 1 ) {
			error += e - 1;
		}
	}
	return 100 - (error * 100 / numentries);
}

/*
================
T&  Hash< T >::operator [] ( const char *key )
================
*/
template< class T >
INLINE T& Hash< T >::operator [] ( const char *key ) 
{
	hashnode_s ** node = FindNode( key );
	if ( !node ) {
		return CreateNode( key, T() )->value;
	}
	return (*node)->value;
}

/*
================
T  Hash< T >::operator [] ( const char *key ) const
================
*/
template< class T >
INLINE const T Hash< T >::operator [] ( const char *key ) const
{
	hashnode_s ** node = FindNode( key );
	if ( !node ) {
		return T();
	}
	return (*node)->value;
}

/*
================
   Returns the value associated with the key key.
   If the hash contains no item with the key key, the function returns a default-constructed value.
================
*/
template< class T >
INLINE T Hash< T >::Value( const char *key, T defaultValue ) {
	int	*p = 0;

	if ( !Get( key, &p ) ) {
		Set( key, defaultValue );
		return defaultValue;
	} else {
		return *p;
	}
}

/*
================
   Adds a ( key, value ) pair. Returns false, if such a pair already exists.
================
*/
template< class T >
INLINE bool Hash< T >::Add( const char *key, T &value ) {
	if ( Get( key ) ) {
		return false;
	}
	Set( key,  value );

	return true;
}

template< class T >
INLINE void Hash< T >::Insert( const char *key, T &value ) {
	Set( key,  value );
}

template< class T >
INLINE bool Hash< T >::Contains( const char *key ) {
	return Get( key );
}

}//end namespace c4

#endif // !__HASH_H__