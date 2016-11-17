//
// Created by kryvashek on 16.11.16.
//

#ifndef LAB4_STACK_EXECUTOR_H
#define LAB4_STACK_EXECUTOR_H

#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <ctime>
#include <locale>

using	std::istream;
using	std::ostream;
using	std::string;

template< typename ResType, class... ArgTypes >
class Executor {
public:
	typedef std::function< ResType( istream &, string &, ArgTypes... )>	Routine;

private:
	typedef std::map< string, Routine >	Coverage;
	typedef std::time_put< char >		TimePutter;

	static ResType _mock( istream & input, string & message, ArgTypes... ) { message = "Default exit"; return ResType(); };

	const TimePutter	& _tmput;
	istream				& _input;
	ostream				& _output;
	Coverage			_scope;

	Executor( const Executor & source ) = delete;
	Executor( Executor && source ) = delete;
	Executor & operator=( Executor && source ) = delete;

	const struct timespec _getMoment( void ) const;
	void _report( const string & message );

public:
	Executor( void );
	Executor( istream & input, ostream & output );
	~Executor( void );
	const Executor & operator=( const Executor & source );
	const Executor & operator+=( const Executor & source );
	void AddRoutine( const string & id, const Routine & routine );
	void SetInput( istream & input );
	void SetOutput( ostream & output );
	void Run( ArgTypes ... values  );

};

template< typename ResType, class... ArgTypes >
const struct timespec Executor< ResType, ArgTypes... >::_getMoment( void ) const {
	struct timespec	moment = { 0 };

	clock_gettime( CLOCK_REALTIME, &moment );
	return moment;
}

template< typename ResType, class... ArgTypes >
void Executor< ResType, ArgTypes... >::_report( const string & message ) {
	static const long		thousand( static_cast< long >( 1e3 ) );
	static const string		_pattern( "%F %T" );
	static const char		* start( _pattern.data() ),
							* finish( start + _pattern.length() );
	const struct timespec	moment( this->_getMoment() );

	this->_tmput.put( this->_output, this->_output, ' ', std::localtime( &( moment.tv_sec ) ), start, finish );
	this->_output << "." << moment.tv_nsec / thousand << " : " << message << std::endl;
}

template< typename ResType, class... ArgTypes >
Executor< ResType, ArgTypes... >::Executor( void ) :
	_input( std::cin ),
	_output( std::cout ),
	_tmput( std::use_facet< TimePutter > ( std::locale() ) )
{
	this->_report( "Executor constructed" );
	this->AddRoutine( "exit", Executor< ResType, ArgTypes... >::_mock );
}

template< typename ResType, class... ArgTypes >
Executor< ResType, ArgTypes... >::Executor( istream & input, ostream & output ) :
	_input( input ),
	_output( output ),
	_tmput( std::use_facet< TimePutter > ( std::locale() ) )
{
	this->_report( "Executor constructed" );
	this->AddRoutine( "exit", Executor< ResType, ArgTypes... >::_mock );
}

template< typename ResType, class... ArgTypes >
Executor< ResType, ArgTypes... >::~Executor( void ) {
	this->_report( "Destructing executor" );
	this->_output.flush();
}

template< typename ResType, class... ArgTypes >
const Executor< ResType, ArgTypes... > & Executor< ResType, ArgTypes... >::operator=( const Executor & source ) {
	this->_scope = source._scope;
}

template< typename ResType, class... ArgTypes >
const Executor< ResType, ArgTypes... > & Executor< ResType, ArgTypes... >::operator+=( const Executor & source ) {
	Coverage::const_iterator	from;

	for( from = source._scope.begin(); from != source._scope.end(); from++ )
		this->_scope[ from->first ] = from->second;
}

template< typename ResType, class... ArgTypes >
void Executor< ResType, ArgTypes... >::AddRoutine( const string & id, const Routine & routine ) {
	this->_scope[ id ] = routine;
	this->_report( "New routine '" + id + "' set" );
}

template< typename ResType, class... ArgTypes >
void Executor< ResType, ArgTypes... >::SetInput( istream & input ) {
	this->_input = input;
	this->_report( "Input stream changed" );
}

template< typename ResType, class... ArgTypes >
void Executor< ResType, ArgTypes... >::SetOutput( ostream & output ) {
	this->_report( "Changing output stream" );
	this->_output.flush();
	this->_output = output;
	this->_report( "Output stream changed" );
}

template< typename ResType, class... ArgTypes >
void Executor< ResType, ArgTypes... >::Run( ArgTypes ... values ) {
	string	temp,
			message;
	ResType	result;

	while( this->_input.good() ) {
		this->_input >> temp;

		if( 0 == this->_scope.count( temp ) )
			this->_report( "Unknown lexeme '" + temp + "'" );
		else {
			message.clear();
			result = this->_scope[ temp ]( this->_input, message, values ... );
			this->_report( "Completed '" + temp + "', " + std::to_string( result ) + ", " + message );
		}

		if( temp == "exit" )
			break;
	}
}

#endif //LAB4_STACK_EXECUTOR_H
