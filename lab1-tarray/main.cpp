#include <iostream>

#include "tarray.hpp"

using std::cin;
using std::cout;
using std::endl;

int main( void )
	{
	tArray< int >	array( 3 );

	cin >> array;
	cout << "array: " << array << endl;

	tArray< int >	array2( array );
	cout << "array2: " << array2 << endl;

	array.Insert( 13, 3 );
	array.Insert( 12, 2 );
	array.Insert( 11, 1 );

	cout << "array: " << array << endl;

	array2 = array;
	cout << "array2: " << array2 << endl;

	array.AddFront( 100 );
	array.AddBack( 100 + array.Size() );

	cout << "array: " << array << endl;

	for( int i = 0; i < array.Size(); i++ )
		cout << "[ " << i << " ] = " << array[ i ] << endl;

	return 0;
	}