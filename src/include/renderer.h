#include "Dependencies.h"

GLuint createVertexArrayObject(std::vector<Vector3> vertices, std::vector <GLuint> indices);

void clearAllBuffers();

void draw(GLuint shaderProgram, GLuint VAO, int ElementsCount);