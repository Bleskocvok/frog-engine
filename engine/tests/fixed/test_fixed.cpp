
#undef NDEBUG

#include "frog/geometry/fixed.hpp"

#include <cassert>      // assert
#include <cmath>        // fabs
#include <cstdio>       // printf
#include <iostream>     // cout
#include <random>       // mt19937_64
#include <string>       // string

template< typename A, typename B >
void assert_eq( const A& a, const B& b )
{
    if (a != b)
    {
        std::cerr << a << " != " << b << std::endl;
        std::abort();
    }
};

bool equal( int a, int b )
{
    bool res = a == b;
    if ( not res )
        std::printf( "a=%d\nb=%d\n", a, b );
    return res;
}

constexpr double DoubleDelta = 10e-3;
constexpr double FloatDelta  = 3 * 10e-3;


bool is_close( double a, double b, double D = DoubleDelta )
{
    bool res = std::fabs( a - b ) <= D;
    if ( not res )
        std::printf( "a=%f\nb=%f\n", a, b );
    return res;
}


bool is_close( float a, float b, float D = FloatDelta )
{
    bool res = std::fabs( a - b ) <= D;
    if ( not res )
        std::printf( "a=%f\nb=%f\n", a, b );
    return res;
}


void test_string_conversion();
void test_string_exact();
void test_exp();

int main()
{
    using namespace frog::geo;

    fx32 a = 0;
    a = 45;
    a = 45L;
    a = 45LU;
    a = float( 45.01f );
    a = double( 45.01 );

    test_string_exact();
    test_string_conversion();
    test_exp();

    assert( is_close( fx32( 5, 10 ), 0.5 ) );
    assert( is_close( fx64( 5, 10 ), 0.5 ) );
    assert( is_close( fx32( 9, 10 ), 0.9 ) );
    assert( is_close( fx64( 9, 10 ), 0.9 ) );
    assert( is_close( fx64( 9 ) / 10, 0.9 ) );

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

    // fraction constructor
    assert( is_close( fx64( 13, 27 ),  13 / 27.0 ) );

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
        // assert( is_close( fx64( a ), a ) );

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

template< class T >
void test_str_value( const char* str, const std::string& num )
{
    auto res = T( num ).to_str();
    std::cout << str << "( " << num << " ) → " << "\"" << res << "\"";

    auto normalized = []( auto str )
    {
        while ( str.size() > 1 && str[ 0 ] == '0' && str[ 1 ] != '.' )
            str.erase( str.begin() );

        while ( str.size() > 1 && str.back() == '0' && str[ str.size() - 2 ] != '.' )
            str.pop_back();

        return str;
    };

    auto prefix = []( const auto& str )
    {
        auto dot = str.find( '.' );

        if ( dot == str.npos )
            return str;

        return str.substr( 0, dot + CheckDigits + 1 );
    };

    if ( not exp.empty() )
        num = exp;

    auto n_num = normalized( num );
    if ( prefix( res ) != prefix( n_num ) )
    {
        std::cout << "\ndouble( " << str << "( " << num << " ) )"
                  << " → " << double( T( num ) ) << "\n";
        std::cout << "\nFAIL: ";
        // assert_eq( res, n_num );
        assert_eq( prefix( res ), prefix( n_num ) );
    }
    else
        std::cout << " OK\n";
}

template< typename Gen >
std::string rand_num_str( unsigned dec, unsigned frac, Gen& gen )
{
    std::string num;

    for ( unsigned i = 0; i < dec; i++ )
        num += '0' + gen() % 10;

    if ( frac >= 0 )
        num += '.';

    for ( unsigned i = 0; i < frac; i++ )
        num += '0' + gen() % 10;

    return num;
}

void test_string_exact()
{
    using namespace frog::geo;
    auto exact = []( const auto& str )
    {
        assert_eq( fx32( str ).to_str(), str );
        assert_eq( fx64( str ).to_str(), str );
    };
    exact( "123456.5" );
    exact( "123456.25" );
    exact( "123456.125" );
    exact( "123456.0625" );
    exact( "123456.03125" );
    exact( "123456.015625" );
    exact( "123456.0078125" );
    exact( "123456.00390625" );
    exact( "123456.001953125" );
    exact( "-456123.5" );
    exact( "-456123.25" );
    exact( "-456123.125" );
    exact( "-456123.0625" );
    exact( "-456123.03125" );
    exact( "-456123.015625" );
    exact( "-456123.0078125" );
    exact( "-456123.00390625" );
    exact( "-456123.001953125" );

    assert_eq( fx64( 1, 256 ).to_str(), "0.00390625" );

    {
        std::mt19937_64 gen32( 1337 );
        std::mt19937_64 gen64( 1337 );
        for ( int i = 0; i < 10000; i++ )
        {
            auto n64 = gen64() % std::uint64_t( fx64::max() );
            auto n32 = gen32() % std::uint32_t( fx32::max() );

            auto str64 = std::to_string( n64 );
            auto str32 = std::to_string( n32 );

            test_str_value< fx64, 1000 >( "fx64", str64, str64 + ".0" );
            test_str_value< fx64, 1000 >( "fx64", str64 + ".0" );

            test_str_value< fx32, 1000 >( "fx32", str32, str32 + ".0" );
            test_str_value< fx32, 1000 >( "fx32", str32 + ".0" );
        }
    }

    std::cout << "test_string_exact OK" << std::endl;
}

void test_string_conversion()
{
    using namespace frog::geo;

    {
        fx32 a = fx32( 1, 2 ) + fx32( 1, 4 ) + fx32( 1, 8 ) + fx32( 1, 16 );
        fx64 b = fx64( 1, 2 ) + fx64( 1, 4 ) + fx64( 1, 8 ) + fx64( 1, 16 );
        assert_eq( fx32( 123456 ).to_str(), "123456.0" );
        assert_eq( fx64( 123456 ).to_str(), "123456.0" );

        assert_eq( a.to_str(), "0.9375" );
        assert_eq( b.to_str(), "0.9375" );

        test_str_value< fx64 >( "fx64", "0.9375" );
        test_str_value< fx32 >( "fx32", "0.9375" );

        for (const char* s : { "0.9375", "123.5", "123.0", /* "31293182312.75" idiot, bigger than fx32.max() */ })
        {
            test_str_value< fx64 >( "fx64", s );
            test_str_value< fx32 >( "fx32", s );
        }
    }

    // {
    //     std::mt19937_64 gen( 1337 );
    //     for ( int i = 0; i < 10000; i++ )
    //     {
    //         auto num = rand_num_str( 10, 10, gen );
    //         test_str_value< fx64 >( "fx64", num );
    //     }
    // }

    // {
    //     std::mt19937_64 gen( 1338 );
    //     for ( int i = 0; i < 10000; i++ )
    //     {
    //         auto num = rand_num_str( 6, 3, gen );
    //         test_str_value< fx32 >( "fx32", num );
    //     }
    // }
}

void test_exp()
{
    using namespace frog::geo;

    std::uint64_t n = 2;
    for ( unsigned i = 1; i < 18; i++ )
    {
        auto f1 = fx32{ 2 }.exp( i );
        auto f2 = fx64{ 2 }.exp( i );
        log() << "2 ^ " << i << " = " << n << "\n"
              << "  fx32 = " << f1 << "\n"
              << "  fx64 = " << f2 << "\n";
        assert_eq( f1, n );
        assert_eq( f2, n );

        n *= 2;
    }

    assert_eq( fx32( 2 ).exp( -1 ), fx32( 1, 2 ) );
    assert_eq( fx64( 2 ).exp( -1 ), fx64( 1, 2 ) );
}
