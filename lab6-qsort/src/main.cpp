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
	typedef std::lock_guard< std::mutex >	MutexGuard;
	typedef unsigned int					ThreadsCount;
	typedef typename Storage::value_type	Data;
	typedef typename Storage::iterator		Iter;

	mutable std::mutex		cntMutex;
	volatile ThreadsCount	count;

	Sorter( const Sorter & source ) = delete;
	Sorter( Sorter && source ) = delete;

	static ThreadsCount threadsOptimal( void ) {
		const ThreadsCount	system( std::thread::hardware_concurrency() );
		return system > 1 ? system : 2;
	}

	void countUp( void ) {
		MutexGuard	guard( this->cntMutex );
		count++;
	}

	void countDown( void ) {
		MutexGuard	guard( this->cntMutex );
		count--;
	}

	bool countCheck( void ) const {
		MutexGuard	guard( this->cntMutex );
		return this->count > 0;
	}

	void operator()( typename Storage::iterator from, typename Storage::iterator till ) {
		Iter	current, last( till );
		size_t	ranger;

		last--;

		if( till == from || last == from )
			return;

		this->countDown();

		for( ranger = 256, current = from; current != till; current++ )
			if( 0 < std::rand() % ranger ) {
				if( 2 < ranger )
					ranger /= 2;
			} else
				break;

		if( current == till )
			current = last;

		if( current != last )
			std::swap( *current, *last );

		const Data	& pivot( *last );
		const Iter	edge = std::partition( from, last, [ &pivot ]( const Data & value ) { return value < pivot; } );

		if( this->countCheck() ) {
			std::future< void > parallel = std::async( std::launch::async, [ & ]( void ) { ( *this )( edge, till ); } );
			( *this )( from, edge );
			parallel.wait();
		} else {
			( *this )( from, edge );
			( *this )( edge, till );
		}
		this->countUp();
	}

public:
	Sorter( void ) : count( Sorter::threadsOptimal() ) {};

	friend void parQsort< Storage >( typename Storage::iterator first, typename Storage::iterator last );
};

template< typename Storage >
void parQsort( typename Storage::iterator first, typename Storage::iterator last ) {
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

	for( index = 0; index < count; index++ )
		source.push_back( std::rand() % 100 );

	std::cout << " done." << std::endl << "Writing random values into the file... ";
	output.open( filename );

	for( auto cur = source.begin(); cur != source.end(); cur++ )
		output << *cur << " ";

	output << std::endl;
	std::cout << " done." << std::endl << "Sorting the sequence... ";
	start = steady_clock::now();
	parQsort< Container >( source.begin(), source.end() );
	finish = steady_clock::now();
	std::cout << " done, took " << duration_cast< milliseconds >( finish - start ).count() << " ms." << std::endl;
	std::cout << "Writing sorted values into the file... ";

	for( auto cur = source.begin(); cur != source.end(); cur++ )
		output << *cur << " ";

	output << std::endl;
	std::cout << " done." << std::endl << "Exiting program... " << std::endl;

	return 0;
}