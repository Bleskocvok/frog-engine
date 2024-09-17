
#include "frog/geometry/fixed.hpp"

#include <iostream>     // cout
#include <cstdio>       // printf
#include <chrono>       // chrono::*
#include <vector>       // vector
#include <cmath>        // fabs
#include <iomanip>      // setw, setprecision


template< typename T >
T plus( [[maybe_unused]] double a,
        [[maybe_unused]] double b, T x, T y )
{
    T sum = 0;
    sum += x + y;
    sum += y + x;
    return sum;
}


template< typename T >
T mult( double a, double b, T x, T y )
{
    T sum = 0;
    sum += x * y;
    sum += y * x;
    if ( std::fabs( b ) >= 0.5 ) sum += x / y;
    if ( std::fabs( a ) >= 0.5 ) sum += y / x;
    return sum;
}


template< typename T, typename Op >
void bench( double min, double max, double step, Op op )
{
    using namespace std;

    long long count = ( ( max - min ) / step );
    auto results = std::vector< T >( count, T( 0 ) );

    auto start = chrono::steady_clock::now();

    for ( double a = min; a <= max; a += step )
    {
        T sum = 0;
        for ( double b = min; b <= max; b += step )
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
        b = !b;
    }

    std::cout << "T= " << std::setw( 10 ) << ms.count() << " ms;  "
              << "sum=" << sum << std::endl;
}

#define BENCH( T, op ) \
    {   std::cout << std::setw( 20 ) << #T << " (" \
                  << std::setw( 2 ) << sizeof( T ) << " B)  "; \
        bench< T >( -100, 100, 0.01, op ); \
        std::cout << std::endl;   }


int main()
{
    using namespace frog::geo;

    auto pl = [](auto... args){ return plus(args...); };
    auto mul = [](auto... args){ return mult(args...); };

    std::cout.precision( 6 );

    std::printf( "PLUS\n" );
    BENCH( fx64, pl );
    BENCH( fx32, pl );
    BENCH( double, pl );
    BENCH( float, pl );
    BENCH( long double, pl );

    std::printf( "MULT\n" );
    BENCH( fx64, mul );
    BENCH( fx32, mul );
    BENCH( double, mul );
    BENCH( float, mul );
    BENCH( long double, mul );

    return 0;
}
