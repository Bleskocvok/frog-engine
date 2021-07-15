#include "renderer.hpp"

#include"graphics/include.hpp"


void frog::gl::renderer::gl_settings()
{
    glEnable(GL_CULL_FACE);
}


frog::gl::renderer::renderer(int major, int minor, bool vsync)
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


void frog::gl::renderer::viewport(int width, int height)
{
    glViewport(0, 0, width, height);
}


void frog::gl::renderer::clear_color(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0);
}


void frog::gl::renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void frog::gl::renderer::prepare2D()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void frog::gl::renderer::prepare3D()
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
