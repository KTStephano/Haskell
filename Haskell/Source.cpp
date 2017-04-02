#include "List.h"
#include "Utility.h"
#include <iostream>

template<class Function, typename T>
static List<T> filter(Function pred, const List<T> & list) {
	if ( list.isNull() ) return List<T>();
	else if ( pred( list.head() ) ) return List<T>{list.head()} + filter( pred, list.tail() );
	else return filter( pred, list.tail() );
}

int main()
{
	auto even = []( int i ) { return i % 2 == 0; };
	std::cout << filter( even, List<int>( 1, 10 ) );

	while ( 1 )
		;
	return 0;
}