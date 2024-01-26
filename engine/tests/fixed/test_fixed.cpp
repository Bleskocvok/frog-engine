
#undef NDEBUG

#include "geometry/fixed.hpp"

#include <cassert>      // assert
#include <cmath>        // fabs
#include <cstdio>       // printf
#include <iostream>     // count


bool equal( int a, int b )
{
    bool res = a == b;
    if ( !res )
        std::printf( "a=%d\nb=%d\n", a, b );
    return res;
}

constexpr double DoubleDelta = 10e-3;
constexpr double FloatDelta  = 3 * 10e-3;


bool is_close( double a, double b, double D = DoubleDelta )
{
    bool res = std::fabs( a - b ) <= D;
    if ( !res )
        std::printf( "a=%f\nb=%f\n", a, b );
    return res;
}


bool is_close( float a, float b, float D = FloatDelta )
{
    bool res = std::fabs( a - b ) <= D;
    if ( !res )
        std::printf( "a=%f\nb=%f\n", a, b );
    return res;
}


int main()
{
    using namespace frog::geo;

    fx32 a = 0;
    a = 45;
    a = 45L;
    a = 45LU;
    a = float( 45.01f );
    a = double( 45.01 );

    assert( equal( fx32( 100 ), 100 ) );
    assert( equal( fx32( 13 ), 13 ) );
    assert( equal( fx32( 100.0f ), 100 ) );
    assert( equal( fx32( 13.0f ), 13 ) );
    assert( equal( fx32( 4541 ), 4541 ) );
    assert( equal( fx32( 45414.0f ), 45414 ) );

    assert( equal( fx64( 4541 ), 4541 ) );
    assert( equal( fx64( 45412 ), 45412 ) );
    assert( equal( fx64( 454123 ), 454123 ) );
    assert( equal( fx64( 4541234.0f ), 4541234 ) );

    assert( is_close( fx32( 100.0f ), 100.0f ) );

    assert( is_close( fx64( -20.1 ) * fx64( -10.5 ), 211.05 ) );

    auto recip = fx64( 1 ) / fx64( 100 );
    assert( is_close( fx64( 100 ) * recip, 1.0 ) );
    assert( is_close( fx64( 1 ) / recip, 100.0 ) );

    assert( is_close( fx64(  10 ).recip(),  0.1 ) );
    assert( is_close( fx64( -10 ).recip(), -0.1 ) );
    assert( is_close( fx64( 100 ).recip(),  0.01 ) );
    assert( is_close( fx64( -100 ).recip(),  -0.01 ) );
    assert( is_close( fx64( 100 ) * fx64( 100 ).recip(), 1.0 ) );
    assert( is_close( fx64( 1 ) / fx64( 100 ).recip(), 100.0 ) );

    for ( int i = -10000; i <= 10000; ++i )
        assert( equal( fx32( i ), i ) );

    // integral identity
    for ( int i = -10000; i <= 10000; ++i )
        assert( is_close( fx32( (float)i ), (float)i ) );

    // floating identity
    for ( float i = 0; i <= 10000; i += 0.01 )
    {
        assert( is_close( fx64( (double)i ), (double)i ) );
        assert( is_close( fx32( (float)i ), (float)i ) );
    }

    for ( float i = -10000; i <= 0; i += 0.01 )
    {
        assert( is_close( fx64( (double)i ), (double)i ) );
        assert( is_close( fx32( (float)i ), (float)i ) );
    }

    for ( float i = 0; i <= 10000; i += 0.001 )
        assert( is_close( fx64( (double)i ), (double)i ) );

    for ( float i = -10000; i <= 0; i += 0.001 )
        assert( is_close( fx64( (double)i ), (double)i ) );

    for ( int a = -100; a <= 100; ++a )
    {
        for ( int b = -100; b <= 100; ++b )
        {
            // std::printf( "a:%d b:%d\n", a, b );
            assert( equal( fx32( a ) + fx32( b ), a + b ) );
            assert( equal( fx32( a ) - fx32( b ), a - b ) );
            assert( equal( fx32( a ) * fx32( b ), a * b ) );
            if ( b != 0 ) assert( equal( fx32( a ) / fx32( b ), a / b ) );
        }
    }

    for ( fx32 a = -100; a <= 100; a += 1 )
    {
        for ( fx32 b = -100; b <= 100; b += 1 )
        {
            assert( equal( a + b, (int)a + (int)b ) );
            assert( equal( a - b, (int)a - (int)b ) );
            assert( equal( a * b, (int)a * (int)b ) );
            if ( b != 0 ) assert( equal( a / b, (int)a / (int)b ) );
        }
    }

    for ( double a = -100; a <= 100; a += 0.01 )
    {
        assert( is_close( fx64( a ), a ) );

        for ( double b = -100; b <= 100; b += 0.01 )
        {
            assert( is_close( fx64( a ) + fx64( b ), a + b ) );
            assert( is_close( fx64( a ) - fx64( b ), a - b ) );
            assert( is_close( fx64( a ) * fx64( b ), a * b ) );
            if ( std::fabs( b ) >= DoubleDelta )
                assert( is_close( fx64( a ) / fx64( b ), a / b ) );
        }
    }

    // harder division and reciprocal

    for ( double a = -100; a <= 100; a += 0.01 )
    {
        if ( std::fabs( a - 0 ) <= DoubleDelta ) continue;

        assert( is_close( fx64( a ).recip(), 1 / a ) );
        assert( is_close( fx64( a ).recip().recip(), a ) );

        assert( is_close( fx64( a ) / fx64( a ), 1.0 ) );

        auto recip = fx64( a ).recip();
        assert( is_close( fx64( 1 ) / fx64( a ), double( recip ) ) );
        assert( is_close( fx64( a ) * recip, 1.0 ) );
    }

    // // VERY LONG! (~5 mins)
    // for ( double a = -1000; a <= 1000; a += 0.01 )
    // {
    //     for ( double b = -1000; b <= 1000; b += 0.01 )
    //     {
    //         assert( is_close( fx64( a ) + fx64( b ), a + b ) );
    //         assert( is_close( fx64( a ) - fx64( b ), a - b ) );
    //         assert( is_close( fx64( a ) * fx64( b ), a * b ) );
    //         // if ( b >= DoubleDelta )
    //         //     assert( is_close( fx64( a ) / fx64( b ), a / b ) );
    //     }
    // }

    return 0;
}
