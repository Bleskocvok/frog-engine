#ifndef NOT_FROG_BUILD_3D

#include "renderer3d.hpp"

#include "gl/include.hpp"


void frog::gl::renderer3d::gl_settings()
{
    glEnable(GL_CULL_FACE);
}


frog::gl::renderer3d::renderer3d(int major, int minor, bool vsync)
{
    // using OpenGL version 'major.minor'
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

    // some sort of opengl profile (?)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // turn on forward compatibility for opengl
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("could not initialize OpenGL extensions");
    }

    glfwSwapInterval(vsync ? 1 : 0);

    gl_settings();
}


void frog::gl::renderer3d::viewport(int width, int height)
{
    glViewport(0, 0, width, height);
}


void frog::gl::renderer3d::clear_color(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0);
}


void frog::gl::renderer3d::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void frog::gl::renderer3d::prepare2D()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void frog::gl::renderer3d::prepare3D()
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

#endif
