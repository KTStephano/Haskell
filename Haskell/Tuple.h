#pragma once

#include <typeinfo>

// See https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-api-4.5/a01066_source.html
// for full and clean standard implementation

template<std::size_t Index, typename Head, bool IsEmpty>
class _HeadBase;

template<std::size_t Index, typename Head>
class _HeadBase<Index, Head, true> : public virtual Head
{
public:
	_HeadBase() : Head()
	{
		std::cout << "Creating with nothing " << std::endl;
	}

	_HeadBase( const Head & head ) : Head( head ) {  }

	_HeadBase(Head && head) : Head(head)
	{
		std::cout << "Creating with  " << head << std::endl;
	}

	virtual ~_HeadBase() {  }

	const Head & get() const
	{
		return *this;
	}
};

template<std::size_t Index, typename Head>
class _HeadBase<Index, Head, false>
{
	Head _data;

public:
	_HeadBase() : _data()
	{
		std::cout << "Creating with nothing " << std::endl;
	}

	_HeadBase(const Head & head) : _data(head)
	{
	}

	_HeadBase(Head && head) : _data(head)
	{
		std::cout << "Creating with  " << head << std::endl;
	}

	virtual ~_HeadBase() {  }

	const Head & get() const
	{
		return _data;
	}
};

template<std::size_t Index, typename ... Elements>
class _TupleImpl;

// Recursive base case, i.e. zero elements
template<std::size_t Index>
class _TupleImpl<Index>
{
};

template<std::size_t Index, typename Head, typename ... Tail>
class _TupleImpl<Index, Head, Tail...> : public virtual _TupleImpl<Index + 1, Tail...>,
										 private virtual _HeadBase<Index, Head, std::is_empty<Head>::value>
{
public:
	typedef _TupleImpl<Index + 1, Tail...> Next;
	typedef _HeadBase<Index, Head, std::is_empty<Head>::value> Data;

	_TupleImpl() : Next(), Data() {  }

	explicit _TupleImpl(const Head & head, const Tail & ... tail) : Next(tail...), Data(head)
	{
	}

	explicit _TupleImpl(Head && head, Tail && ... tail) : Next(std::forward<Tail>(tail)...), Data(std::forward<Head>(head))
	{
		std::cout << "Creating impl with " << head << std::endl;
	}

	_TupleImpl(const _TupleImpl & other) : Next(other.tail()), Data(other.head()) {  }

	_TupleImpl(_TupleImpl && other) : Next(std::move(other.tail())), Data(std::move(other.head())) {  }

	virtual ~_TupleImpl() {  }

	const Head & head() const
	{
		return Data::get();
	}

	const Next & tail() const
	{
		return *this;
	}
};

template<typename ... Types>
class Tuple : public virtual _TupleImpl<0, Types...>
{
	typedef _TupleImpl<0, Types...> _Inherited;
	std::size_t _length;

public:
	Tuple() : _Inherited(), _length(0)
	{
		std::cout << "Creating with nothing" << std::endl;
	}

	explicit Tuple(const Types & ... elements) : _Inherited(elements...),
												 _length(sizeof...(elements))
	{ }

	explicit Tuple(Types && ... elements) : _Inherited(std::forward<Types>(elements)...),
											_length(sizeof...(elements))
	{
		std::cout << "Creating with rvalue length " << _length << std::endl;
	}

	Tuple(const Tuple & other) : _Inherited(static_cast<const _Inherited &>(other)),
								 _length(other._length)
	{ }

	Tuple(Tuple && other) : _Inherited(static_cast<_Inherited &&>(other)),
							_length(other._length)
	{ }

	virtual ~Tuple() {  }

	std::size_t length() const
	{
		return _length;
	}
};

// Struct for getting the type of the element at the ith index
template<std::size_t Index, typename T>
struct TupleElementType;

template<std::size_t Index, typename Head, typename ... Tail>
struct TupleElementType<Index, Tuple<Head, Tail...>> : TupleElementType<Index - 1, Tuple<Tail...>>
{ };

// Base case
template<typename Head, typename ... Tail>
struct TupleElementType<0, Tuple<Head, Tail...>>
{
	typedef Head type;
};

template<std::size_t Index, typename Head, typename ... Tail>
inline const Head & _getHelper(const _TupleImpl<Index, Head, Tail...> & tuple)
{
	return tuple.head();
}

template<std::size_t Index, typename ... Elements>
inline const typename TupleElementType<Index, Tuple<Elements...>>::type & get(const Tuple<Elements...> & tuple)
{
	return _getHelper( tuple );
}