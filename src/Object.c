#include "Object.h"
#include <stdio.h>
#include <stdlib.h>

Object * Object_init(const unsigned int n_points, const unsigned char n_attributes, unsigned char * layout, const unsigned int n_faces)
{
    Object * obj = (Object *) calloc(1, sizeof(Object));
    if (!obj) {
        fprintf(stderr, "Error: Memory allocation failed for object.\n");
        exit(EXIT_FAILURE);
    }

    obj->n_points = n_points;
    obj->n_attributes = n_attributes;
    obj->n_faces = n_faces;
    obj->layout = layout;

    unsigned int size = 0;
    for (unsigned char i = 0; i < n_attributes; i++)
        size += layout[i];

    obj->vertexBuffer = (float *) calloc(size * obj->n_points ,sizeof(float) );
    if (!obj->vertexBuffer) {
        fprintf(stderr, "Error: Memory allocation failed for vertex buffer array.\n");
        exit(EXIT_FAILURE);
    }

    obj->indexBuffer = (unsigned int *) calloc(3 * obj->n_faces ,sizeof(unsigned int) );
    if (!obj->indexBuffer) {
        fprintf(stderr, "Error: Memory allocation failed for index buffer array.\n");
        exit(EXIT_FAILURE);
    }    

    return obj;
}