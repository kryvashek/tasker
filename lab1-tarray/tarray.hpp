#ifndef _TARRAY_H_
#define _TARRAY_H_

#include <iostream>
#include "tarrayitem.hpp"

using namespace std;

typedef int	count_type;

template< class value_type >
class t_array
	{
	public: // this entries will be public only for debug reasons

	const t_array_item< value_type >	point;
	count_type							count;
	};



#endif /* _TARRAY_H_ */