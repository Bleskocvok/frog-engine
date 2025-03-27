#include"frog/graphics/include.hpp"

#include <iostream>
#include <exception>
#include <memory>


/*
 * GLFW RAII wrapper.
 */
class GLFW
{
public:
    GLFW()
    {
        if (not glfwInit())
        {
            throw std::runtime_error("could not initialize GLFW");
        }
    }

    GLFW(const GLFW&) = delete;
    GLFW& operator=(const GLFW&) = delete;

    ~GLFW()
    {
        glfwTerminate();
    }
};


struct Deleter
{
    void operator()(GLFWwindow* window){ glfwDestroyWindow(window); }
};
using Window = std::unique_ptr<GLFWwindow, Deleter>;

// standalone callback
// (why?)
void error_callback(int error, const char* description)
{
    std::cerr << "ERROR '" << error << "': " << description << std::endl;
}

/*
 * Callbacks class.
 */
template<typename T>
class Callbacks {
public:
    static void resize(GLFWwindow* window, int w, int h)
    {
        toT(glfwGetWindowUserPointer(window))->on_resize(window, w, h);
    }
    static void mouse_move(GLFWwindow* window, double x, double y)
    {
        toT(glfwGetWindowUserPointer(window))->on_mouse_move(window, x, y);
    }
    static void mouse_press(GLFWwindow* window, int but, int action, int mods)
    {
        toT(glfwGetWindowUserPointer(window))->on_mouse_pressed(window, but, action, mods);
    }
    static void key_press(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        toT(glfwGetWindowUserPointer(window))->on_key_pressed(window, key, scancode, action, mods);
    }
    static void scroll(GLFWwindow* window, double xoff, double yoff)
    {
        toT(glfwGetWindowUserPointer(window))->on_scroll(window, xoff, yoff);
    }

private:
    template<typename W>
    static T* toT(W* w)
    {
        return static_cast<T*>(w);
    }
};


/*
 * MAIN.
 */
int main()
{
    int width = 1280;
    int height = 720;
    std::string title = "Game";

    // RAII glfw init
    GLFW glfw;

    // set error callback as the first thing (just to be sure)
    glfwSetErrorCallback(error_callback);

    // set opengl parameters
    {
        // using version 4.5
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        // some sort of opengl profile (?)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // turn on forward compatibility for opengl
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

    // create window
    Window window(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));
    if (not window)
    {
        throw std::runtime_error("could not create a window");
    }
    glfwMakeContextCurrent(window.get());

    // load opengl extensions
    if (not gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("could not initialize opengl extensions");
    }

    /*
     * Game starts here.
     */
    bool quit = false;
    do {
        // start our application
        // Application app(width, height);

        // set appropriate callbacks
        // using clb = Callbacks<decltype(app)>;
        // glfwSetWindowUserPointer(window.get(), &app);
        // glfwSetWindowSizeCallback(window.get(), clb::resize);
        // glfwSetCursorPosCallback(window.get(), clb::mouse_move);
        // glfwSetMouseButtonCallback(window.get(), clb::mouse_press);
        // glfwSetKeyCallback(window.get(), clb::key_press);
        // glfwSetScrollCallback(window.get(), clb::scroll);

        // no chance to miss callbacks
        glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GLFW_TRUE);

        /*
         * Constant delta for stable physics.
         */
        double time = glfwGetTime();
        double delta = 0.016;
        double accum = 0.0;
        /*
         * Main loop.
         */
        while (not glfwWindowShouldClose(window.get()) /*&& not app.has_quit()*/)
        {
            // update
            while (accum >= delta)
            {
                // app.update(delta);
                accum -= delta;
            }

            // render
            // app.render(accum / delta);
            glfwSwapBuffers(window.get());

            // poll for events
            glfwPollEvents();

            auto current = glfwGetTime();
            accum += current - time;
            time = current;
        }
        // restart the game
    } while (not quit);
}
