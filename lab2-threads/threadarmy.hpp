#ifndef _THREADARMY_H_
#define _THREADARMY_H_

#include <vector>
#include <thread>

using std::vector;
using std::thread;

template< typename ItemT, class StorageT >
class ThreadArmy
	{
	private:
		const int			_threadsCount;
		vector< thread * >	_threads;
		vector< ItemT >		_byThreadResults;
		ItemT				_sumResult;
		bool				_isWorking;

		ThreadArmy( const ThreadArmy & ) = delete;
		ThreadArmy & operator=( const ThreadArmy & ) = delete;

		const int calcThreadsCount( void ) const;

	public:

		ThreadArmy( void );
		ThreadArmy( const StorageT & kitA, const StorageT & kitB, const int & length );
		~ThreadArmy( void );

		void Execute( const StorageT & kitA, const StorageT & kitB, const int & length );
		const ItemT Stop( void );
	};

// calculates available amount of threads on the system ================
template< typename ItemT, class StorageT >
const int ThreadArmy< ItemT, StorageT >::calcThreadsCount( void ) const
	{
	const int	amount( thread::hardware_concurrency() );
	return amount < 2 ? 2 : amount;
	}

// initiates army but performs no other actions ========================
template< typename ItemT, class StorageT >
ThreadArmy< ItemT, StorageT >::ThreadArmy( void ) :
	_threadsCount( calcThreadsCount() ),
	_sumResult( 0 ),
	_isWorking( false )
	{
	_threads.reserve( _threadsCount );
	_byThreadResults.reserve( _threadsCount );
	}

// initiates army and starts calculation by calling Execute ============
template< typename ItemT, class StorageT >
ThreadArmy< ItemT, StorageT >::ThreadArmy( const StorageT & kitA, const StorageT & kitB, const int & length ) :
	_threadsCount( calcThreadsCount() ),
	_sumResult( 0 ),
	_isWorking( false )
	{
	_threads.reserve( _threadsCount );
	_byThreadResults.reserve( _threadsCount );
	this->Execute( kitA, kitB, length );
	}

// stop calculations if they are continuing ============================
template< typename ItemT, class StorageT >
ThreadArmy< ItemT, StorageT >::~ThreadArmy( void )
	{
	this->Stop();
	}

// performs main calculations with specified data ======================
template< typename ItemT, class StorageT >
void ThreadArmy< ItemT, StorageT >::Execute( const StorageT & kitA, const StorageT & kitB, const int & length )
	{
	int 	part = ( length + _threadsCount - 1 ) / _threadsCount,
			start = 0,
			finish = part;

	typename vector< ItemT >::iterator	resultPlace = _byThreadResults.begin();

	_isWorking = true;

	for( int i = 0; i < _threadsCount; i++, start += part, finish += part, resultPlace++ )
		{
		if( finish > length )
			finish = length;

		_threads[ i ] = new thread( [ start, finish, resultPlace, kitA, kitB ]( void )
										{
										for( int j = start; j < finish; j++ )
											*resultPlace += kitA[ j ] * kitB[ j ];
										} );
		}
	}

// stops all calculations (waits for it) and returns their result ======
template< typename ItemT, class StorageT >
const ItemT ThreadArmy< ItemT, StorageT >::Stop( void )
	{
	if( _isWorking )
		{
		for( int i = 0; i < _threadsCount; i++ )
			{
			if( _threads[ i ]->joinable() )
				_threads[ i ]->join();
			_sumResult += _byThreadResults[ i ];
			delete _threads[ i ];
			}

		_isWorking = false;
		}

	return _sumResult;
	}

#endif // _THREADARMY_H_
