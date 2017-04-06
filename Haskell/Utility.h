#pragma once

#include <type_traits>
#include <typeinfo>
#include "List.h"
#include "Tuple.h"

template<std::size_t Arg, typename T>
T _getArgumentHelper( std::size_t argument, T curr )
{
	if ( argument == Arg ) return curr;
	throw std::out_of_range( "Index out of range to _getArgumentHelper" );
};

template<std::size_t Arg, typename T, typename ... Elements>
T _getArgumentHelper( std::size_t argument, T curr, Elements ... elems )
{
	if ( argument == Arg ) return curr;
	return _getArgumentHelper<Arg>( argument, elems... );
};

template<typename T>
List<T> _hlistinitializerlist( T last )
{
	return List<T>{last};
}

template<typename T, typename ... Elements>
List<T> _hlistinitializerlist( T first, Elements ... elements )
{
	return List<T>{first} +_hlistinitializerlist( elements... );
};

List<int> hlist()
{
	return List<int>();
}

String hlist( const char * str )
{
	if ( !str[0] ) return String();
	return String{ str[0] } +hlist( str + 1 );
}

template<typename T>
List<T> hlist( T last )
{
	return List<T>{last};
}

template<typename T, typename ... Rest>
List<T> hlist( T first, Rest ... rest )
{
	if ( sizeof...( rest ) > 1 ) return _hlistinitializerlist( first, rest... );
	else return List<T>( first, _getArgumentHelper<1>( 1, rest... ) );
};

template<typename ... Types>
Tuple<Types...> htuple( const Types & ... args )
{
	return Tuple<Types...>( args... );
}

template<class Lambda, typename T>
List<T> filter( Lambda pred, const List<T> & list )
{
	if ( list.isNull() ) return List<T>();
	else if ( pred( list.head() ) ) return List<T>{list.head()} +filter( pred, list.tail() );
	else return filter( pred, list.tail() );
}

template<class Lambda, typename T, typename R = typename std::result_of<Lambda( T )>::type>
List<R> map( Lambda proc, const List<T> & list )
{
	if ( list.isNull() ) return List<R>();
	return List<R>{ proc( list.head() ) } +map( proc, list.tail() );
}

template<class Lambda, typename T, typename K>
List<K> select( Lambda pred, const List<T> & predList, const List<K> & pollList )
{
	if ( predList.isNull() ) return List<K>();
	else if ( pred( predList.head() ) )
	{
		return List<K>{pollList.head()} +select( pred, predList.tail(), pollList.tail() );
	}
	return select( pred, predList.tail(), pollList.tail() );
}

template<typename T, typename K>
List<Tuple<T, K>> zip( const List<T> & first, const List<K> & second )
{
	if ( first.isNull() ) return List<Tuple<T, K>>();
	return hlist( htuple( first.head(), second.head() ) ) + zip( first.tail(), second.tail() );
};

template<typename T, class Function1, class Function2, typename R = typename std::result_of<Function1( T )>::type>
auto compose( Function1 f, Function2 g )
{
	return [f, g]( T i ) -> R { return f( g( i ) ); };
}


template<typename T>
void print( T & item )
{
	std::cout << std::boolalpha;
	std::cout << item << std::endl;
}

template<typename T>
void print( T && item )
{
	print( item );
}

template<typename T, typename ... Types>
void print( const T & item, const Types & ... rest )
{
	std::cout << std::boolalpha;
	std::cout << item << " ";
	print( rest... );
};

template<typename T, typename ... Types>
void print( T && item, Types && ... rest )
{
	std::cout << std::boolalpha;
	std::cout << item << " ";
	print( std::forward<Types>( rest )... );
};

template<typename T>
auto replicate( std::size_t times, const T & element )
{
	if ( times == 0 ) return List<T>();
	return hlist( element ) + replicate( times - 1, element );
}