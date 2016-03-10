#include <iostream>
#include <thread>
#include <list>
#include "cortege.hpp"

using std::endl;
using std::cout;
using std::thread;
using std::move;
using std::fstream;

int main( void )
	{
	//fstream		input( "input.txt", fstream::in );
	int			n;
	CortegeT	cortege;

	std::cin >> n;

	for( int i = 0; i < n; i++ )
		std::cin >> cortege;

	cout << cortege << endl;

	//thread	t( say_smth );
	//t.join();

	return 0;
	}
