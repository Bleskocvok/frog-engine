#pragma once

#ifndef NOT_FROG_BUILD_2D

#include <cstdint>      // uint64_t, int32_t, uint32_t
#include <string>       // string


namespace frog::lib2d
{


class init_core
{
public:
    init_core( std::uint32_t flags );
    ~init_core();

    init_core( const init_core& ) = delete;
    init_core& operator=( const init_core& ) = delete;
    init_core( init_core&& ) = default;
    init_core& operator=( init_core&& ) = default;
};


class init_image
{
public:
    init_image( int flags );
    ~init_image();

    init_image( const init_image& ) = delete;
    init_image& operator=( const init_image& ) = delete;
    init_image( init_image&& ) = default;
    init_image& operator=( init_image&& ) = default;
};


class init_ttf
{
public:
    init_ttf();
    ~init_ttf();

    init_ttf( const init_ttf& ) = delete;
    init_ttf& operator=( const init_ttf& ) = delete;
    init_ttf( init_ttf&& ) = default;
    init_ttf& operator=( init_ttf&& ) = default;
};


class initializer
{
    init_core core;
    init_ttf ttf;
public:
    enum : std::uint32_t { None = 0, OnlyEvents = 0x1, Video = 0x7,
                           TTF = 0xf, Audio = 0x10, Net = 0x70 };

    // TODO: Move ttf to optional<ttf>
    // TODO: Finish audio and net
    initializer(std::uint32_t flags = None);

    static std::uint32_t flags_to_lib(std::uint32_t flags);
};


namespace fs
{
    enum class mode : bool { Text = false, Binary = true, };

    [[nodiscard]] std::string file_read( const char* filename, mode m );

    void file_write( const char* filename, const std::string& data, mode m );

}  // namespace fs


namespace os
{
    void wait( long ms );

    class timer
    {
        std::uint64_t point = 0;
    public:
        void reset();
        std::uint64_t reset_ms();
        std::uint64_t get_ms() const;

        unsigned duration_us() const;
        unsigned reset_duration_us();
    };

    void error_box( const std::string& title, const std::string& msg );

}  // namespace os


} // namespace frog::lib2d


#endif
