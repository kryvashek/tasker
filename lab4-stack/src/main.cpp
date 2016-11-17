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
	string	temp;

	input >> temp;
	message = "Read from input '" + temp + "'";
	return 0;
}

int top( istream & input, string & message, SafeStack< int > & locker ) {
	message = std::to_string( locker.top() );
	return 0;
}

int push( istream & input, string & message, SafeStack< int > & locker ) {
	int value;

	input >> value;
	locker.push( value );
	message = std::to_string( value );
	return 0;
}

int pop( istream & input, string & message, SafeStack< int > & locker ) {
	locker.pop();
	return 0;
}

int main( int argc, char *argv[] ) {
	ifstream	inone( "first.txt" ),
				intwo( "second.txt" );
	ofstream	outone( "first.log"),
				outtwo( "second.log" );

	ThreadSquad			squad( 2 );
	SafeStack< int >	locker;

	Executor< int, SafeStack< int > & >	one( inone, cout ),
										two( intwo, cout );

	one.AddRoutine( "top", top );
	one.AddRoutine( "push", push );
	one.AddRoutine( "pop", pop );
	two = one; // copying of the instruction set

	squad.AddTask( [ & ]( void ) { one.Run( locker ); } );
	squad.AddTask( [ & ]( void ) { two.Run( locker ); } );

	squad.Run();
	squad.StopWait();

	return 0;
}
