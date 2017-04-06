#pragma once

//#include <allocators>
#include <initializer_list>
#include <stdexcept>
#include <string>

template<typename T, typename A = std::allocator<T>>
class List
{
	T * _list = nullptr;
	A * _allocator;
	size_t _length;

private:
	List( size_t size ) : _allocator( new A() ),
		_length( 0 )
	{
		if ( size > 0 ) _reserve( size );
	}

public:
	typedef T value_type;

	explicit List() : List( 0 )
	{
	}

	explicit List( const std::initializer_list<T> & list ) : List( list.size() )
	{
		size_t i = 0;
		for ( const T & elem : list )
		{
			_allocator->construct( &_list[i], elem );
			++i;
		}
	}

	List( T startInclusive, T endInclusive ) : List( endInclusive - startInclusive + 1 )
	{
		for ( T i = 0; i < _length; ++i )
		{
			_allocator->construct( &_list[i], startInclusive + i );
		}
	}

	List( const List & list ) : List()
	{
		_deepCopy( list );
	}

	explicit List( const List & list0, const List & list1 ) : List( list0._length + list1._length )
	{
		_copyNoAlloc( list0, 0, list0._length, 0, list0._length );
		_copyNoAlloc( list1, list0._length, _length, 0, list1._length );
	}

	explicit List( const List & list, const T & newItem ) : List( list._length + 1 )
	{
		_copyNoAlloc( list );
		_allocator->construct( &_list[_length - 1], newItem );
	}

	explicit List( const T & newItem, const List & list ) : List( list._length + 1 )
	{
		_copyNoAlloc( list, 1, _length, 0, list._length );
		_allocator->construct( &list[0], newItem );
	}

	explicit List( const List & list, size_t copyStartInclusive, size_t copyEndExclusive )
		: List( copyEndExclusive - copyStartInclusive )
	{
		_copyNoAlloc( list, 0, _length, copyStartInclusive, copyEndExclusive );
	}

	List( List && list ) : List()
	{
		_shallowCopy( list );
	}

	~List()
	{
		_destroy();
		delete _allocator;
		_allocator = nullptr;
	}

	List<T> getNullVariant() const
	{
		return List<T>();
	}

	bool isNull() const
	{
		return _length == 0;
	}

	bool contains( const T & elem ) const
	{
		for ( int i = 0; i < _length; ++i )
		{
			if ( ( *this )[i] == elem ) return true;
		}
		return false;
	}

	bool contains( T && elem ) const
	{
		return contains( elem );
	}

	size_t length() const
	{
		return _length;
	}

	const T & head() const
	{
		return _list[0];
	}

	const T & last() const
	{
		return _list[_length - 1];
	}

	List tail() const
	{
		if ( _length == 0 ) throw std::out_of_range( "Attempting to access null tail position" );
		return List( *this, 1, _length );
	}

	/**
	* Returns everything except the last element
	*/
	List init() const
	{
		if ( _length == 0 ) throw std::out_of_range( "Attempting to access null tail position" );
		return List( *this, 0, _length - 1 );
	}

	List reverse() const
	{
		return _reverse( *this );
	}

	List take( size_t numElements ) const
	{
		if ( numElements > _length ) return *this;
		return List( *this, 0, numElements );
	}

	List drop( size_t numElements ) const
	{
		if ( numElements > _length ) return List();
		return List( *this, numElements, _length );
	}

	/** Overloaded operators */
	const T & operator[]( size_t index ) const
	{
		if ( !_isInRange( index ) )
		{
			throw std::out_of_range( "List index out of range at " + std::to_string( index ) );
		}
		return _list[index];
	}

	List operator+( const T & item ) const
	{
		return List( *this, item );
	}

	List operator+( const List & other ) const
	{
		return List( *this, other );
	}

	bool operator>( const List & other ) const
	{
		bool isGreater = length() > other.length();
		std::size_t indices = isGreater ? other.length() : length();
		for ( std::size_t i = 0; i < indices; ++i )
		{
			if ( _list[i] > other[i] ) return true;
			else if ( _list[i] == other[i] ) continue;
			return false;
		}
		return isGreater;
	}

	bool operator<( const List & other ) const
	{
		return other > *this;
	}

	bool operator>=( const List & other ) const
	{
		bool equal = *this == other;
		return equal ? true : *this > other;
	}

	bool operator<=( const List & other ) const
	{
		return other >= *this;
	}

	bool operator==( const List & other ) const
	{
		if ( _length != other._length ) return false;
		for ( size_t i = 0; i < _length; ++i )
		{
			if ( _list[i] != other[i] ) return false;
		}
		return true;
	}

	friend std::ostream & operator<<( std::ostream & out, const List & list )
	{
		std::string begin = "[";
		std::string end = "]";
		std::string delim = ",";
		if ( std::is_same<T, char>::value )
		{
			begin = "\"";
			end = "\"";
			delim = "";
		}
		out << begin;
		for ( size_t i = 0; i < list.length(); ++i )
		{
			out << list[i];
			if ( i + 1 != list.length() ) out << delim;
		}
		out << end;
		return out;
	}

private:
	void _deepCopy( const List & other )
	{
		_reserve( other._length );
		_copyNoAlloc( other );
	}

	void _shallowCopy( List & other )
	{
		_list = other._list;
		_allocator = other._allocator;
		_length = other._length;
		other._list = nullptr;
		other._allocator = nullptr;
		other._length = 0;
	}

	void _copyNoAlloc( const List & other )
	{
		for ( size_t i = 0; i < _length; ++i )
		{
			_allocator->construct( &_list[i], other._list[i] );
		}
	}

	void _copyNoAlloc( const List & other,
					   size_t thisStartInclusive, size_t thisEndExclusive,
					   size_t otherStartInclusive, size_t otherEndExclusive )
	{
		for ( size_t i = thisStartInclusive, j = otherStartInclusive; i < thisEndExclusive; ++i, ++j )
		{
			_allocator->construct( &_list[i], other._list[j] );
		}
	}

	void _destroy()
	{
		if ( isNull() ) return;
		for ( size_t i = 0; i < _length; ++i )
		{
			_allocator->destroy( &_list[i] );
		}
		_allocator->deallocate( _list, _length );
		_length = 0;
		_list = nullptr;
	}

	void _reserve( size_t size )
	{
		_destroy();
		_length = size;
		_list = _allocator->allocate( _length );
	}

	bool _isInRange( size_t index ) const
	{
		return index >= 0 && index < _length;
	}

	static List _reverse( const List & list )
	{
		if ( list.isNull() ) return List();
		return List{ list.last() } +_reverse( list.init() );
	}
};

typedef List<char> String;

template<typename T>
bool null( const List<T> & list )
{
	return list.isNull();
}

template<typename T>
auto head( const List<T> & list )
{
	return list.head();
}

template<typename T>
auto tail( const List<T> & list )
{
	return list.tail();
}

template<typename T>
auto last( const List<T> & list )
{
	return list.last();
}

template<typename T>
auto init( const List<T> & list )
{
	return list.init();
}

template<typename T>
auto length( const List<T> & list )
{
	return list.length();
}

template<typename T>
auto take( std::size_t num, const List<T> & list )
{
	return list.take( num );
}

template<typename T>
auto drop( std::size_t num, const List<T> & list )
{
	return list.drop( num );
}

template<typename T>
auto _minimum( const List<T> & list, const T & min )
{
	if ( null( list ) ) return min;
	else if ( head( list ) < min ) return _minimum( tail( list ), head( list ) );
	return _minimum( tail( list ), min );
}

template<typename T>
auto minimum( const List<T> & list )
{
	return _minimum( tail( list ), head( list ) );
}

template<typename T>
auto _maximum( const List<T> & list, const T & max )
{
	if ( null( list ) ) return max;
	else if ( head( list ) > max ) return _maximum( tail( list ), head( list ) );
	return _maximum( tail( list ), max );
}

template<typename T>
auto maximum( const List<T> & list )
{
	return _maximum( tail( list ), head( list ) );
}

template<typename T>
auto sum( const List<T> & list, const T & acc = T() )
{
	if ( null( list ) ) return acc;
	return sum( tail( list ), acc + head( list ) );
}

template<typename T>
auto product( const List<T> & list, const T & acc = 1 )
{
	if ( null( list ) ) return acc;
	return product( tail( list ), acc * head( list ) );
}

template<typename T>
auto elem( const T & item, const List<T> & list )
{
	return list.contains( item );
}

template<typename T>
auto reverse( const List<T> & list )
{
	return list.reverse();
}