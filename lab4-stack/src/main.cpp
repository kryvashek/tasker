//
// Created by kryvashek on 10.11.16.
//

#include <iostream>
#include <ThreadSquad.h>
#include <Executor.h>
#include <SafeStack.h>

using	std::cout;
using	std::endl;

__time_t getMoment( void ) {
	struct timespec			moment;

	clock_gettime( CLOCK_MONOTONIC, &moment );
	return moment.tv_sec;
}

void someFunc( char a, __time_t value ) {
	timespec period{ .tv_sec = 3, .tv_nsec = 0 };

	cout << a << " started: " << getMoment() - value << endl;
	nanosleep( &period, NULL );
	cout << a << " finished: " << getMoment() - value << endl;
}

int write( istream & input, string & message ) {
	string temp;
	input >> temp;
	message = "Read from input '" + temp + "'";
	return 0;
}

int main( int argc, char *argv[] ) {
	const __time_t	allStart( getMoment() );
	const int		count( 8 );
	int 			index;

	ThreadGuard< char, __time_t >	guard( someFunc );
	ThreadSquad						squad( 2 );

	guard.Run( '0', allStart );

	for( index = 0; index < count; index++ )
		squad.AddTask( [ index, allStart ]( void ) {
			timespec period{ .tv_sec = ( 5 + ( index % 2 ) * 3 ), .tv_nsec = 0 };

			cout << index + 1 << " started: " << getMoment() - allStart << endl;
			nanosleep( &period, NULL );
			cout << index + 1 << " finished: " << getMoment() - allStart << endl;
		} );
	squad.Run();
	squad.StopWait();

	guard.StopWait();
	guard.Set( []( char a, __time_t value ) {
		timespec period{ .tv_sec = 3, .tv_nsec = 0 };

		cout << a << " started: " << getMoment() - value << endl;
		nanosleep( &period, NULL );
		cout << a << " finished: " << getMoment() - value << endl;
	} );
	guard.Run( '9', allStart );
	guard.StopWait();

	cout << "All got: " << getMoment() - allStart << endl;

	Executor< int >	executor;

	executor.AddRoutine( "write", write );
	executor.Run();

	SafeStack< int >	locker;

	for( int i = 0; i < 10; i++ ) {
		cout << ( 2 * i + 1 ) << " ";
		locker.push( 2 * i + 1 );
	}

	cout << endl;

	while( !locker.empty() ) {
		cout << locker.top() << " ";
		locker.pop();
	}

	return 0;
}
