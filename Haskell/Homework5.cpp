#include "Utility.h"

auto test = [](auto a)
{
    return [a](auto b) {
        return a + b;
    };
};

int main()
{
    show(test(hlist(1,2,3))(hlist(4,5,6)));
    show(filter([](auto x) { return x != ' '; }, hlist("hello what")));
	return 0;
}