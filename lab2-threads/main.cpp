#include <iostream>
#include <thread>

using std::endl;
using std::cout;
using std::thread;

void say_smth( void )
	{
	cout << "First step of coding" << endl;
	return;
	}

int main( void )
	{
	thread	t( say_smth );
	t.join();
	return 0;
	}
