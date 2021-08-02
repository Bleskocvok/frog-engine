#pragma once

#include"graphics/include.hpp"

#include <stdexcept>


namespace frog::gl
{


class renderer_3D
{
    void gl_settings()
    {
        glEnable(GL_CULL_FACE);
    }

public:
    renderer_3D(int major, int minor, bool vsync = true)
    {
        // the below code should be executed BEFORE window and
        // gl context creation

        // using OpenGL version 'major.minor'
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

        // use core profile, without deprecated functions
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);

        // turn on forward compatibility for opengl
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                            SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);


        // now create gl context and window

        // TODO

        // create gl context first, then setup glad with proc address

        // setup glad
        // it needs a pointer to a function that returns the address for
        // a given opengl extension function
        if (!gladLoadGLLoader(
                reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
        {
            throw std::runtime_error(
                    "could not initialize OpenGL extensions");
        }

        SDL_GL_SetSwapInterval(vsync ? 1 : 0);

        gl_settings();

    }

    void viewport(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void clear_color(float r, float g, float b)
    {
        glClearColor(r, g, b, 1.0);
    }

    void clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void prepare2D()
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void prepare3D()
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
};


} // namespace frog::gl
