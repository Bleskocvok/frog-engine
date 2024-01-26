#ifndef NOT_FROG_BUILD_3D


#include "gl/include.hpp"

#include "window.hpp"

#include <string>


std::string frog::os::glfw::get_error()
{
    const char* desc = nullptr;
    int code = glfwGetError(&desc);
    return desc
        ? std::to_string(code) + ": " + std::string(desc)
        : std::string{};
}


void frog::os::glfw::throw_error()
{
    auto err = get_error();
    if (!err.empty())
    {
        throw std::runtime_error(err);
    }
}


struct callbacks
{
    using type = frog::os::input;

    static type* ptr(GLFWwindow* win)
    {
        return static_cast<type*>(glfwGetWindowUserPointer(win));
    }

    static void resize(GLFWwindow* win, int w, int h)
    {
        ptr(win)->on_resize(w, h);
    }

    static void mouse_move(GLFWwindow* win, double x, double y)
    {
        ptr(win)->on_mouse_move(x, y);
    }

    static void mouse_press(GLFWwindow* win, int but, int action, int mods)
    {
        ptr(win)->on_mouse_press(but, action, mods);
    }

    static void key_press(
            GLFWwindow* win, int key, int scancode, int action, int mods)
    {
        ptr(win)->on_key_press(key, scancode, action, mods);
    }
};


void frog::os::window::set_callbacks(input* in)
{
    auto win = _window.get();
    glfwSetWindowUserPointer(   win, in                     );
    glfwSetWindowSizeCallback(  win, callbacks::resize      );
    glfwSetCursorPosCallback(   win, callbacks::mouse_move  );
    glfwSetMouseButtonCallback( win, callbacks::mouse_press );
    glfwSetKeyCallback(         win, callbacks::key_press   );
    // glfwSetScrollCallback(_window.get(), callbacks::scroll);
}


#endif
