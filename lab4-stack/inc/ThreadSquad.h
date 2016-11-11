//
// Created by kryvashek on 10.11.16.
//

#ifndef LAB4_STACK_THREADSQUAD_H
#define LAB4_STACK_THREADSQUAD_H

#include <queue>
#include <vector>
#include <list>
#include "ThreadGuard.h"

using	std::queue;
using	std::list;

class ThreadSquad {
public:
	typedef function< void( void ) >	Routine;

	queue< Routine >	Tasks;

private:
	typedef ThreadGuard<>	Member;
	typedef	list< Member >	Squad;
	
	const size_t	_amount;
	Squad			_passive,
					_active;

	ThreadSquad( const ThreadSquad & source ) = delete;
	ThreadSquad & operator=( const ThreadSquad & source ) = delete;
	static const size_t _threadsCount( void );
	static const size_t _threadsCountOptimum( const size_t amount );
	inline void _move( Squad & source, Squad::iterator & member, Squad & destination );
	inline const bool _braking( void );
	inline void _speedup( void );

public:
	ThreadSquad( void );
	ThreadSquad( const size_t amount, const bool optimize = false );
	ThreadSquad( ThreadSquad && source );
	~ThreadSquad( void );
	inline const bool Busy( void ) const;
	void Run( void );
	const bool Stop( const bool wait = false );
};

const size_t ThreadSquad::_threadsCount( void ) {
	const size_t	amount( thread::hardware_concurrency() );

	return amount < 2 ? 2 : amount;
}

const size_t ThreadSquad::_threadsCountOptimum( const size_t amount ) {
	const size_t	optimal( ThreadSquad::_threadsCount() );

	return optimal < amount ? optimal : amount;
}

inline void ThreadSquad::_move( Squad & source, Squad::iterator & member, Squad & destination ) {
	destination.emplace_back( move( *member ) );
	member = source.erase( member );
}

inline const bool ThreadSquad::_braking( void ) {
	Squad::iterator	member;

	for( member = this->_active.begin(); member != this->_active.end(); )
		if( member->Stop() )
			this->_move( this->_active, member, this->_passive );
		else
			member++;

	return !this->Busy();
}

inline void ThreadSquad::_speedup( void ) {
	Squad::iterator	member;

	for( member = this->_passive.begin(); member != this->_passive.end() && !this->Tasks.empty(); ) {
		member->Run( this->Tasks.front() );
		this->_move( this->_passive, member, this->_active );
		this->Tasks.pop();
	}
}

ThreadSquad::ThreadSquad( void ) :
	_amount( ThreadSquad::_threadsCount() ),
	_passive( this->_amount )
{}

ThreadSquad::ThreadSquad( const size_t amount, const bool optimize ) :
	_amount( optimize ? ThreadSquad::_threadsCountOptimum( amount ) : ThreadSquad::_threadsCount() ),
	_passive( this->_amount )
{}

ThreadSquad::ThreadSquad( ThreadSquad && source ) :
	_amount( source._amount ),
	_passive( move( source._passive ) ),
	_active( move( source._active ) )
{
	source._passive.clear();
	source._active.clear();
}

ThreadSquad::~ThreadSquad( void ) {
	this->Stop( true );
}

inline const bool ThreadSquad::Busy( void ) const {
	return !this->_active.empty();
}

void ThreadSquad::Run( void ) {
	while( !this->Tasks.empty() ) {
		this->_braking();
		this->_speedup();
	}
}

const bool ThreadSquad::Stop( const bool wait ) {
	while( !this->_braking() )
		if( !wait )
			return false;

	return true;
}

#endif //LAB4_STACK_THREADSQUAD_H
