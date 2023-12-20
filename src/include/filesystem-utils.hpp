#include <glad/glad.h>
#include <string> 

struct ConfigData
{
    std::string vertexShader;
    std::string fragmentShader;
    bool fullscreen;
    bool borderless;
    int width;
    int height;
};

enum PathNodeType { configJson, vertexShader, fragmentShader };

void addShaderPath(GLenum shaderType, const std::string& shaderName);

std::string getAbsolutePath(PathNodeType type);

std::string getShaderAbsolutePath(GLenum shaderType, const std::string& jsonKey);

void deletePath();

ConfigData getConfig();