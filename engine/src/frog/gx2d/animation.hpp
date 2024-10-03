#pragma once

#include "frog/geometry/rectangle.hpp"
#include "frog/utils/exception.hpp"
#include "sprite.hpp"

#include <string>           // string
#include <utility>          // move
#include <unordered_map>    // unordered_map


namespace frog::gx2d {


struct animation_frame
{
    int index = 0;
    bool flipped = false;
    int length = 1;
    std::string next;

    animation_frame(int index, int length, std::string next, bool flipped = false)
        : index(index)
        , length(length)
        , next(std::move(next)) {}
};


class animation
{
    sprite atlas;
    geo::ivec2 atlas_size;
    std::unordered_map<std::string, animation_frame> map;
    std::string _current = "";
    std::string next = "";
    int _frame = 0;

    float delay = 0.16667;
    float accum = 0;

public:
    animation(sprite atlas, geo::ivec2 atlas_size, float delay = 0.16667, std::string start = "")
        : atlas(std::move(atlas))
        , atlas_size(atlas_size)
        , delay(delay)
        , _current(std::move(start))
    {}

    void add_frame(std::string name, animation_frame frame)
    {
        map.emplace(std::move(name), std::move(frame));
    }

    void set(std::string name)
    {
        _current = std::move(name);
        next = current().next;
        _frame = 0;
    }

    void buffer(std::string name)
    {
        next = std::move(name);
    }

    void update(float delta)
    {
        accum += delta;
        while (accum >= delay)
        {
            accum -= delay;
            ++_frame;
            if (_frame >= ( current().length == -1 ? atlas_size.x() : current().length ))
                set(next);
        }
    }

    const animation_frame& current() const
    {
        try
        {
            return map.at(_current);
        }
        catch (...)
        {
            // If it were:
            //     catch (std::exception& ex)
            // then:
            //     throw frog::error(ex, "animation invalid frame '", _current, "'");
            throw frog::error("animation invalid frame '", _current, "'");
        }
    }

    const std::string& current_name() const { return _current; }

    sprite frame() const
    {
        sprite img = atlas;

        geo::vec2 units = { 1.0f / atlas_size.x(), 1.0f / atlas_size.y() };
        geo::ivec2 pos = { _frame, current().index };

        img.tex.pos = { units.x() / 2 + pos.x() * units.x(),
                        units.y() / 2 + pos.y() * units.y() };
        img.tex.size = units;
        img.flipped = current().flipped;
        return img;
    }
};


}  // namespace frog::gx2d
