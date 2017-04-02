#pragma once

#include <allocators>
#include <initializer_list>

template<typename T, typename A = std::allocator<T>>
class List
{
	T * _list;
	A _allocator;
	size_t _length;

public:
	typedef T value_type;

	List(size_t size = 0) : _list(_allocator.allocate( size )),
							_length( size ) {
	}

	List(const std::initializer_list<T> & list) : List(list.size()) {
		for (size_t i = 0; i < _length; ++i) {
			_allocator.construct( &_list[i], list[i] );
		}
	}
};