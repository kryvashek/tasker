#ifndef _TARRAY_H_
#define _TARRAY_H_

#include <iostream>
#include "tarrayitem.hpp"

using	std::ostream;
using	std::istream;

template< class ValueType >
class tArray
	{
	private:

		tArrayItem< ValueType > * getItemWithIndex( int index );

	public: // this entries will be public only for debug reasons

		tArrayItem< ValueType >	_point,
				* _temp;
		int		_count,
				_tempIdx;

	public:

		// default constructor
		tArray( void );

		// copy constructor
		tArray( const tArray< ValueType > & source );

		// length constructor
		tArray( const int count );

		// destructor
		~tArray( void );

		// item adding to the head method
		tArray< ValueType > & AddFront( const ValueType & value );

		// item adding to the tail method
		tArray< ValueType > & AddBack( const ValueType & value );

		// item adding to the specified place method
		tArray< ValueType > & Insert( const ValueType & value, const int number );

		// returns count of items
		int Size( void ) const;

		// index-access operator
		ValueType & operator[]( const int number );

		// output operator
		template< ValueType >
		friend ostream & operator<<( ostream & output, const tArray < ValueType > & array );

		// input operator
		template< ValueType >
		friend istream & operator>>( istream & input, tArray < ValueType > & array );
	};

// ========================================================= default constructor
template< class ValueType >
tArray< ValueType >::tArray( void ) :
	_temp( &_point ),
	_count( 0 ),
	_tempIdx( 0 )
	{}

// ============================================================ copy constructor
template< class ValueType >
tArray< ValueType >::tArray( const tArray< ValueType > & source ) :
	_temp( &_point ),
	_count( 0 ),  // will be increased during fullfilling
	_tempIdx( 0 )
	{
	for( source._temp = source._point.Next(); source._temp != &( source._point ); source._temp = _temp->Next() )
		this->AddBack( source._temp->Get() );
	}

// ========================================================== length constructor
template< class ValueType >
tArray< ValueType >::tArray( const int count ) :
	_temp( &_point ),
	_count( 0 ), // will be increased during fullfilling
	_tempIdx( 0 )
	{
	ValueType	defaultValue( reinterpret_cast< ValueType >( 0 ) );

	for( int counter = 0; counter < count; counter++ )
		this->AddBack( defaultValue );
	}

// ================================================================== destructor
template< class ValueType >
tArray< ValueType >::~tArray( void )
	{
	while( _point.Next() != &_point )
		delete _point.Next();
	}

// ============================================== item adding to the head method
template< class ValueType >
tArray< ValueType > & tArray< ValueType >::AddFront( const ValueType & value )
	{
	new tArrayItem< ValueType >( value, &_point, _point.Next() );
	_count++;
	return *this;
	}

// ============================================== item adding to the tail method
template< class ValueType >
tArray< ValueType > & tArray< ValueType >::AddBack( const ValueType & value )
	{
	new tArrayItem< ValueType >( value, _point.Previous(), &_point );
	_count++;
	return *this;
	}

// ======================================================== inner private method
template< class ValueType >
tArrayItem< ValueType > * tArray< ValueType >::getItemWithIndex( int index )
	{
	for( index %= _count, _temp = _point.Next(), _tempIdx = 0; _tempIdx < index; _tempIdx++ )
		_temp = _temp->Next();

	return _temp;
	}

// =================================== item adding to the specified place method
template< class ValueType >
tArray< ValueType > & tArray< ValueType >::Insert( const ValueType & value, const int number )
	{
	if( number == _count )
		this->AddBack( value );

	else
		{
		tArrayItem< ValueType >	* post_item = getItemWithIndex( number );

		new tArrayItem< ValueType >( value, post_item->Previous(), post_item );
		_count++;
		}
	return *this;
	}

// ====================================================== returns count of items
template< class ValueType >
int tArray< ValueType >::Size( void ) const
	{
	return _count;
	}

// ================================================= index-access operator( [] )
template< class ValueType >
ValueType & tArray< ValueType >::operator[]( const int number )
	{
	return getItemWithIndex( number )->Get();
	}

// ============================================================= output operator
template< class ValueType >
ostream & operator<<( ostream & output, const tArray< ValueType > & array )
	{
	for( tArrayItem< ValueType > * item = array._point.Next(); item != &( array._point ); item = item->Next() )
		output << item->Get() << ' ';

	return output;
	}

// ============================================================== input operator
template< class ValueType >
istream & operator>>( istream & input, tArray< ValueType > & array )
	{
	for( tArrayItem< ValueType > * item = array._point.Next(); item != &( array._point ); item = item->Next() )
		input >> item->Get();

	return input;
	}

#endif /* _TARRAY_H_ */