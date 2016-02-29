#include <iostream>

#include "tarray.hpp"

using std::cin;
using std::cout;
using std::endl;

int main( void )
	{
	tArray< int >	array( 7 );

	cin >> array;

	array.Insert( 17, 7 );
	array.Insert( 16, 6 );
	array.Insert( 15, 5 );
	array.Insert( 14, 4 );
	array.Insert( 13, 3 );
	array.Insert( 12, 2 );
	array.Insert( 11, 1 );
	array.Insert( 10, 0 );

	array.Insert( 100, 0 );
	array.Insert( 102, 2 );
	array.Insert( 104, 4 );
	array.Insert( 106, 6 );
	array.Insert( 108, 8 );
	array.Insert( 110, 10 );
	array.Insert( 112, 12 );
	array.Insert( 114, 14 );
	array.Insert( 116, 16 );
	array.Insert( 118, 18 );
	array.Insert( 120, 20 );
	array.Insert( 122, 22 );
	array.Insert( 124, 24 );
	array.Insert( 126, 26 );
	array.Insert( 128, 28 );
	array.Insert( 130, 30 );

	cout << endl << array << endl;

	for( int i = 0; i < array.Size(); i++ )
		cout << "[ " << i << " ] = " << array[ i ] << endl;
	return 0;
	}