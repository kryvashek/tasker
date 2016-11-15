//
// Created by kryvashek on 10.11.16.
//

#ifndef AIPOVK_BLOCK2_THREADGUARD_H
#define AIPOVK_BLOCK2_THREADGUARD_H

#include <thread>
#include <functional>
#include <mutex>

using	std::thread;
using	std::function;
using	std::move;
using	std::mutex;

template< class... Types >
class ThreadGuard {
public:
	typedef function< void( Types ... )>	Routine;

private:
	static void _mock( Types ... ) { return; };	// dumb function to use as default value for the field _routine

	Routine			_routine;
	thread			* _performer;
	volatile bool	_ended;
	mutable mutex	_endMutex;

	ThreadGuard( const ThreadGuard & source ) = delete;
	ThreadGuard & operator=( const ThreadGuard & source ) = delete;
	inline void _setEnded( const bool value );
	inline void _setFields( const Routine & newRoutine, thread * performer, const bool status );
	const bool _stop( const bool wait );

public:
	ThreadGuard( void );
	ThreadGuard( const Routine & routine );
	ThreadGuard( ThreadGuard && source );
	~ThreadGuard( void );
	ThreadGuard & operator=( ThreadGuard && source );
	inline const bool Busy( void ) const;
	inline const bool IsEnded( void ) const;
	bool Set( const Routine & newRoutine );
	void Run( Types ... values );
	void Run( const Routine & newRoutine, Types ... values );
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
inline void ThreadGuard< Types ... >::_setFields( const Routine & newRoutine, thread * performer, const bool status ) {
	this->_setEnded( false );
	this->_routine = newRoutine;
	this->_performer = performer;
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

	if( this->_performer->joinable() )
		this->_performer->join();

	delete this->_performer;
	this->_performer = NULL;

	return true;
}

template< class... Types >
ThreadGuard< Types ... >::ThreadGuard( void ) {
	this->_setFields( ThreadGuard::_mock, NULL, true );
}

template< class... Types >
ThreadGuard< Types ... >::ThreadGuard( const Routine & routine ) {
	this->_setFields( routine, NULL, true );
}

template< class... Types >
ThreadGuard< Types ... >::ThreadGuard( ThreadGuard && source ) {
	this->_setFields( source._routine, source._performer, source.IsEnded() );
	source._setFields( ThreadGuard::_mock, NULL, true );
}

template< class... Types >
ThreadGuard< Types ... >::~ThreadGuard( void ) {
	this->StopWait();
}

template< class... Types >
ThreadGuard< Types ... > & ThreadGuard< Types ... >::operator=( ThreadGuard && source ) {
	this->_setFields( source._routine, source._performer, source.IsEnded() );
	source._setFields( ThreadGuard::_mock, NULL, true );
	return *this;
}

template< class... Types >
inline const bool ThreadGuard< Types ... >::Busy( void ) const {
	return NULL != this->_performer;
}

template< class... Types >
inline const bool ThreadGuard< Types ... >::IsEnded( void ) const {
	this->_endMutex.lock();
	const bool	temp( this->_ended );
	this->_endMutex.unlock();
	return temp;
}

template< class... Types >
bool ThreadGuard< Types ... >::Set( const Routine & newRoutine ) {
	if( this->Busy() )
		return false;

	this->_routine = newRoutine;
	return true;
}

template< class... Types >
void ThreadGuard< Types ... >::Run( Types ... values ) {
	if( !this->Busy() )
		this->_performer = new thread( [ &, this, values ... ]( void ) mutable {
			this->_setEnded( false );
			this->_routine( values ... );
			this->_setEnded( true );
		} );
}

template< class... Types >
void ThreadGuard< Types ... >::Run( const Routine & newRoutine, Types ... values ) {
	if( this->Set( newRoutine ) )
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
