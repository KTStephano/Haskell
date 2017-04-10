#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <math.h>

template<typename T>
class List
{
	typedef struct _Data
	{
		T * item = nullptr;
		std::shared_ptr<_Data> next;

		_Data(const T & item) : item(new T(item)) {  }

		~_Data()
		{
			delete item;
		}
	};

	std::shared_ptr<_Data> _list;
	std::shared_ptr<_Data> _last;
	size_t _length;

public:
	typedef T value_type;

	explicit List() : _length( 0 ) {  }

	explicit List( const std::initializer_list<T> & list ) : List()
	{
		for ( const T & element : list ) _insertLast( element );
	}

	// for [x..n]
	explicit List(T startInclusive, T endInclusive) : List()
	{
		size_t len = endInclusive - startInclusive + 1;
		for ( T i = 0; i < len; ++i ) _insertLast( startInclusive + i );
	}

	// for [x,y..n]
	explicit List(T startInclusive, T nextInclusive, T endInclusive) : List()
	{
		if ( nextInclusive < 0 ) return; // Leave as empty list (Haskell does this too)
		T offset = nextInclusive - startInclusive;
		if ( offset < 0 && startInclusive < endInclusive ) offset *= -1;
		T curr = startInclusive;
		std::cout << startInclusive << " " << nextInclusive << " " << endInclusive << " " << offset << " " << curr << std::endl;
		while (true)
		{
			if ( startInclusive < endInclusive && curr > endInclusive ) break;
			if ( startInclusive > endInclusive && curr < endInclusive ) break;
			_insertLast( curr );
			curr += offset;
		}
	}

	List(const List & list) : List()
	{
		_shallowCopy( list ); // Shallow copy is intentional
	}

	List(List && list) : List()
	{
		_shallowCopy( list );
	}

	explicit List(const List & list0, const List & list1) : List()
	{
		_copyTwoLs( list0, list1 );
	}

	explicit List(const List & list, const T & newItem) : List()
	{
		_copyLsItem( list, newItem );
	}

	explicit List(const T & newItem, const List & list) : List()
	{
		_copyItemLs( newItem, list );
	}

	explicit List(const List & list, size_t startInclusive, size_t endExclusive) : List()
	{
		_shallowCopy( list, startInclusive, endExclusive );
	}

	List<T> getNullVariant() const
	{
		return List<T>();
	}

	bool isNull() const
	{
		return _length == 0;
	}

	bool contains(const T & elem) const
	{
		for (size_t i = 0; i < _length; ++i)
		{
			if ( ( *this )[i] == elem ) return true;
		}
		return false;
	}

	size_t length() const
	{
		return _length;
	}

	// Constant time
	const T & head() const
	{
		return ( *this )[0];
	}

	// Constant time
	const T & last() const
	{
		return ( *this )[_length - 1];
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
		if ( _length == 0 ) throw std::out_of_range( "Attempting to call init on the empty list" );
		return List( *this, 0, _length - 1 );
	}

	List reverse() const
	{
		return _reverse( *this );
	}

	List take(size_t numElements) const
	{
		if ( numElements > _length ) return *this;
		return List( *this, 0, numElements );
	}

	List drop(size_t numElements) const
	{
		if ( numElements > _length ) return List();
		return List( *this, numElements, _length );
	}

	/** Overloaded operators */
	const T & operator[](size_t index) const
	{
		if (!_isInRange(index))
		{
			throw std::out_of_range( "List index out of range at " + std::to_string( index ) );
		}
		if ( index + 1 == _length && _length > 1 ) return *( _last->item );
		std::shared_ptr<_Data> ret = _list;
		for (size_t i = 0; i < index; ++i)
		{
			ret = ret->next;
		}
		return *( ret->item );
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
		std::shared_ptr<_Data> currThis = _list;
		std::shared_ptr<_Data> currOther = other._list;
		for (size_t i = 0; i < indices; ++i)
		{
			if ( *( currThis->item ) > *( currOther->item ) ) return true;
			if ( *( currThis->item ) == *( currOther->item ) )
			{
				currThis = currThis->next;
				currOther = currOther->next;
				continue;
			}
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
		std::shared_ptr<_Data> currThis = _list;
		std::shared_ptr<_Data> currOther = other._list;
		for (size_t i = 0; i < _length; ++i)
		{
			if ( *( currThis->item ) != *( currOther->item ) ) return false;
			currThis = currThis->next;
			currOther = currOther->next;
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
		list._printElements( out, delim );
		out << end;
		return out;
	}

private:
	void _printElements(std::ostream & out, const std::string delim) const
	{
		std::shared_ptr<_Data> curr = _list;
		for (size_t i = 0; i < _length; ++i)
		{
			out << *( curr->item );
			if ( i + 1 != _length ) out << delim;
			curr = curr->next;
		}
	}

	// This is a dangerous function - only call when doing the equivalent of 
	// a deep copy since it modifies the last object of the list (which could
	// be referenced by many other lists)
	void _insertLast(const T & item)
	{
		if (_length == 0)
		{
			_list = std::make_shared<_Data>( item );
			_last = _list;
		}
		else
		{
			std::shared_ptr<_Data> newEntry = std::make_shared<_Data>( item );
			_last->next = newEntry;
			_last = newEntry;
		}
		++_length;
	}

	void _deepCopy(const List & other)
	{
		std::shared_ptr<_Data> curr = other._list;
		for ( size_t i = 0; i < other._length; ++i )
		{
			_insertLast( *( curr->item ) );
			curr = curr->next;
		}
	}

	void _shallowCopy(const List & other)
	{
		_list = other._list;
		_last = other._last;
		_length = other._length;
	}

	void _shallowCopy(const List & other, size_t startInclusive, size_t endExclusive)
	{
		if ( startInclusive == 0 && endExclusive == other._length )
		{
			_shallowCopy( other );
			return;
		}

		_length = endExclusive - startInclusive;
		std::shared_ptr<_Data> curr = other._list;
		for (size_t i = 0; i < endExclusive; ++i)
		{
			if ( i == startInclusive )
			{
				_list = curr;
				if (endExclusive == other._length && _length > 1)
				{
					_last = other._last;
					return;
				}
			}
			if ( i + 1 == endExclusive ) _last = curr;
			curr = curr->next;
		}
	}

	void _copyTwoLs(const List & list0, const List & list1)
	{
		_deepCopy( list0 );
		if (list0._length == 0)
		{
			_list = list1._list;
			_last = list1._last;
		}
		else
		{
			_last->next = list1._list;
			_last = list1._last;
		}
		_length += list1._length;
	}

	void _copyLsItem(const List & list, const T & item)
	{
		_deepCopy( list );
		_insertLast( item );
	}

	void _copyItemLs(const T & item, const List & list)
	{
		_insertLast( item );
		_list->next = list._list;
		_last = list._last;
		_length += list._length;
	}

	bool _isInRange(size_t index) const 
	{
		return index >= 0 && index < _length;
	}

	static List _reverse(const List & list)
	{
		if ( list.isNull() ) return List();
		if ( list.length() == 1 ) return List{ list.head() };
		return _reverse( list.tail() ) + List{ list.head() };
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

template<typename T>
auto empty( const List<T> & list )
{
	return list.getNullVariant();
}