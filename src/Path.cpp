#include "path.h"

struct PathNode {
    std::string name;
    PathNode *parent;
    std::vector<PathNode*> children;
};

class Path {
public:
    PathNode *rootPath;
    PathNode *resourcesPath;
    PathNode *shadersPath;
    PathNode *vertexShaderPath;
    PathNode *fragmentShaderPath;
    PathNode *configPath;
};

Path path;

PathNode *createPath(std::string name, PathNode *parent) {
    PathNode *newPath = new PathNode();
    newPath->name = name;
    newPath->parent = parent;
    if (parent) parent->children.push_back(newPath);
    return newPath;
}

std::string GetSrcPathEnvVar()
{
    const unsigned short int PATHBUFFERSIZE = 100;
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

void initializePath() {
    std::string srcPath = GetSrcPathEnvVar();
    path.rootPath = createPath(srcPath, NULL);
    path.resourcesPath = createPath("resources", path.rootPath);
    path.shadersPath = createPath("shaders", path.resourcesPath);
    path.vertexShaderPath = NULL;
    path.fragmentShaderPath = NULL;
    path.configPath = createPath("config.json", path.rootPath);
}

std::string getFullPath(PathNode *path) {
    const char pathSeparator =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif
    std::string parentName = path->parent == NULL ? "" : getFullPath(path->parent) + pathSeparator;
    return parentName + path->name;
}

std::string getConfigJsonPath() {
    if (path.rootPath == NULL)
        initializePath();
    return getFullPath(path.configPath);
}

std::string getVertexShaderPath(std::string vertexShaderName) {
    if (path.rootPath == NULL)
        initializePath();
    path.vertexShaderPath = createPath(vertexShaderName, path.shadersPath);
    return getFullPath(path.vertexShaderPath);
}

std::string getFragmentShaderPath(std::string fragmentShaderName) {
    if (path.rootPath == NULL)
        initializePath();
    path.fragmentShaderPath = createPath(fragmentShaderName, path.shadersPath);
    return getFullPath(path.fragmentShaderPath);
}