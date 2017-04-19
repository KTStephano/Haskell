#include <iostream>
#include "Prelude.h"
#include "Integer.h"

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

	while ( 1 )
		;
	return 0;
}