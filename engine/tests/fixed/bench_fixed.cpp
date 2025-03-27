
#include "frog/geometry/fixed.hpp"

#include <iostream>     // cout
#include <cstdio>       // printf
#include <chrono>       // chrono::*
#include <vector>       // vector
#include <iomanip>      // setw, setprecision


template< typename T >
T plus( [[maybe_unused]] T a,
        [[maybe_unused]] T b, T x, T y )
{
    T sum = 0;
    sum += x + y;
    sum += y + x;
    return sum;
}


template< typename T >
T minus( [[maybe_unused]] T a,
         [[maybe_unused]] T b, T x, T y )
{
    T sum = 0;
    sum += -x - x;
    sum -= y - x + y;
    return sum;
}


template< typename T >
T mult( [[maybe_unused]] T a, [[maybe_unused]] T b, T x, T y )
{
    T sum = 0;
    sum += x * y;
    sum += y * x;
    return sum;
}


template< typename T >
T mult_div( T a, T b, T x, T y )
{
    T sum = 0;
    sum += x * y;
    sum += y * x;
    auto top = T( 1 ) / T( 2 );
    auto bottom = -top;
    if ( b <= bottom || b >= top ) sum += x / y;
    if ( a <= bottom || a >= top ) sum += y / x;
    return sum;
}


template< typename T, typename Op >
void bench( T min, T step, long long count, Op op )
{
    using namespace std;

    step = T( 1 ) / step;

    auto results = std::vector< T >();
    results.reserve(count);

    auto start = chrono::steady_clock::now();

    T a = min;
    for ( long long i = 0; i < count; a += step, i++)
    {
        T sum = 0;
        T b = min;
        for ( long long j = 0; j < count; b += step, j++)
        {
            T x = a;
            T y = b;

            sum += op( a, b, x, y );
        }
        results.push_back( sum );
    }

    auto end = chrono::steady_clock::now();
    auto ms = std::chrono::duration< double, std::milli >( end - start );

    volatile auto copy = results;

    T sum = 0;
    bool b = false;
    for ( const T& x : results )
    {
        sum += b ? x : -x;
        b = not b;
    }

    std::cout << "T= " << std::setw( 10 ) << ms.count() << " ms;  "
              << "sum=" << sum << std::endl;
}

#define BENCH( T, op ) \
    {   std::cout << std::setw( 20 ) << #T << " (" \
                  << std::setw( 2 ) << sizeof( T ) << " B)  "; \
        bench< T >( -100, 100, 20000, op ); \
        std::cout << std::endl;   }


int main()
{
    using namespace frog::geo;

    auto pl = [](auto... args){ return plus(args...); };
    auto mi = [](auto... args){ return minus(args...); };
    auto mul = [](auto... args){ return mult(args...); };
    auto muldiv = [](auto... args){ return mult_div(args...); };

    std::cout.precision( 6 );

    std::cout << "fx32: " << "< " <<  fx32::min() << "; " << fx32::max() << " >" << "\n";
    std::cout << "fx64: " << "< " <<  fx64::min() << "; " << fx64::max() << " >" << "\n" << std::endl;

    std::printf( "PLUS\n" );
    BENCH( fx64, pl );
    BENCH( fx32, pl );
    BENCH( double, pl );
    BENCH( float, pl );
    BENCH( long double, pl );

    std::printf( "MINUS\n" );
    BENCH( fx64, mi );
    BENCH( fx32, mi );
    BENCH( double, mi );
    BENCH( float, mi );
    BENCH( long double, mi );

    std::printf( "MULT\n" );
    BENCH( fx64, mul );
    BENCH( fx32, mul );
    BENCH( double, mul );
    BENCH( float, mul );
    BENCH( long double, mul );

    std::printf( "MULT & DIV\n" );
    BENCH( fx64, muldiv );
    BENCH( fx32, muldiv );
    BENCH( double, muldiv );
    BENCH( float, muldiv );
    BENCH( long double, muldiv );

    return 0;
}
