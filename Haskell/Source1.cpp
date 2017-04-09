#include <iostream>
#include "List.h"

int _main()
{
	auto ls = List<int>{ 1,2,3 };
	std::cout << ls << std::endl;
	std::cout << List<int>( ls ) << std::endl;
	auto ls2 = List<int>( 1, 1000000 );
	auto ls3 = ls2;
	std::cout << "Done" << std::endl;

	while ( 1 )
		;
	return 0;
}