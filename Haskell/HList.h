#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <math.h>

template<typename T>
class HList
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

	explicit HList() : _length( 0 ) {  }

	explicit HList( const std::initializer_list<T> & list ) : HList()
	{
		for ( const T & element : list ) _insertLast( element );
	}

	// for [x..n]
	explicit HList(T startInclusive, T endInclusive) : HList()
	{
		size_t len = endInclusive - startInclusive + 1;
		for ( T i = 0; i < len; ++i ) _insertLast( startInclusive + i );
	}

	// for [x,y..n]
	explicit HList(T startInclusive, T nextInclusive, T endInclusive) : HList()
	{
		if ( nextInclusive < 0 ) return; // Leave as empty list (Haskell does this too)
		T offset = nextInclusive - startInclusive;
		if ( offset < 0 && startInclusive < endInclusive ) offset *= -1;
		T curr = startInclusive;
		while (true)
		{
			if ( startInclusive < endInclusive && curr > endInclusive ) break;
			if ( startInclusive > endInclusive && curr < endInclusive ) break;
			_insertLast( curr );
			curr += offset;
		}
	}

	HList(const HList & list) : HList()
	{
		_shallowCopy( list ); // Shallow copy is intentional
	}

	HList(HList && list) : HList()
	{
		_shallowCopy( list );
	}

	explicit HList(const HList & list0, const HList & list1) : HList()
	{
		_copyTwoLs( list0, list1 );
	}

	explicit HList(const HList & list, const T & newItem) : HList()
	{
		_copyLsItem( list, newItem );
	}

	explicit HList(const T & newItem, const HList & list) : HList()
	{
		_copyItemLs( newItem, list );
	}

	explicit HList(const HList & list, size_t startInclusive, size_t endExclusive) : HList()
	{
		_shallowCopy( list, startInclusive, endExclusive );
	}

	virtual ~HList()
	{
		_length = 0;
	}

	HList<T> getNullVariant() const
	{
		return HList<T>();
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

	HList tail() const
	{
		if ( _length == 0 ) throw std::out_of_range( "Attempting to access null tail position" );
		return HList( *this, 1, _length );
	}

	/**
	  * Returns everything except the last element
	  */
	HList init() const
	{
		if ( _length == 0 ) throw std::out_of_range( "Attempting to call init on the empty list" );
		return HList( *this, 0, _length - 1 );
	}

	HList reverse() const
	{
		return _reverse( *this );
	}

	HList take(size_t numElements) const
	{
		if ( numElements > _length ) return *this;
		return HList( *this, 0, numElements );
	}

	HList drop(size_t numElements) const
	{
		if ( numElements > _length ) return HList();
		return HList( *this, numElements, _length );
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

	HList operator+( const T & item ) const
	{
		return HList( *this, item );
	}

	// This should only be used for appending the empty list
	template<typename K>
	HList operator+( const HList<K> & other ) const
	{
		if ( other.length() != 0 ) throw std::runtime_error( "Can not append lists of different type which are not the empty list" );
		return HList( *this );
	}

	HList operator+( const HList & other ) const
	{
		return HList( *this, other );
	}

	friend HList<T> operator+( const T & item, const HList<T> & list )
	{
		return HList<T>( item, list );
	}

	bool operator>( const HList & other ) const
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

	bool operator<( const HList & other ) const
	{
		return other > *this;
	}

	bool operator>=( const HList & other ) const
	{
		bool equal = *this == other;
		return equal ? true : *this > other;
	}

	bool operator<=( const HList & other ) const
	{
		return other >= *this;
	}

	bool operator==( const HList & other ) const
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

	template<typename K>
	bool operator==( const HList<K> & other ) const
	{
		if ( _length == 0 && other.length() == 0 ) return true;
		return false;
	}

	friend std::ostream & operator<<( std::ostream & out, const HList & list )
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

	void _deepCopy(const HList & other)
	{
		std::shared_ptr<_Data> curr = other._list;
		for ( size_t i = 0; i < other._length; ++i )
		{
			_insertLast( *( curr->item ) );
			curr = curr->next;
		}
	}

	void _shallowCopy(const HList & other)
	{
		_list = other._list;
		_last = other._last;
		_length = other._length;
	}

	void _shallowCopy(const HList & other, size_t startInclusive, size_t endExclusive)
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

	void _copyTwoLs(const HList & list0, const HList & list1)
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

	void _copyLsItem(const HList & list, const T & item)
	{
		_deepCopy( list );
		_insertLast( item );
	}

	void _copyItemLs(const T & item, const HList & list)
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

	static HList _reverse(const HList & list)
	{
		if ( list.isNull() ) return HList();
		if ( list.length() == 1 ) return HList{ list.head() };
		return _reverse( list.tail() ) + HList{ list.head() };
	}
};

typedef HList<char> String;

template<typename T>
bool null( const HList<T> & list )
{
	return list.isNull();
}

template<typename T>
auto head( const HList<T> & list )
{
	return list.head();
}

template<typename T>
auto tail( const HList<T> & list )
{
	return list.tail();
}

template<typename T>
auto last( const HList<T> & list )
{
	return list.last();
}

template<typename T>
auto init( const HList<T> & list )
{
	return list.init();
}

template<typename T>
auto length( const HList<T> & list )
{
	return list.length();
}

template<typename T>
auto take( std::size_t num, const HList<T> & list )
{
	return list.take( num );
}

template<typename T>
auto drop( std::size_t num, const HList<T> & list )
{
	return list.drop( num );
}

template<typename T>
auto _minimum( const HList<T> & list, const T & min )
{
	if ( null( list ) ) return min;
	else if ( head( list ) < min ) return _minimum( tail( list ), head( list ) );
	return _minimum( tail( list ), min );
}

template<typename T>
auto minimum( const HList<T> & list )
{
	return _minimum( tail( list ), head( list ) );
}

template<typename T>
auto _maximum( const HList<T> & list, const T & max )
{
	if ( null( list ) ) return max;
	else if ( head( list ) > max ) return _maximum( tail( list ), head( list ) );
	return _maximum( tail( list ), max );
}

template<typename T>
auto maximum( const HList<T> & list )
{
	return _maximum( tail( list ), head( list ) );
}

template<typename T>
auto sum( const HList<T> & list, const T & acc = T() )
{
	if ( null( list ) ) return acc;
	return sum( tail( list ), acc + head( list ) );
}

template<typename T>
auto product( const HList<T> & list, const T & acc = 1 )
{
	if ( null( list ) ) return acc;
	return product( tail( list ), acc * head( list ) );
}

template<typename T>
auto elem( const T & item, const HList<T> & list )
{
	return list.contains( item );
}

template<typename T>
auto reverse( const HList<T> & list )
{
	return list.reverse();
}

template<typename T>
auto empty( const HList<T> & list )
{
	return list.getNullVariant();
}

