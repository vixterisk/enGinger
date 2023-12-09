#include "Utils.h"

const int HEIGHT = 1080;
const int WIDTH = 1920;

void setViewport(int width = WIDTH, int height = HEIGHT)
{
    /** sets the viewport (a rectangle in pixels on the screen that you wish to render to), transforms NDC coordinates to screen coordinates.
    OpenGL will automatically scale the rendering so it fits into the given viewport. */
    glViewport(0, 0, width, height);
}

void InitGLFW()
{
    /* This function initializes the GLFW library.Before most GLFW functions can be used, GLFW must be initialized
    (GLFW provides programmers with the ability to create and manage windows and OpenGL contexts,
    as well as handle joystick, keyboard and mouse input.) */
    glfwInit();
    /* There are a number of hints that can be set before the creation of a window and context. 
    Some affect the window itself, others affect the framebuffer or context. 
    Hints below specify the client API version that the created context must be compatible with. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* This hint specifies which OpenGL profile to create the context for. 
    Core-profile gets access to a smaller subset of OpenGL features without backwards-compatible features we don't need. */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

/* Window resize callback*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    setViewport(width, height);
}

GLFWwindow* createWindow(const char* windowName)
{
    InitGLFW();
    /* This function creates a window and its associated OpenGL or OpenGL ES context. 
    A context stores all of the state associated with this instance of OpenGL.  */
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, windowName, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "::Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    /* In order for any OpenGL commands to work, a context must be current */
    glfwMakeContextCurrent(window);

#ifdef __EMSCRIPTEN__
    EGLDisplay display = glfwGetEGLDisplay();
    int egl_version = gladLoaderLoadEGL(display);
#else

    /* GLFW returns glfwGetProcAddress that defines the correct function based on which OS we're compiling for. */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "::Failed to initialize GLAD" << std::endl;
        return NULL;
    }
#endif
    setViewport();
    /* Sets the framebuffer resize callback for the specified window. */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

/* Creates memory on the GPU to store vertex data ( via so called vertex buffer objects (VBO) ) as large batches of data, 
configures how OpenGL should interpret the said memory, 
specifies how to send the data to the graphics card. 

P.S. Sending data to the graphics card from the CPU is relatively slow, so whenever is possible it's best to send as much data as possible at once.
Once the data is in the graphics card's memory the vertex shader has almost instant access to the vertices making it extremely fast.*/
GLuint CreateVertexArrayObject(std::vector<Point> vertices, std::vector <GLuint> indices)
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

    glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(Point), &vertices[0], GL_STATIC_DRAW);
    /* defines an array of generic vertex attribute data per each attribute. */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
    /* enables the generic vertex attribute array specified by index. */
    glEnableVertexAttribArray(0);
    return VAO;
}

void ClearAllBuffers()
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

#define PATHBUFFERSIZE 100

std::string GetSrcPathEnvVar()
{
    char srcPath[PATHBUFFERSIZE];
    const char* engingeSrcPath = "ENGINGER_SRC_PATH";

    if (!getenv(engingeSrcPath))
    {
        std::cout << "The environment variable " << engingeSrcPath << " was not found.\n" << std::endl;
        exit(1);
    }

    if (snprintf(srcPath, PATHBUFFERSIZE, "%s", getenv(engingeSrcPath)) >= PATHBUFFERSIZE)
    {
        std::cout << "Path buffer size of " << PATHBUFFERSIZE << " was too small. Aborting\n" << std::endl;
        exit(1);
    }
    return srcPath;
}

pathNode* createPath(std::string name, pathNode* parent) {
    pathNode* newPath = new pathNode();
    newPath->name = name;
    newPath->parent = parent;
    if (parent) parent->children.push_back(newPath);
    return newPath;
}

std::string getFullPath(pathNode* path) {
    std::string parentName = path->parent == NULL ? "" : getFullPath(path->parent) + +"\\";
    return parentName + path->name;
}

Path getInitializedPath() {
    std::string srcPath = GetSrcPathEnvVar();
    Path path;
    path.rootPath = createPath(srcPath, NULL);
    path.resourcesPath = createPath("resources", path.rootPath);
    path.shadersPath = createPath("shaders", path.resourcesPath);
    path.vertexShaderPath = NULL;
    path.fragmentShaderPath = NULL;
    path.configPath = createPath("config.json", path.rootPath);
    return path;
}

void setShadersPath(Path* path, std::string vertexShaderName, std::string fragmentShaderName) {
    path->vertexShaderPath = createPath(vertexShaderName, path->shadersPath);;
    path->fragmentShaderPath = createPath(fragmentShaderName, path->shadersPath);;
}