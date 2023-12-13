#include <glad/glad.h>
#include <vector>
#include "geometry/point.hpp"

// TODO perenesti
class VertexArrayData
{
private:
    int boundVAOCount;
    int boundVBOCount;
    int boundEBOCount;
public:
    GLuint *boundVAO;
    GLuint *boundVBO;
    GLuint *boundEBO;
    VertexArrayData(int VAOCount, int VBOCount, int EBOCount)
    {
        boundVAOCount = VAOCount;
        boundVBOCount = VBOCount;
        boundEBOCount = EBOCount;
        boundVAO = (GLuint*)malloc(VAOCount * sizeof(GLuint));
        boundVBO = (GLuint*)malloc(VBOCount * sizeof(GLuint));
        boundEBO = (GLuint*)malloc(EBOCount * sizeof(GLuint));
    }

    int getBoundVAOCount()
    {
        return boundVAOCount;
    }
    int getBoundVBOCount()
    {
        return boundVBOCount;
    }
    int getBoundEBOCount()
    {
        return boundEBOCount;
    }
    void deleteVertexArrayData()
    {
        free(boundVAO);
        free(boundVBO);
        free(boundEBO);
    }
};

VertexArrayData getVertexArrayData(Matrix vertices, std::vector<GLuint> indices);

void clearAllBuffers();

void draw(GLuint shaderProgram, GLuint VAO, int ElementsCount);

void cleanGlResources(VertexArrayData vertexArrayData, GLuint shaderProgram);