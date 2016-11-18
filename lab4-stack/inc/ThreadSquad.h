//
// Created by kryvashek on 10.11.16.
//

#ifndef LAB4_STACK_THREADSQUAD_H
#define LAB4_STACK_THREADSQUAD_H

#include <queue>
#include <list>
#include <mutex>
#include <ThreadGuard.h>

class ThreadSquad {
public:
	typedef std::function< void( void ) >	Task;

private:
	typedef ThreadGuard<>							Member;
	typedef	std::list< Member >						Squad;
	typedef std::queue< Task, std::list< Task > >	Schedule;

	Schedule	_tasks;
	Squad		_team;
	std::mutex	_tasksMutex;

	ThreadSquad( const ThreadSquad & source ) = delete;
	ThreadSquad( ThreadSquad && source ) = delete;
	ThreadSquad & operator=( const ThreadSquad & source ) = delete;
	ThreadSquad & operator=( ThreadSquad && source ) = delete;

	static const size_t _threadsCount( void );
	static const size_t _threadsCountOptimum( const size_t amount );
	inline const bool _getTask( Task & task );

public:
	ThreadSquad( void );
	ThreadSquad( const size_t amount, const bool optimize = false );
	~ThreadSquad( void );
	const bool Busy( void );
	void Run( void );
	const bool StopTry( void );
	const bool StopWait( void );
	void AddTask( const Task & task );
};

const size_t ThreadSquad::_threadsCount( void ) {
	const size_t	amount( std::thread::hardware_concurrency() );

	return amount < 2 ? 2 : amount;
}

const size_t ThreadSquad::_threadsCountOptimum( const size_t amount ) {
	const size_t	optimal( ThreadSquad::_threadsCount() );

	return optimal < amount ? optimal : amount;
}

inline const bool ThreadSquad::_getTask( ThreadSquad::Task & task ) {
	this->_tasksMutex.lock();
	const bool	has( !this->_tasks.empty() );
	if( has ) {
		task = move( this->_tasks.front() );
		this->_tasks.pop();
	}
	this->_tasksMutex.unlock();
	return has;
}

ThreadSquad::ThreadSquad( void ) :
	_team( ThreadSquad::_threadsCount() )
{}

ThreadSquad::ThreadSquad( const size_t amount, const bool optimize ) :
	_team( optimize ? ThreadSquad::_threadsCountOptimum( amount ) : amount )
{}

ThreadSquad::~ThreadSquad( void ) {
	this->StopWait();
}

const bool ThreadSquad::Busy( void ) {
	Squad::iterator	member;
	size_t 			count( 0 );

	for( member = this->_team.begin(); member !=  this->_team.end(); member++ )
		if( !member->StopTry() )
			count++;

	return count > 0;
}

void ThreadSquad::Run( void ) {
	Squad::iterator	member;

	for( member = this->_team.begin(); member !=  this->_team.end(); member++ )
		member->Run( [ this ]( void ) mutable {
			ThreadSquad::Task	task;
			while( this->_getTask( task ) )
				task();
		} );
}

const bool ThreadSquad::StopTry( void ) {
	return !this->Busy();
}

const bool ThreadSquad::StopWait( void ) {
	while( this->Busy() );

	return true;
}

void ThreadSquad::AddTask( const Task & task ) {
	this->_tasksMutex.lock();
	this->_tasks.push( task );
	this->_tasksMutex.unlock();
}

#endif //LAB4_STACK_THREADSQUAD_H
