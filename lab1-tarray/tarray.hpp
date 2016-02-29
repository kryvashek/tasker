#ifndef _TARRAY_H_
#define _TARRAY_H_

#include <iostream>
#include "tarrayitem.hpp"

using	std::ostream;
using	std::istream;

template< class ValueType >
class tArray;

// pre-declaration of output operator
template< class ValueType >
ostream & operator<<( ostream & output, tArray< ValueType > const & array );

// pre-declaration of input operator
template< class ValueType >
istream & operator>>( istream & input, tArray< ValueType > & array );

template< class ValueType >
class tArray
	{
	private:

		tArrayItem< ValueType >	_point;
		int	_count;

		tArrayItem< ValueType > * getItemWithIndex( int index );
		void cleanUp( void );
		void addFrom( const tArray< ValueType > & source );

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

		// assignment operator
		tArray< ValueType > & operator=( const tArray< ValueType > & source );

		// output operator
		friend ostream & operator<< <>( ostream & output, tArray< ValueType > const & array );

		// input operator
		friend istream & operator>> <>( istream & input, tArray< ValueType > & array );
	};

// ------------------------------------------- ( inner private ) clearing memory
template< class ValueType >
void tArray< ValueType >::cleanUp( void )
	{
	while( _point.Next() != &_point )
		delete _point.Next();
	}

// --------------------------------------------- ( inner private ) finding place
template< class ValueType >
tArrayItem< ValueType > * tArray< ValueType >::getItemWithIndex( int index )
	{
	tArrayItem< ValueType >	* tmp;
	int	idx;

	for( index %= _count, tmp = _point.Next(), idx = 0; idx < index; idx++ )
		tmp = tmp->Next();

	return tmp;
	}

// -------------------------------------------- ( inner private ) copying values
template< class ValueType >
void tArray< ValueType >::addFrom( const tArray< ValueType > & source )
	{
	tArrayItem< ValueType >	* tmp;

	for( tmp = source._point.Next(); tmp != &( source._point ); tmp = tmp->Next() )
		this->AddBack( tmp->Get() );
	}

// ========================================================= default constructor
template< class ValueType >
tArray< ValueType >::tArray( void ) :
	_count( 0 )
	{}

// ============================================================ copy constructor
template< class ValueType >
tArray< ValueType >::tArray( const tArray< ValueType > & source ) :
	_count( 0 ) // will be increased during fullfilling
	{
	this->addFrom( source );
	}

// ========================================================== length constructor
template< class ValueType >
tArray< ValueType >::tArray( const int count ) :
	_count( 0 ) // will be increased during fullfilling
	{
	ValueType	defaultValue( reinterpret_cast< ValueType >( 0 ) );
	int	idx;

	for( idx = 0; idx < count; idx++ )
		this->AddBack( defaultValue );
	}

// ================================================================== destructor
template< class ValueType >
tArray< ValueType >::~tArray( void )
	{
	this->cleanUp();
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

// ================================================ index-access operator ( [] )
template< class ValueType >
ValueType & tArray< ValueType >::operator[]( const int number )
	{
	return getItemWithIndex( number )->Get();
	}

// =================================================== assignment operator ( = )
template< class ValueType >
tArray< ValueType > & tArray< ValueType >::operator=( const tArray< ValueType > & source )
	{
	this->cleanUp();
	_count = 0;
	this->addFrom( source );
	}

// ============================================================= output operator
template< class ValueType >
ostream & operator<<( ostream & output, tArray< ValueType > const & array )
	{
	tArrayItem< ValueType >	* tmp;

	for( tmp = array._point.Next(); tmp != &( array._point ); tmp = tmp->Next() )
		output << tmp->Get() << ' ';

	return output;
	}

// ============================================================== input operator
template< class ValueType >
istream & operator>>( istream & input, tArray< ValueType > & array )
	{
	tArrayItem< ValueType >	* tmp;

	for( tmp = array._point.Next(); tmp != &( array._point ); tmp = tmp->Next() )
		input >> tmp->Get();

	return input;
	}

#endif /* _TARRAY_H_ */