#include <iostream>
#include "Prelude.h"
#include "Integer.h"

Integer factHelper(Integer curr, Integer n)
{
	if ( curr == n ) return curr;
	return curr * factHelper( curr + 1, n );
}

Integer fact(Integer n)
{
	return factHelper( 1, n );
}

int main()
{
	show( 10 + hlist( 1, 2, 3 ) + hlist() );
	auto i = Integer();
	for ( int j = 0; j < 200010; ++j ) ++i;
	show( i );
	show( Integer( 23 ) );
	show( Integer( 200010 ) );
	show( Integer( 2 ) + Integer( 2 ) );
	show( Integer( 500 ) - Integer( 499 ) );
	show( Integer( -16 ) * -16 );
	show( Integer(500)*Integer(500) );

	while ( 1 )
		;
	return 0;
}