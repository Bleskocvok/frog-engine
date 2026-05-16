#ifndef NOT_FROG_BUILD_2D

#include "events.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER

#include <stdexcept>        // runtime_error
#include <vector>           // vector
#include <utility>          // move
#include <optional>


namespace frog::lib2d::gx {


Events::Events( bool debug )
    : debug( debug )
{
    for (int i = 0; i < SDL_NumSensors(); i++)
    {
        auto sensor = detail::Sensor( SDL_SensorOpen( i ) );

        if ( not sensor )
        {
            // TODO: Log?
            continue;
        }

        if ( SDL_SensorGetType( sensor.get() ) == SDL_SENSOR_ACCEL )
        {
            if ( m_accelerometer )
                continue;
            m_accelerometer = std::move( sensor );
        }
    }
}


std::optional<Events::AccelerometerState> Events::accelerometer()
{
    if ( not m_accelerometer )
        return std::nullopt;

    float data[ 3 ] = { 0 };
    int rv = SDL_SensorGetData( m_accelerometer.get(), data, 3 );
    if ( rv == -1 )
    {
        // TODO: Log?
        return std::nullopt;
    }

    return AccelerometerState{ .x = data[ 0 ], .y = data[ 1 ], .z = data[ 2 ] };
}


Events::Display Events::display() const
{
    return m_display;
}


void Events::k_reset()
{
    for ( auto& k : m_keys )
    {
        k.pressed = false;
        k.released = false;
    }
}


void Events::f_reset()
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

    for (auto &[ key, f ] : m_fingers )
    {
        f.dx = 0;
        f.dy = 0;
    }
}


void Events::reset_key( KeyState& k )
{
    k.pressed = false;
    k.released = false;
}


void Events::m_reset( Mouse& m )
{
    reset_key( m.but_l );
    reset_key( m.but_r );
    reset_key( m.but_m );
    m.dx = 0;
    m.dy = 0;
    m.wheel = 0;
}


void Events::reset()
{
    k_reset();
    f_reset();
    resized.reset();
    m_reset( m_mouse );

    m_reset( m_mouse_debug );

    m_app = App{};
}


Events::KeyState& Events::mouse_button( Uint8 but )
{
    switch ( but )
    {
        case SDL_BUTTON_LEFT:   return m_mouse.but_l;
        case SDL_BUTTON_MIDDLE: return m_mouse.but_m;
        case SDL_BUTTON_RIGHT:  return m_mouse.but_r;
        default: return other_mouse_buttons[but];
        // default: throw std::runtime_error("invalid mouse button");
    }
}


void Events::update()
{
    // reset();

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
                                   Finger{ true, false,
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
                m_fingers[ event.tfinger.fingerId ].dx += event.tfinger.dx;
                m_fingers[ event.tfinger.fingerId ].dy += event.tfinger.dy;
                m_fingers[ event.tfinger.fingerId ].pressure = event.tfinger.pressure;
                break;

            case SDL_WINDOWEVENT:
                if ( event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
                    resized = { event.window.data1, event.window.data2 };
                if ( event.window.event == SDL_WINDOWEVENT_FOCUS_LOST )
                {
                    // TODO
                }

                if ( event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED )
                {
                    // TODO
                }
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
                m_mouse.dx += event.motion.xrel;
                m_mouse.dy += event.motion.yrel;
                break;

            case SDL_MOUSEWHEEL:
                m_mouse.wheel = event.wheel.y;
                break;

            case SDL_APP_TERMINATING:
                m_app.terminating = true;
                break;

            case SDL_APP_LOWMEMORY:
                m_app.low_memory = true;
                break;

            case SDL_APP_WILLENTERBACKGROUND:
                m_app.will_enter_bg = true;
                break;

            case SDL_APP_DIDENTERBACKGROUND:
                m_app.entered_bg = true;
                break;

            case SDL_APP_WILLENTERFOREGROUND:
                m_app.will_enter_fg = true;
                break;

            case SDL_APP_DIDENTERFOREGROUND:
                m_app.entered_fg = true;
                break;

            default: break;
        }
    }

    keyboard_state = SDL_GetKeyboardState( &keyboard_state_count );

    auto get_enum = [](auto val)
    {
        switch (val)
        {
            // TODO: This might be wrong.
            case SDL_ORIENTATION_UNKNOWN: return Display::Unknown;
            case SDL_ORIENTATION_LANDSCAPE: return Display::LandscapeRight;
            case SDL_ORIENTATION_LANDSCAPE_FLIPPED: return Display::LandscapeLeft;
            case SDL_ORIENTATION_PORTRAIT: return Display::Portrait;
            case SDL_ORIENTATION_PORTRAIT_FLIPPED: return Display::PortraitUpsideDown;
        }
    };
    m_display.orientation = get_enum(SDL_GetDisplayOrientation(0));

    if ( debug )
        apply_debug();
}

void Events::apply_debug()
{
    if ( m_mouse.but_l.released && m_mouse_debug.but_l.down )
        m_mouse_debug.but_l.released = true;

    if ( not k_at( SDL_SCANCODE_LCTRL ).down )
        return;

    auto link = [](auto& src, auto& dst)
    {
        if ( src )
            dst = true;
        src = false;
    };

    link( m_mouse.but_l.pressed,    m_mouse_debug.but_l.pressed );
    link( m_mouse.but_l.released,   m_mouse_debug.but_l.released );
    link( m_mouse.but_l.down,       m_mouse_debug.but_l.down );
}

const Events::KeyState& Events::k_at( SDL_Scancode k ) const
{
    return m_keys.at( k );
}


bool Events::kb_down( SDL_Scancode k ) const
{
    if ( k >= keyboard_state_count )
        throw std::runtime_error( "Events::kb_down: k outside range" );

    return keyboard_state[ k ];
}


} // namespace frog::lib2d::gx

#endif
