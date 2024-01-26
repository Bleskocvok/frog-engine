#ifndef NOT_FROG_BUILD_2D

#include "core.hpp"

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


void events::update()
{
    k_reset();
    f_reset();

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
