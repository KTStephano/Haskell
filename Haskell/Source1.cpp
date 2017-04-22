#include <iostream>
#include "Prelude.h"
#include "Integer.h"

Integer fact(Integer n)
{
    show("fact",n);
	if ( n == 1 ) return n;
    auto res = n * fact(n-1);
    show("fact res", res);
	return res;
}

Integer factacc(Integer n, Integer acc)
{
    if (n == 1) return acc;
    return factacc(n - 1, n * acc);
}

Integer add1n(Integer x, int n)
{
	if ( n == 0 ) return x;
	return add1n( x + 1, n - 1 );
}

auto add1 = []( auto x ) { return x + 1; };

int main()
{
	/**
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
	show( Integer(2)*Integer(3)*Integer(4)*Integer(5)*Integer(6)*Integer(7)*Integer(8)*Integer(9)*Integer(10) );
	show( Integer( 5 ) * Integer( 24 ) );
	show( fact( 300 ) );
    show( add1n( 1, 300 ) );
	show( fact( 50 ) );
	show( Integer()*-1 );
	show( Integer( 2 ) / 2 );
	show( Integer( "18446744073709551615" ) * Integer( "18446744073709551615" ) * Integer( "18446744073709551615" ) - Integer( "18446744073709551615" ) );
	*/
	/*
	show( Integer( 1234 )*Integer( 4321 ) );
	show( Integer( 8 ) + Integer( 8 ) );
	//show( fact( 300 ) );

	show( compose( add1, add1 )( 30000 ) );
    //show(pow(Integer(2),4086));
    //show(pow(Integer(2),-1024));
    show(Integer(42)/2);
    show(1000000 << 3);
    show (Integer(1000000) << 3);
    show(Integer(20)*Integer(2000));
    show(Integer(10)*Integer(213));
    show(Integer(10)*Integer(9)*Integer(8)*Integer(7)*Integer(6)*Integer(5)*Integer(4)*Integer(3)*Integer(2)*Integer(1));
    show(fact(10));
    */
    show(Integer(10)*Integer(362880));

	return 0;
}