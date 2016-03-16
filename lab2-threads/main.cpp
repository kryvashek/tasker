#include <fstream>
#include "kit.hpp"
#include "threadarmy.hpp"

using std::endl;
using std::cout;
using std::fstream;

typedef float			ItemT;
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
	return 0;
	}
