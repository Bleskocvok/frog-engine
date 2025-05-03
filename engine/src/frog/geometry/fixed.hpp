#pragma once

#include <cstdint>      // uint32_t, uint64_t
#include <stdexcept>
#include <type_traits>  // is_integral, enable_if
#include <cmath>        // round
#include <ostream>      // ostream
#include <string_view>
#include <cctype>       // isdigit
#include <sstream>      // stringstream


namespace frog::geo
{


template<typename Integral, int Decimals>
class fixed;

using fx32 = fixed< std::int32_t, 11 >;
using fx64 = fixed< std::int64_t, 22 >;


template< typename Integral, int Decimals >
class fixed
{
    // since C++20 this isn't implementation-defined anymore
    static_assert( Integral( -4 ) >> 1 == -2,
        "Necessary for arithmetics to work." );

    // and neither is this
    static_assert( 1 + ~Integral( 0xABCD ) == -0xABCD,
        "This platform must be using two's complement" );

    static constexpr Integral Div = Integral( 1 ) << Decimals;
    static constexpr Integral Mask = Div - 1;

    Integral value = 0;

public:
    fixed() = default;

    fixed( Integral x ) : value( x << Decimals ) {}

    fixed( const char* str ) { from_str( str ); }
    fixed( const std::string& str ) { from_str( str ); }

    template< typename T >
    fixed( T n ) : fixed( Integral( n ) )
    {
        static_assert( std::is_integral< T >::value, "T must be integral" );
    }

    fixed( float  x ) : value( x * Div ) {}
    fixed( double x ) : value( x * Div ) {}

    template< typename T >
    fixed( T numerator, T denominator ) : fixed( numerator )
    {
        *this /= denominator;
    }

    static fixed min()
    {
        fixed n;
        unsigned char* data = reinterpret_cast< unsigned char* >( &n.value );

        int end = 1;
        // little or big endian
        if ( reinterpret_cast< unsigned char* >( &end )[ 0 ] == 1 )
            data[ sizeof( Integral ) - 1 ] = 0x80;
        else
            data[ 0 ] = 0x80;
        return n;
    }

    static fixed max()
    {
        fixed n;
        n.value = 1;
        for ( unsigned i = 0; i < sizeof( Integral ) * 8 - 2; i++ )
        {
            n.value <<= 1;
            n.value += 1;
        }
        return n;
    }

    operator double() const
    {
        Integral n = to< Integral >();
        Integral bits = ( value & Mask );

        if ( value > 0 )
            return n + bits / ( 1.0 * Mask );

        // necessary to account for two's complement here
        // therefore one is subtracted from “decimal” bits
        // and then they are inverted
        double pt = ( ~( bits - 1 ) & Mask ) / ( 1.0 * Mask );
        return n - pt;
    }

    operator float() const { return static_cast< double >( *this ); }

    template< typename T >
    T to() const
    {
        static_assert( std::is_integral< T >::value, "T must be integral" );
        return T( value / Div );
    }

    operator int()                const { return to< int >(); };
    operator long()               const { return to< long >(); };
    operator long long()          const { return to< long long >(); };
    operator unsigned int()       const { return to< unsigned int >(); };
    operator unsigned long()      const { return to< unsigned long >(); };
    operator unsigned long long() const { return to< unsigned long long >(); };

    fixed& operator+=( fixed b )
    {
        value += b.value;
        return *this;
    }

    fixed& operator-=( fixed b )
    {
        value -= b.value;
        return *this;
    }

    fixed& operator*=( fixed b )
    {
        Integral v = value;
        value = (   v * ( b.value >> Decimals ) )
              + ( ( v * ( b.value & Mask ) ) >> Decimals );

        return *this;
    }

    fixed& operator/=( fixed b )
    {
        value = ( value << Decimals ) / b.value;
        return *this;
    }


    fixed& invert()
    {
        value = ( fixed{ 1 }.value * Div / value );
        return *this;
    }

    fixed recip() const { auto a = *this; a.invert(); return a; }

    template< typename T >
    fixed& operator+=( T b ) { return *this += fixed( b ); }

    template< typename T >
    fixed& operator-=( T b ) { return *this -= fixed( b ); }

    template< typename T >
    fixed& operator*=( T b ) { return *this *= fixed( b ); }

    template< typename T >
    fixed& operator/=( T b ) { return *this /= fixed( b ); }

    friend fixed operator-( fixed a ) { a.value = -a.value; return a; }

    friend fixed operator+( fixed a, fixed b ) { return a += b; }
    friend fixed operator-( fixed a, fixed b ) { return a -= b; }
    friend fixed operator*( fixed a, fixed b ) { return a *= b; }
    friend fixed operator/( fixed a, fixed b ) { return a /= b; }
    friend bool operator<(  fixed a, fixed b ) { return a.value < b.value; }
    friend bool operator>=( fixed a, fixed b ) { return not ( a < b ); }
    friend bool operator>(  fixed a, fixed b ) { return a.value > b.value; }
    friend bool operator<=( fixed a, fixed b ) { return not ( a > b ); }
    friend bool operator==( fixed a, fixed b ) { return a.value == b.value; }
    friend bool operator!=( fixed a, fixed b ) { return not ( a == b ); }

    // :: fixed -> T -> fixed/bool

    template< typename T >
    friend fixed operator+( fixed a, T b ) { return a += fixed( b ); }

    template< typename T >
    friend fixed operator-( fixed a, T b ) { return a -= fixed( b ); }

    template< typename T >
    friend fixed operator*( fixed a, T b ) { return a *= fixed( b ); }

    template< typename T >
    friend fixed operator/( fixed a, T b ) { return a /= fixed( b ); }

    template< typename T >
    friend bool operator<(  fixed a, T b ) { return a < fixed( b ); }

    template< typename T >
    friend bool operator>=( fixed a, T b ) { return not ( a < b ); }

    template< typename T >
    friend bool operator>(  fixed a, T b ) { return a > fixed( b ); }

    template< typename T >
    friend bool operator<=( fixed a, T b ) { return not ( a > b ); }

    template< typename T >
    friend bool operator==( fixed a, T b ) { return a == fixed( b ); }

    template< typename T >
    friend bool operator!=( fixed a, T b ) { return not ( a == b ); }

    // :: T -> fixed -> fixed/bool

    template< typename T >
    friend fixed operator+( T a, fixed b ) { return fixed( a ) += b; }

    template< typename T >
    friend fixed operator-( T a, fixed b ) { return fixed( a ) -= b; }

    template< typename T >
    friend fixed operator*( T a, fixed b ) { return fixed( a ) *= b; }

    template< typename T >
    friend fixed operator/( T a, fixed b ) { return fixed( a ) /= b; }

    template< typename T >
    friend bool operator<(  T a, fixed b ) { return fixed( a ) < b; }

    template< typename T >
    friend bool operator>=( T a, fixed b ) { return not ( a < b ); }

    template< typename T >
    friend bool operator>(  T a, fixed b ) { return fixed( a ) > b; }

    template< typename T >
    friend bool operator<=( T a, fixed b ) { return not ( a > b ); }

    template< typename T >
    friend bool operator==( T a, fixed b ) { return fixed( a ) == b; }

    template< typename T >
    friend bool operator!=( T a, fixed b ) { return not ( a == b ); }

    std::string to_str() const
    {
        std::stringstream o;
        o << *this;
        return o.str();
    }

    friend std::ostream& operator<<( std::ostream& out, fixed a )
    {
        using buf_t = std::uint64_t;

        buf_t stuff = 100000000000llu;

        bool negative = false;
        Integral val = a.value;

        if ( val < 0 )
        {
            val = ~val;
            val += 1;
            negative = true;
        }

        // remove rightmost zeroes
        auto normalize = []( auto n )
        {
            while ( n > 1 && n % 10 == 0 )
                n /= 10;
            return n;
        };

        auto integral = Integral( negative ? -a : a );

        buf_t ds = stuff * ( val & Mask ) / Div;
        return out << integral << "." << normalize( ds );
    }

    void from_str( std::string_view str )
    {
        auto error = [&]()
        {
            return std::runtime_error( "invalid number string \""
                                       + std::string( str ) + "\"" );
        };

        bool dot = false;
        for ( unsigned i = 0; i < str.size(); i++ )
        {
            char c = str[ i ];
            if ( c == '.' )
            {
                if ( dot )
                    throw error();
                dot = true;
            }
            else if ( !std::isdigit( static_cast< unsigned char >( c ) ) )
                throw error();
        }

        unsigned i = 0;

        for ( ; i < str.size(); i++ )
        {
            char c = str[ i ];
            if ( c == '.' )
                break;

            unsigned digit = c - '0';
            *this *= 10;
            *this += digit;
        }

        if ( dot )
        {
            using buf_t = std::uint64_t;
            buf_t exp10 = 1;

            for ( i++; i < str.size(); i++ )
            {
                char c = str[ i ];
                buf_t digit = c - '0';

                exp10 *= 10;
                *this += fixed( digit, exp10 );
            }

            // *this += fixed{ digits } / fixed{ exp10 };
        }
    }
};


}  // namespace frog::geo
