#include "Dependencies.h"

enum PathNodeType { configJson, vertexShader, fragmentShader };

void addShaderToPath(GLenum shaderType, std::string shaderName);

std::string getAbsolutePath(PathNodeType type);

std::string getShaderAbsolutePath(GLenum shaderType, std::string jsonKey);