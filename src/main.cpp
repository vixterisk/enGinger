#include "glfw-utils.hpp"
#include "filesystem-utils.hpp"
#include "renderer.hpp"
#include "shader-program.hpp"
#include <windows.h>

/* vertices within Normalized Device Coordinates (NDC) range
Unlike usual screen coordinates the positive y-axis points in the up-direction and the (0,0) coordinates are at the center of the graph, 
instead of top-left. Eventually all the (transformed) coordinates should end up in this coordinate space, otherwise they won't be visible. 
These coordinates will then be transformed to screen-space coordinates (via the viewport transform). The resulting screen-space coordinates 
are then transformed to fragments as inputs to fragment shader. */

std::vector<Vertex> getVertices()
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
    std::vector<Vertex> vertices = getVertices();
    std::vector<GLuint> indices = {
       0, 1, 2,
       1, 2, 3
    };

    initGLFW();
    ConfigData configData = getConfig();
    GLFWwindow* window = createWindow(
            "enGinger",
            configData.isFullscreen,
            configData.isBorderless,
            configData.width,
            configData.height
            );
    auto errorHandler = []() { glfwTerminate(); };
    window = checkNotNull(window, errorHandler, "::Failed to create GLFW window");

    VertexArrayData vertexArrayData = getVertexArrayData(vertices, indices);
    vertexArrayData = checkNotNull(vertexArrayData, errorHandler, "Failed to create Vertex Array Object.");

    std::string vertexShaderPath = getShaderAbsolutePath(GL_VERTEX_SHADER, configData.vertexShader);
    std::string fragmentShaderPath = getShaderAbsolutePath(GL_FRAGMENT_SHADER, configData.fragmentShader);
    GLuint shaderProgram = createShaderProgramUsingFile(vertexShaderPath, fragmentShaderPath);
    shaderProgram = checkNotNull(shaderProgram, errorHandler, "Failed to create shader program.");
    glUseProgram(shaderProgram);

    ShowWindow(GetConsoleWindow(), SW_HIDE);
    while (!glfwWindowShouldClose(window)) {
        clearAllBuffers();
        draw(shaderProgram, *vertexArrayData.boundVAO, indices.size());
        glfwPollEvents();                                                                                                // This function processes only those events that are already in the event queue and then returns immediately. Processing events will cause the window and input callbacks associated with those events to be called.
        glfwSwapBuffers(window);                                                                                         // Swaps the front and back buffers of the specified window that are used to prevent screen tearing.
    }
    ShowWindow(GetConsoleWindow(), SW_RESTORE);

    cleanGlResources(vertexArrayData, shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    deletePath();

    return EXIT_SUCCESS;
}
