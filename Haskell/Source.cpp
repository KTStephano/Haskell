#include <iostream>
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

auto even = []( auto i ) { return i % 2 == 0; };

int main()
{
	print( myTakeWhile( []( char i ) { return i != ' '; }, hlist( "this is Practice." ) ) );
	print( mySpan( []( char i ) { return i != ' '; }, hlist( "this is Practice." ) ) );
	print( hlist( 1, 10 ), hlist( 'a', 'z' ) );
	print( elem( 1, hlist( 1, 10 ) ) );
	print( product( hlist( 1, 10 ) ) );
	print( replicate( 4, 'h' ) );
	print( take( 3, hlist( 5, 4, 3, 2, 1 ) ) );
	print( drop( 100, hlist( 1, 2, 3, 4 ) ) );
	print( minimum( hlist( 8, 4, 2, 1, 5, 6 ) ) );
	print( maximum( hlist( 1, 9, 2, 3, 4 ) ) );
	print( null( hlist() ) );
	print( null( hlist( 1, 2, 3 ) ) );

	while ( 1 )
		;
	return 0;
}