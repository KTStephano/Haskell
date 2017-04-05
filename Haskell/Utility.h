#pragma once

#include "List.h"

template<typename T, typename ... Args>
List<T> HList(T first, Args ... rest)
{
	return List<T>( first, rest... );
}

template<class Lambda, typename T>
List<T> filter( Lambda pred, const List<T> & list )
{
	if ( list.isNull() ) return List<T>();
	else if ( pred( list.head() ) ) return List<T>{list.head()} + filter( pred, list.tail() );
	else return filter( pred, list.tail() );
}

template<class Lambda, typename T, typename R = std::result_of<Lambda(T)>::type>
List<R> map(Lambda proc, const List<T> & list) {
	if ( list.isNull() ) return List<R>();
	return List<R>{ proc( list.head() ) } + map( proc, list.tail() );
}

template<class Lambda, typename T, typename K>
List<K> select(Lambda pred, const List<T> & predList, const List<K> & pollList) {
	if ( predList.isNull() ) return List<K>();
	else if ( pred( predList.head() ) ) {
		return List<K>{pollList.head()} + select( pred, predList.tail(), pollList.tail() );
	}
	return select( pred, predList.tail(), pollList.tail() );
}

template<typename T, class Function1, class Function2, typename R = std::result_of<Function1( T )>::type>
auto compose( Function1 f, Function2 g )
{
	return [f, g]( T i ) -> R { return f( g( i ) ); };
}