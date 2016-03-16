#ifndef _KIT_H_
#define _KIT_H_

#include <algorithm>
#include <iostream>
#include <vector>

using std::for_each;
using std::istream;
using std::ostream;

// gets one value of Kit entry type and puts it to the end of given Kit
template< class StorageT >
istream & operator>>( istream & input, StorageT & destination )
	{
	static typename StorageT::value_type	value;

	input >> value;
	destination.push_back( value );
	return input;
	}

// prints values from given Kit one-by-one in striaght order
template< class StorageT >
ostream & operator<<( ostream & output, StorageT & source )
	{
	for_each( source.begin(), source.end(), [ &output ]( typename StorageT::value_type & value ){ output << value << " "; } );
	return output;
	}

#endif // _KIT_H_
