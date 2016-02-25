#ifndef _TARRAY_H_
#define _TARRAY_H_

#include <iostream>
#include "tarrayitem.hpp"

using	std::ostream;
using	std::istream;

template< class ValueType, typename CountType = int >
class tArray
	{
	public:

	typedef ValueType *							ValPtr;
	typedef ValueType &							ValRef;
	typedef tArray< ValueType, CountType > *	ArrPtr;
	typedef tArray< ValueType, CountType > &	ArrRef;
	typedef tArrayItem< ValueType > *			ItemPtr;
	typedef tArrayItem< ValueType > &			ItemRef;

	public: // this entries will be public only for debug reasons

	const tArrayItem< ValueType >	_point;
	CountType						_count;

	// default constructor
	tArray( void );

	// copy constructor
	tArray( const ArrRef source );

	// length constructor
	tArray( const CountType count );

	// output operator
	friend ostream< ValueType > & ostream< ValueType >::operator<<( const ArrRef array ) const;

	// input operator
	friend istream< ValueType > & istream< ValueType >::operator>>( const ArrRef array );

	// index-access operator
	ValRef operator[]( const CountType );

	// item adding method
	ArrRef Add( const ValRef value );

	// destructor
	~tArray( void );
	};

// ========================================================= default constructor
template< class ValueType, typename CountType >
tArray< ValueType, CountType >::tArray( void ) :
	_count( reinterpret_cast< CountType >( 0 ) )
	{}

// ============================================================ copy constructor
template< class ValueType, typename CountType >
tArray< ValueType, CountType >::tArray( const ArrRef source ) :
	_count( source._count )
	{
	CountType	counter;

	for( counter = reinterpret_cast< CountType >( 1 ); counter <= _count; counter++ )
		this->Add( source[ counter ] );
	}

// ========================================================== length constructor
template< class ValueType, typename CountType >
tArray< ValueType, CountType >::tArray( const CountType count ) :
	_count( count )
	{
	ValueType	default_value;
	CountType	counter;

	for( counter = reinterpret_cast< CountType >( 1 ); counter <= _count; counter++ )
		this->Add( default_value );
	}

// ============================================================= output operator
template< class ValueType, typename CountType >
ostream< ValueType > & ostream< ValueType >::operator<<( const tArray< ValueType, CountType > & array ) const
	{
	CountType	counter;

	for( counter = reinterpret_cast< CountType >( 1 ); counter <= _count; counter++ )
		*this << array[ counter ];

	return *this;
	}

// ============================================================== input operator
template< class ValueType, typename CountType >
istream< ValueType > & istream< ValueType >::operator>>( const tArray< ValueType, CountType > & array )
	{
	ValueType	temporary;
	CountType	counter;

	for( counter = reinterpret_cast< CountType >( 1 ); counter <= _count; counter++ )
		{
		*this >> temporary;
		array[ counter ] = temporary;
		}

	return *this;
	}

// ================================================= index-access operator( [] )
template< class ValueType, typename CountType >
ValRef tArray< ValueType, CountType >::operator[]( const CountType number )
	{
	ItemPtr		item = &_point;
	CountType	counter;

	for( counter = reinterpret_cast< CountType >( 1 ); counter <= number; counter++ )
		item = item->Next();

	return item->Get();
	}

// ========================================================== item adding method
template< class ValueType, typename CountType >
ArrRef tArray< ValueType, CountType >::Add( const ValRef value )
	{
	ItemPtr	item = new tArrayItem< ValueType >( value, _point.Previous(), &_point );
	}

// ================================================================== destructor
template< class ValueType, typename CountType >
tArray< ValueType, CountType >::~tArray( void )
	{
	ItemRef	item = _point;

	while( _point.Next() != &_point )
		delete _point.Next();
	}

#endif /* _TARRAY_H_ */