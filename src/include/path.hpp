#include <glad/glad.h>
#include <string> 

enum PathNodeType { configJson, vertexShader, fragmentShader };

void addShaderToPath(GLenum shaderType, const std::string& shaderName);

void deletePath();

std::string getAbsolutePath(PathNodeType type);

std::string getShaderAbsolutePath(GLenum shaderType, const std::string& jsonKey);