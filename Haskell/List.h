#pragma once

#include <allocators>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <iostream>

template<typename T, typename A = std::allocator<T>>
class List
{
	T * _list = nullptr;
	A * _allocator;
	size_t _length;

private:
	List( size_t size ) : _allocator( new A() ),
							  _length(0) {
		if ( size > 0 ) _reserve( size );
	}

public:
	typedef T value_type;

	explicit List() : List(0) {
	}

	explicit List(const std::initializer_list<T> & list) : List(list.size()) {
		size_t i = 0;
		for (const T & elem : list) {
			_allocator->construct( &_list[i], elem );
			++i;
		}
	}

	List(T startInclusive, T endInclusive) : List (endInclusive - startInclusive + 1) {
		for ( T i = 0; i < _length; ++i )
		{
			_allocator->construct( &_list[i], startInclusive + i );
		}
	}

	List(const List & list) : List() {
		_deepCopy( list );
	}

	explicit List(const List & list0, const List & list1) : List(list0._length + list1._length) {
		_copyNoAlloc( list0, 0, list0._length, 0, list0._length );
		_copyNoAlloc( list1, list0._length, _length, 0, list1._length );
	}

	explicit List(const List & list, const T & newItem) : List(list._length + 1) {
		_copyNoAlloc( list );
		_allocator->construct( &_list[_length - 1], newItem );
	}

	explicit List(const T & newItem, const List & list) : List( list._length + 1 ) {
		_copyNoAlloc( list, 1, _length, 0, list._length );
		_allocator->construct( &list[0], newItem );
	}

	explicit List(const List & list, size_t copyStartInclusive, size_t copyEndExclusive)
		: List(copyEndExclusive - copyStartInclusive) {
		_copyNoAlloc( list, 0, _length, copyStartInclusive, copyEndExclusive );
	}

	List(List && list) : List() {
		_shallowCopy( list );
	}

	~List() {
		_destroy();
		delete _allocator;
		_allocator = nullptr;
	}

	bool isNull() const
	{
		return _length == 0;
	}

	size_t length() const {
		return _length;
	}

	const T & head() const {
		return _list[0];
	}

	const T & last() const {
		return _list[_length - 1];
	}

	List tail() const {
		if ( _length == 0 ) throw std::out_of_range( "Attempting to access null tail position" );
		return List( *this, 1, _length );
	}

	/**
	  * Returns everything except the last element
	  */
	List init() const {
		if ( _length == 0 ) throw std::out_of_range( "Attempting to access null tail position" );
		return List( *this, 0, _length - 1 );
	}

	List reverse() const {
		return _reverse( *this );
	}

	List take(size_t numElements) const {
		if ( numElements > _length ) return *this;
		return List( *this, 0, numElements );
	}

	List drop(size_t numElements) const {
		if ( numElements > _length ) return List();
		return List( *this, numElements, _length );
	}

	/** Overloaded operators */
	const T & operator[](size_t index) const {
		if ( !_isInRange( index ) ) {
			throw std::out_of_range( "List index out of range at " + std::to_string( index ) );
		}
		return _list[index];
	}

	List operator+( const T & item ) const {
		return List( *this, item);
	}

	List operator+( const List & other ) const {
		return List( *this, other );
	}

	bool operator==( const List & other ) const {
		if ( _length != other._length ) return false;
		for (size_t i = 0; i < _length; ++i) {
			if ( _list[i] != other[i] ) return false;
		}
		return true;
	}

	friend std::ostream & operator<<( std::ostream & out, const List & list ){
		std::string beginEnd;
		std::string delim;
		out << "[";
		for (size_t i = 0; i < list.length(); ++i) {
			out << list[i];
			if ( i + 1 != list.length() ) out << ",";
		}
		out << "]";
		return out;
	}

private:
	void _deepCopy( const List & other ) {
		_reserve( other._length );
		_copyNoAlloc( other );
	}

	void _shallowCopy(List && other) {
		_list = other._list;
		_allocator = other._allocator;
		_length = other._length;
		other._list = nullptr;
		other._allocator = nullptr;
		other._length = 0;
	}

	void _copyNoAlloc(const List & other) {
		for ( size_t i = 0; i < _length; ++i )
		{
			_allocator->construct( &_list[i], other._list[i] );
		}
	}

	void _copyNoAlloc( const List & other,
					   size_t thisStartInclusive, size_t thisEndExclusive,
					   size_t otherStartInclusive, size_t otherEndExclusive)
	{
		for ( size_t i = thisStartInclusive, j = otherStartInclusive; i < thisEndExclusive; ++i, ++j )
		{
			_allocator->construct( &_list[i], other._list[j] );
		}
	}

	void _destroy() {
		if ( isNull() ) return;
		for (size_t i = 0; i < _length; ++i) {
			_allocator->destroy( &_list[i] );
		}
		_allocator->deallocate( _list, _length );
		_length = 0;
		_list = nullptr;
	}

	void _reserve(size_t size) {
		_destroy();
		_length = size;
		_list = _allocator->allocate( _length );
	}

	bool _isInRange(size_t index) const {
		return index >= 0 && index < _length;
	}

	static List _reverse(const List & list) {
		if ( list.isNull() ) return List();
		return List{ list.last() } + _reverse( list.init() );
	}
};