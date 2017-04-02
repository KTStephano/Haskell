#include "List.h"
#include "Utility.h"
#include <iostream>

template<typename Function, typename T>
static List<T> filter( Function proc, const List<T> & list )
{
	if ( list.isNull() ) return List<T>();
	else if ( proc( list.head() ) ) return List<T>{ list.head() } +filter( proc, list.tail() );
	return filter( proc, list.tail() );
}

int main()
{
	std::cout << filter( []( int i ) { return i % 2 == 0; }, List<int>( 1, 10 ) ) << std::endl;
	return 0;
}