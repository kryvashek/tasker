#ifndef _TARRAY_H_
#define _TARRAY_H_

#include <iostream>
#include "tarrayitem.hpp"

using	std::ostream;
using	std::istream;

template< class ValueType >
class tArray
	{
	public:

		typedef ValueType &				tValRef;
		typedef tArray< ValueType > &	tArrRef;
		typedef tArrayItem< ValueType >	tItem;

	private:

		tItem * getItemWithIndex( const int index );

	public: // this entries will be public only for debug reasons

		tItem	_point,
				* _temp;
		int		_count,
				_tempIdx;

	public:

		// default constructor
		tArray( void );

		// copy constructor
		tArray( const tArrRef source );

		// length constructor
		tArray( const int count );

		// destructor
		~tArray( void );

		// item adding to the head method
		tArrRef AddFront( const tValRef value );

		// item adding to the tail method
		tArrRef AddBack( const tValRef value );

		// item adding to the specified place method
		tArrRef Insert( const tValRef value, const int number );

		// index-access operator
		tValRef operator[]( const int number );

		// output operator
		template< ValueType >
		friend ostream & operator<<( ostream & output, const tArray < ValueType > & array );

		// input operator
		template< ValueType >
		friend istream & operator>>( istream & input, const tArray < ValueType > & array );
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
tArray< ValueType >::tArray( const tArrRef source ) :
	_temp( &_point ),
	_count( source._count ),
	_tempIdx( 0 )
	{
	for( _temp = source._point.Next(); _temp != &( source._point ); _temp = _temp->Next() )
		this->AddBack( _temp->Get() );

	_temp = &_point;
	}

// ========================================================== length constructor
template< class ValueType >
tArray< ValueType >::tArray( const int count ) :
	_temp( &_point ),
	_count( count ),
	_tempIdx( 0 )
	{
	ValueType	defaultValue;
	int			counter;

	for( counter = 0; counter < _count; counter++ )
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
tArray< ValueType > & tArray< ValueType >::AddFront( const tValRef value )
	{
	new tArrayItem< ValueType >( value, &_point, &_point.Next() );
	_count++;
	return *this;
	}

// ============================================== item adding to the tail method
template< class ValueType >
tArray< ValueType > & tArray< ValueType >::AddBack( const tValRef value )
	{
	new tArrayItem< ValueType >( value, _point.Previous(), &_point );
	_count++;
	return *this;
	}

// ======================================================== inner private method
template< class ValueType >
tArrayItem< ValueType > * tArray< ValueType >::getItemWithIndex( const int index )
	{
	tItem	* bgn;
	tItem *	( tItem::*direction )( void );
	int		bgnIdx, step, middle1 = _tempIdx / 2,
			middle2 = ( _tempIdx + _count ) / 2;

	if( middle1 <= index && index < middle2 )
		{// начинаем искать от кэшированного положения
		bgn = _temp;
		bgnIdx = _tempIdx;
		}

	else
		{// начинаем искать от фиктивного элемента
		bgn = &_point;
		bgnIdx = ( index < middle1 ) ? 0 : _count;
		}

	if( middle2 <= index || ( middle1 <= index && index < _tempIdx ) )
		{// движемся в направлении "назад"
		direction = &tItem::Previous;
		step = -1;
		}

	else
		{// движемся в направлении "вперёд"
		direction = &tItem::Next;
		step = 1;
		}

	for( _temp = bgn, _tempIdx = bgnIdx; _tempIdx != index; _tempIdx += step )
		_temp = ( _temp->*direction )();

	return _temp;
	}

// =================================== item adding to the specified place method
template< class ValueType >
tArray< ValueType > & tArray< ValueType >::Insert( const tValRef value, const int number )
	{
	tItem	* pre_item = getItemWithIndex( number - 1 );

	new tArrayItem< ValueType >( value, pre_item, pre_item->Next() );
	_count++;
	return *this;
	}

// ================================================= index-access operator( [] )
template< class ValueType >
ValueType & tArray< ValueType >::operator[]( const int number )
	{
	return getItemWithIndex( number - 1 )->Get();
	}

// ============================================================= output operator
template< class ValueType >
ostream & operator<<( ostream & output, const tArray< ValueType > & array )
	{
	for( tArrayItem< ValueType > * item = array._point.Next(); item != &( array._point ); item = item->Next() )
		output << item.Get() << ' ';

	return output;
	}

// ============================================================== input operator
template< class ValueType >
istream& operator>> ( istream & input, const tArray< ValueType > & array )
	{
	for( tArrayItem< ValueType > * item = array._point.Next(); item != &( array._point ); item = item->Next() )
		input >> item.Get();

	return input;
	}

#endif /* _TARRAY_H_ */