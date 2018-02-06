#pragma once

#include <type_traits>
#include "HList.h"
#include "HTuple.h"
#include "Integer.h"

HList<int> hlist();

template<typename T>
auto hlist( const T & last )
{
	return HList<T>{last};
}

template<typename T, typename ... Rest>
auto hlist( const T & first, const Rest & ... rest )
{
	return first + hlist( rest... );
};

template<typename T>
String hlist( const T * str )
{
	if ( !str[0] ) return String();
	return str[0] + hlist( str + 1 );
}

template<typename T, typename ... Strings>
HList<String> hlist( const T * str, const Strings * ... strings )
{
	return hlist( str ) + HList<String>{ hlist( strings... ) };
}

template<typename T>
auto hlsrange( const T & start, const T & end )
{
	return HList<T>( start, end );
}

template<typename T>
auto hlsrange( const T & start, const T & next, const T & end )
{
	return HList<T>( start, next, end );
}

template<typename ... Types>
HTuple<Types...> htuple( const Types & ... args )
{
	return HTuple<Types...>( args... );
}

template<class Lambda, typename T>
HList<T> filter( Lambda pred, const HList<T> & list )
{
	if ( list.isNull() ) return HList<T>();
	else if ( pred( list.head() ) ) return list.head() +filter( pred, list.tail() );
	else return filter( pred, list.tail() );
}

template<class Lambda, typename T, typename R>
HList<R> __map( Lambda proc, const HList<T> & list, const HList<R> & acc )
{
	if ( list.isNull() ) return acc;
	return __map( proc, tail( list ), acc + HList<R>{proc( list.head() )} );
}

template<class Lambda, typename T, typename R = typename std::result_of<Lambda( T )>::type>
HList<R> map( Lambda proc, const HList<T> & list )
{
	//if ( list.isNull() ) return List<R>();
	//return List<R>{ proc( list.head() ) } +map( proc, list.tail() );
	return __map( proc, list, HList<R>() );
}

template<class Lambda, typename T, typename K>
HList<K> select( Lambda pred, const HList<T> & predList, const HList<K> & pollList )
{
	if ( predList.isNull() ) return HList<K>();
	else if ( pred( predList.head() ) )
	{
		return HList<K>{pollList.head()} +select( pred, predList.tail(), pollList.tail() );
	}
	return select( pred, predList.tail(), pollList.tail() );
}

template<typename T, typename K>
HList<HTuple<T, K>> zip( const HList<T> & first, const HList<K> & second )
{
	if ( first.isNull() ) return HList<HTuple<T, K>>();
	return hlist( htuple( first.head(), second.head() ) ) + zip( first.tail(), second.tail() );
};

template<class Lambda1, class Lambda2>
auto compose( Lambda1 f, Lambda2 g )
{
	return [=]( auto ... xs ) { return f( g( xs... ) ); };
}


template<typename T>
void show( const T & item )
{
	std::cout << std::boolalpha;
	std::cout << item << std::endl;
}

template<typename T, typename ... Types>
void show( const T & item, const Types & ... rest )
{
	std::cout << std::boolalpha;
	std::cout << item << " ";
	show( rest... );
};

template<typename T>
auto replicate( std::size_t times, const T & element )
{
	if ( times == 0 ) return HList<T>();
	return hlist( element ) + replicate( times - 1, element );
}

template<typename T>
auto replicate( std::size_t times )
{
	return [=]( const T & elem ) { return ::replicate<T>( times, elem ); };
}

template<typename T>
T max( const T & first, const T & second )
{
	return first > second ? first : second;
}

template<typename Lambda, typename T>
auto zipWith( Lambda proc, const HList<T> & first, const HList<T> & second )
{
	if ( null( first ) || null( second ) ) return HList<T>();
	return hlist( proc( head( first ), head( second ) ) ) + zipWith( proc, tail( first ), tail( second ) );
}

template<typename Lambda, typename T, typename K>
auto foldr( Lambda proc, const K & seed, const HList<T> & list )
{
	if ( null( list ) ) return K( seed );
	return proc( head( list ), foldr( proc, seed, tail( list ) ) );
};

template<typename Lambda, typename T, typename K>
auto foldl( Lambda proc, const K & acc, const HList<T> & list )
{
	if ( null( list ) ) return K( acc );
	return foldl( proc, proc( head( list ), acc ), tail( list ) );
};

// concat :: [[a]] -> [a]
template<typename T>
auto concat( const HList<T> & list )
{
	if ( null( list ) ) return T();
	return head( list ) + concat( tail( list ) );
}

// identity :: a -> (b -> a)
template<typename T>
auto identity( const T & item )
{
	return [=]( auto a ) { return item; };
}

// concatMap
template<typename Lambda, typename T>
auto concatMap( Lambda proc, const HList<T> & list )
{
	return concat( map( proc, list ) );
}

// and :: [Bool] -> Bool
template<typename T = bool>
auto andf( const HList<T> & list )
{
	if ( null( list ) ) return true;
	if ( head( list ) != true ) return false;
	return andf( tail( list ) );
}