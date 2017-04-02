#include "List.h"
#include <iostream>

int main()
{
	List<int> l{ 1,2,3 };
	int i = 10;
	std::cout << l.tail() << std::endl;
	std::cout << l + List<int>{4, 5, 6} << std::endl;
	List<char> phrase{ ' ', 'A' };
	std::cout << phrase << std::endl;
	std::cout << "Done\n";
	while ( 1 )
		;
	return 0;
}