#pragma once
#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <nlohmann-json/json.hpp>
//#include "geometry/point.h"
#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>
//#include <vector>

GLuint createShaderProgramUsingFile(std::string vertexShaderPath, std::string fragmentShaderPath);

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);