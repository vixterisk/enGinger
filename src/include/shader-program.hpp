#include <glad/glad.h>
#include <string>

GLuint createShaderProgramUsingFile(std::string vertexShaderPath, std::string fragmentShaderPath);

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);