#ifndef NOT_FROG_BUILD_3D

#include "frog/gx3d/include.hpp"

#pragma once

#include "input.hpp"
#include "window_base.hpp"

#include <string>
#include <memory>       // unique_ptr
#include <stdexcept>    // runtime_error

#include "frog/debug.hpp"
#include "frog/utils/todo.hpp"

namespace frog::os
{


namespace glfw
{


std::string get_error();

void throw_error();


struct Deleter
{
    void operator()(GLFWwindow* win) { glfwDestroyWindow(win); }
};

using window = std::unique_ptr<GLFWwindow, Deleter>;


} // namespace glfw


struct GLFW
{
    GLFW()
    {
        if (not glfwInit())
        {
            glfw::throw_error();
        }
    }

    GLFW(const GLFW&) = delete;
    GLFW& operator=(const GLFW&) = delete;

    ~GLFW()
    {
        glfwTerminate();
    }

    void poll_events()
    {
        glfwPollEvents();
    }
};


class window : public window_base
{
    glfw::window _window;

public:
    window(int w, int h, const std::string& title)
        : _window(glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr))
    {
        if (not _window)
        {
            glfw::throw_error();
        }

        // important; needs to be set before graphics extensions are loaded
        make_current_context();

        LOG("window");
    }

    bool should_close() const override
    {
        return glfwWindowShouldClose(_window.get());
    }

    void make_current_context()
    {
        glfwMakeContextCurrent(_window.get());
    }

    void swap_buffers() override
    {
        glfwSwapBuffers(_window.get());
    }

    void set_input_mode(int mode, int value)
    {
        glfwSetInputMode(_window.get(), mode, value);
    }

    void set_callbacks(input* in);

    int width() const override { TODO(); }
    int height() const override { TODO(); }
};


} // namespace frog::os


#endif
