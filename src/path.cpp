#include "path.hpp"
#include <nlohmann-json/json.hpp>
#include <iostream>
#include <utility>

struct PathNode 
{
    std::string name;
    PathNode *parent = nullptr;
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

//clear memory
PathNode *createPath(std::string name, PathNode *parent) 
{
    PathNode *newPath = new PathNode();
    newPath->name = std::move(name);
    newPath->parent = parent;
    if (parent) parent->children.push_back(newPath);
    return newPath;
}

void deletePath(PathNode *root)
{
    while (root->children.size() != 0)
    {
        deletePath(root->children.front());
        root->children.erase(root->children.begin());
    }
    delete(root);
    root = nullptr;
}

void deletePath() { if (&path != nullptr) deletePath(path.rootPath); }

std::string getSrcPathEnvVar()
{
    const unsigned short int pathBufferSize = 100;
    char srcPath[pathBufferSize];
    const char* engingerSrcPath = "ENGINGER_SRC_PATH";

    if (!getenv(engingerSrcPath))
    {
        std::cout << "The environment variable " << engingerSrcPath << " was not found.\n" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (snprintf(srcPath, pathBufferSize, "%s", getenv(engingerSrcPath)) >= pathBufferSize)
    {
        std::cout << "Path buffer size of " << pathBufferSize << " was too small. Aborting\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    return srcPath;
}

void initializePath() 
{
    std::string srcPath = getSrcPathEnvVar();
    path.rootPath = createPath(srcPath, nullptr);
    path.resourcesPath = createPath("resources", path.rootPath);
    path.shadersPath = createPath("shaders", path.resourcesPath);
    path.vertexShaderPath = nullptr;
    path.fragmentShaderPath = nullptr;
    path.configPath = createPath("config.json", path.rootPath);
}


void addShaderToPath(GLenum shaderType, const std::string& shaderName)
{
    if (path.rootPath == nullptr)
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

std::string getAbsolutePath(PathNode* currPath, PathNodeType type)
{
    if (currPath == nullptr)
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
    std::string parentName = currPath->parent == nullptr ? "" : getAbsolutePath(currPath->parent, type) + pathSeparator;
    return parentName + currPath->name;
}

std::string getAbsolutePath(PathNodeType type) 
{
    if (path.rootPath == nullptr)
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
            exit(EXIT_FAILURE);
    }
}

std::string getShaderAbsolutePath(GLenum shaderType, const std::string& vertexShaderName)
{
    PathNodeType ShaderPathNodeType;
    if (shaderType == GL_VERTEX_SHADER)
        ShaderPathNodeType = PathNodeType::vertexShader;
    else
        ShaderPathNodeType = PathNodeType::fragmentShader;

    addShaderToPath(shaderType, vertexShaderName);
    return getAbsolutePath(ShaderPathNodeType);
}