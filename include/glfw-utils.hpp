#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

void initGLFW();

GLFWwindow* createWindow(const char* windowName, bool isFullscreen, bool isBorderless, int width, int height);

template<class F>
void checkCondition(bool condition, F errorHandler = {}, std::string errorMessage = "");

#include "check-condition.tpp"