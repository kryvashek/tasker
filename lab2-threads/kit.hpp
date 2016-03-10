#ifndef _KIT_H_
#define _KIT_H_

#include <functional>
#include <algorithm>
#include <iostream>
#include <utility>
#include <list>

using std::function;
using std::for_each;
using std::istream;
using std::ostream;
using std::list;
using std::pair;

typedef float								ItemT;
typedef list< ItemT >						KitT;
typedef KitT::iterator						KitPosT;
typedef pair< KitPosT, KitPosT >			KitPairT;
typedef function< ItemT ( KitPairT & ) >	KitPairProdT;

// gets one value of Kit entry type and puts it to the end of given Kit
istream & operator>>( istream & input, KitT & destination );

// prints values from given Kit one-by-one in striaght order
ostream & operator<<( ostream & output, KitT & source );

#endif // _KIT_H_
