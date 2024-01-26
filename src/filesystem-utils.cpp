#include "filesystem-utils.hpp"
#include <nlohmann-json/json.hpp>
#include <iostream>
#include <fstream>

struct SrcPathNode
{
    std::string name;
    SrcPathNode* parent = nullptr;
    std::vector<SrcPathNode*> children;
};

class SourceTree
{
public:
    SrcPathNode* rootPath;
    SrcPathNode* resourcesPath;
    SrcPathNode* shadersPath;
    SrcPathNode* vertexShaderPath;
    SrcPathNode* fragmentShaderPath;
    SrcPathNode* configPath;

    bool isInitialized() const { return rootPath != nullptr; }
    SrcPathNode *getResourcePath() const { return resourcesPath; }
    SrcPathNode *getShadersPath() const { return shadersPath; }
    SrcPathNode *getVertexShaderPath() const { return vertexShaderPath; }
    SrcPathNode *getFragmentShaderPath() const { return fragmentShaderPath; }
    SrcPathNode *getConfigPath() const { return configPath; }
};

std::map<PathNodeType, std::string> pathNodeName
{
    { PathNodeType::configJson, "configJson" },
    { PathNodeType::vertexShader, "vertexShader" },
    { PathNodeType::fragmentShader, "fragmentShader" }
};

SourceTree sourceTree;

SrcPathNode* createPath(std::string name, SrcPathNode *parent)
{
    SrcPathNode* newPath = new SrcPathNode();
    newPath->name = std::move(name);
    newPath->parent = parent;
    if (parent)
        parent->children.push_back(newPath);
    return newPath;
}

std::string getSrcPathEnvVar()
{
    const unsigned short int pathBufferSize = 100;
    char srcPath[pathBufferSize];
    const char* engingerSrcPath = "ENGINGER_SRC_PATH";

    if (!getenv(engingerSrcPath)) {
        std::cout << "The environment variable " << engingerSrcPath << " was not found.\n" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (snprintf(srcPath, pathBufferSize, "%s", getenv(engingerSrcPath)) >= pathBufferSize) {
        std::cout << "SourceTree buffer size of " << pathBufferSize << " was too small. Aborting\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    return srcPath;
}

void initializePath() 
{
    std::string srcPath = getSrcPathEnvVar();
    sourceTree.rootPath = createPath(srcPath, nullptr);
    sourceTree.vertexShaderPath = nullptr;
    sourceTree.fragmentShaderPath = nullptr;
    sourceTree.configPath = createPath("config.json", sourceTree.rootPath);
    sourceTree.resourcesPath = createPath("resources", sourceTree.rootPath);
    sourceTree.shadersPath = createPath("shaders", sourceTree.resourcesPath);
}

void addShaderPath(GLenum shaderType, const std::string& shaderName)
{
    if (!sourceTree.isInitialized())
        initializePath();

    for (int i = 0; i < sourceTree.shadersPath->children.size(); i++)
        if (sourceTree.shadersPath->children[i]->name == shaderName)
            return;

    SrcPathNode *shaderPathNode = createPath(shaderName, sourceTree.shadersPath);
    if (shaderType == GL_VERTEX_SHADER)
        sourceTree.vertexShaderPath = shaderPathNode;
    else
        sourceTree.fragmentShaderPath = shaderPathNode;
}

std::string getAbsolutePath(SrcPathNode* currPath, PathNodeType type)
{
    if (currPath == nullptr) {
        std::cout << "SourceTree for " << pathNodeName[type] << " does not exist. Aborting... " << std::endl;
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
    if (!sourceTree.isInitialized())
        initializePath();

    switch (type) {
        case configJson:
            return getAbsolutePath(sourceTree.configPath, type);
        case vertexShader:
            return getAbsolutePath(sourceTree.vertexShaderPath, type);
        case fragmentShader:
            return getAbsolutePath(sourceTree.fragmentShaderPath, type);
        default:
            exit(EXIT_FAILURE);
    }
}

std::string getShaderAbsolutePath(GLenum shaderType, const std::string &vertexShaderName)
{
    PathNodeType ShaderPathNodeType;
    if (shaderType == GL_VERTEX_SHADER)
        ShaderPathNodeType = PathNodeType::vertexShader;
    else
        ShaderPathNodeType = PathNodeType::fragmentShader;

    addShaderPath(shaderType, vertexShaderName);
    return getAbsolutePath(ShaderPathNodeType);
}

void deletePath(SrcPathNode* &root)
{
    if (root == nullptr)
        return;

    while (!root->children.empty()) {
        deletePath(root->children.front());
        root->children.erase(root->children.begin());
    }

    delete(root);
    root = nullptr;
}

void deletePath() {
    deletePath(sourceTree.rootPath);
    sourceTree.rootPath = nullptr;
    sourceTree.resourcesPath = nullptr;
    sourceTree.shadersPath = nullptr;
    sourceTree.vertexShaderPath = nullptr;
    sourceTree.fragmentShaderPath = nullptr;
    sourceTree.configPath = nullptr;
}

template<class T>
void readValue(nlohmann::json data, const std::string& jsonKey, T &result)
{
    nlohmann::json value = data[jsonKey];
    result = value.template get<T>();
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
    readValue(data, "fullscreen", result.isFullscreen);
    readValue(data, "borderless", result.isBorderless);
    readValue(data, "width", result.width);
    readValue(data, "height", result.height);
    return result;
}