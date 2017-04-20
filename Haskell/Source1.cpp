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
	show( Integer( 16 ) * Integer(16) );
    show( Integer(24) * Integer(25));
	show( Integer(500)*Integer(500) );
	show( Integer( "18446744073709551615" ) * Integer( "18446744073709551615" ) );
    show( Integer( "1844674407370" ) + Integer( "18446744073709551615" ));
    show(Integer(16) + Integer(16));
    show(Integer(96) + Integer(160));
    show(Integer(-32) - Integer(-32));
    show(Integer(3)-Integer(5));
    show(Integer(-15)-Integer(-5));
    show(Integer(-5)-Integer(-15));
    show(Integer(-500)-Integer(700));
    show(Integer(-2)+Integer(2));

	return 0;
}