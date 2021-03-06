#include <iostream>
#include "../Source/Prelude.h"
#include "../Source/Integer.h"

Integer fact(Integer n)
{
	if ( n == 1 ) return n;
    return n * fact(n-1);
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

template<typename Lambda>
auto Y(Lambda f)
{
	return f( [=]( auto ... x ) { return Y( f )( x... ); } );
}

auto almost_factorial = [](auto f)
{
	return [=](auto n)
	{
		if ( n == 1 ) return n;
		return n * f( n - 1 );
	};
};

//auto factorial2 = Y( almost_factorial );

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
	show( Integer( 1234 )*Integer( 4321 ) );
	show( Integer( 8 ) + Integer( 8 ) );
	//show( fact( 300 ) );
	show( fact( 2000 ) );

	show( compose( add1, add1 )( 30000 ) );
    show(pow(Integer(2),4086));
    show(Integer(42)/2);
    show(1000000 << 3);
    //show (Integer(1000000) << 3);
    show(Integer(20)*Integer(2000));
	show( UINT32_MAX );
    show(Integer(10)*Integer(213));
    show(Integer(10)*Integer(9)*Integer(8)*Integer(7)*Integer(6)*Integer(5)*Integer(4)*Integer(3)*Integer(2)*Integer(1));
    show(fact(10));
    show(Integer(10)*Integer(362880));
	show( Integer( 914 ) * 84 );
	show( Integer( "8124895812349890123901239045195121239095041820948123908130945015012931823123908" ) );
	*/

	//show( Integer( 2 ) * 2 );
	show( Integer( 1234 ) / 27 );
	show( Integer( 1234551 ) / Integer( -23415 ) );
	show( Integer( "8124895812349890123901239045195121239095041820948123908130945015012931823123908" ) / Integer( 234 ) );
	show( Integer( 234 ) / Integer( 34 ) );

	//show( factorial2( Integer(300) ) );
	return 0;
}
