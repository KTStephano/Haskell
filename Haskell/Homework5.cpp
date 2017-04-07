#include "Utility.h"

// Question 1

auto myTakeWhile = [&](auto pred, auto xs)
{
	if ( null( xs ) ) return xs;
	if ( pred( head( xs ) ) == false ) return empty( xs );
	return hlist( head( xs ) ) + ::myTakeWhile( pred, tail( xs ) );
};

// Question 2

auto mySpanHelper = [&](auto pred, auto xs, auto acc)
{
	if ( null( xs ) ) return htuple(acc,xs);
	if ( pred( head( xs ) ) == false ) return htuple(acc,xs);
	return ::mySpanHelper( pred, tail( xs ), acc + hlist( head( xs ) ) );
};

auto mySpan = [&]( auto pred, auto xs )
{
	return ::mySpanHelper( pred, xs, empty( xs ) );
};

// Quesiton 3

auto combinations3Helper = [&]( auto x, auto ls )
{
	if ( null( ls ) ) return List<decltype( ls )>();
	if ( length( ls ) < 2 ) return List<decltype( ls )>();
	auto y = head( ls );
	return map( [&]( auto z ) { return hlist( x, y, z ); }, tail( ls ) ) + ::combinations3Helper( x, tail( ls ) );
};

auto combinations3 = [&](auto ls)
{
	if ( null( ls ) ) return List<decltype( ls )>();
	auto x = head( ls );
	return ::combinations3Helper( x, tail( ls ) ) + ::combinations3( tail( ls ) );
};

// Question 4

auto runLengthEncodeHelper = [&](auto item, auto ls, int count = 1)
{
	if ( null( ls ) ) return hlist(htuple( item, count + 1 ));
	if ( item != head( ls ) ) return hlist(htuple( item, count )) + ::runLengthEncodeHelper( head( ls ), tail( ls ) );
	return ::runLengthEncodeHelper( item, tail( ls ), count + 1 );
};

auto runLengthEncode = [&](auto ls)
{
	return ::runLengthEncodeHelper( head( ls ), tail( ls ) );
};

// Question 5

auto runLengthDecode = []( auto ls )
{
	return flatten( map( []( auto tuple ) { return replicate( snd( tuple ), fst( tuple ) ); }, ls ) );
};

int main()
{
	show( myTakeWhile( []( auto x ) { return x != ' '; }, hlist( "This is practice." ) ) );
	show( mySpan( []( auto x ) { return x != ' '; }, hlist( "This is practice." ) ) );
	show( combinations3( hlist( "ABCDE" ) ) );
	show( runLengthEncode( hlist( 4, 2, 2, 1, 1, 1, 1, 4, 4, 4 ) ) );
	show( runLengthDecode( runLengthEncode( hlist( 4, 2, 2, 1, 1, 1, 1, 4, 4, 4 ) ) ) );

	while ( 1 )
		;
	return 0;
}