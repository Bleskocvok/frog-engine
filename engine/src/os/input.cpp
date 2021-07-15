#include"graphics/include.hpp"

#include "input.hpp"


using frog::os::input;


void input::on_resize(int width, int height)
{
    resized = { width, height };
}


void input::on_mouse_move(double x, double y)
{
    cursor = { x, y };
}


void input::on_mouse_press(int but
                          , int action
                          , [[maybe_unused]]int mods)
{
    switch (action)
    {
        case GLFW_PRESS:
            _on_press(mouse{ but });
        break;
        case GLFW_RELEASE:
            _on_release(mouse{ but });
        break;
        default:
        break;
    }
}


void input::on_key_press(int key
                        , int scancode
                        , int action
                        , [[maybe_unused]]int mods)
{
    switch (action)
    {
        case GLFW_PRESS:
            _on_press(frog::os::key{ key });
            _on_press(frog::os::scancode{ scancode });
        break;
        case GLFW_RELEASE:
            _on_release(frog::os::key{ key });
            _on_release(frog::os::scancode{ scancode });
        break;
        default:
        break;
    }
}

input::buttons& frog::os::input::get_buttons(key) { return keys; }

input::buttons& input::get_buttons(scancode) { return scancodes; }

input::buttons& input::get_buttons(mouse) { return mouse_buttons; }

const input::buttons& input::get_buttons(key) const { return keys; }

const input::buttons& input::get_buttons(scancode) const { return scancodes; }

const input::buttons& input::get_buttons(mouse) const { return mouse_buttons; }


void input::reset()
{
    resized = std::nullopt;
    keys.clear();
    scancodes.clear();
    mouse_buttons.clear();
}


std::optional<std::pair<int, int>> input::has_resized() const
{
    return resized;
}


std::pair<double, double> input::cursor_pos() const { return cursor; }


