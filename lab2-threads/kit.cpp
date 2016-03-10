#include "kit.hpp"

// gets one value of Kit entry type and puts it to the end of given Kit
istream & operator>>( istream & input, KitT & destination )
	{
	static ItemT	value;

	input >> value;
	destination.push_back( value );
	return input;
	}

// prints values from given Kit in striaght order one-by-one
ostream & operator<<( ostream & output, KitT & source )
	{
	for_each( source.begin(), source.end(), [ &output ]( ItemT & value ){ output << value << " "; } );
	return output;
	}
