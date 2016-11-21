#include <fstream>
#include <list>
#include <random>
#include <future>
#include <algorithm>
#include <mutex>

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
		const Iter	edge = std::partition( from, till, [ &pivot ]( const Data & value ) { return value < pivot; } );

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

int main() {
	typedef std::list< int >	Container;
	Container 					source;
	int							index;
	std::ofstream				output( "output.txt" );

	for( index = 0; index < 1000000; index++ ) {
		source.push_back( std::rand() % 100 );
		output << source.back() << " ";
	}

	output << std::endl;
	parQsort< Container >( source.begin(), source.end() );

	for( auto cur = source.begin(); cur != source.end(); cur++ )
		output << *cur << " ";

	output << std::endl;

	return 0;
}