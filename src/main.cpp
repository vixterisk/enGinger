#include <windows.h>
#include "glfw-utils.hpp"
#include "renderer.hpp"
#include "config-data.hpp"
#include "path.hpp"
#include "shader-program.hpp"

/* vertices within Normalized Device Coordinates (NDC) range
Unlike usual screen coordinates the positive y-axis points in the up-direction and the (0,0) coordinates are at the center of the graph, 
instead of top-left. Eventually all the (transformed) coordinates should end up in this coordinate space, otherwise they won't be visible. 
These coordinates will then be transformed to screen-space coordinates (via the viewport transform). The resulting screen-space coordinates 
are then transformed to fragments as inputs to fragment shader. */

std::vector<Vertex> generateTriangle()
{
    GLfloat left = -0.8f, bottom = -0.8f;
    GLfloat right = 0.8f, top = 0.8f;
    Position leftBottom = Position(left, bottom);
    Position rightBottom = Position(right, bottom);
    Position leftTop = Position(left, top);
    Position rightTop = Position(right, top);
    Vertex vertex1 = Vertex(leftBottom, Color::magenta(), UV(), Position());
    Vertex vertex2 = Vertex(rightBottom, Color::cyan(), UV(), Position());
    Vertex vertex3 = Vertex(leftTop, Color::yellow(), UV(), Position());
    Vertex vertex4 = Vertex(rightTop, Color::white(), UV(), Position());
    return std::vector<Vertex> { vertex1, vertex2, vertex3, vertex4 };
}

int main(int, char*[])
{
    deletePath();
    std::vector<Vertex> triangle = generateTriangle();

    std::vector<GLuint> indices =
    {
       0, 1, 2,
       1, 2, 3
    };

    initGLFW();

    ConfigData data = readConfig();

    GLFWwindow* window = createWindow("enGinger", data.fullscreen, data.borderless, data.width, data.height);
    exitWhenNull(!window, "Failed to create GLFW window.");

    VertexArrayData vertexArrayData = getVertexArrayData(triangle, indices);
    exitWhenNull(!vertexArrayData.boundVAO, "Failed to create Vertex Array Object.");

    std::string vertexShaderPath = getShaderAbsolutePath(GL_VERTEX_SHADER, data.vertexShader);
    std::string fragmentShaderPath = getShaderAbsolutePath(GL_FRAGMENT_SHADER, data.fragmentShader);
    GLuint shaderProgram = createShaderProgramUsingFile(vertexShaderPath, fragmentShaderPath);
    exitWhenNull(!shaderProgram, "Failed to create shader program.");
    glUseProgram(shaderProgram);
    /* Frame -  All draw calls here*/
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    while (!glfwWindowShouldClose(window))
    {
        clearAllBuffers();
        draw(shaderProgram, *vertexArrayData.boundVAO, indices.size());
        /* This function processes only those events that are already in the event queue and then returns immediately. 
        Processing events will cause the window and input callbacks associated with those events to be called. */
        glfwPollEvents();
        /* Swaps the front and back buffers of the specified window that are used to prevent screen tearing. */
        glfwSwapBuffers(window);
    }
    ShowWindow(GetConsoleWindow(), SW_RESTORE);

    cleanGlResources(vertexArrayData, shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    deletePath();

    return EXIT_SUCCESS;
}
