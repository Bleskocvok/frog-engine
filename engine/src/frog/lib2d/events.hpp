#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "core.hpp"
#include "structs.hpp"

#include <map>          // map
#include <array>        // array
#include <optional>     // optional
#include <utility>      // pair
#include <cstdint>      // uint8_t

#include "sdl_include.hpp"
#include SDL_HEADER


namespace frog::lib2d::gx {

class Events
{
public:
    static constexpr int SCANCODES_MAX = SDL_NUM_SCANCODES;
    using KeyPtr = Uint8;

    struct KeyState { bool pressed=false, released=false, down=false; };
    struct Finger { bool pressed=0, released=0;
                    float x=0, y=0, dx=0, dy=0, pressure=0; };

    struct Mouse
    {
        std::int32_t x=0, y=0, dx=0, dy=0;
        KeyState but_l;
        KeyState but_r;
        KeyState but_m;
        int wheel = 0;
    };

    struct AccelerometerState
    {
        float x = 0,
              y = 0,
              z = 0;
    };

    enum Orientation : std::uint32_t {
        Portrait            = 0x1,
        PortraitUpsideDown  = 0x2,
        LandscapeLeft       = 0x4,
        LandscapeRight      = 0x8,
        Unknown             = 0x10
    };

    struct Display
    {
        using enum Orientation;

        Orientation orientation = Unknown;

        bool changed = false;
    };

    void reset();

private:
    bool has_quit = false;

    std::array< KeyState, SCANCODES_MAX > m_keys;

    const KeyPtr* keyboard_state = nullptr;
    int keyboard_state_count = 0;

    Mouse m_mouse;

    std::map< SDL_FingerID, Finger > m_fingers;

    std::optional< std::pair< int, int > > resized;

    KeyState& mouse_button( Uint8 but );
    void reset_key( KeyState& k );

    std::map<std::uint8_t, KeyState> other_mouse_buttons;

    detail::Sensor m_accelerometer;

    Display m_display;

    void k_reset();
    void f_reset();
    void m_reset();

public:
    Events();

    Events( const Events& ) = delete;
    Events& operator=( const Events& ) = delete;

    void update();

    bool quit() const { return has_quit; }

    // keys
    const decltype( m_keys )& keys() const { return m_keys; }
    const KeyState& k_at( SDL_Scancode k ) const;

    // keyboard_state
    bool kb_down( SDL_Scancode k ) const;

    // mouse
    const Mouse& mouse() const { return m_mouse; }

    // touch
    const decltype( m_fingers )& fingers() const { return m_fingers; }

    decltype( resized ) has_resized() const
    {
        return resized;
    }

    std::optional<AccelerometerState> accelerometer();

    Display display() const;
};

} // frog::lib2d::gx


#endif
