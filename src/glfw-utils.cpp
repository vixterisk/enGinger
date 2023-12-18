#include "glfw-utils.hpp"
#include <iostream>
#include "path.hpp"
#include <glad/glad.h>
#include <vector>
#include "geometry/vertex-utils.hpp"

void exitWhenNull(bool isNull, const std::string& errorMessage)
{
    if (isNull)
    {
        if (!errorMessage.empty())
            std::cout << errorMessage << "\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

void errorCallback(int code, const char* desc)
{
    fputs(desc, stderr);
}

void setViewport(int width, int height)
{
    glViewport(0, 0, width, height);
}

/* Window resize callback*/
void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    /** sets the viewport (a rectangle in pixels on the screen that you wish to render to), transforms NDC coordinates to screen coordinates.
    OpenGL will automatically scale the rendering so it fits into the given viewport. */
    setViewport(width, height);
}

void initGLFW()
{
    glfwSetErrorCallback(errorCallback);
    /* This function initializes the GLFW library.Before most GLFW functions can be used, GLFW must be initialized
    (GLFW provides programmers with the ability to create and manage windows and OpenGL contexts,
    as well as handle joystick, keyboard and mouse input.) */
    if (!glfwInit())
        exit(EXIT_FAILURE);
    /* There are a number of hints that can be set before the creation of a window and context.
    Some affect the window itself, others affect the framebuffer or context.
    Hints below specify the client API version that the created context must be compatible with. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* This hint specifies which OpenGL profile to create the context for.
    Core-profile gets access to a smaller subset of OpenGL features without backwards-compatible features we don't need. */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

void mouseCallback(GLFWwindow* window, double x, double y)
{
}

GLFWwindow* createWindow(const char* windowName, bool isFullscreen, bool isBorderless, int width, int height)
{
    glfwWindowHint(GLFW_DECORATED, !isBorderless);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (isFullscreen)
    {
        width = mode->width;
        height = mode->height;
    }
    GLFWwindow* window = glfwCreateWindow(width, height, windowName, isFullscreen ? monitor : NULL, NULL);
    exitWhenNull(!window, "::Failed to create GLFW window");
    /* In order for any OpenGL commands to work, a context must be current */
    glfwMakeContextCurrent(window); 
    if (!isFullscreen)
    {
        glfwSetWindowPos(window, mode->width / 2 - width / 2, mode->height / 2 - height / 2);
    }

//#ifdef __EMSCRIPTEN__
//    EGLDisplay display = glfwGetEGLDisplay();
//    int egl_version = gladLoaderLoadEGL(display);
//#else

    /* GLFW returns glfwGetProcAddress that defines the correct function based on which OS we're compiling for. */
    exitWhenNull(!window, "::Failed to create GLFW window");
    exitWhenNull(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "::Failed to initialize GLAD");
//#endif
    setViewport(width, height);

    /* Sets the framebuffer resize callback for the specified window. */
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSwapInterval(1);

    return window;
}