#pragma once

#include <iostream>
#include <cmath>

#define MAX_DIGITS 2048

// A possible implementation of a base-10 Big Integer
// See http://faculty.cse.tamu.edu/djimenez/ut/utsa/cs3343/lecture20.html
// See https://silentmatt.com/blog/2011/10/how-bigintegers-work-part-2-multiplication/
class Integer
{
	int * _integer;
	size_t _maxDigits;
	size_t _usedDigits;
	const size_t _BASE = 10;
	bool _isNegative = false;

public:
	Integer() : _maxDigits( MAX_DIGITS ), _usedDigits( 0 )
	{
		_integer = new int[_maxDigits]();
	}

	Integer(const Integer & other) : Integer()
	{
		_deepCopy( other );
	}

	Integer(Integer && other)
	{
		_shallowCopy( other );
	}

	Integer(int i) : Integer()
	{
		_setToInteger( i );
	}

	~Integer()
	{
		delete _integer;
	}

	/** Overloaded operators */
	friend std::ostream & operator<<( std::ostream & out, const Integer & integer )
	{
		if ( integer._isNegative ) out << "-";
		for (int i = integer._usedDigits - 1; i >= 0; --i)
		{
			out << integer._integer[i];
		}
		return out;
	}

	Integer & operator+( const Integer & other )
	{
		size_t maxIndex = _usedDigits > other._usedDigits ? _usedDigits : other._usedDigits;
		_usedDigits = maxIndex;
		int carry = 0;
		for (size_t index = 0; index <= maxIndex, index < _maxDigits; ++index)
		{
			if ( index == _usedDigits && carry == 1 ) ++_usedDigits;
			int sum = _integer[index] + other._integer[index] + carry;
			if (sum >= _BASE)
			{
				carry = 1;
				sum -= _BASE; // Something like 8*8 gives 16, so subtracting it by 10 gives us 6
			}
			else carry = 0;
			_integer[index] = sum;
		}
		return *this;
	}

	Integer & operator-( const Integer & other )
	{
		for (size_t i = 0; i < other._usedDigits; ++i)
		{
			if (_integer[i] < other._integer[i])
			{
				for (size_t j = i + 1; j < _usedDigits; ++j)
				{
					if ( _integer[j] > 0)
					{
						_integer[j] -= 1;
						for ( size_t k = j - 1; k > i; --k )
						{
							_integer[k] = 9;
						}
						break;
					}
				}
				_integer[i] += 10;
			}
			_integer[i] -= other._integer[i];
		}
		return *this;
	}

	Integer & operator++()
	{
		size_t index = 0;
		if ( _usedDigits == 0 ) ++_usedDigits;
		while (index < _maxDigits)
		{
			if ( index >= _usedDigits ) ++_usedDigits;
			_integer[index] += 1;
			if (_integer[index] % _BASE == 0)
			{
				_integer[index] = 0;
				++index;
				continue;
			}
			break;
		}
		return *this;
	}

	bool operator==( const Integer & other ) const
	{
		if ( _isNegative && !other._isNegative || !_isNegative && other._isNegative ) return false;
		if ( _usedDigits != other._usedDigits ) return false;
		for (size_t index = 0; index < _usedDigits; ++index)
		{
			if ( _integer[index] != other._integer[index] ) return false;
		}
		return true;
	}

	bool operator>( const Integer & other ) const
	{
		if ( _isNegative && !other._isNegative ) return false;
		if ( !_isNegative && other._isNegative ) return true;
		bool bothNegative = _isNegative && other._isNegative;
		if ( _usedDigits > other._usedDigits && !bothNegative ) return true;
		if ( _usedDigits > other._usedDigits && bothNegative ) return false;
		if ( _usedDigits < other._usedDigits && !bothNegative ) return false;
		if ( _usedDigits < other._usedDigits && bothNegative ) return true;
		for ( int index = _usedDigits - 1; index >= 0; ++index )
		{
			if ( bothNegative && _integer[index] < other._integer[index] ) return true;
			if ( !bothNegative && _integer[index] > other._integer[index] ) return true;
			if ( _integer[index] == other._integer[index] ) continue;
			break;
		}
		return false;
	}

	bool operator>=( const Integer & other ) const
	{
		return ( *this ) > other || ( *this ) == other;
	}

	bool operator<( const Integer & other ) const
	{
		return !( ( *this ) > other );
	}

	bool operator<=( const Integer & other ) const
	{
		return ( *this ) < other || ( *this ) == other;
	}

private:
	void _setToInteger(int i)
	{
		size_t index = 0;
		size_t usedBitsMarker = _usedDigits;
		if (i < 0)
		{
			i *= -1;
			_isNegative = true;
		}

		while (i)
		{
			_integer[index] = i % _BASE;
			++index;
			i = i / _BASE;
		}
		_usedDigits = index;
	}

	void _deepCopy(const Integer & other)
	{
		_usedDigits = other._usedDigits;
		_isNegative = other._isNegative;
		for (size_t i = 0; i < _usedDigits; ++i)
		{
			_integer[i] = other._integer[i];
		}
	}

	void _shallowCopy(Integer & other)
	{
		delete _integer;
		_usedDigits = other._usedDigits;
		_isNegative = other._isNegative;
		_integer = other._integer;
		other._integer = nullptr;
	}
};