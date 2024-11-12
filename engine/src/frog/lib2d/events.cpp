#ifndef NOT_FROG_BUILD_2D

#include "events.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER

#include <stdexcept>        // runtime_error
#include <vector>           // vector


namespace frog::lib2d::gx
{


void events::k_reset()
{
    for ( auto& k : m_keys )
    {
        k.pressed = false;
        k.released = false;
    }
}


void events::f_reset()
{
    auto to_delete = std::vector< decltype( m_fingers )::key_type >{};
    to_delete.reserve( m_fingers.size() );

    for ( auto& [ key, f ] : m_fingers )
    {
        f.pressed = false;
        if ( f.released )
            to_delete.push_back( key );
    }
    for ( const auto& key : to_delete )
        m_fingers.erase( key );
}


void events::reset_key( key_state& k )
{
    k.pressed = false;
    k.released = false;
}


void events::reset()
{
    k_reset();
    f_reset();
    resized.reset();
    reset_key( m_mouse.but_l );
    reset_key( m_mouse.but_r );
    reset_key( m_mouse.but_m );
}


events::key_state& events::mouse_button( Uint8 but )
{
    switch ( but )
    {
        case SDL_BUTTON_LEFT:   return m_mouse.but_l;
        case SDL_BUTTON_MIDDLE: return m_mouse.but_m;
        case SDL_BUTTON_RIGHT:  return m_mouse.but_r;
        default: throw std::runtime_error("invalid mouse button");
    }
}


void events::update()
{
    reset();

    SDL_Event event;
    while ( SDL_PollEvent( &event ) )
    {
        switch ( event.type )
        {
            case SDL_QUIT: has_quit = true; break;

            case SDL_KEYDOWN:
                m_keys[ event.key.keysym.scancode ].pressed = true;
                m_keys[ event.key.keysym.scancode ].down = true;
                break;
            case SDL_KEYUP:
                m_keys[ event.key.keysym.scancode ].released = true;
                m_keys[ event.key.keysym.scancode ].down = false;
                break;

            case SDL_FINGERDOWN:
                m_fingers.emplace( event.tfinger.fingerId,
                                   finger{ true, false,
                                           event.tfinger.x,
                                           event.tfinger.y,
                                           event.tfinger.dx,
                                           event.tfinger.dx,
                                           event.tfinger.pressure } );
                break;

            case SDL_FINGERUP:
                m_fingers[ event.tfinger.fingerId ].released = true;
                break;

            case SDL_FINGERMOTION:
                m_fingers[ event.tfinger.fingerId ].x = event.tfinger.x;
                m_fingers[ event.tfinger.fingerId ].y = event.tfinger.y;
                m_fingers[ event.tfinger.fingerId ].dx = event.tfinger.dx;
                m_fingers[ event.tfinger.fingerId ].dy = event.tfinger.dy;
                m_fingers[ event.tfinger.fingerId ].pressure = event.tfinger.pressure;
                break;

            case SDL_WINDOWEVENT:
                if ( event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
                    resized = { event.window.data1, event.window.data2 };
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouse_button( event.button.button ).down = true;
                mouse_button( event.button.button ).pressed = true;
                break;

            case SDL_MOUSEBUTTONUP:
                mouse_button( event.button.button ).down = false;
                mouse_button( event.button.button ).released = true;
                break;

            case SDL_MOUSEMOTION:
                m_mouse.x = event.motion.x;
                m_mouse.y = event.motion.y;
                m_mouse.dx = event.motion.xrel;
                m_mouse.dy = event.motion.yrel;
                break;

            default: break;
        }
    }

    keyboard_state = SDL_GetKeyboardState( &keyboard_state_count );
}


const events::key_state& events::k_at( SDL_Scancode k ) const
{
    return m_keys.at( k );
}


bool events::kb_down( SDL_Scancode k ) const
{
    if ( k >= keyboard_state_count )
        throw std::runtime_error( "events::kb_down: k outside range" );

    return keyboard_state[ k ];
}


} // namespace frog::lib2d::gx

#endif
