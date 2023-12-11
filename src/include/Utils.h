#pragma once

#include "Dependencies.h"

void errorCallback(int code, const char* desc);

nlohmann::json readConfig(std::string jsonKey);

void exitWhenNull(int parameter, std::string errorMessage);

void initGLFW();

GLFWwindow* createWindow(const char* windowName, bool isFullscreen);

GLuint createVertexArrayObject(std::vector<Point> vertices, std::vector<GLuint> indices);

void clearAllBuffers();

void draw(GLuint shaderProgram, GLuint VAO, int ElementsCount);