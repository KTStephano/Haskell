#include "Utility.h"

int main()
{
	show( concat( map( replicate<int>( 4 ), hlsrange( 1, 4 ) ) ) );
	show( concatMap( replicate<int>( 4 ), hlsrange( 1, 4 ) ) );

	while ( 1 )
		;
	return 0;
}