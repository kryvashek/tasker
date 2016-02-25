#ifndef _TARRAYITEM_H_
#define _TARRAYITEM_H_

template< class ValueType >
class tArrayItem
	{
	public:

		typedef ValueType &				tValRef;
		typedef tArrayItem< ValueType >	tItem;

	private:  // this entries will be public only for debug reasons

		ValueType	_value;
		tItem 		* _prev,
					* _next;

	public:

		// default constructor
		tArrayItem( void );

		// copy constructor
		tArrayItem( const tItem & source );

		// constructor by storing value
		tArrayItem( const tValRef value, const tItem * prev, const tItem * next );

		// assignment operator
		tArrayItem< ValueType > & operator=( const tItem & source );

		// method for accessing item value
		inline ValueType & Get( void );

		// getting next entry
		inline tItem * Next( void );

		// getting previous entry
		inline tItem * Previous( void );

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

// ============================================================ copy constructor
template< class ValueType >
tArrayItem< ValueType >::tArrayItem( const tItem & source ) :
	_value( source._value ),
	_prev( this ),
	_next( this )
	{}

// ================================================ constructor by storing value
template< class ValueType >
tArrayItem< ValueType >::tArrayItem( const tValRef value, const tItem * prev, const tItem * next ) :
	_value( value ),
	_prev( prev ),
	_next( next )
	{
	_prev->_next = this;
	_next->_prev = this;
	}

// ================================================== assignment opearator ( = )
template< class ValueType >
tArrayItem< ValueType > & tArrayItem< ValueType >::operator=( const tItem & source )
	{
	this->_value = source._value;
	return *this;
	}

// ============================================= method for accessing item value
template< class ValueType >
inline ValueType & tArrayItem< ValueType >::Get( void )
	{
	return _value;
	}

// ========================================================== getting next entry
template< class ValueType >
inline tArrayItem< ValueType > * tArrayItem< ValueType >::Next( void )
	{
	return _next;
	}

// ====================================================== getting previous entry
template< class ValueType >
inline tArrayItem< ValueType > * tArrayItem< ValueType >::Previous( void )
	{
	return _prev;
	}

// ================================================================== destructor
template< class ValueType >
tArrayItem< ValueType >::~tArrayItem( void )
	{
	_next->_prev = this->_prev;
	_prev->_next = this->_next;
	}

#endif // _TARRAYITEM_H_