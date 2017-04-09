#include "Utility.h"

auto test = [](auto a)
{
    return [a](auto b) {
        return a + b;
    };
};

template<typename T>
auto permutations(const List<T> & xs)
{
	if ( null( xs ) ) return hlist( xs );

	return 
	flatten(map(
	[=]( auto x )
	{
		return 
			map( [=]( auto y ) { return hlist( x ) + y; },
				 permutations( filter( [=]( auto z )
									   {
									       return x != z;
								       }, xs )));
	}, xs
	));
}

int main()
{
    show(test(hlist(1,2,3))(hlist(4,5,6)));
	show( permutations( hlist( "ABC" ) ) );
    show(filter([](auto x) { return x != ' '; }, hlist("hello what")));

	while ( 1 )
		;
	return 0;
}