#include "renderer.hpp"

int attributeCount = 0;

void enableVertexAttributeFloat(GLint size, void* pointer, GLboolean normalized = GL_FALSE)
{
    glVertexAttribPointer(attributeCount, size, GL_FLOAT, normalized, sizeof(Vertex), pointer);                          // Defines an array of generic vertex attribute data per each attribute.
    glEnableVertexAttribArray(attributeCount);                                                                           // Enables the generic vertex attribute array specified by index.
    attributeCount++;
}

VertexArrayData getVertexArrayData(std::vector<Vertex> vertices, std::vector <GLuint> indices)                           // Creates memory on the GPU to store vertex data ( via so-called vertex buffer objects (VBO) ) as large batches of data, configures how OpenGL should interpret the said memory, specifies how to send the data to the graphics card.
{                                                                                                                        // P.S. Sending data to the graphics card from the CPU is relatively slow, so whenever is possible it's best to send as much data as possible at once. Once the data is in the graphics card's memory the vertex shader has almost instant access to the vertices making it extremely fast.
    VertexArrayData vertexArrayData = VertexArrayData(1, 1, 1);

    glGenVertexArrays(vertexArrayData.getBoundVAOCount(), vertexArrayData.boundVAO);                                     // returns buffer object name in VAO.
    glGenBuffers(vertexArrayData.getBoundEBOCount(), vertexArrayData.boundEBO);                                          // returns buffer object name in EBO.
    glGenBuffers(vertexArrayData.getBoundVBOCount(), vertexArrayData.boundVBO);                                          // Returns buffer object name in VBO.
    glBindVertexArray(*vertexArrayData.boundVAO);                                                                        // Binds the vertex array object with name VAO.

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *vertexArrayData.boundEBO);                                                    // set EBO as currently bound GL_ELEMENT_ARRAY_BUFFER (Vertex array indices).
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);                                 // Creates and initializes a buffer object's data store.
                                                                                                                         // With GL_STATIC_DRAW data store contents will be modified once and used many times as the source for GL drawing commands.
    glBindBuffer(GL_ARRAY_BUFFER, *vertexArrayData.boundVBO);                                                            // Set VBO as currently bound GL_ARRAY_BUFFER (Vertex attributes).
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    enableVertexAttributeFloat( sizeof(Position{}) / sizeof (GLfloat), (void*)0);                            // Position attribute enabled.
    enableVertexAttributeFloat(sizeof(Color{}) / sizeof (GLfloat), (void*)(sizeof(Position)));               // Color attribute enabled.
    enableVertexAttributeFloat(sizeof(UV{}) / sizeof (GLfloat), (void*)(sizeof(Color)));                     // UV attribute enabled.
    enableVertexAttributeFloat(sizeof(Position{}) / sizeof (GLfloat), (void*)(sizeof(UV)));                  // Normals attribute enabled.

    return vertexArrayData;
}

void clearAllBuffers()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);                                                                                // State-setting function: glClearColor specifies the red, green, blue, and alpha values used by glClear to clear the color buffers. Values specified by glClearColor are clamped to the range [0,1].
    glClear(GL_COLOR_BUFFER_BIT);                                                                                        // State-using function: clears buffers to preset values, previously selected by glClearColor, glClearDepth, and glClearStencil. As many color buffers can be selected to be drawn into as there is in glDrawBuffer.
}

void draw(GLuint shaderProgram, GLuint VAO, int ElementsCount)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, ElementsCount, GL_UNSIGNED_INT, 0);                                                     // Primitives is an interpretation scheme used by OpenGL to determine what a stream of vertices represents when being rendered e.g. "GL_POINTS".
}

void cleanGlResources(VertexArrayData vertexArrayData, GLuint shaderProgram)
{
    glDeleteVertexArrays(vertexArrayData.getBoundVAOCount(), vertexArrayData.boundVAO);
    glDeleteBuffers(vertexArrayData.getBoundVBOCount(), vertexArrayData.boundVBO);
    glDeleteBuffers(vertexArrayData.getBoundEBOCount(), vertexArrayData.boundEBO);
    vertexArrayData.deleteVertexArrayData();
    glDeleteProgram(shaderProgram);
}