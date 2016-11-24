#include <fstream>
#include <list>
#include <random>
#include <future>
#include <algorithm>
#include <mutex>
#include <iostream>
#include <chrono>

template< typename Storage >
void parQsort( typename Storage::iterator first, typename Storage::iterator last );

template< class Storage >
class Sorter {
private:
	typedef unsigned int					ThreadsCount;
	typedef std::lock_guard< std::mutex >	MutexGuard;
	typedef typename Storage::value_type	Data;
	typedef typename Storage::iterator		Iter;

	std::mutex		cntMutex;
	ThreadsCount	count;

	Sorter( const Sorter & source ) = delete;
	Sorter( Sorter && source ) = delete;

	static ThreadsCount threadsOptimal( void ) {
		const ThreadsCount	system( std::thread::hardware_concurrency() );
		return system > 1 ? system : 2;
	}

	void countUp( void ) {
		MutexGuard	guard( this->cntMutex );
		this->count++;
	}

	bool countDownCheck( void ) {
		MutexGuard	guard( this->cntMutex );
		if( this->count == 0 )
			return false;
		this->count--;
		return true;
	}

	void operator()( const typename Storage::iterator from, const typename Storage::iterator till ) {
		Iter	last( till ), next( from );

		last--;

		if( till == from || last == from ) // if there is no elements or only one in the container
			return;

		next++;

		if( next == last ) { // if there are two elements in the container
			if( *next > *last )	// if they need some sorting
				std::swap( *next, *last ); // perform proper simple sorting

			return; // they are already sorted
		}

		const Iter	edgeLeft = std::partition( from, last, [ & ]( const Data & value ) { return value < *last; } );

		if( edgeLeft != last )
			std::swap( *edgeLeft, *last );

		next = edgeLeft;
		next++;

		const Iter	edgeRight = std::partition( next, till, [ & ]( const Data & value ) { return value == *edgeLeft; } );

		if( this->countDownCheck() ) {
			std::future< void > parallel = std::async( std::launch::async, [ & ]( void ) { ( *this )( edgeRight, till ); } );
			( *this )( from, edgeLeft );
			parallel.wait();
			this->countUp();
		} else {
			( *this )( from, edgeLeft );
			( *this )( edgeRight, till );
		}
	}

public:
	Sorter( void ) : count( Sorter::threadsOptimal() ) {};

	friend void parQsort< Storage >( const typename Storage::iterator first, const typename Storage::iterator last );
};

template< typename Storage >
void parQsort( const typename Storage::iterator first, const typename Storage::iterator last ) {
	Sorter< Storage >	sorter;

	sorter( first, last );
};

using namespace std::chrono;

int main() {
	typedef std::list< int >	Container;

	Container 		source;
	int				index, count;
	std::string		filename;
	std::ofstream	output;

	steady_clock::time_point	start, finish;

	std::cout << "Enter sequence length: ";
	std::cin >> count;
	std::cout << "Enter output file name: ";
	std::cin >> filename;

	if( 0 == filename.length() )
		filename == "output.txt";

	std::cout << "Generating " << count << " random values... ";
	std::cout.flush();

	for( index = 0; index < count; index++ )
		source.push_back( std::rand() % 100 );

	std::cout << " done." << std::endl << "Writing random values into the file... ";
	std::cout.flush();
	output.open( filename );

	for( auto cur = source.begin(); cur != source.end(); cur++ )
		output << *cur << " ";

	output << std::endl;
	std::cout << " done." << std::endl << "Sorting the sequence... ";
	std::cout.flush();
	start = steady_clock::now();
	parQsort< Container >( source.begin(), source.end() );
	finish = steady_clock::now();
	std::cout << " done, took " << duration_cast< milliseconds >( finish - start ).count() << " ms." << std::endl;
	std::cout << "Writing sorted values into the file... ";
	std::cout.flush();

	for( auto cur = source.begin(); cur != source.end(); cur++ )
		output << *cur << " ";

	output << std::endl;
	std::cout << " done." << std::endl << "Exiting program... " << std::endl;

	return 0;
}