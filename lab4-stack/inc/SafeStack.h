//
// Created by kryvashek on 17.11.16.
//

#ifndef LAB4_STACK_SAFESTACK_H
#define LAB4_STACK_SAFESTACK_H

#include <stack>
#include <mutex>
#include <list>

template< class EntryType >
class SafeStack : protected std::stack< EntryType, std::list< EntryType > > {
private:
	typedef	std::stack< EntryType, std::list< EntryType > >	Base;

	mutable std::mutex	_stackMutex;

public:
	~SafeStack( void );
	typename Base::reference top( void );
	typename Base::const_reference top( void ) const;
	void push( const typename Base::value_type & val );
	void push( typename Base::value_type && val );
	void pop( void );
	bool empty( void ) const;
	typename Base::size_type size( void ) const;
};


template< class EntryType >
SafeStack< EntryType >::~SafeStack( void ) {
	this->_stackMutex.lock();
}

template< class EntryType >
typename SafeStack< EntryType >::Base::reference SafeStack< EntryType >::top( void ) {
	this->_stackMutex.lock();
	typename Base::reference	result( Base::top() );
	this->_stackMutex.unlock();
	return result;
}

template< class EntryType >
typename SafeStack< EntryType >::Base::const_reference SafeStack< EntryType >::top( void ) const {
	this->_stackMutex.lock();
	typename Base::const_reference	result( Base::top() );
	this->_stackMutex.unlock();
	return result;
}

template< class EntryType >
void SafeStack< EntryType >::push( const typename Base::value_type & val ) {
	this->_stackMutex.lock();
	Base::push( val );
	this->_stackMutex.unlock();
}

template< class EntryType >
void SafeStack< EntryType >::push( typename Base::value_type && val ) {
	this->_stackMutex.lock();
	Base::push( val );
	this->_stackMutex.unlock();
}

template< class EntryType >
void SafeStack< EntryType >::pop( void ) {
	this->_stackMutex.lock();
	Base::pop();
	this->_stackMutex.unlock();
}

template< class EntryType >
bool SafeStack< EntryType >::empty( void ) const {
	this->_stackMutex.lock();
	bool	result( Base::empty() );
	this->_stackMutex.unlock();
	return result;
}

template< class EntryType >
typename SafeStack< EntryType >::Base::size_type SafeStack< EntryType >::size( void ) const {
	this->_stackMutex.lock();
	typename Base::size_type	result( Base::size() );
	this->_stackMutex.unlock();
	return result;
}

#endif //LAB4_STACK_SAFESTACK_H
