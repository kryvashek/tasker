#ifndef _CORTEGE_H_
#define _CORTEGE_H_

#include <iostream>
#include <list>

using std::istream;
using std::ostream;
using std::list;

typedef float				ItemT;
typedef list< ItemT >		CortegeT;
typedef CortegeT::iterator	CortegePosT;

// gets one value of cortege entry type and puts it to the end of given cortege
istream & operator>>( istream & input, CortegeT & destination );

// prints values from given cortege one-by-one in striaght order
ostream & operator<<( ostream & output, CortegeT & source );

#endif // _CORTEGE_H_
