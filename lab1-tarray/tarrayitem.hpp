#ifndef _TARRAYITEM_H_
#define _TARRAYITEM_H_

template< class ValueType >
class tArrayItem
	{
	public:

	typedef ValueType *					ValPtr;
	typedef ValueType &					ValRef;
	typedef tArrayItem< ValueType > *	ItemPtr;
	typedef tArrayItem< ValueType > &	ItemRef;

	public:  // this entries will be public only for debug reasons

	ValueType	_value;
	ItemPtr		_prev,
				_next;

	public:

	// default constructor
	tArrayItem( void );

	// constructor by storing value
	tArrayItem( const ValRef value, const ItemPtr prev, const ItemPtr next );

	// copy constructor
	tArrayItem( const ItemRef source );

	// assignment operator
	const tArrayItem< ValueType > & operator=( const ItemRef source );

	// comparison operator
	const bool operator==( const ItemRef source );

	// uncomparison opearator ( != )
	inline const bool operator!=( const ItemRef source );

	// destructor
	~tArrayItem( void );
	};

// ========================================================= default constructor
template< class ValueType >
tArrayItem< ValueType >::tArrayItem( void ) :
	_value( reinterpret_cast< ValueType >( 0 ) ),
	_prev( this ),
	_next( this )
	{}

// ================================================ constructor by storing value
template< class ValueType >
tArrayItem< ValueType >::tArrayItem( const ValRef value, const ItemPtr prev, const ItemPtr next ) :
	_value( value ),
	_prev( prev ),
	_next( next )
	{}

// ============================================================ copy constructor
template< class ValueType >
tArrayItem< ValueType >::tArrayItem( const ItemRef source )
	{
	this->tArrayItem( source._value, source._prev, source._next );
	}

// ================================================== assignment opearator ( = )
template< class ValueType >
const tArrayItem< ValueType > & tArrayItem< ValueType >::operator=( const ItemRef source )
	{
	this->_value = source._value;
	return *this;
	}

// ================================================= comparison opearator ( == )
template< class ValueType >
const bool tArrayItem< ValueType >::operator==( const ItemRef source )
	{
	return this->_value == source._value;
	}

// =============================================== uncomparison opearator ( != )
template< class ValueType >
inline const bool tArrayItem< ValueType >::operator!=( const ItemRef source )
	{
	return !( operator==( source ) );
	}

// ================================================================== destructor
template< class ValueType >
tArrayItem< ValueType >::~tArrayItem( void )
	{
	_next->_prev = this->_prev;
	_prev->_next = this->_next;
	}

#endif // _TARRAYITEM_H_