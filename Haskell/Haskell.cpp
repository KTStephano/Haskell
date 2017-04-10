#include "Utility.h"

auto item = [](auto ls)
{
	if ( null( ls ) ) return ls;
	return hlist( htuple( head( ls ), tail( ls ) ) );
};

auto unit = [](auto a)
{
	return [=]( auto s ) { return hlist( htuple( a, s ) ); };
};