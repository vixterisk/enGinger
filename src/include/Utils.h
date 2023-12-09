#pragma once

#include "Dependencies.h"

void InitGLFW();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* createWindow(const char* windowName);

void processInput(GLFWwindow* window);

GLuint CreateVertexArrayObject(std::vector<Point> vertices, std::vector<GLuint> indices);

void ClearAllBuffers();

void draw(GLuint shaderProgram, GLuint VAO, int ElementsCount);

struct pathNode {
    std::string name;
    pathNode* parent;
    std::vector<pathNode*> children;
};

class Path {
public:
    pathNode* rootPath;
    pathNode* resourcesPath;
    pathNode* shadersPath;
    pathNode* vertexShaderPath;
    pathNode* fragmentShaderPath;
    pathNode* configPath;
};

std::string getFullPath(pathNode* path);

Path getInitializedPath();

void setShadersPath(Path* path, std::string vertexShaderName, std::string fragmentShaderName);