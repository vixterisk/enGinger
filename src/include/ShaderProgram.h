#pragma once

#include "Dependencies.h"

GLuint CreateShaderProgramUsingFile(std::string vertexShaderPath, std::string fragmentShaderPath);

GLuint CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);