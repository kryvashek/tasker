#include "cortege.hpp"

// gets one value of cortege entry type and puts it to the end of given cortege
istream & operator>>( istream & input, CortegeT & destination )
	{
	static ItemT	value;

	input >> value;
	destination.push_back( value );
	return input;
	}

// prints values from given cortege in striaght order one-by-one
ostream & operator<<( ostream & output, CortegeT & source )
	{
	for( CortegePosT pos = source.begin(); pos != source.end(); pos++ )
		output << *pos << " ";

	return output;
	}
