#include "Utils.h"
#include "Path.h"

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

void exitWhenNull(bool isNull, std::string errorMessage)
{
    if (isNull)
    {
        if (errorMessage != "")
            std::cout << errorMessage << "\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

nlohmann::json readConfig(std::string jsonKey)
{
    using json = nlohmann::json;
    std::string configJson = getAbsolutePath(PathNodeType::configJson);
    std::ifstream config(configJson);
    json data = json::parse(config);
    return data[jsonKey];
}

void initGLFW()
{
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

    return window;
}

/* Creates memory on the GPU to store vertex data ( via so called vertex buffer objects (VBO) ) as large batches of data, 
configures how OpenGL should interpret the said memory, 
specifies how to send the data to the graphics card. 

P.S. Sending data to the graphics card from the CPU is relatively slow, so whenever is possible it's best to send as much data as possible at once.
Once the data is in the graphics card's memory the vertex shader has almost instant access to the vertices making it extremely fast.*/
GLuint createVertexArrayObject(std::vector<Point> vertices, std::vector <GLuint> indices)
{
    /* A Vertex Array Object (or VAO) is an object that describes how the vertex attributes are stored in a Vertex Buffer Object (or VBO) */
    GLuint VAO;
    /* returns vertex array object name in VAO */
    glGenVertexArrays(1, &VAO);
    /* binds the vertex array object with name VAO */
    glBindVertexArray(VAO);

    /* element buffer object allows to reuse vertex data without duplicating all attributes values */
    GLuint EBO;
    /* returns buffer object name in EBO */
    glGenBuffers(1, &EBO);
    /* set EBO as currently bound GL_ELEMENT_ARRAY_BUFFER (Vertex array indices) */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    /* creates and initializes a buffer object's data store. 
    With GL_STATIC_DRAW data store contents will be modified once and used many times as the source for GL drawing commands. */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

    /* vertex buffer object provides methods for uploading vertex data (position, normal vector, color, etc.) */
    GLuint VBO;
    /* returns buffer object name in VBO */
    glGenBuffers(1, &VBO);
    /* set VBO as currently bound GL_ARRAY_BUFFER (Vertex attributes) */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point), &vertices[0], GL_STATIC_DRAW);
    /* defines an array of generic vertex attribute data per each attribute. */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
    /* enables the generic vertex attribute array specified by index. */
    glEnableVertexAttribArray(0);
    return VAO;
}

void clearAllBuffers()
{
    /* State-setting function:
    glClearColor specifies the red, green, blue, and alpha values used by glClear to clear the color buffers. 
    Values specified by glClearColor are clamped to the range [0,1].  */
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    /* State-using function:
    clears buffers to preset values, previously selected by glClearColor, glClearDepth, and glClearStencil. 
    As many color buffers can be selected to be drawn into as there is in glDrawBuffer.  */
    glClear(GL_COLOR_BUFFER_BIT);
}

void draw(GLuint shaderProgram, GLuint VAO, int ElementsCount)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    /* primitives is an interpretation scheme used by OpenGL to determine what a stream of vertices represents 
    when being rendered e.g. "GL_POINTS" */
    glDrawElements(GL_TRIANGLES, ElementsCount, GL_UNSIGNED_INT, 0);
}