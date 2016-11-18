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

void stay( int secs, int nsecs ) {
	const timespec	period = { .tv_sec = secs, .tv_nsec = nsecs };

	nanosleep( &period, NULL );
}

int wait( istream & input, string & message, SafeStack< int > & locker ) {
	int	s, n;

	input >> s;
	input >> n;
	stay( s, n );
	message = std::to_string( s ) + ", " + std::to_string( n );
	return 0;
}

void some( int start, const int step, const int finish ) {
	for( int stepnum = 1; start < finish; start += step, stepnum++ ) {
		cout << stepnum << endl;
		stay( 0, 500000000 );
	}
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
	one.AddRoutine( "pop", pop );
	one.AddRoutine( "wait", wait );
	two = one; // copying of the instruction set
	three = one; // and again

	squad.AddTask( [ & ]( void ) { one.Run( locker ); } );
	squad.AddTask( [ & ]( void ) { two.Run( locker ); } );
	squad.AddTask( [ & ]( void ) { three.Run( locker ); } );
	squad.AddTask( [ & ]( void ) { some( 0, 1, 10 ); } );

	squad.Run();
	squad.StopWait();

	return 0;
}
