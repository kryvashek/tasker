//
// Created by kryvashek on 10.11.16.
//

#include <iostream>
#include <ThreadSquad.h>

using	std::cout;
using	std::endl;


int main( int argc, char *argv[] ) {
	const size_t	count( 8 );
	size_t 			index;
	int 			ints[ count ] = { 0, 11111, 202020, 333, 4444, 55555, 666666, 77 },
					* one = ints;
	char 			chars[ count ] = { 'A', '!', 'e', '0', '|', '=', '6', 'F' },
					* two = chars;
	ThreadSquad		squad( 2 );

	for( index = 0; index < count; index++, one++, two++ )
		squad.Tasks.push( [ &, one, two, index ]( void ) {
			timespec period{ .tv_sec = 3 + index % 2, .tv_nsec = 0 };

			nanosleep( &period, NULL );
			cout << index << ") one = " << *one << "\t two = '" << *two << '\'' << endl;
		} );

	squad.Run();
	squad.Stop( true );

	return 0;
}
