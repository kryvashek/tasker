#ifndef _THREADARMY_H_
#define _THREADARMY_H_

#include <thread>

using std::thread;
using std::move;

template< typename ItemT >
class ThreadArmy
	{
	private:
		thread	* _threads;
		ItemT	* _results;
		int		_count;

		ThreadArmy( const ThreadArmy & ) = delete;
		operator=( const ThreadArmy & ) = delete;

	public:
		typedef function< ItemT ( ItemT & ) >

		ThreadArmy( void );
		ThreadArmy( const int count );
		ThreadArmy( ThreadArmy && source );
		operator=( ThreadArmy && source );
	};

template< typename ItemT >
ThreadArmy::ThreadArmy( void ) :
	_threads( NULL ),
	_results( NULL ),
	_count( 0 )
	{}

template< typename ItemT >
ThreadArmy::ThreadArmy( const int count ) :

#endif // _THREADARMY_H_
