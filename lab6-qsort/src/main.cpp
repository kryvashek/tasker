#include <fstream>
#include <list>
#include <random>
#include <iostream>
#include <future>
#include <algorithm>

int optimalThreads( void ) {
	const int	system( std::thread::hardware_concurrency() );
	return system > 1 ? system : 2;
}

template< class Data >
void parQsort( std::list< Data > & storage ) {
	static std::mutex	cntMutex;
	static int 			cnt = 0;

	typename std::list< Data >::const_iterator	prev, current;

	std::list< Data >	more;
	size_t 				ranger;

	cntMutex.lock();
	const int curCnt( ++cnt );
	cntMutex.unlock();

	if( storage.empty() )
		return;
/*
	for( ranger = 256, current = storage.begin(); current != storage.end(); current++ )
		if( 0 < std::rand() % ranger ) {
			prev = current;

			if( 2 < ranger )
				ranger /= 2;
		} else
			break;

	const Data	& pivot( current == storage.end() ? *prev : *current );
 */
	const Data	& pivot( storage.front() );

	current = std::partition( storage.begin(), storage.end(), [ pivot ]( Data & value ) { return value < pivot; } );
	more.splice( more.begin(), storage, current, storage.end() );

	if( curCnt < optimalThreads() ) {
		std::future< bool > moreSorted( std::async( [ &more ]( void ) {
			parQsort( more );
			return true;
		} ) );

		parQsort( storage );
		moreSorted.get();
	} else {
		parQsort( storage );
		parQsort( more );
	}

	storage.splice( storage.end(), more, more.begin(), more.end() );

	cntMutex.lock();
	cnt--;
	cntMutex.unlock();
};

int main() {
	std::list< int >	source;
	int					index;
	std::ofstream		output( "output.txt" );

	for( index = 0; index < 256; index++ ) {
		source.push_back( std::rand() % 100 );
		output << source.back() << " ";
	}

	output << std::endl;
	parQsort( source );

	for( auto cur = source.begin(); cur != source.end(); cur++ )
		output << *cur << " ";

	output << std::endl;

	return 0;
}