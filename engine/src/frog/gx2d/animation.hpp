#pragma once

#include "frog/geometry/rectangle.hpp"
#include "frog/utils/exception.hpp"
#include "sprite.hpp"

#include <string>           // string
#include <utility>          // move
#include <unordered_map>    // unordered_map


namespace frog::gx2d {


struct AnimationFrame
{
    int index = 0;
    int length = 1;
    std::string next;
    bool flipped = false;

    AnimationFrame(int index, int length, std::string next, bool flipped = false)
        : index(index)
        , length(length)
        , next(std::move(next))
        , flipped(flipped) {}
};


class Animation
{
    Sprite atlas;
    geo::ivec2 atlas_size;
    std::unordered_map<std::string, AnimationFrame> map;
    std::string current_ = "";
    std::string next = "";
    int frame_ = 0;

    float delay_ = 0.16667;
    float accum = 0;

public:
    Animation(Sprite atlas, geo::ivec2 atlas_size, float delay_ = 0.16667, std::string start = "")
        : atlas(std::move(atlas))
        , atlas_size(atlas_size)
        , current_(std::move(start))
        , delay_(delay_)
    { }

    Animation(geo::ivec2 atlas_size, float delay_ = 0.16667, std::string start = "")
        : Animation({}, atlas_size, delay_, start)
    { }

    const Sprite& get_atlas() const { return atlas; }

    float  delay() const { return delay_; }
    float& delay()       { return delay_; }

    void add_frame(std::string name, AnimationFrame frame)
    {
        map.emplace(name, std::move(frame));
        if (current_.empty())
            set(std::move(name));
    }

    void set(std::string name)
    {
        current_ = std::move(name);
        next = current().next;
        frame_ = 0;
    }

    void buffer(std::string name)
    {
        next = std::move(name);
    }

    void reset_accum() { accum = 0; }

    void update(float delta)
    {
        accum += delta;
        while (accum >= delay_)
        {
            accum -= delay_;
            ++frame_;
            if (frame_ >= length_of(current()))
                set(next);
        }
    }

    const AnimationFrame& at(const std::string& f) const
    {
        try
        {
            return map.at(f);
        }
        catch (...)
        {
            // If it were:
            //     catch (std::exception& ex)
            // then:
            //     throw frog::error(ex, "animation invalid frame '", f, "'");
            throw frog::error("animation invalid frame '", f, "'");
        }
    }

    const AnimationFrame& current() const { return at(current_); }

    float frame_duration(const std::string& f) const
    {
        return length_of(at(f)) * delay_;
    }

    int length_of(const AnimationFrame& f) const
    {
        return f.length == -1 ? atlas_size.x() : f.length;
    }

    int frame_index() const { return frame_; }

    const std::string& current_name() const { return current_; }
    const std::string& next_name() const { return next; }

    frog::geo::rect texture_rect() const
    {
        geo::vec2 units = { 1.0f / atlas_size.x(),
                            1.0f / atlas_size.y() };
        geo::ivec2 pos = { frame_, current().index };

        frog::geo::rect res;
        res.pos = { pos.x() * units.x(),
                    pos.y() * units.y() };
        res.size = units;

        return res;
    }

    Sprite frame() const
    {
        Sprite img = atlas;
        img.tex = texture_rect();
        img.flipped = current().flipped;
        return img;
    }
};


}  // namespace frog::gx2d
