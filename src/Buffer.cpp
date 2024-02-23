#include "Buffer.h"
#include <stdio.h>
#include <stdlib.h>

Buffer * Buffer_init(Object * obj)
{
    Buffer * buf = (Buffer *) calloc(1, sizeof(Buffer) );
    if (!buf) {
        fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
        exit(EXIT_FAILURE);
    }

    buf->VAO = 0;
    buf->VBO = 0;
    buf->EBO = 0;
    buf->object = obj;

    glGenVertexArrays(1, &buf->VAO);
    glGenBuffers(1, &buf->VBO);

    // Bind the VAO
    glBindVertexArray(buf->VAO);

    unsigned char i;
    unsigned char size = 0;
    for (i = 0; i < obj->n_attributes; i++)
        size += obj->layout[i];

    // Bind the VBO and set vertex data
    glBindBuffer(GL_ARRAY_BUFFER, buf->VBO);
    glBufferData(GL_ARRAY_BUFFER, obj->n_points * size * sizeof(float), obj->vertexBuffer, GL_STATIC_DRAW);

    unsigned short stride = 0;
    for (i = 0; i < obj->n_attributes; i++)
    {
        glVertexAttribPointer(i, obj->layout[i], GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(stride * sizeof(float) ));
        glEnableVertexAttribArray(i);
        stride += obj->layout[i];
    }

    // Generate an Element Buffer Object (EBO) for the cube indices
    glGenBuffers(1, &buf->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->n_faces * 3 * sizeof(unsigned int), obj->indexBuffer, GL_STATIC_DRAW);

    // Unbind VAO and VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return buf;
}

void Buffer_update(Object * obj, Buffer * buf)
{
    if (!buf) {
        fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
        exit(EXIT_FAILURE);
    }

    buf->VAO = 0;
    buf->VBO = 0;
    buf->EBO = 0;
    buf->object = obj;

    glGenVertexArrays(1, &buf->VAO);
    glGenBuffers(1, &buf->VBO);

    // Bind the VAO
    glBindVertexArray(buf->VAO);

    unsigned char i;
    unsigned char size = 0;
    for (i = 0; i < obj->n_attributes; i++)
        size += obj->layout[i];

    // Bind the VBO and set vertex data
    glBindBuffer(GL_ARRAY_BUFFER, buf->VBO);
    glBufferData(GL_ARRAY_BUFFER, obj->n_points * size * sizeof(float), obj->vertexBuffer, GL_STATIC_DRAW);

    unsigned short stride = 0;
    for (i = 0; i < obj->n_attributes; i++)
    {
        glVertexAttribPointer(i, obj->layout[i], GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(stride * sizeof(float) ));
        glEnableVertexAttribArray(i);
        stride += obj->layout[i];
    }

    // Generate an Element Buffer Object (EBO) for the cube indices
    glGenBuffers(1, &buf->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->n_faces * 3 * sizeof(unsigned int), obj->indexBuffer, GL_STATIC_DRAW);

    // Unbind VAO and VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer_bind(const Buffer * buf)
{
    glBindVertexArray(buf->VAO);
}

void Buffer_draw(const Buffer * buf)
{
    Buffer_bind(buf);
    glDrawElements(GL_TRIANGLES, buf->object->n_faces * 3, GL_UNSIGNED_INT, 0);
}