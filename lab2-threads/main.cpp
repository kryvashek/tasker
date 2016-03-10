#include <iostream>
#include "kit.hpp"

using std::endl;
using std::cout;
using std::fstream;

int main( void )
	{
	//fstream		input( "input.txt", fstream::in );
	int			n;
	KitT	cortege;

	KitPairProdT lambda = []( KitPairT & pair )-> ItemT { return *pair.first + *pair.second; };

	std::cin >> n;

	for( int i = 0; i < n; i++ )
		std::cin >> cortege;

	cout << cortege << endl;

	//thread	t( say_smth );
	//t.join();

	return 0;
	}
