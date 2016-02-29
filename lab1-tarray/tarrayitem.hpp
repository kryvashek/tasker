#ifndef _TARRAYITEM_H_
#define _TARRAYITEM_H_

template< class ValueType >
class tArrayItem
	{
	private:

		ValueType				_value;
		tArrayItem< ValueType >	* _prev,
								* _next;

	public:

		// default constructor
		tArrayItem( void );

		// copy constructor
		tArrayItem( const tArrayItem< ValueType > & source );

		// constructor by storing value
		tArrayItem( const ValueType & value, const tArrayItem< ValueType > * prev, const tArrayItem< ValueType > * next );

		// assignment operator
		tArrayItem< ValueType > & operator=( const tArrayItem< ValueType > & source );

		// method for accessing item value
		inline ValueType & Get( void );

		// getting next entry
		inline tArrayItem< ValueType > * Next( void ) const;

		// getting previous entry
		inline tArrayItem< ValueType > * Previous( void ) const;

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
tArrayItem< ValueType >::tArrayItem( const tArrayItem< ValueType > & source ) :
	_value( source._value ),
	_prev( this ),
	_next( this )
	{}

// ================================================ constructor by storing value
template< class ValueType >
tArrayItem< ValueType >::tArrayItem( const ValueType & value, const tArrayItem< ValueType > * prev, const tArrayItem< ValueType > * next ) :
	_value( value ),
	_prev( const_cast< tArrayItem< ValueType > * >( prev ) ),
	_next( const_cast< tArrayItem< ValueType > * >( next ) )
	{
	_prev->_next = this;
	_next->_prev = this;
	}

// ================================================== assignment opearator ( = )
template< class ValueType >
tArrayItem< ValueType > & tArrayItem< ValueType >::operator=( const tArrayItem< ValueType > & source )
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
inline tArrayItem< ValueType > * tArrayItem< ValueType >::Next( void ) const
	{
	return _next;
	}

// ====================================================== getting previous entry
template< class ValueType >
inline tArrayItem< ValueType > * tArrayItem< ValueType >::Previous( void ) const
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