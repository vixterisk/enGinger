#include <Dependencies.h>
#include <Utils.h>
#include <ShaderProgram.h>

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
    if (argc > 1)
    {
        resourcesPath = argv[1];
    }
    else
    {
        std::cout << "Please pass path to resources." << std::endl;
        return 0;
    }

    GLFWwindow* window = createWindow("ENginger");
    GLuint ModelVAO = CreateVertexArrayObject(vertices, indices);
    GLuint shaderProgram = CreateShaderProgram(resourcesPath, "vertexShader.glsl", "fragmentShader.glsl");
    //delete vertexShaderSource;
    //delete fragmentShaderSource;    
    while (!glfwWindowShouldClose(window))
    {
        ClearAllBuffers();
        processInput(window);
        draw(shaderProgram, ModelVAO, indices.size());
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
