#pragma once

#include <typeinfo>
#include <iostream>
#include <type_traits>

// See https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-api-4.5/a01066_source.html
// for full and clean standard implementation

template<std::size_t Index, typename Head, bool IsEmpty>
class _HeadBase;

template<std::size_t Index, typename Head>
class _HeadBase<Index, Head, true> : public Head
{
public:
	_HeadBase() : Head()
	{
	}

	_HeadBase( const Head & head ) : Head( head ) { }

	_HeadBase( Head && head ) : Head( std::forward<Head>( head ) )
	{
	}

	virtual ~_HeadBase() { }

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
	}

	_HeadBase( const Head & head ) : _data( head )
	{
	}

	_HeadBase( Head && head ) : _data( std::forward<Head>( head ) )
	{
		std::cout << "Creating head with " << head << std::endl;
	}

	virtual ~_HeadBase() { }

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
public:
	virtual ~_TupleImpl() { }

protected:
	virtual void _print( std::ostream & out, std::size_t length ) const { }
};

template<std::size_t Index, typename Head, typename ... Tail>
class _TupleImpl<Index, Head, Tail...> : public _TupleImpl<Index + 1, Tail...>,
	protected _HeadBase<Index, Head, std::is_empty<Head>::value>
{
	typedef _TupleImpl<Index + 1, Tail...> _Next;
	typedef _HeadBase<Index, Head, std::is_empty<Head>::value> _Data;

public:
	_TupleImpl() : _Next(), _Data()
	{
	}

	explicit _TupleImpl( const Head & head, const Tail & ... tail ) : _Next( tail... ), _Data( head )
	{
	}

	explicit _TupleImpl( Head && head, Tail && ... tail ) : _Next( std::forward<Tail>( tail )... ),
		_Data( std::forward<Head>( head ) )
	{
	}

	_TupleImpl( const _TupleImpl & other ) : _Next( other.tail() ), _Data( other.head() ) { }

	_TupleImpl( _TupleImpl && other ) : _Next( std::move( other.tail() ) ), _Data( std::move( other.head() ) ) { }

	virtual ~_TupleImpl() { }

	const Head & head() const
	{
		return _Data::get();
	}

	const _Next & tail() const
	{
		return *this;
	}

protected:
	void _print( std::ostream & out, std::size_t length ) const override
	{
		std::string beforeAfterData = "";
		if ( std::is_same<Head, char>::value ) beforeAfterData = "'";
		out << beforeAfterData;
		out << _Data::get();
		out << beforeAfterData;
		if ( Index + 1 < length ) out << ",";
		_Next::_print( out, length );
	}
};

template<typename ... Types>
class HTuple : public _TupleImpl<0, Types...>
{
	typedef _TupleImpl<0, Types...> _Inherited;
	std::size_t _length;

public:
	HTuple() : _Inherited(), _length( 0 )
	{
	}

	explicit HTuple( const Types & ... elements ) : _Inherited( elements... ),
		_length( sizeof...( elements ) )
	{
	}

	explicit HTuple( Types && ... elements ) : _Inherited( std::forward<Types>( elements )... ),
		_length( sizeof...( elements ) )
	{
	}

	HTuple( const HTuple & other ) : _Inherited( static_cast<const _Inherited &>( other ) ),
		_length( other._length )
	{
	}

	HTuple( HTuple && other ) : _Inherited( static_cast<_Inherited &&>( other ) ),
		_length( other._length )
	{
	}

	virtual ~HTuple() { }

	std::size_t length() const
	{
		return _length;
	}

	/** Overloaded operators */
	friend std::ostream & operator<<( std::ostream & out, const HTuple & tuple )
	{
		std::string beginEnd;
		std::string delim;
		out << "(";
		tuple._print( out, tuple.length() );
		out << ")";
		return out;
	}

private:
	void _print( std::ostream & out, std::size_t length ) const override
	{
		_Inherited::_print( out, length );
	}
};

// Struct for getting the type of the element at the ith index
template<std::size_t Index, typename T>
struct TupleElementType;

template<std::size_t Index, typename Head, typename ... Tail>
struct TupleElementType<Index, HTuple<Head, Tail...>> : TupleElementType<Index - 1, HTuple<Tail...>>
{
};

// Base case
template<typename Head, typename ... Tail>
struct TupleElementType<0, HTuple<Head, Tail...>>
{
	typedef Head type;
};

template<std::size_t Index, typename Head, typename ... Tail>
inline const Head & _getHelper( const _TupleImpl<Index, Head, Tail...> & tuple )
{
	return tuple.head();
}

template<std::size_t Index, typename ... Elements>
inline const typename TupleElementType<Index, HTuple<Elements...>>::type & get( const HTuple<Elements...> & tuple )
{
	if ( Index < 0 || Index >= tuple.length() ) throw std::out_of_range( "Tuple: index out of range" );
	return _getHelper<Index>( tuple );
}

template<typename ... Elements>
bool operator==( const HTuple<Elements...> & tuple0, const HTuple<Elements...> & tuple1 )
{
	if ( tuple0.length() != tuple1.length() ) return false;
	for (size_t i = 0; i < tuple0.length(); ++i)
	{
		if ( get<i>( tuple0 ) != get<i>( tuple1 ) ) return false;
	}
	return true;
}

template<typename ... Elements>
bool operator>( const HTuple<Elements...> & tuple0, const HTuple<Elements...> & tuple1 )
{
	for (size_t i = 0; i < tuple0.length(); ++i)
	{
		if ( get<i>( tuple0 ) > get<i>( tuple1 ) ) return true;
	}
	return false;
}

template<typename ... Elements>
bool operator>=( const HTuple<Elements...> & tuple0, const HTuple<Elements...> & tuple1 )
{
	return tuple0 > tuple1 || tuple0 == tuple1;
}

template<typename ... Elements>
bool operator<( const HTuple<Elements...> & tuple0, const HTuple<Elements...> & tuple1 )
{
	return !( tuple0 > tuple1 );
}

template<typename ... Elements>
bool operator<=( const HTuple<Elements...> & tuple0, const HTuple<Elements...> & tuple1 )
{
	return tuple0 < tuple1 || tuple0 == tuple1;
}

template<typename T, typename K>
auto fst( const HTuple<T, K> & tuple)
{
	return get<0>( tuple );
}

template<typename T, typename K>
auto snd( const HTuple<T, K> & tuple )
{
	return get<1>( tuple );
}