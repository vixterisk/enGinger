#include "Utils.h"

const int HEIGHT = 1080;
const int WIDTH = 1920;

void InitGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* createWindow(const char* windowName)
{
    InitGLFW();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, windowName, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "::Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

#ifdef __EMSCRIPTEN__
    EGLDisplay display = glfwGetEGLDisplay();
    int egl_version = gladLoaderLoadEGL(display);
#else

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "::Failed to initialize GLAD" << std::endl;
        return NULL;
    }
#endif
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

GLuint CreateVertexArrayObject(std::vector<Point> vertices, std::vector <GLuint> indices)
{
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(Point), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
    glEnableVertexAttribArray(0);
    return VAO;
}

void ClearAllBuffers()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void draw(GLuint shaderProgram, GLuint VAO, int ElementsCount)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, ElementsCount, GL_UNSIGNED_INT, 0);
}