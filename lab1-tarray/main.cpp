#include <iostream>

#include "tarray.hpp"

using namespace std;

int main( void )
	{
	t_array_entry< int >	* entry = new t_array_entry< int >;
	cout << entry->item << endl;
	delete entry;
	return 0;
	}