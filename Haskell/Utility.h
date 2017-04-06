#pragma once

#include <type_traits>
#include "List.h"
#include "Tuple.h"

/**

template<std::size_t Arg, typename T>
const T & _getArgumentHelper( std::size_t argument, const T & curr )
{
	if ( argument == Arg ) return curr;
	throw std::out_of_range( "Index out of range to _getArgumentHelper" );
};

template<std::size_t Arg, typename T, typename ... Elements>
T _getArgumentHelper( std::size_t argument, const T & curr, const Elements & ... elems )
{
	if ( argument == Arg ) return curr;
	return _getArgumentHelper<Arg>( argument, elems... );
};

template<typename T>
List<T> _hlistinitializerlist( const T & last )
{
	return List<T>{last};
}

template<typename T, typename ... Elements>
List<T> _hlistinitializerlist( const T & first, const Elements & ... elements )
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

template<typename ... Strings>
List<String> hlist(const char * str, const Strings * ... strings)
{
	return List<String>{ hlist( str ) } + List<String>{ hlist( strings... ) };
}

template<typename T>
List<T> hlist( const T & last )
{
	return List<T>{last};
}

template<typename T, typename ... Rest>
List<T> hlist( const T & first, const Rest & ... rest )
{
	if ( sizeof...( rest ) > 1 ) return _hlistinitializerlist( first, rest... );
	else return List<T>( first, _getArgumentHelper<1>( 1, rest... ) );
};

template<typename T, typename ... Types>
List<List<T>> hlist(const List<T> & first, const Types & ... rest)
{
	return _hlistinitializerlist( first, rest... );
}
 */

auto hlist()
{
    return List<int>();
}

template<typename T>
auto hlist(const T & last)
{
    return List<T>{last};
}

template<typename T, typename ... Rest>
auto hlist(const T & first, const Rest & ... rest)
{
    return List<T>{first} + hlist(rest...);
};

String hlist( const char * str )
{
    if ( !str[0] ) return String();
    return String{ str[0] } +hlist( str + 1 );
}

template<typename ... Strings>
List<String> hlist(const char * str, const Strings * ... strings)
{
    return List<String>{ hlist( str ) } + List<String>{ hlist( strings... ) };
}

template<typename T>
auto hlsrange(const T & start, const T & end)
{
    return List<T>(start, end);
}

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
void show( T & item )
{
	std::cout << std::boolalpha;
	std::cout << item << std::endl;
}

template<typename T>
void show( T && item )
{
	show( item );
}

template<typename T, typename ... Types>
void show( const T & item, const Types & ... rest )
{
	std::cout << std::boolalpha;
	std::cout << item << " ";
	show( rest... );
};

template<typename T, typename ... Types>
void show( T && item, Types && ... rest )
{
	std::cout << std::boolalpha;
	std::cout << item << " ";
	show( std::forward<Types>( rest )... );
};

template<typename T>
auto replicate( std::size_t times, const T & element )
{
	if ( times == 0 ) return List<T>();
	return hlist( element ) + replicate( times - 1, element );
}

template<typename T>
T max(const T & first, const T & second)
{
	return first > second ? first : second;
}

template<typename Lambda, typename T>
auto zipWith( Lambda proc, const List<T> & first, const List<T> & second)
{
    if ( null( first ) || null( second ) ) return List<T>();
    return hlist( proc( head( first ), head( second ) ) ) + zipWith( proc, tail( first ), tail( second ) );
}