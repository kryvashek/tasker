#ifndef _TARRAY_H_
#define _TARRAY_H_

#include <iostream>

using namespace std;

typedef int	count_type;

template< class item_type >
class t_array_entry
	{
	public:

	typedef item_type &							val_ref;
	typedef const item_type &					const_val_ref;
	typedef t_array_entry< item_type > &		item_ref;
	typedef const t_array_entry< item_type > &	const_item_ref;

	public:  // this entries will be public only for debug reasons

	val_ref		item;
	item_ref	prev,
				next;
	bool		self;

	public:

	// default constructor
	t_array_entry( void );

	// constructor by storing value
	t_array_entry( const_val_ref value, const_item_ref previous, const_item_ref next );

	// copy constructor
	t_array_entry( const_item_ref source );

	// assignment operator
	//const_item_ref operator=( const_item_ref source );

	// destructor
	~t_array_entry( void );
	};

template< class item_type >
class t_array
	{
	public: // this entries will be public only for debug reasons

	const t_array_entry< item_type >	point;
	count_type							count;
	};


// ========================================================= default constructor
template< class item_type >
t_array_entry< item_type >::t_array_entry( void ) :
	item( *( new item_type ) ),
	prev( *this ),
	next( *this ),
	self( true )
	{}

// ================================================ constructor by storing value
template< class item_type >
t_array_entry< item_type >::t_array_entry( const_val_ref value, const_item_ref previous, const_item_ref next ) :
	item( value ),
	prev( previous ),
	next( next ),
	self( false )
	{}

// ============================================================ copy constructor
template< class item_type >
t_array_entry< item_type >::t_array_entry( const_item_ref source )
	{
	this->t_array_entry( source.item, source.prev, source.next );
	}

// ================================================== assignment opearator ( = )
/*template< class item_type >
const t_array_entry< item_type > & t_array_entry< item_type >::operator=( const_item_ref source )
	{
	if( self )
		delete &item;

	this->item = source.item;
	this->prev = source.prev;
	this->next = source.next;
	}*/


// ================================================================== destructor
template< class item_type >
t_array_entry< item_type >::~t_array_entry( void )
	{
	if( prev != *this && next != *this )
		{
		cout << "prev.next before is " << &( prev.next ) << endl;
		cout << "this before is " << this << endl;
		prev.next = ( item_ref )*this;//->next;
		cout << "prev.next after is " << &( prev.next ) << endl;
		//next.prev = this->prev;
		}

	if( self )
		delete &item;
	}

#endif /* _TARRAY_H_ */