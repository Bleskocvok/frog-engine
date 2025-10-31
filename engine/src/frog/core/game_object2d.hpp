#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "game_object_base.hpp"
#include "script.hpp"
#include "frog/gx2d/sprite.hpp"


namespace frog
{


class engine2d;
struct script2d;


class game_object2d : public game_object_base<game_object2d, script2d, engine2d>
{
public:
    using Engine = engine2d;

private:
    gx2d::sprite img;

    void calculate_prev()
    {
        gx2d::calculate_prev(img);

        for (auto& ui : elements())
            gx2d::calculate_prev(ui->sprite);
    }

public:
    game_object2d() = default;

    const gx2d::sprite& model() const { return img; }
          gx2d::sprite& model()       { return img; }

    virtual void init(Engine& engine) override
    {
        bool prev = not initialized;
        game_object_base<game_object2d, script2d, engine2d>::init(engine);

        if (prev)
            calculate_prev();
    }

    virtual void pre_update(Engine& engine) override
    {
        calculate_prev();

        game_object_base<game_object2d, script2d, engine2d>::pre_update(engine);
    }
};


} // namespace frog


#endif
