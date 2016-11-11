//
// Created by kryvashek on 10.11.16.
//

#include <fstream>
#include <ThreadGuard.h>

using	std::ofstream;
using	std::endl;

void printSomeValues( int one, char two ) {
	ofstream	output( "output.txt" );

	output << "one = " << one << '\t' << "two = '" << two << "'" << endl;
}

int main( int argc, char *argv[] ) {
	ThreadGuard< int, char >	tg;

	tg.Set( printSomeValues );
	tg.Run( 789, 'R' );

	return 0;
}
