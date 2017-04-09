#include "Utility.h"

int main()
{
	show( concat( map( replicate<int>( 4 ), hlsrange( 1, 4 ) ) ) );
	show( concatMap( replicate<int>( 4 ), hlsrange( 1, 4 ) ) );

	show( replicate( 5 )( hlist( "hello" ) ) );

	while ( 1 )
		;
	return 0;
}