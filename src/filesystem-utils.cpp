#include "filesystem-utils.hpp"
#include <nlohmann-json/json.hpp>
#include <iostream>
#include <fstream>

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

    bool isInitialized() { return rootPath != nullptr; }
    PathNode *getResourcePath() const { return resourcesPath; }
    PathNode *getShadersPath() const { return shadersPath; }
    PathNode *getVertexShaderPath() const { return vertexShaderPath; }
    PathNode *getFragmentShaderPath() const { return fragmentShaderPath; }
    PathNode *getConfigPath() const { return configPath; }
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
    newPath->name = std::move(name);
    newPath->parent = parent;
    if (parent) parent->children.push_back(newPath);
    return newPath;
}

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

void addShaderPath(GLenum shaderType, const std::string& shaderName)
{
    if (!path.isInitialized())
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
    if (!path.isInitialized())
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

    addShaderPath(shaderType, vertexShaderName);
    return getAbsolutePath(ShaderPathNodeType);
}

void deletePath(PathNode *&root)
{
    if (root == nullptr)
        return;

    while (!root->children.empty())
    {
        deletePath(root->children.front());
        root->children.erase(root->children.begin());
    }

    delete(root);
    root = nullptr;
}

void deletePath() {
    deletePath(path.rootPath);
    path.rootPath = nullptr;
    path.resourcesPath = nullptr;
    path.shadersPath = nullptr;
    path.vertexShaderPath = nullptr;
    path.fragmentShaderPath = nullptr;
    path.configPath = nullptr;
}

template<class ConfigField>
void readValue(nlohmann::json data, const std::string& jsonKey, ConfigField &result)
{
    nlohmann::json value = data[jsonKey];
    result = value.template get<ConfigField>();
}

ConfigData getConfig()
{
    using json = nlohmann::json;
    ConfigData result;
    std::string configAbsolutePath = getAbsolutePath(PathNodeType::configJson);
    std::ifstream configFile(configAbsolutePath);
    json data = json::parse(configFile);

    readValue(data, "vertexShader", result.vertexShader);
    readValue(data, "fragmentShader", result.fragmentShader);
    readValue(data, "fullscreen", result.fullscreen);
    readValue(data, "borderless", result.borderless);
    readValue(data, "width", result.width);
    readValue(data, "height", result.height);
    return result;
}