//
// Created by kryvashek on 10.11.16.
//

#ifndef AIPOVK_BLOCK2_THREADGUARD_H
#define AIPOVK_BLOCK2_THREADGUARD_H

#include <thread>
#include <functional>

using	std::thread;
using	std::function;
using	std::move;

template< class... Types >
class ThreadGuard {
public:
	typedef function< void( Types ... )>	Routine;

private:
	static void _mock( Types ... ) { return; };	// dumb function to use as default value for the field _routine

	Routine	_routine;
	thread	* _performer;

	ThreadGuard( const ThreadGuard & source ) = delete;
	ThreadGuard & operator=( const ThreadGuard & source ) = delete;

public:
	ThreadGuard( void );
	ThreadGuard( const Routine & routine, const bool run = false );
	ThreadGuard( ThreadGuard && source );
	~ThreadGuard( void );
	ThreadGuard & operator=( ThreadGuard && source );
	inline const bool Busy( void ) const;
	bool Set( const Routine & newRoutine );
	void Run( Types ... values );
	void Run( const Routine & newRoutine, Types ... values );
	bool Stop( const bool wait = false );
};

template< class... Types >
ThreadGuard< Types ... >::ThreadGuard( void ) :
	_routine( ThreadGuard::_mock ),
	_performer( NULL )
{}

template< class... Types >
ThreadGuard< Types ... >::ThreadGuard( const Routine & routine, const bool run ) :
	_routine( routine ),
	_performer( NULL )
{
	if( run )
		this->Run();
}

template< class... Types >
ThreadGuard< Types ... >::ThreadGuard( ThreadGuard && source ) :
	_routine( move( source._routine ) ),
	_performer( move( source._performer ) )
{
	source._routine = move( ThreadGuard::_mock );
	source._performer = NULL;
}

template< class... Types >
ThreadGuard< Types ... >::~ThreadGuard( void ) {
	this->Stop( true );
}

template< class... Types >
ThreadGuard< Types ... > & ThreadGuard< Types ... >::operator=( ThreadGuard && source ) {
	this->_routine = move( source._routine );
	this->_performer = move( source._performer );
	source._routine = move( ThreadGuard::_mock );
	source._performer = NULL;
	return *this;
}

template< class... Types >
inline const bool ThreadGuard< Types ... >::Busy( void ) const {
	return NULL != this->_performer;
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
		this->_performer = new thread( this->_routine, values ... );
}

template< class... Types >
void ThreadGuard< Types ... >::Run( const Routine & newRoutine, Types ... values ) {
	if( this->Set( newRoutine ) )
		this->Run( values ... );
}

template< class... Types >
bool ThreadGuard< Types ... >::Stop( const bool wait ) {
	if( !this->Busy() )
		return true;

	while( !this->_performer->joinable() )
		if( !wait )
			return false;

	this->_performer->join();
	delete this->_performer;
	this->_performer = NULL;
	return true;
}

#endif //AIPOVK_BLOCK2_THREADGUARD_H
