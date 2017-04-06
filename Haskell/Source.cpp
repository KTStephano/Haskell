#include <iostream>
#include <cmath>
#include "Tuple.h"
#include "List.h"
#include "Utility.h"

template<typename Lambda, typename T>
auto myTakeWhile( Lambda pred, const List<T> & input, const List<T> & acc = List<T>() )
{
	if ( null( input ) ) return reverse( acc );
	else if ( pred( head( input ) ) == false ) return reverse( acc );
	return myTakeWhile( pred, tail( input ), hlist( head( input ) ) + acc );
};

template<typename Lambda, typename T>
auto mySpan( Lambda pred, const List<T> & input, const List<T> & acc = List<T>() )
{
	if ( null( input ) ) return htuple( reverse( acc ), List<T>() );
	else if ( pred( input.head() ) == false ) return htuple( reverse( acc ), input );
	return mySpan( pred, tail( input ), hlist( head( input ) ) + acc );
};

template<typename T>
auto quicksort(const List<T> & list)
{
	if ( null( list ) ) return List<T>();
	auto x = head( list );
	auto smallerSorted = quicksort( filter( [&x]( auto a ) { return a <= x; }, tail( list ) ) );
	auto biggerSorted = quicksort( filter( [&x]( auto a ) { return a > x; }, tail( list ) ) );
	return smallerSorted + hlist( x ) + biggerSorted;
}

template<typename Lambda, typename T>
auto zipWith( Lambda proc, const List<T> & first, const List<T> & second)
{
	if ( null( first ) || null( second ) ) return List<T>();
	return hlist( proc( head( first ), head( second ) ) ) + zipWith( proc, tail( first ), tail( second ) );
}

auto factorial = []( long long int n ) { return product( hlist( 1LL, n ) ); };

auto even = []( auto i ) { return i % 2 == 0; };

int main()
{
	show( myTakeWhile( []( char i ) { return i != ' '; }, hlist( "this is Practice." ) ) );
	show( mySpan( []( char i ) { return i != ' '; }, hlist( "this is Practice." ) ) );
	show( hlist( 1, 10 ), hlist( 'a', 'z' ) );
	show( elem( 1, hlist( 1, 10 ) ) );
	show( product( hlist( 1, 10 ) ) );
	show( replicate( 4, 'h' ) );
	show( take( 3, hlist( 5, 4, 3, 2, 1 ) ) );
	show( drop( 100, hlist( 1, 2, 3, 4 ) ) );
	show( minimum( hlist( 8, 4, 2, 1, 5, 6 ) ) );
	show( maximum( hlist( 1, 9, 2, 3, 4 ) ) );
	show( null( hlist() ) );
	show( null( hlist( 1, 2, 3 ) ) );
	show( hlist( "hello", "goodbye", "what" ) );
	show( factorial(50) );
	show( fst ( htuple( true, hlist("hello") ) ) );
	show( snd ( htuple( true, hlist("hello") ) ) );
	show( hlist( 3, 2, 1 ) > hlist( 2, 1, 0 ) );
	show( hlist( 2, 2, 1 ) > hlist( 2, 10, 100 ) );
	show( hlist( 3, 4, 2 ) >= hlist( 3, 4, 2 ) );
	show( hlist( 3, 4, 2 ) == hlist( 3, 4, 2 ) );
	show( hlist( "Abrakadabra" ) < hlist( "Zebra" ) );
	show( quicksort( hlist( 10, 2, 5, 3, 1, 6, 7, 4, 2, 3, 4, 8, 9 ) ) );
	show( quicksort( hlist( "the quick brown fox jumps over the lazy dog" ) ) );
	show( max( 1, 2 ) );
	show( zipWith( []( auto x, auto y ) { return x + y; }, hlist( 4, 2, 5, 6 ), hlist( 2, 6, 2, 3 ) ) );
	show( zipWith (max<int>, hlist( 6, 3, 2, 1 ), hlist( 7, 3, 1, 5 ) ));
	show( zipWith( []( auto x, auto y ) { return x + y; }, hlist( "foo ", "bar ", "baz " ), hlist( "fighters", "hoppers", "aldrin" ) ) );
	show( zipWith( []( auto x, auto y ) { return x * y; }, replicate( 5, 2 ), hlist( 1, 10000 ) ) );
	show( hlist( hlist( 1, 2, 3 ) ) );
	show( map( []( auto x ) { return x + 3; }, hlist( 1, 5, 3, 1, 6 ) ) );
	show( map( []( auto x ) { return x + hlist( "!" ); }, hlist( "BIFF", "BANG", "POW" ) ) );
	show( map( []( auto x ) { return replicate( 3, x ); }, hlist( 3, 6 ) ) );
	show( map( []( auto x ) { return map( []( auto y ) { return std::pow( y, 2 ); }, x ); },
			   hlist( hlist( 1, 2 ), 
					  hlist( 3, 4, 5, 6 ), 
					  hlist( 7, 8 ) ) ) );
	show( map( []( auto x )
	{
		return fst( x );
	}, List<Tuple<int, int>>{htuple( 1, 2 ), htuple( 3, 5 ), htuple( 6, 3 ), htuple( 2, 6 ), htuple( 2, 5 )} ) );
	show( hlist( hlist( 1, 2 ), hlist( 3, 7 ), hlist( 4, 5 ) ) );
	List<List<int>> list{ hlist( 1,2 ), hlist( 1,2,3 ),hlist( 1,2,3,4 ) };
	show( list );

	while ( 1 )
		;
	return 0;
}