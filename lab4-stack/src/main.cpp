//
// Created by kryvashek on 10.11.16.
//

#include <iostream>
#include <fstream>
#include <ThreadSquad.h>
#include <Executor.h>
#include <SafeStack.h>

using	std::cout;
using	std::endl;
using	std::ifstream;
using	std::ofstream;

void stay( int secs, int nsecs ) {
	const timespec	period = { .tv_sec = secs, .tv_nsec = nsecs };

	nanosleep( &period, NULL );
}

int count( istream & input, string & message, SafeStack< int > & locker ) {
	int	start, step, finish, counter;

	input >> start >> step >> finish;
	for( counter = start; counter < finish; counter += step ) {
		cout << counter << endl;
		stay( 0, static_cast< unsigned long >( 5e8 ) );
	}
	message = std::to_string( start ) + ", " + std::to_string( step ) + ", " + std::to_string( finish );
	return 0;
}

int write( istream & input, string & message, SafeStack< int > & locker ) {
	string	temp;

	input >> temp;
	message = "'" + temp + "'";
	return 0;
}

int wait( istream & input, string & message, SafeStack< int > & locker ) {
	int	s, n;

	input >> s >> n;
	stay( s, n );
	message = std::to_string( s ) + ", " + std::to_string( n );
	return 0;
}

int top( istream & input, string & message, SafeStack< int > & locker ) {
	int	value;

	if( locker.GetVal( value ) )
		message = std::to_string( value );
	else
		message = "empty";

	return 0;
}

int push( istream & input, string & message, SafeStack< int > & locker ) {
	int value;

	input >> value;
	locker.AddVal( value );
	message = std::to_string( value );
	return 0;
}

int main( int argc, char *argv[] ) {
	ifstream	inone( "first.txt" ),
				intwo( "second.txt" ),
				inthree( "third.txt" );
	ofstream	outone( "first.log"),
				outtwo( "second.log" ),
				outthree( "third.log" );

	ThreadSquad			squad( 3 );
	SafeStack< int >	locker;

	Executor< int, SafeStack< int > & >	one( inone, outone ),
										two( intwo, outtwo ),
										three( inthree, outthree );

	one.AddRoutine( "top", top );
	one.AddRoutine( "push", push );
	two = one; // copying of the instruction set
	three = one; // and again
	one.AddRoutine( "wait", wait );
	two.AddRoutine( "count", count );
	three.AddRoutine( "write", write );

	squad.AddTask( [ & ]( void ) { one.Run( locker ); } );
	squad.AddTask( [ & ]( void ) { two.Run( locker ); } );
	squad.AddTask( [ & ]( void ) { three.Run( locker ); } );

	squad.Run();
	squad.StopWait();

	return 0;
}
