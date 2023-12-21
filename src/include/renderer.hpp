#include <glad/glad.h>
#include <vector>
#include <cstdlib>
#include "geometry/vertex-utils.hpp"

class VertexArrayData                                                                                                    // TODO make decent class hpp/cpp division
{
private:
    int boundVAOCount;
    int boundVBOCount;
    int boundEBOCount;
public:
    GLuint* boundVAO;
    GLuint* boundVBO;
    GLuint* boundEBO;
    VertexArrayData(int VAOCount, int VBOCount, int EBOCount)
    {
        boundVAOCount = VAOCount;                                                                                        // A Vertex Array Object (or VAO) is an object that describes how the vertex attributes are stored in a Vertex Buffer Object (or VBO).
        boundVBOCount = VBOCount;                                                                                        // Vertex buffer object provides methods for uploading vertex data (position, normal vector, color, etc.).
        boundEBOCount = EBOCount;                                                                                        // Element buffer object allows to reuse vertex data without duplicating all attributes values.
        boundVAO = (GLuint*)malloc(VAOCount * sizeof(GLuint));
        boundVBO = (GLuint*)malloc(VBOCount * sizeof(GLuint));
        boundEBO = (GLuint*)malloc(EBOCount * sizeof(GLuint));
    }

    int getBoundVAOCount() const { return boundVAOCount; }

    int getBoundVBOCount() const { return boundVBOCount; }

    int getBoundEBOCount() const { return boundEBOCount; }
    
    void deleteVertexArrayData() const
    {
        free(boundVAO);
        free(boundVBO);
        free(boundEBO);
    }
};

VertexArrayData getVertexArrayData(std::vector<Vertex> vertices, std::vector<GLuint> indices);

void clearAllBuffers();

void draw(GLuint shaderProgram, GLuint VAO, int ElementsCount);

void cleanGlResources(VertexArrayData vertexArrayData, GLuint shaderProgram);