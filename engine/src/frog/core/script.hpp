#pragma once

namespace frog {


class game_object;
class game_object2d;
class engine;
class engine2d;

struct script;
struct script2d;

template<typename GameObject, typename Engine>
struct script_base;

template<typename _GameObject, typename _Engine>
struct script_base
{
    using GameObject = _GameObject;
    using Engine = _Engine;

    virtual ~script_base() = default;

    virtual void init(GameObject&, Engine&) {}

    virtual void stable_update(GameObject&, Engine&) {}

    virtual void end_update(GameObject&, Engine&) {}

    virtual void frame_update(GameObject&, Engine&) {}

    virtual void destroyed(GameObject&, Engine&) {}
};


struct script : script_base<game_object, engine> {};
struct script2d : script_base<game_object2d, engine2d> {};


} // namespace frog
