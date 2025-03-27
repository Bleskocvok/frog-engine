#ifndef NOT_FROG_BUILD_3D

#pragma once

#include <utility>  // pair
#include <optional>
#include <unordered_set>


namespace frog::os
{


struct key { int val = 0; };

struct scancode { int val = 0; };

struct mouse { int val = 0; };


class input
{
    struct buttons
    {
        std::unordered_set<int> pressed;
        std::unordered_set<int> down;
        std::unordered_set<int> released;

        void clear()
        {
            pressed.clear();
            released.clear();
        }
    }
        keys,
        scancodes,
        mouse_buttons;

    std::optional<std::pair<int, int>> resized;
    std::pair<double, double> cursor;

    buttons& get_buttons(key);
    buttons& get_buttons(scancode);
    buttons& get_buttons(mouse);
    const buttons& get_buttons(key) const;
    const buttons& get_buttons(scancode) const;
    const buttons& get_buttons(mouse) const;

    template<typename T>
    void _on_press(T but)
    {
        get_buttons(but).pressed.insert(but.val);
        get_buttons(but).down.insert(but.val);
    }

    template<typename T>
    void _on_release(T but)
    {
        get_buttons(but).released.insert(but.val);
        get_buttons(but).down.erase(but.val);
    }

public:
    void on_resize(int width, int height);

    void on_mouse_move(double x, double y);
    void on_mouse_press(int but, int action, int mods);
    void on_key_press(int key, int scancode, int action, int mods);

    void reset();

    std::optional<std::pair<int, int>> has_resized() const;

    template<typename T>
    bool is_down(T but) const
    {
        return get_buttons(but).down.count(but.val);
    }

    template<typename T>
    bool is_up(T but) const { return not is_down(but); }

    template<typename T>
    bool is_pressed(T but) const
    {
        return get_buttons(but).pressed.count(but.val);
    }

    template<typename T>
    bool is_released(T but) const
    {
        return get_buttons(but).released.count(but.val);
    }

    std::pair<double, double> cursor_pos() const;
};


} // namespace frog


#endif
