#include <glad/glad.h>
#include <string>

GLuint createShaderProgramUsingFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);