#include <iostream>
#include <cmath>
#include "Prelude.h"

template<typename Lambda, typename T>
auto myTakeWhile( Lambda pred, const HList<T> & input, const HList<T> & acc = HList<T>() )
{
	if ( null( input ) ) return reverse( acc );
	else if ( pred( head( input ) ) == false ) return reverse( acc );
	return myTakeWhile( pred, tail( input ), hlist( head( input ) ) + acc );
};

template<typename Lambda, typename T>
auto mySpan( Lambda pred, const HList<T> & input, const HList<T> & acc = HList<T>() )
{
	if ( null( input ) ) return htuple( reverse( acc ), HList<T>() );
	else if ( pred( input.head() ) == false ) return htuple( reverse( acc ), input );
	return mySpan( pred, tail( input ), hlist( head( input ) ) + acc );
};

template<typename T>
auto quicksort(const HList<T> & list)
{
	if ( null( list ) ) return HList<T>();
	auto x = head( list );
	auto smallerSorted = quicksort( filter( [&x]( auto a ) { return a <= x; }, tail( list ) ) );
	auto biggerSorted = quicksort( filter( [&x]( auto a ) { return a > x; }, tail( list ) ) );
	return smallerSorted + hlist( x ) + biggerSorted;
}

auto factorial = []( Integer n ) { return product( hlist( Integer(1), n ) ); };

auto even = []( auto i ) { return i % 2 == 0; };

auto odd = []( auto i ) { return i % 2 != 0; };

auto chain(unsigned int n = 1)
{
    if (n == 1) return hlist(n);
    return even(n) ? hlist(n)+chain(n/2) : hlist(n)+chain(n*3+1);
}

auto addThree = [](auto x, auto y, auto z) { return x + y + z; };

auto reverse_foldl = [](auto ls)
{
    auto helper = [](auto x, auto y) { return hlist(x) + y; };
    return foldl(helper, empty(ls), ls);
};

auto reverse_foldr = [](auto ls)
{
    auto helper = [](auto x, auto y) { return y + hlist(x); };
    return foldr(helper, empty(ls), ls);
};

auto numLongChain = []()
{
    auto isLong = [](auto x) { return length(x) > 15; };
    return length(filter(isLong, (map (chain, hlsrange(1,100)))));
};

int _main3333324123125513125125()
{
	show( myTakeWhile( []( char i ) { return i != ' '; }, hlist( "this is Practice." ) ) );
	show( mySpan( []( char i ) { return i != ' '; }, hlist( "this is Practice." ) ) );
	show( hlsrange( 1, 10 ), hlsrange( 'a', 'z' ) );
	show( elem( 1, hlsrange( 1, 10 ) ) );
	show( product( hlsrange( 1, 10 ) ) );
	show( replicate( 4, 'h' ) );
	show( take( 3, hlist( 5, 4, 3, 2, 1 ) ) );
	show( drop( 100, hlist( 1, 2, 3, 4 ) ) );
	show( minimum( hlist( 8, 4, 2, 1, 5, 6 ) ) );
	show( maximum( hlist( 1, 9, 2, 3, 4 ) ) );
	//show( null( hlist() ) );
	show( null( hlist( 1, 2, 3 ) ) );
	show( hlist( "hello", "goodbye", "what" ) );
	show( "factorial", factorial( 50 ) );
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
	}, hlist(htuple( 1, 2 ), htuple( 3, 5 ), htuple( 6, 3 ), htuple( 2, 6 ), htuple( 2, 5 )) ) );
	show( hlist( hlist( 1, 2 ), hlist( 3, 7 ), hlist( 4, 5 ) ) );
    show(chain(10));
    show(chain(1));
    show(chain(30));
    show(numLongChain());
    show(map ([](auto a) { return fst(a) + snd(a); }, hlist(htuple(1,2),htuple(3,5),htuple(6,3),htuple(2,6),htuple(2,5))));
    show("reverse_foldl",reverse_foldl(hlist(1,2,3,4)));
    show("reverse_foldr",reverse_foldr(hlist(1,2,3,4)));

	while ( 1 )
		;
	return 0;
}