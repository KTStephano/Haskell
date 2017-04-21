#pragma once

#include <iostream>
#include <cmath>
#include <stdexcept>

#define MAX_DIGITS 2048

// A possible implementation of a base-10 Big Integer
// See http://faculty.cse.tamu.edu/djimenez/ut/utsa/cs3343/lecture20.html
// See https://silentmatt.com/blog/2011/10/how-bigintegers-work-part-2-multiplication/
// See https://courses.csail.mit.edu/6.006/spring11/exams/notes3-karatsuba
// See https://web.stanford.edu/class/ee486/doc/chap5.pdf
class Integer
{
	int * _integer;
	size_t _maxDigits;
	size_t _usedDigits;
	const size_t _BASE = 10;
	bool _isNegative = false;

public:
	Integer() : _maxDigits( MAX_DIGITS ), _usedDigits( 1 )
	{
		_integer = new int[_maxDigits]();
	}

	Integer(const char * num) : Integer()
	{
		size_t len = strlen( num );
		if ( len >= _maxDigits ) throw std::runtime_error( "Integer greater than 2048 digits" );
		for (int i = len - 1; i >= 0; --i)
		{
			_integer[i] = _toInt( num[len - i - 1] );
		}
		_usedDigits = len;
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

	Integer operator+( const Integer & other ) const
    {
        //std::cout << "Plus " << *this << " " << other << std::endl;
		Integer result = Integer( *this );
		// -x + y
        if (_isNegative && !other._isNegative)
        {
            return other - result * -1;
        }
		// x + -y
		if (!_isNegative && other._isNegative)
		{
			return result - other * -1;
		}
		size_t maxIndex = _usedDigits > other._usedDigits ? _usedDigits : other._usedDigits;
		result._usedDigits = maxIndex;
		int carry = 0;
		for (size_t index = 0; index <= maxIndex && index < _maxDigits; ++index)
		{
			if ( index == maxIndex && carry == 1 ) ++result._usedDigits;
			int sum = result._integer[index] + other._integer[index] + carry;
			if (sum >= _BASE)
			{
				carry = 1;
				sum -= _BASE; // Something like 8*8 gives 16, so subtracting it by 10 gives us 6
			}
			else carry = 0;
			result._integer[index] = sum;
		}
		if ( result == 0 ) result._isNegative = false; // Enforce this
		return result;
	}

	// This one REALLY needs to be reworked if you can find an algorithm that is
	// cleaner
	Integer operator-( const Integer & other ) const
	{
		Integer result = Integer( *this );
        // -x-y
        if (_isNegative && !other._isNegative)
        {
            result = (result * -1) + (other * -1);
            return result * -1;
        }
        // x-(-y) => x+y
        if (!_isNegative && other._isNegative) return result + other * -1;
        // -x-(-y) => -x+y => y-x
        if (_isNegative && other._isNegative)
        {
            return other * -1 - result * -1;
        }
        // x - y, x < y
        if (result < other)
        {
            result = other - result;
            return result * -1;
        }

		for (size_t i = 0; i < other._usedDigits; ++i)
		{
			if ( result._integer[i] < other._integer[i])
			{
				for (size_t j = i + 1; j < _usedDigits; ++j)
				{
					if ( result._integer[j] > 0)
					{
						result._integer[j] -= 1;
						for ( size_t k = j - 1; k > i; --k )
						{
							result._integer[k] = 9;
						}
						break;
					}
				}
				result._integer[i] += 10;
			}
			result._integer[i] -= other._integer[i];
		}

		result._calculateUsedDigits();
		if ( result == 0 ) result._isNegative = false; // Enforce this
		return result;
	}

	Integer operator*( const Integer & other ) const
	{
		Integer row0;
		Integer row1;
		int offset = 1;
		int carry = 0;
		//std::cout << "Times this " << *this << " versus that " << other << std::endl;

		row0._usedDigits = _usedDigits;
		for (size_t i = 0; i <= _usedDigits && i < _maxDigits; ++i)
		{
			if ( i == _usedDigits && carry != 0 ) ++row0._usedDigits;
            //else if (i < _usedDigits) ++row0._usedDigits;
			row0._integer[i] = _integer[i] * other._integer[0] + carry;
			carry = row0._integer[i] / 10;
			row0._integer[i] = row0._integer[i] % _BASE;
		}

		for (size_t i = 1; i < other._usedDigits; ++i)
		{
			//row1._usedDigits = offset + _usedDigits;// +( _usedDigits > other._usedDigits ? _usedDigits : other._usedDigits );
			for (size_t j = 0; j <= _usedDigits && j < _maxDigits; ++j )
			{
				//if ( j + offset == row1._usedDigits && ( carry != 0 || ( _integer[j] > 0 && other._integer[i] > 0 ) ) ) ++row1._usedDigits;
				//++row1._usedDigits;
				row1._integer[j + offset] = (_integer[j] * other._integer[i]) + carry;
				carry = row1._integer[j + offset] / 10;
				row1._integer[j + offset] = row1._integer[j + offset] % _BASE;
			}
			row1._calculateUsedDigits();
			//std::cout << "row1 " << row1 << std::endl;
			row0 = row0 + row1;
			++offset;
			row1._zero();
		}

		if ( row0 == 0 ) row0._isNegative = false;
		else if ( ( _isNegative && !other._isNegative ) || ( !_isNegative && other._isNegative ) ) row0._isNegative = true;
		//std::cout << "row0 " << row0 << std::endl;
		return row0;
	}

	Integer operator/( const Integer & other ) const
	{
		Integer numerator = Integer( *this );
		Integer denominator = Integer( other );
		Integer quotient = Integer();
		Integer remainder = Integer();
		_divide( numerator, denominator, quotient, remainder );
		return quotient;
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
	/** Privately-overloaded operators */
	Integer & operator=( const Integer & other )
	{
		_deepCopy( other );
		return *this;
	}

	Integer & operator=( Integer & other )
	{
		_shallowCopy( other );
		return *this;
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
		_usedDigits = index == 0 ? 1 : index;
	}

	void _deepCopy(const Integer & other)
	{
		_usedDigits = other._usedDigits;
		_isNegative = other._isNegative;
        _maxDigits = other._maxDigits;
		for (size_t i = 0; i < _maxDigits; ++i)
		{
			_integer[i] = other._integer[i];
		}
	}

	void _shallowCopy(Integer & other)
	{
		//delete _integer;
		_usedDigits = other._usedDigits;
		_isNegative = other._isNegative;
		_integer = other._integer;
        _maxDigits = other._maxDigits;
		other._integer = nullptr;
	}

	void _zero()
	{
		for ( int i = 0; i < _maxDigits; ++i ) _integer[i] = 0;
		_usedDigits = 1;
		_isNegative = false;
	}

	void _calculateUsedDigits()
	{
		for (int i = _maxDigits - 1; i >= 0; --i)
		{
			if (_integer[i] > 0)
			{
				_usedDigits = i + 1;
				return;
			}
		}
		_usedDigits = 1;
	}

	// See https://en.wikipedia.org/wiki/Division_algorithm
	void _divide(const Integer & numerator, const Integer & denominator, Integer & quotient, Integer & remainder) const
	{
		if ( denominator == 0 ) throw std::invalid_argument( "Division by zero error" );
		if (denominator < 0)
		{
			_divide( numerator, denominator * -1, quotient, remainder );
			quotient = quotient * -1;
		}
		if (numerator < 0)
		{
			_divide( numerator * -1, denominator, quotient, remainder );
			if ( remainder == 0 ) quotient = quotient * -1;
			else
			{
				quotient = quotient * -1 - 1;
				remainder = denominator - remainder;
			}
		}

		quotient = 0;
		remainder = numerator;
		while (remainder >= denominator)
		{
			++quotient;
			remainder = remainder - denominator;
		}
	}

	static int _toInt(char c)
	{
		return c - '0';
	}
};