#pragma once
#include <glad/glad.h>
#include "geometry/point.h"
#include <vector>
#include "renderer.h"

/* Creates memory on the GPU to store vertex data ( via so called vertex buffer objects (VBO) ) as large batches of data,
configures how OpenGL should interpret the said memory,
specifies how to send the data to the graphics card.

P.S. Sending data to the graphics card from the CPU is relatively slow, so whenever is possible it's best to send as much data as possible at once.
Once the data is in the graphics card's memory the vertex shader has almost instant access to the vertices making it extremely fast.*/
VertexArrayData createVertexArrayObject(std::vector<Vector3> vertices, std::vector <GLuint> indices)
{
    VertexArrayData result = VertexArrayData();
    /* A Vertex Array Object (or VAO) is an object that describes how the vertex attributes are stored in a Vertex Buffer Object (or VBO) */
    /* returns vertex array object name in VAO */
    glGenVertexArrays(1, &result.boundVAO);
    /* binds the vertex array object with name VAO */
    glBindVertexArray(result.boundVAO);

    /* element buffer object allows to reuse vertex data without duplicating all attributes values */
    /* returns buffer object name in EBO */
    glGenBuffers(1, &result.boundEBO);
    /* set EBO as currently bound GL_ELEMENT_ARRAY_BUFFER (Vertex array indices) */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.boundEBO);
    /* creates and initializes a buffer object's data store.
    With GL_STATIC_DRAW data store contents will be modified once and used many times as the source for GL drawing commands. */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

    /* vertex buffer object provides methods for uploading vertex data (position, normal vector, color, etc.) */
    /* returns buffer object name in VBO */
    glGenBuffers(1, &result.boundVBO);
    /* set VBO as currently bound GL_ARRAY_BUFFER (Vertex attributes) */
    glBindBuffer(GL_ARRAY_BUFFER, result.boundVBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), &vertices[0], GL_STATIC_DRAW);
    /* defines an array of generic vertex attribute data per each attribute. */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
    /* enables the generic vertex attribute array specified by index. */
    glEnableVertexAttribArray(0);
    return result;
}

void clearAllBuffers()
{
    /* State-setting function:
    glClearColor specifies the red, green, blue, and alpha values used by glClear to clear the color buffers.
    Values specified by glClearColor are clamped to the range [0,1].  */
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    /* State-using function:
    clears buffers to preset values, previously selected by glClearColor, glClearDepth, and glClearStencil.
    As many color buffers can be selected to be drawn into as there is in glDrawBuffer.  */
    glClear(GL_COLOR_BUFFER_BIT);
}

void draw(GLuint shaderProgram, GLuint VAO, int ElementsCount)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    /* primitives is an interpretation scheme used by OpenGL to determine what a stream of vertices represents
    when being rendered e.g. "GL_POINTS" */
    glDrawElements(GL_TRIANGLES, ElementsCount, GL_UNSIGNED_INT, 0);
}