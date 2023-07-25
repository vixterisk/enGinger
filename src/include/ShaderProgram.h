#pragma once

#include "Dependencies.h"

GLuint CreateShaderProgram(std::string resourcesPath, std::string vertexShaderName, std::string fragmentShaderName);

GLuint CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);