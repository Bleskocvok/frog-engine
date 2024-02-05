#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "core.hpp"
#include "structs.hpp"

#include <map>          // map
#include <array>        // array
#include <optional>     // optional
#include <utility>      // pair

#include "sdl_include.hpp"
#include SDL_HEADER


namespace frog::lib2d::gx
{
    class events
    {
    public:
        static constexpr int SCANCODES_MAX = SDL_NUM_SCANCODES;
        using KeyPtr = Uint8;

        struct key_state { bool pressed, released, down; };
        struct finger { bool pressed=0, released=0;
                        float x=0, y=0, dx=0, dy=0, pressure=0; };
        struct mouse { std::int32_t x, y, xrel, yrel;
                       std::uint32_t state; };

        void k_reset();
        void f_reset();
        void reset();

    private:
        bool has_quit = false;

        std::array< key_state, SCANCODES_MAX > m_keys;

        const KeyPtr* keyboard_state = nullptr;
        int keyboard_state_count = 0;

        std::map< SDL_FingerID, finger > m_fingers;

        std::optional< std::pair< int, int > > resized;

    public:
        events() = default;

        events( const events& ) = delete;
        events& operator=( const events& ) = delete;

        void update();

        bool quit() const { return has_quit; }

        // keys
        const decltype( m_keys )& keys() const { return m_keys; }
        const key_state& k_at( SDL_Scancode k ) const;

        // keyboard_state
        bool kb_down( SDL_Scancode k ) const;

        // touch
        const decltype( m_fingers )& fingers() const { return m_fingers; }

        decltype( resized ) has_resized() const
        {
            return resized;
        }
    };
}


#endif
