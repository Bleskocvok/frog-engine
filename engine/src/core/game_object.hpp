#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "game_object_base.hpp"
#include "gl/model.hpp"


namespace frog
{


class engine;
struct script;


class game_object : public game_object_base<game_object, script, engine>
{
public:
    using Engine = engine;

private:
    gl::model _model;

public:
    game_object() = default;

    const gl::model& model() const { return _model; }
          gl::model& model()       { return _model; }
};


} // namespace frog


#endif
