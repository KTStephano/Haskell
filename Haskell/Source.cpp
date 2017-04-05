#include "List.h"
#include "Utility.h"
#include <iostream>

/**
template<class Function, typename T>
static List<T> filter(Function pred, const List<T> & list) {
	if ( list.isNull() ) return List<T>();
	else if ( pred( list.head() ) ) return List<T>{list.head()} + filter( pred, list.tail() );
	else return filter( pred, list.tail() );
}
*/

/**
template<class Function1, class Function2, typename R = std::result_of<Function1(Function2)>::type>
static auto compose(Function1 f, Function2 g) {
	using I = std::result_of<Function2( R )>;
	return [f, g]( I i ) -> R { return f( g( i ) ); };
}
*/

template<typename Car>
void what( Car head )
{
	std::cout << head << std::endl;
}

template<typename Car, typename ... Cdr>
void what(Car head, Cdr ... tail)
{
	std::cout << head << " ";
	what( tail... );
}

int main()
{
	auto test = HList( 'a','z' );
	std::cout << test << std::endl;
	std::cout << TAKE( 1, test ) << std::endl;
	auto iterable = List<int>( 1, 10 );
	std::cout << TAIL( iterable ) << std::endl;
	auto sqrtLambda = []( float num ) -> float { return sqrtf( num ); };
	auto add1 = []( int num ) -> int { return num + 1; };
	auto composed = [sqrtLambda, add1]( int num ) -> float { return sqrtLambda( add1( num ) ); };
	std::cout << composed( 25 ) << std::endl;

	auto even = []( int i ) -> bool { return i % 2 == 0; };
	std::cout << filter( even, List<int>( 1, 10 ) ) << std::endl;

	std::cout << map( []( int i ) { return List<int>{i}; }, List<int>( 1, 10 ) ) << std::endl;

	std::cout << select( even, List<int>( 1, 4 ), List<char>( 'a', 'z' ) ) << std::endl;

	while ( 1 )
		;
	return 0;
}