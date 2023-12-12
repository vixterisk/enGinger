struct VertexArrayData
{
    GLuint boundVAO;
    GLuint boundVBO;
    GLuint boundEBO;
};

VertexArrayData getVertexArrayData(std::vector<Vector3> vertices, std::vector <GLuint> indices);

void clearAllBuffers();

void draw(GLuint shaderProgram, GLuint VAO, int ElementsCount);

void cleanGlResources(VertexArrayData vertexArrayData, GLuint shaderProgram);