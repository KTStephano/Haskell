#include "Utility.h"

template<typename T>
auto permutations( const List<T> & xs )
{
	if ( null( xs ) ) return hlist( xs );

	return
		concat( map(
			[=]( auto x )
	{
		return
			map( [=]( auto y ) { return hlist( x ) + y; },
				 permutations( filter( [=]( auto z )
		{
			return x != z;
		}, xs ) ) );
	}, xs
		) );
}

// Question 4
template<typename T>
auto runLengthEncodeHelper(const T & curr, const List<T> & list, int acc)
{
    if (null(list)) return hlist(htuple(curr, acc + 1));
    if (head(list) != curr) return hlist(htuple(curr, acc + 1)) + runLengthEncodeHelper(head(list),tail(list),0);
    return runLengthEncodeHelper(curr, tail(list), acc + 1);
}

auto runLengthEncode = [](auto ls)
{
    return ::runLengthEncodeHelper(head(ls),tail(ls),0);
};

// Question 5
auto runLengthDecode = [](auto lsTuples)
{
    return concatMap ([](auto pair) { return replicate(snd(pair),fst(pair)); }, lsTuples);
};

// Question 6
template<typename Lambda>
auto splitTextHelper(Lambda pred, const String & text, const String & acc)
{
    if (null(text)) return hlist(acc);
    if (pred(head(text)) != true) return hlist(acc) + splitTextHelper(pred, tail(text),hlist(""));
    return splitTextHelper(pred, tail(text), acc+hlist(head(text)));
}

auto splitText = [](auto pred, auto ls) { return ::splitTextHelper(pred,ls,hlist("")); };

// Question 7

auto encipher = [](auto ls0, auto ls1, auto msg)
{
    // This is a list comprehension in the current version of Haskell C++, and it corresponds to
    // map (\x -> snd x) $ concat [[x | x <- zip ls0 ls1, fst x == y] | y <- msg]
    auto zipped = zip(ls0,ls1);
    return map ([](auto x) { return snd(x); },
                concatMap([=](auto c) { return filter([=](auto pair) { return fst(pair) == c; }, zipped); }, msg));
};

// Question 8

auto factors = [](auto n)
{
    // filter (\pair -> fst pair * snd pair == n) $ [(x,y) | x <- [1..n], y <- [1..n]]
   return filter([=](auto pair) { return fst(pair)*snd(pair)==n; },concatMap ([=](auto x)
         {
             return map([=](auto y) { return htuple(x,y);}, hlsrange(1,n));
         }, hlsrange(1,n)));
};

auto primes = [](auto n)
{
    // filter (\z -> length (factors z) == 2) $ [x | x <- [1..n]]
    return filter([=](auto z) { return length(factors(z)) == 2; }, hlsrange(1,n));
};

auto goldbach = [=](auto n)
{
	auto p = ::primes( n );
	// filter (\pair -> fst pair + snd pair == n) $ concat [(x,y) | x <- p, y <- p]
	return filter( [=]( auto pair )
	{
		return fst( pair ) + snd( pair ) == n;
	}, concatMap( [=]( auto x )
	{
		return map( [=]( auto y ) { return htuple( x, y ); }, p );
	}, p ) );
};

// Question 9
auto increasing = [](auto ls)
{
	auto len = length( ls );
	auto zipped = zip( ls, hlsrange( size_t(1), len ) );
	// and $ map (\x -> if fst (head x) <= fst (head (tail x)) then True else False) $ 
	//		 [[x,y] | x <- zip ls [1..(length ls)], y <- zip ls [1..(length ls), snd y > snd x]]
	return and(map( []( auto a )
	{
		if ( fst( head( a ) ) <= fst( head( tail( a ) ) ) ) return true;
		return false;
	}, concatMap( [=]( auto x )
	{
		return map( [=]( auto y ) { return hlist( x, y ); }, filter( [=]( auto pair )
		{
			return snd( pair ) > snd( x );
		}, zipped) );
	}, zipped ) ) );
};

// Question 10

auto select2 = [](auto pred, auto ls0, auto ls1)
{
	// [snd x | x <- zip ls0 ls1, pred (fst x) == True]
	return map( []( auto x )
	{
		return snd( x );
	}, filter( [=]( auto x ) { return pred( fst( x ) ) == true; }, zip( ls0, ls1 ) ) );
};

int main()
{
    show(runLengthEncode(hlist (4,2,2,1,1,1,1,4,4,4,4)));
    show(runLengthDecode(runLengthEncode(hlist (4,2,2,1,1,1,1,4,4,4,4))));
    show(splitText ([](auto x) { return x != ' ';}, hlist("This is practice.")));
    show(2+3);
    show(hlist("hello")+hlist("george"));
    show(encipher(hlsrange('A','Z'),hlsrange('a','z'),hlist("THIS")));
    show(and(hlist(true,true,true,true,true,true,false)));
    show(primes(25));
	show( goldbach( 6 ) );
	show( increasing( hlist( "ABBD" ) ) );
	show( increasing( hlsrange( 10,9,1 ) ) );
	show( increasing( hlsrange( 1, 10 ) ) );
	auto even = []( auto x ) { return x % 2 == 0; };
	show( select2( even, hlsrange( 1, 26 ), hlsrange( 'a', 'z' ) ) );
	show( select2( []( auto x ) { return x <= 'g'; }, hlsrange( 'a', 'z' ), hlsrange( 1, 26 ) ) );

	while ( 1 )
		;
    return 0;
}