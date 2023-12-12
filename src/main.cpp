#include "glfwUtils.h"
#include "renderer.h"
#include "configData.h"
#include "Path.h"
#include "ShaderProgram.h"

/* vertices within Normalized Device Coordinates (NDC) range
Unlike usual screen coordinates the positive y-axis points in the up-direction and the (0,0) coordinates are at the center of the graph, 
instead of top-left. Eventually all the (transformed) coordinates should end up in this coordinate space, otherwise they won't be visible. 
These coordinates will then be transformed to screen-space coordinates (via the viewport transform). The resulting screen-space coordinates 
are then transformed to fragments as inputs to fragment shader. */

std::vector<Vector3> vertices = 
{
   { 0.5f, 0.5f, 0.0f },
   { 0.5f, -0.5f, 0.0f },
   { -0.5f,  -0.5f, 0.0f },
   { -0.5f,  0.5f, 0.0f }
};

std::vector<GLuint> indices = 
{
   0, 1, 3,
   1, 2, 3
};

void cleanGlResources(VertexArrayData vertexArrays, GLuint shaderPrograms)
{
    glDeleteVertexArrays(1, &vertexArrays.boundVAO);
    glDeleteBuffers(1, &vertexArrays.boundVBO);
    glDeleteBuffers(1, &vertexArrays.boundEBO);
    glDeleteProgram(shaderPrograms);
}

int main(int argc, char* argv[])
{
    initGLFW();

    ConfigData data = readConfig();

    GLFWwindow* window = createWindow("enGinger", data.fullscreen, data.borderless, data.width, data.height);
    exitWhenNull(!window, "Failed to create GLFW window.");

    VertexArrayData VAD = createVertexArrayObject(vertices, indices);

    exitWhenNull(!VAD.boundVAO, "Failed to create Vertex Array Object.");

    std::string vertexShaderPath = getShaderAbsolutePath(GL_VERTEX_SHADER, data.vertexShader);
    std::string fragmentShaderPath = getShaderAbsolutePath(GL_FRAGMENT_SHADER, data.fragmentShader);
    GLuint shaderProgram = createShaderProgramUsingFile(vertexShaderPath, fragmentShaderPath);
    exitWhenNull(!shaderProgram, "Failed to create shader program.");
    glUseProgram(shaderProgram);
    /* Frame */
    while (!glfwWindowShouldClose(window))
    {
        //std::cout << glfwGetTime() << "\n";
        clearAllBuffers();
        /* All drawcalls here*/
        draw(shaderProgram, VAD.boundVAO, indices.size());
        /* This function processes only those events that are already in the event queue and then returns immediately. 
        Processing events will cause the window and input callbacks associated with those events to be called. */
        glfwPollEvents();
        /* Swaps the front and back buffers of the specified window that are used to prevent screen tearing. */
        glfwSwapBuffers(window);
    }
    cleanGlResources(VAD, shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
