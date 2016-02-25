#include <iostream>

#include "tarray.hpp"

using namespace std;

int main( void )
	{
	tArrayItem< int >	* entry = new tArrayItem< int >;
	entry->Get() = 5;
	cout << entry->Get() << endl;
	delete entry;
	return 0;
	}