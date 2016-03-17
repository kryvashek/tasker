#include <fstream>
#include "threadarmy.hpp"

using std::endl;
using std::cout;
using std::fstream;
using std::istream;

// gets one value of Kit entry type and puts it to the end of given Kit
template< class StorageT >
istream & operator>>( istream & input, StorageT & destination )
	{
	static typename StorageT::value_type	value;

	input >> value;
	destination.push_back( value );
	return input;
	}

typedef int				ItemT;
typedef vector< ItemT >	KitT;

int main( void )
	{
	ThreadArmy< ItemT, KitT >	army;
	fstream						input( "input.txt", fstream::in );
	KitT	cortege1, cortege2;
	int		i, n;

	input >> n;

	for( i = 0; i < n; i++ )
		input >> cortege1;

	for( i = 0; i < n; i++ )
		input >> cortege2;

	army.Execute( cortege1, cortege2, n );
	cout << army.Stop() << endl;
	army.Execute( cortege1, cortege2, n - 1 );
	cout << army.Stop() << endl;
	return 0;
	}
