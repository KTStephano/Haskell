#pragma once

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>

#define MAX_DIGITS 2048
#define INTEGER_DEBUG 1 // Set this to 0 when done

// A possible implementation of a base-10 Big Integer
// See http://faculty.cse.tamu.edu/djimenez/ut/utsa/cs3343/lecture20.html
// See https://silentmatt.com/blog/2011/10/how-bigintegers-work-part-2-multiplication/
// See https://courses.csail.mit.edu/6.006/spring11/exams/notes3-karatsuba
// See https://web.stanford.edu/class/ee486/doc/chap5.pdf
// See http://stackoverflow.com/questions/16965915/convert-a-big-hex-number-string-format-to-a-decimal-number-string-format-w (printing numbers in base 10 which are stored in other bases)
class Integer
{
	size_t * _integer;
	size_t _maxDigits;
	size_t _usedDigits;
	const size_t _BASE = 10;
	bool _isNegative = false;

	Integer(const Integer & other, size_t startInclusive, size_t endExclusive) : Integer()
	{
		_deepCopy( other, startInclusive, endExclusive );
	}

public:
	Integer() : _maxDigits( MAX_DIGITS ), _usedDigits( 1 )
	{
		_integer = new size_t[_maxDigits]();
	}

	Integer(const char * num, size_t base = 10) //: Integer()
	{
		size_t len = strlen( num );
		//if ( len >= _maxDigits ) throw std::runtime_error( "Integer greater than 2048 digits" );
		Integer result = 0;
		for (int i = 0; i < len; ++i)
		{
			result = result * base + _toInt( num[i] );
		}
		_shallowCopy( result );
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
		/**
		for (int i = integer._usedDigits - 1; i >= 0; --i)
		{
			out << integer._integer[i] << " ";
		}
		*/
		std::vector<int> dec = std::vector<int>{ 0 };
		dec.reserve( 2048 );
		for (int i = integer._usedDigits - 1; i >= 0; --i)
		{
			int carry = integer[i];
			for (int j = 0; j < dec.size(); ++j)
			{
				int val = dec[j] * integer._BASE + carry;
				dec[j] = val % 10;
				carry = val / 10;
			}
			while (carry > 0)
			{
				dec.push_back( carry % 10 );
				carry /= 10;
			}
		}
		// Now dec stores the answer backwards
		for ( int i = dec.size() - 1; i >= 0; --i ) out << dec[i];
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
			int sum = result[index] + other[index] + carry;
			if (sum >= _BASE)
			{
				carry = 1;
				sum -= _BASE; // Something like 8*8 gives 16, so subtracting it by 10 gives us 6
			}
			else carry = 0;
			result[index] = sum;
		}
		if ( result == 0 ) result._isNegative = false; // Enforce this
		return result;
	}

	// This one REALLY needs to be reworked if you can find an algorithm that is
	// cleaner
	Integer operator-( const Integer & other ) const
	{
		Integer result = *this;
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
			if ( result[i] < other[i])
			{
				for (size_t j = i + 1; j < _usedDigits; ++j)
				{
					if ( result[j] > 0)
					{
						result[j] -= 1;
						for ( size_t k = j - 1; k > i; --k )
						{
							result[k] = _BASE - 1;
						}
						break;
					}
				}
				result[i] += _BASE;
			}
			result[i] -= other[i];
		}

		result._calculateUsedDigits();
		if ( result == 0 ) result._isNegative = false; // Enforce this
		return result;
	}

    /*
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
			carry = row0._integer[i] / _BASE;
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
				carry = row1._integer[j + offset] / _BASE;
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
     */

	// See http://library.aceondo.net/ebooks/Computer_Science/algorithm-the_art_of_computer_programming-knuth.pdf
	// page 253-254
    Integer operator*(const Integer & other) const
    {
        Integer result = 0;
        size_t n = _usedDigits;
        size_t m = other._usedDigits;
        //result._usedDigits = m + n + 3;
        unsigned long long temp = 0;
        size_t j, i;
        for (j = 0; j < m; ++j)
        {
            if (other[j] == 0)
            {
                result[m + j] = 0; // This offset might be wrong
                continue;
            }
            size_t carry = 0;
            for (i = 0; i < n; ++i)
            {
                temp = _integer[i] * other[j] + result[i + j] + carry;
                //std::cout << "\t\t" << temp << std::endl;
                result[i + j] = temp % _BASE;
                carry = temp / _BASE;
            }
            //result._integer[i + j + 1] = carry;
            result[i + j] = carry;
        }

        result._calculateUsedDigits();
        if ((_isNegative && !other._isNegative) || (!_isNegative && other._isNegative)) result._isNegative = true;
        if (result == 0) result._isNegative = false;
        return result;
    }

    // See http://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
    friend Integer pow(Integer base, unsigned int exp)
    {
        Integer result = 1;
        while (exp)
        {
            if (exp & 1)
            {
                result = result * base;
            }
            exp >>= 1;
            base = base * base;
        }
        return result;
    }

	// See http://library.aceondo.net/ebooks/Computer_Science/algorithm-the_art_of_computer_programming-knuth.pdf
	// page 257-258
	Integer operator/( const Integer & other ) const
	{
		//std::cout << "Got " << *this << " " << other << std::endl;
		/**
		Integer numerator = Integer( *this );
		Integer denominator = Integer( other );
		Integer quotient = Integer();
		Integer remainder = Integer();
		_divide( numerator, denominator, quotient, remainder );
		return quotient;
		*/
		Integer u = *this;
		Integer v = other;
		size_t n = v._usedDigits;
		size_t m = _usedDigits - n;
		if ( u < 0 ) u = u * -1;
		if ( v < 0 ) v = v * -1;
		if ( v == 0 ) throw std::runtime_error( "Division by 0 in Integer class" );
		if ( u < v ) return 0;
		Integer q = 0;
		Integer temp1 = 0;
		Integer temp2 = 0;
		size_t d = _BASE / ( v[n - 1] + 1 );
		u = u * d;
		v = v * d;
		long long j;
		for (long long k = 0; k <= m; ++k)
		{
			j = m + n - k;
			size_t q_norm;
			if ( u[j] == v[n - 1] ) q_norm = _BASE - 1;
			else q_norm = ( u[j] * _BASE + u[j - 1] ) / v[n - 1];
			
			while ( v[n - 2] * q_norm > ( u[j] * _BASE + u[j - 1] - q_norm * v[n - 1] ) * _BASE + u[j - 2] )
			{
				--q_norm;
			}

			temp1 = Integer( u, j - n, j + 1 );
			temp2 = temp1 - v * q_norm;
			if ( temp2 < 0 )
			{
				std::cout << "TRIPPED\n";
				temp2 = temp1 * pow( Integer( _BASE ), n + 1 );
				--q_norm;
				temp2 = v + temp2;
			}
			for ( long long i = j - n; i < j + 1; ++i ) u[i] = temp2[i - (j - n)];

			q = q * _BASE + q_norm;
		}
		
		if ( (_isNegative && !other._isNegative) || (!_isNegative && other._isNegative) ) q._isNegative = true;
		return q;
	}

    Integer operator<<(unsigned int digits) const
    {
        return *this * pow(2,digits);
    }

	Integer & operator++()
	{
		size_t index = 0;
		if ( _usedDigits == 0 ) ++_usedDigits;
		while (index < _maxDigits)
		{
			if ( index >= _usedDigits ) ++_usedDigits;
			(*this)[index] += 1;
			if ((*this)[index] % _BASE == 0)
			{
				(*this)[index] = 0;
				++index;
				continue;
			}
			break;
		}
		return *this;
	}

	friend Integer operator+( int a, Integer b )
	{
		return Integer( a ) + b;
	}

	friend Integer operator-( int a, Integer b )
	{
		return Integer( a ) - b;
	}

	friend Integer operator*( int a, Integer b )
	{
		return Integer( a ) * b;
	}

	friend Integer operator/( int a, Integer b )
	{
		return Integer( a ) / b;
	}

	bool operator==( const Integer & other ) const
	{
		if ( (_isNegative && !other._isNegative) || (!_isNegative && other._isNegative) ) return false;
		if ( _usedDigits != other._usedDigits ) return false;
		for (size_t index = 0; index < _usedDigits; ++index)
		{
			if ( (*this)[index] != other[index] ) return false;
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
		for ( int index = _usedDigits - 1; index >= 0; --index )
		{
			if ( bothNegative && (*this)[index] < other[index] ) return true;
			if ( !bothNegative && (*this)[index] > other[index] ) return true;
			if ( (*this)[index] == other[index] ) continue;
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
		return *this  > other == false && (*this == other) == false;
	}

	bool operator<=( const Integer & other ) const
	{
		return ( *this ) < other || ( *this ) == other;
	}

    bool operator!=(const Integer & other) const
    {
        return *this < other || *this > other;
    }

private:
	/** Privately-overloaded operators */
	Integer & operator=( const Integer & other )
	{
		_deepCopy( other );
		return *this;
	}

	Integer & operator=( Integer && other )
	{
		_shallowCopy( other );
		return *this;
	}

	size_t operator[](size_t index) const
	{
#if INTEGER_DEBUG
		if ( !_isInRange(index) ) throw std::runtime_error( "Index out of bounds inside of Integer class" );
#endif
		return _integer[index];
	}

	size_t & operator[]( size_t index )
	{
#if INTEGER_DEBUG
		if ( !_isInRange(index) ) throw std::runtime_error( "Index out of bounds inside of Integer class" );
#endif
		return _integer[index];
	}

	bool _isInRange(size_t index) const
	{
		return index < _maxDigits;
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
			(*this)[index] = i % _BASE;
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
			(*this)[i] = other[i];
		}
	}

	void _deepCopy(const Integer & other, size_t startInclusive, size_t endExclusive)
	{
		_maxDigits = other._maxDigits;
		_isNegative = other._isNegative;
		_usedDigits = endExclusive - startInclusive;
		size_t index = 0;
		for (size_t i = startInclusive; i < endExclusive; ++i)
		{
			(*this)[index] = other[i];
			++index;
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
		for ( int i = 0; i < _maxDigits; ++i ) (*this)[i] = 0;
		_usedDigits = 1;
		_isNegative = false;
	}

	void _calculateUsedDigits()
	{
		for (int i = _maxDigits - 1; i >= 0; --i)
		{
			if ((*this)[i] > 0)
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
