#include <iostream>

#include "tarrayitem.hpp"

using namespace std;

int main( void )
	{
	tArrayItem< int >	* entry = new tArrayItem< int >;
	cout << entry->_value << endl;
	delete entry;
	return 0;
	}