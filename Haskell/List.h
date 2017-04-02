#pragma once

#include <allocators>
#include <initializer_list>
#include <stdexcept>
#include <string>

template<typename T, typename A = std::allocator<T>>
class List
{
	T * _list = nullptr;
	A * _allocator;
	size_t _length;

public:
	typedef T value_type;

	List(size_t size = 0) : _allocator(new A()) {
		_reserve( size );
	}

	List(const std::initializer_list<T> & list) : List(list.size()) {
		for (size_t i = 0; i < _length; ++i) {
			_allocator->construct( &_list[i], list[i] );
		}
	}

	List(const List & list) {
		_deepCopy( list );
	}

	List(const List & list0, const List & list1) : List(list0._length + list1._length) {
		_copyNoAlloc( list0, 0, list0._length );
		_copyNoAlloc( list1, list0._length, list1._length );
	}

	List(const List & list, const T & newItem) : List(list._length + 1) {
		_copyNoAlloc( list );
		_allocator->construct( &_list[_length - 1], newItem );
	}

	List(const T & newItem, const List & list) : List( list._length + 1 ) {
		_copyNoAlloc( list, 1, _length );
		_allocator->construct( &list[0], newItem );
	}

	List(List && list) {
		_shallowCopy( list );
	}

	~List() {
		_destroy();
		delete _allocator;
		_allocator = nullptr;
	}

	bool isNull() const
	{
		return _length == 0 && _list == nullptr;
	}

	size_t length() const {
		return _length;
	}

	/** Overloaded operators */
	const T & operator[](size_t index) const {
		if ( !_isInRange( index ) ) {
			throw std::out_of_range( "List index out of range at " + std::to_string( index ) );
		}
		return _list[index];
	}

	List operator+( const T & item ) const {
		return List( *this, item );
	}

	List operator+( const List & other ) const {
		return List( *this, other );
	}

	List operator==( const List & other ) const {
		if ( _length != other._length ) return false;
		for (size_t i = 0; i < _length; ++i) {
			if ( ( *this )[i] != other[i] ) return false;
		}
		return true;
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

	void _copyNoAlloc( const List & other, size_t startInclusive, size_t endExclusive )
	{
		for ( size_t i = startInclusive; i < endExclusive; ++i )
		{
			_allocator->construct( &_list[i], other._list[i] );
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
};