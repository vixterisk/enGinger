#include "path.hpp"
#include <nlohmann-json/json.hpp>
#include <iostream> 

struct PathNode 
{
    std::string name;
    PathNode *parent;
    std::vector<PathNode*> children;
};

class Path 
{
public:
    PathNode *rootPath;
    PathNode *resourcesPath;
    PathNode *shadersPath;
    PathNode *vertexShaderPath;
    PathNode *fragmentShaderPath;
    PathNode *configPath;
};

std::map<PathNodeType, std::string> pathNodeName
{
    { PathNodeType::configJson, "configJson" },
    { PathNodeType::vertexShader, "vertexShader" },
    { PathNodeType::fragmentShader, "fragmentShader" }
};

Path path;

PathNode *createPath(std::string name, PathNode *parent) 
{
    PathNode *newPath = new PathNode();
    newPath->name = name;
    newPath->parent = parent;
    if (parent) parent->children.push_back(newPath);
    return newPath;
}

std::string getSrcPathEnvVar()
{
    const unsigned short int PATHBUFFERSIZE = 100;
    char srcPath[PATHBUFFERSIZE];
    const char* engingeSrcPath = "ENGINGER_SRC_PATH";

    if (!getenv(engingeSrcPath))
    {
        std::cout << "The environment variable " << engingeSrcPath << " was not found.\n" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (snprintf(srcPath, PATHBUFFERSIZE, "%s", getenv(engingeSrcPath)) >= PATHBUFFERSIZE)
    {
        std::cout << "Path buffer size of " << PATHBUFFERSIZE << " was too small. Aborting\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    return srcPath;
}

void initializePath() 
{
    std::string srcPath = getSrcPathEnvVar();
    path.rootPath = createPath(srcPath, NULL);
    path.resourcesPath = createPath("resources", path.rootPath);
    path.shadersPath = createPath("shaders", path.resourcesPath);
    path.vertexShaderPath = NULL;
    path.fragmentShaderPath = NULL;
    path.configPath = createPath("config.json", path.rootPath);
}


void addShaderToPath(GLenum shaderType, std::string shaderName) 
{
    if (path.rootPath == NULL)
        initializePath();

    for (int i = 0; i < path.shadersPath->children.size(); i++)
        if (path.shadersPath->children[i]->name == shaderName)
            return;

    PathNode *shaderPathNode = createPath(shaderName, path.shadersPath);
    if (shaderType == GL_VERTEX_SHADER)
        path.vertexShaderPath = shaderPathNode;
    else
        path.fragmentShaderPath = shaderPathNode;
}

std::string getAbsolutePath(PathNode* path, PathNodeType type)
{
    if (path == NULL)
    {
        std::cout << "Path for " << pathNodeName[type] << " does not exist. Aborting... " << std::endl;
        exit(EXIT_FAILURE);
    }

    const char pathSeparator =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif
    std::string parentName = path->parent == NULL ? "" : getAbsolutePath(path->parent, type) + pathSeparator;
    return parentName + path->name;
}

std::string getAbsolutePath(PathNodeType type) 
{
    if (path.rootPath == NULL)
        initializePath();

    switch (type)
    {
        case configJson:
            return getAbsolutePath(path.configPath, type);
        case vertexShader:
            return getAbsolutePath(path.vertexShaderPath, type);
        case fragmentShader:
            return getAbsolutePath(path.fragmentShaderPath, type);
        default:
            return NULL;
    }
}

std::string getShaderAbsolutePath(GLenum shaderType, std::string vertexShaderName)
{
    PathNodeType ShaderPathNodeType;
    if (shaderType == GL_VERTEX_SHADER)
        ShaderPathNodeType = PathNodeType::vertexShader;
    else
        ShaderPathNodeType = PathNodeType::fragmentShader;

    addShaderToPath(shaderType, vertexShaderName);
    return getAbsolutePath(ShaderPathNodeType);
}