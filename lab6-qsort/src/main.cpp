#include <fstream>
#include <list>
#include <random>
#include <iostream>
#include <future>

template< typename Data, typename InputIter, class Storage = std::list< Data > >
Storage parQsort( InputIter from, InputIter till ) {
	Storage					morequal, less;
	InputIter				prev, current;
	size_t 					ranger;

	if( from == till )
		return morequal;

	for( ranger = 256, current = from; current != till; current++ )
		if( 0 < std::rand() % ranger ) {
			prev = current;

			if( 2 < ranger )
				ranger /= 2;
		} else
			break;

	const InputIter	pivot( current == till ? prev : current );

	for( current = from; current != till; current++ )
		if( pivot == current )
			continue;
		else if( *pivot > *current )
			less.push_back( *current );
		else
			morequal.push_back( *current );

	{
		std::future< Storage >	fLess, fMorequal;

		fLess = std::async( [ &less ]( void ) { return parQsort< Data >( less.begin(), less.end() ); } );
		fMorequal = std::async( [ &morequal ]( void ) { return parQsort< Data >( morequal.begin(), morequal.end() ); } );
		less = fLess.get();
		morequal = fMorequal.get();
	}

	less.insert( less.end(), *pivot );
	less.insert( less.end(), morequal.begin(), morequal.end() );

	return less;
};

int main() {
	std::list< int >	source;
	int					index;
	std::fstream		output( "output.txt" );

	for( index = 0; index < 1000000; index++ ) {
		source.push_back( std::rand() % 100 );
		output << source.back() << " ";
	}

	output << std::endl;
	source = parQsort< int >( source.begin(), source.end() );

	for( auto cur = source.begin(); cur != source.end(); cur++ )
		output << *cur << " ";

	output << std::endl;

	return 0;
}