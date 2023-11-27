#include <Dependencies.h>
#include <Utils.h>
#include <ShaderProgram.h>

// TODO config
#define DEBUG 1

/* vertices within Normalized Device Coordinates (NDC) range
Unlike usual screen coordinates the positive y-axis points in the up-direction and the (0,0) coordinates are at the center of the graph, 
instead of top-left. Eventually all the (transformed) coordinates should end up in this coordinate space, otherwise they won't be visible. 
These coordinates will then be transformed to screen-space coordinates (via the viewport transform). The resulting screen-space coordinates 
are then transformed to fragments as inputs to fragment shader. */
std::vector<Point> vertices = {
   { 0.5f, 0.5f, 0.0f },
   { 0.5f, -0.5f, 0.0f },
   { -0.5f,  -0.5f, 0.0f },
   { -0.5f,  0.5f, 0.0f }
};

std::vector<GLuint> indices = {
   0, 1, 3,
   1, 2, 3
};

int main(int argc, char* argv[])
{
    const char* resourcesPath = "";
#if DEBUG
    resourcesPath = "C:\\Users\\ermolina\\Desktop\\enGINGER\\src\\resources";
#else
    if (argc > 1)
    {
        resourcesPath = argv[1];
    }
    else
    {
        std::cout << "Please pass path to resources." << std::endl;
        return 0;
    }
#endif

    GLFWwindow* window = createWindow("ENginger");
    GLuint ModelVAO = CreateVertexArrayObject(vertices, indices);
    GLuint shaderProgram = CreateShaderProgram(resourcesPath, "vertexShader.glsl", "fragmentShader.glsl");
    glUseProgram(shaderProgram);
    /* Frame */
    while (!glfwWindowShouldClose(window))
    {
        ClearAllBuffers();
        processInput(window);

        /* All drawcalls here*/
        draw(shaderProgram, ModelVAO, indices.size());

        /* This function processes only those events that are already in the event queue and then returns immediately. 
        Processing events will cause the window and input callbacks associated with those events to be called. */
        glfwPollEvents();
        /* Swaps the front and back buffers of the specified window that are used to prevent screen tearing. */
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
