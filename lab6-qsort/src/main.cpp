#include <iostream>
#include <list>
#include <random>
#include <iostream>

template< typename Data, typename InputIter, class Storage = std::list< Data > >
Storage parQsort( InputIter from, InputIter till ) {
	Storage		morequal,
				less;
	InputIter	prev,
				current;
	size_t 		ranger;

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

	less = parQsort< Data >( less.begin(), less.end() );
	morequal = parQsort< Data >( morequal.begin(), morequal.end() );
	less.insert( less.end(), *pivot );
	less.insert( less.end(), morequal.begin(), morequal.end() );

	return less;
};

int main() {
	std::list< int >	source;
	int					count, index;

	for( int stage = 1; stage <= 10; stage++ ) {
		count = std::rand() % 30;

		for( index = 0; index < count; index++ ) {
			source.push_back( std::rand() % 100 );
			std::cout << source.back() << " ";
		}

		std::cout << std::endl;
		source = parQsort< int >( source.begin(), source.end() );

		for( auto cur = source.begin(); cur != source.end(); cur++ )
			std::cout << *cur << " ";

		std::cout << std::endl << std::endl;
		source.clear();
	}

	return 0;
}