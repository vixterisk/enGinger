#pragma once
#include "Dependencies.h"

nlohmann::json readConfig(std::string jsonKey);

void exitWhenNull(bool isNull, std::string errorMessage);

void initGLFW();

GLFWwindow* createWindow(const char* windowName, bool isFullscreen, bool isBorderless, int width, int height);