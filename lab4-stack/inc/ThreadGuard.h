//
// Created by kryvashek on 10.11.16.
//

#ifndef AIPOVK_BLOCK2_THREADGUARD_H
#define AIPOVK_BLOCK2_THREADGUARD_H

#include <thread>
#include <functional>
#include <mutex>

template< class... Types >
class ThreadGuard {
public:
	typedef std::function< void( Types ... )>	Duty;

private:
	typedef std::thread	Worker;
	
	static void _mock( Types ... ) { return; };	// dumb function to use as default value for the field _duty

	Duty				_duty;
	Worker				* _worker;
	volatile bool		_ended;
	mutable std::mutex	_endMutex;

	ThreadGuard( const ThreadGuard & source ) = delete;
	ThreadGuard & operator=( const ThreadGuard & source ) = delete;
	inline void _setEnded( const bool value );
	inline void _setFields( const Duty & newDuty, Worker * newWorker, const bool status );
	const bool _stop( const bool wait );

public:
	ThreadGuard( void );
	ThreadGuard( const Duty & duty );
	ThreadGuard( ThreadGuard && source );
	~ThreadGuard( void );
	const ThreadGuard & operator=( ThreadGuard && source );
	inline const bool Busy( void ) const;
	inline const bool IsEnded( void ) const;
	bool Set( const Duty & newDuty );
	void Run( Types ... values );
	void Run( const Duty & newDuty, Types ... values );
	inline const bool StopTry( void );
	inline const bool StopWait( void );
};

template< class... Types >
inline void ThreadGuard< Types ... >::_setEnded( const bool value ) {
	this->_endMutex.lock();
	this->_ended = value;
	this->_endMutex.unlock();
}

template< class... Types >
inline void ThreadGuard< Types ... >::_setFields( const Duty & newDuty, Worker * newWorker, const bool status ) {
	this->StopWait();
	this->_setEnded( false );
	this->_duty = newDuty;
	this->_worker = newWorker;
	this->_setEnded( status );
}

template< class... Types >
const bool ThreadGuard< Types ... >::_stop( const bool wait ) {
	if( !this->Busy() )
		return true;

	if( wait )
		while( !this->IsEnded() );
	else if( !this->IsEnded() )
		return false;

	if( this->_worker->joinable() )
		this->_worker->join();

	delete this->_worker;
	this->_worker = NULL;

	return true;
}

template< class... Types >
ThreadGuard< Types ... >::ThreadGuard( void ) :
	_ended( false ),
	_duty( ThreadGuard::_mock ),
	_worker( NULL )
{
	this->_setEnded( true );
}

template< class... Types >
ThreadGuard< Types ... >::ThreadGuard( const Duty & duty ) :
	ThreadGuard()
{
	this->_setEnded( false );
	this->_duty = duty;
	this->_setEnded( true );
}

template< class... Types >
ThreadGuard< Types ... >::ThreadGuard( ThreadGuard && source ) :
	ThreadGuard()
{
	this->_setFields( source._duty, source._worker, source.IsEnded() );
	source._setFields( ThreadGuard::_mock, NULL, true );
}

template< class... Types >
ThreadGuard< Types ... >::~ThreadGuard( void ) {
	this->StopWait();
}

template< class... Types >
const ThreadGuard< Types ... > & ThreadGuard< Types ... >::operator=( ThreadGuard && source ) {
	this->_setFields( source._duty, source._worker, source.IsEnded() );
	source._setFields( ThreadGuard::_mock, NULL, true );
	return *this;
}

template< class... Types >
inline const bool ThreadGuard< Types ... >::Busy( void ) const {
	return NULL != this->_worker;
}

template< class... Types >
inline const bool ThreadGuard< Types ... >::IsEnded( void ) const {
	this->_endMutex.lock();
	const bool	temp( this->_ended );
	this->_endMutex.unlock();
	return temp;
}

template< class... Types >
bool ThreadGuard< Types ... >::Set( const Duty & newDuty ) {
	if( this->Busy() )
		return false;

	this->_duty = newDuty;
	return true;
}

template< class... Types >
void ThreadGuard< Types ... >::Run( Types ... values ) {
	if( !this->Busy() ) {
		this->_setEnded( false );
		this->_worker = new Worker( [ & ]( void ) mutable {
			this->_duty( values ... );
			this->_setEnded( true );
		} );
	}
}

template< class... Types >
void ThreadGuard< Types ... >::Run( const Duty & newDuty, Types ... values ) {
	if( this->Set( newDuty ) )
		this->Run( values ... );
}

template< class... Types >
inline const bool ThreadGuard< Types ... >::StopTry( void ) {
	return this->_stop( false );
}

template< class... Types >
inline const bool ThreadGuard< Types ... >::StopWait( void ) {
	return this->_stop( true );
}
#endif //AIPOVK_BLOCK2_THREADGUARD_H
