This codebase tries to bring certain functional features/concepts to the C++ language in a Haskell-oriented style.

For example, instead of creating an std::vector and then inserting elements within a loop, you would create an "hlist"
and then recursively add to it. The process of adding elements to an hlist actually creates whole new lists at each step,
making them immutable. Certain things are done to mitigate the performance impact that this brings with it, such as abusing
the fact that all objects should be immutable, so instances can be shared among lists without issue.

If you need a loop to iterate over a list, you can use map and if you want to put two sets of things together in pairs then
you can use zip. These are just a few examples of how this codebase tries to use C++ to perform common operations in a way
that would be more familiar to Haskell programmers.

There are a lot of things that I left up to future people/future iterations of this project. For example, common large
Integer operations (+,-,*,exponentiation) have been implemented, but other things including division that Haskell easily
allows you to do with large integers have been left out.

The biggest area that has been entirely untouched that is firmly entrenched in Haskell at this point are things like
Monads, MonadTransformers, etc. I have done some research into this area and experimented a little, and from what I understand
these things can be done in C++ but it will require significantly more time to actually implement. Unfortunately I am not
(yet) convinced that it can be done in a way that looks and feels clean to use in C++.
