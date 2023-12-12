#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nlohmann-json/json.hpp>
#include "geometry/point.h"

void exitWhenNull(bool isNull, std::string errorMessage);

void initGLFW();

GLFWwindow* createWindow(const char* windowName, bool isFullscreen, bool isBorderless, int width, int height);